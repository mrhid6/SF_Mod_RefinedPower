// ILikeBanas


#include "RPSolarPanel.h"
#include "RPSolarController.h"
#include "Kismet/GameplayStatics.h"
#include "FGTimeSubsystem.h"
#include "util/Logging.h"


#include "DrawDebugHelpers.h"

void URPSolarPanelRCO::SetPanelEnabled_Implementation(ARPSolarPanel* panel, bool enabled) {
	panel->mPanelEnabled = enabled;
	//panel->panelStateUpdated();

	panel->ForceNetUpdate();
}

bool URPSolarPanelRCO::SetPanelEnabled_Validate(ARPSolarPanel* panel, bool enabled) {
	return true;
}

void URPSolarPanelRCO::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URPSolarPanelRCO, bTest)
}

ARPSolarPanel::ARPSolarPanel()
{
	FGPowerConnection = CreateDefaultSubobject<UFGPowerConnectionComponent>(TEXT("FGPowerConnection"));
	FGPowerConnection->SetupAttachment(RootComponent);


	SetReplicates(true);
	bReplicates = true;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	PrimaryActorTick.SetTickFunctionEnable(true);

	mFactoryTickFunction.bCanEverTick = true;
	mFactoryTickFunction.bAllowTickOnDedicatedServer = true;
	mFactoryTickFunction.bStartWithTickEnabled = true;

	mFactoryTickFunction.SetTickFunctionEnable(true);
}

ARPSolarPanel::~ARPSolarPanel() {};

void ARPSolarPanel::BeginPlay() 
{
	Super::BeginPlay();

	PrimaryActorTick.TickInterval = 5;

	if (HasAuthority()) {
		timeSys = AFGTimeOfDaySubsystem::Get(GetWorld());
		FGPowerConnection->SetPowerInfo(GetPowerInfo());
		GetSolarController();
		if (mSolarController) {

			FTransform temp = FTransform();

			//mSolarController->SpawnIM(GetActorTransform(), GetUniqueID());
			mSolarController->SpawnIM(temp, GetUniqueID());
		}
		//CacheTraceLineComponents();
	}
}

void ARPSolarPanel::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARPSolarPanel, mPanelEnabled);
	DOREPLIFETIME(ARPSolarPanel, mSolarController);
}

void ARPSolarPanel::Tick(float dt) {
	Super::Tick(dt);
}

void ARPSolarPanel::Factory_Tick(float dt) {
	Super::Factory_Tick(dt);
	if (HasAuthority()) {

		if (mDetectShadowsTimer >= 300) {
			mDetectShadowsTimer = 0.0f;
			//DetectObjectsInWay();
			//SetPowerOutput();
		}
		else {
			mDetectShadowsTimer += 1;
		}
	}
}

void ARPSolarPanel::EndPlay(const EEndPlayReason::Type endPlayReason) {

	if (endPlayReason == EEndPlayReason::Destroyed) {
		mSolarController->DestroyIM(GetUniqueID());
	}
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	Super::EndPlay(endPlayReason);
}


float ARPSolarPanel::GetPowerOutput()
{
	float powerout = 0.0f;

	if (mSolarController && timeSys) {
		float scalar = mSolarController->GetCurrectProductionScalar();

		if (timeSys->IsDay()) {
			powerout = mMaxSolarPanelProduction * scalar;
		}
		else if (mPanelType == ESolarPanelType::RP_DAYONLY && timeSys->IsNight()) {
			powerout = 0.0f;
		}
		else if (mPanelType == ESolarPanelType::RP_DAYNIGHT && timeSys->IsNight()) {
			powerout = (mMaxSolarPanelProduction * mNightTimePowerReduction) * scalar;
		}
	}

	return powerout;

}

void ARPSolarPanel::SetPowerOutput(){
	UFGPowerInfoComponent* TempFGPowerInfo = FGPowerConnection->GetPowerInfo();

	if (TempFGPowerInfo != nullptr) {
		TempFGPowerInfo->SetBaseProduction(GetPowerOutput());
	}
}

ARPSolarController* ARPSolarPanel::GetSolarController() {
	if (mSolarController == nullptr) {
		mSolarController = ARPSolarController::Get(GetWorld());
	}

	return mSolarController;
}

void ARPSolarPanel::CacheTraceLineComponents(){
	FName tag = FName(TEXT("RP_LineTrace"));
	mCachedTraceLineComponents = GetComponentsByTag(URPLineTraceComponent::StaticClass(), tag);

	if (mCachedTraceLineComponents.Num() > 0) {
		SML::Logging::info("[RefinedPower] - Found Solar Line Traces: ", mCachedTraceLineComponents.Num());
	}
	else {
		SML::Logging::warning("[RefinedPower] - Couldn't find line traces");
	}
}

void ARPSolarPanel::DetectObjectsInWay() {

	if (mCachedTraceLineComponents.Num() == 0) {
		return;
	}

	mTotalBlockingHits = 0;

	for (UActorComponent* comp : mCachedTraceLineComponents) {
		URPLineTraceComponent* linetracecomp = Cast<URPLineTraceComponent>(comp);

		FHitResult OutHit;
		linetracecomp->TraceLine(OutHit);

		if (OutHit.bBlockingHit)
		{
			mTotalBlockingHits++;
		}
	}
}

