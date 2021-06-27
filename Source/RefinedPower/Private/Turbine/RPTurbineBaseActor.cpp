#include "Turbine/RPTurbineBaseActor.h"
#include "Math/UnrealMathUtility.h"
#include "FGPowerConnectionComponent.h"
#include "FGPowerInfoComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine.h"
#include "FGPlayerController.h"
#include "LandscapeProxy.h"

void URPTurbineBaseRCO::SetTurbineEnabled_Implementation(ARPTurbineBaseActor* turbine, bool enabled)
{
    turbine->mTurbineEnabled = enabled;
    turbine->TurbineStateUpdated();

    turbine->calculateTurbinePowerProduction();
    turbine->ForceNetUpdate();
}

bool URPTurbineBaseRCO::SetTurbineEnabled_Validate(ARPTurbineBaseActor* turbine, bool enabled)
{
    return true;
}

void URPTurbineBaseRCO::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(URPTurbineBaseRCO, bTest)
}

ARPTurbineBaseActor::ARPTurbineBaseActor()
{
    FGPowerConnection = CreateDefaultSubobject<UFGPowerConnectionComponent>(TEXT("FGPowerConnection1"));
    FGPowerConnection->SetupAttachment(RootComponent);

    SetReplicates(true);
    bReplicates = true;

    mTurbineEnabled = true;
}

ARPTurbineBaseActor::~ARPTurbineBaseActor()
{
}

void ARPTurbineBaseActor::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        FGPowerConnection->SetPowerInfo(GetPowerInfo());
        LineTraceToGround();
    }

    calcNearbyWindTurbines();
    calculateTurbinePowerProduction();
    TurbineStateUpdated();
}

void ARPTurbineBaseActor::EndPlay(const EEndPlayReason::Type endPlayReason)
{
    if (endPlayReason == EEndPlayReason::Destroyed)
    {
        calcNearbyWindTurbines();
    }
    Super::EndPlay(endPlayReason);
}

void ARPTurbineBaseActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ARPTurbineBaseActor, mTurbineEnabled);
    DOREPLIFETIME(ARPTurbineBaseActor, mWindTurbinesInArea);
    DOREPLIFETIME(ARPTurbineBaseActor, mDistanceFromRound);
}

void ARPTurbineBaseActor::LineTraceToGround()
{

    if(mIsHeightBasedTurbine == false)
    {
        return;
    }
    
    FVector Start = GetActorLocation();
    FVector End = Start - FVector(0,0,50000);

    FHitResult outHit;

    const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = TArray<TEnumAsByte<EObjectTypeQuery>>{
        EObjectTypeQuery::ObjectTypeQuery1, EObjectTypeQuery::ObjectTypeQuery2
    };

    TArray<AActor*> ActorsToIgnore;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), ActorsToIgnore);

    TArray<AActor*> ActorsToInclude;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALandscapeProxy::StaticClass(), ActorsToInclude);

    for(auto landscape: ActorsToInclude)
    {
        ActorsToIgnore.Remove(landscape);
    }
    
    UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), Start, End, ObjectTypes, true, ActorsToIgnore,EDrawDebugTrace::None,outHit, true);

    if(outHit.bBlockingHit)
    {
        ALandscapeProxy* LandscapeActor = Cast<ALandscapeProxy>(outHit.Actor);

        if(LandscapeActor != nullptr)
        {
            mDistanceFromRound = outHit.Distance;
        }
    }
}

bool ARPTurbineBaseActor::ShouldSave_Implementation() const
{
    return true;
}

void ARPTurbineBaseActor::calculateTurbinePowerProduction()
{
    if (mTurbineType == ETurbineType::RP_Wind)
    {
        if (mIsHeightBasedTurbine == true)
        {
            mTurbinePowerProduction = getTurbineHeightPowerProduction();
        }
        else
        {
            mTurbinePowerProduction = getTurbineBasePowerProduction();
        }

        if ((mWindTurbinesInArea + 1) >= mMaxWindTurbinesInArea)
        {
            mTurbinePowerProduction = 0;
        }
    }

    if (mTurbineEnabled == false)
    {
        mTurbinePowerProduction = 0;
    }

    setTurbinePowerOutput();
}

void ARPTurbineBaseActor::setTurbinePowerOutput()
{
    UFGPowerInfoComponent* TempFGPowerInfo = FGPowerConnection->GetPowerInfo();

    if (TempFGPowerInfo != nullptr)
    {
        const float powerOutput = getTurbineActualPowerProduction();
        TempFGPowerInfo->SetBaseProduction(powerOutput);
    }
}

float ARPTurbineBaseActor::getTurbineBasePowerProduction()
{
    return mPowerProduction;
}

float ARPTurbineBaseActor::getTurbineHeightPowerProduction()
{
    const FVector ActorLocation = GetActorLocation();

    float ActorHeight = ActorLocation.Z / 800;
    const float basePower = getTurbineBasePowerProduction();

    float tempPower = 0;

    if (ActorHeight < 7)
    {
        tempPower = basePower;
    }
    else
    {
        tempPower = ActorHeight;
    }

    tempPower -= basePower;
    tempPower *= float(2.1);
    tempPower += basePower;

    tempPower = FMath::Clamp(tempPower, float(10.0), float(30.0));
    tempPower = int(tempPower);

    return tempPower;
}

float ARPTurbineBaseActor::getTurbineActualPowerProduction()
{
    float powerOutput = 0.0f;
    if (mTurbineType == ETurbineType::RP_Wind)
    {
        float degradeMultiplier = float(getNearbyWindTurbinesCount()) / mMaxWindTurbinesInArea;
        float PowerReduction = mTurbinePowerProduction * degradeMultiplier;
        
        powerOutput = mTurbinePowerProduction - PowerReduction;
    }
    else
    {
        powerOutput = mTurbinePowerProduction;
    }
    return powerOutput;
}

float ARPTurbineBaseActor::getMaxTurbinePowerProduction()
{
    return mMaxTurbinePowerProduction;
}

void ARPTurbineBaseActor::UpdateCachedNearbyWindTurbines()
{
    const FVector ActorLocation = GetActorLocation();

    const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = TArray<TEnumAsByte<EObjectTypeQuery>>{
        EObjectTypeQuery::ObjectTypeQuery1, EObjectTypeQuery::ObjectTypeQuery2
    };
    TArray<AActor*> ActorsToIgnore = TArray<AActor*>{this};
    TArray<AActor*> OutActors;


    bool foundTurbines = UKismetSystemLibrary::SphereOverlapActors(this, ActorLocation, mAreaOfWindTurbines,
                                                                   ObjectTypes, this->GetClass(), ActorsToIgnore,
                                                                   OutActors);

    mCachedNearbyWindTurbines.Empty();

    if (foundTurbines)
    {
        for (const AActor* turbine : OutActors)
        {
            ARPTurbineBaseActor* RPTurbine = (ARPTurbineBaseActor*)turbine;

            if (RPTurbine->mTurbineType == ETurbineType::RP_Wind)
            {
                mCachedNearbyWindTurbines.Add(RPTurbine);
            }
        }
    }
}

int ARPTurbineBaseActor::getNearbyWindTurbinesCount()
{
    return mWindTurbinesInArea;
}

void ARPTurbineBaseActor::calcNearbyWindTurbines()
{
    if (mTurbineType == ETurbineType::RP_Water)
    {
        return;
    }

    updateNearbyWindTurbineCount();

    for (ARPTurbineBaseActor* turbine : mCachedNearbyWindTurbines)
    {
        turbine->updateNearbyWindTurbineCount();
        turbine->calculateTurbinePowerProduction();
        turbine->setTurbinePowerOutput();
    }
}

void ARPTurbineBaseActor::updateNearbyWindTurbineCount()
{
    UpdateCachedNearbyWindTurbines();
    mWindTurbinesInArea = 0;
    mWindTurbinesInArea = FMath::Clamp(mCachedNearbyWindTurbines.Num(), 0, 9999);
}

bool ARPTurbineBaseActor::isTurbineEnabled()
{
    return mTurbineEnabled;
}

void ARPTurbineBaseActor::setTurbineEnabled(bool turbineEnabled)
{
    auto rco = Cast<URPTurbineBaseRCO>(
        Cast<AFGPlayerController>(GetWorld()->GetFirstPlayerController())->GetRemoteCallObjectOfClass(
            URPTurbineBaseRCO::StaticClass()));

    if (rco)
    {
        rco->SetTurbineEnabled(this, turbineEnabled);
    }
}
