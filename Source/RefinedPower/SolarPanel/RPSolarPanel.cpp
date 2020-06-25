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

void URPSolarPanelRCO::SetMaintainMW_Implementation(ARPSolarPanel* panel, float amount) {
	panel->mMaintainPowerOutputAmount = amount;
	SML::Logging::info("[RefinedPower] - ", amount);

	panel->ForceNetUpdate();
}

bool URPSolarPanelRCO::SetMaintainMW_Validate(ARPSolarPanel* panel, float amount) {
	return true;
}

void URPSolarPanelRCO::SetAmountToStore_Implementation(ARPSolarPanel* panel, float amount) {
	panel->mPercentageToStore = amount;

	panel->ForceNetUpdate();
}

bool URPSolarPanelRCO::SetAmountToStore_Validate(ARPSolarPanel* panel, float amount) {
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
	GetSolarController();

	if (HasAuthority()) {
		timeSys = AFGTimeOfDaySubsystem::Get(GetWorld());
		FGPowerConnection->SetPowerInfo(GetPowerInfo());
		CacheTraceLineComponents();
		GetWorld()->GetTimerManager().SetTimer(mSolarPanelHandle, this, &ARPSolarPanel::UpdateLineTraceRotation, 5.0f, true);
	}

	if (mSolarController && mRotatesTowardSun) {

		FTransform panelTemp = GetActorTransform();
		FTransform supportTemp = GetActorTransform();
		FVector position1 = panelTemp.TransformPosition(FVector(0, -631, 407));
		FVector position2 = panelTemp.TransformPosition(FVector(0, -631, 255));

		panelTemp.SetLocation(position1);
		supportTemp.SetLocation(position2);


		mSolarController->SpawnIM(panelTemp, supportTemp, GetUniqueID());

		SML::Logging::info("[RefinedPower] - Spawned SolarPanel Panel!");

	}
	
}

void ARPSolarPanel::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARPSolarPanel, mPanelEnabled);
	DOREPLIFETIME(ARPSolarPanel, mSolarController);
	DOREPLIFETIME(ARPSolarPanel, mPercentageToStore);
	DOREPLIFETIME(ARPSolarPanel, mMaintainPowerOutputAmount);
	DOREPLIFETIME(ARPSolarPanel, mTotalBlockingHits);
}

void ARPSolarPanel::Tick(float dt) {
	Super::Tick(dt);
}

void ARPSolarPanel::Factory_Tick(float dt) {
	Super::Factory_Tick(dt);
	if (HasAuthority()) {
		SetPowerOutput();

		ForceNetUpdate();
	}
}

void ARPSolarPanel::EndPlay(const EEndPlayReason::Type endPlayReason) {

	if (endPlayReason == EEndPlayReason::Destroyed) {
		if (mSolarController && mRotatesTowardSun) {
			mSolarController->DestroyIM(GetUniqueID());
		}
	}
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	Super::EndPlay(endPlayReason);
}

float ARPSolarPanel::GetPanelPowerOutput()
{
	float powerout = 0.0f;

	if (mSolarController && timeSys) {
		float scalar = mSolarController->GetCurrectProductionScalar();

		if (timeSys->IsDay() && mPanelType != ESolarPanelType::RP_NIGHT) {
			powerout = mMaxSolarPanelProduction * scalar;
			powerout = FMath::Clamp(powerout, mMinSolarPanelProduction, mMaxSolarPanelProduction);
		}
		else if (mPanelType == ESolarPanelType::RP_DAYONLY && timeSys->IsNight()) {
			powerout = 0.0f;
		}
		else if (mPanelType != ESolarPanelType::RP_DAYONLY && timeSys->IsNight()) {
			powerout = (mMaxSolarPanelProduction * mNightTimePowerReduction) * scalar;

			powerout = FMath::Clamp(powerout, 0.0f, mMaxSolarPanelProduction);
		}

		if (mTotalBlockingHits > 0) {
			float reducePower = (powerout * mObstructionPowerPercentLoss) * mTotalBlockingHits;
			powerout -= reducePower;
		}
		powerout = FMath::Clamp(powerout, 0.0f, mMaxSolarPanelProduction);
	}

	return powerout;
}

float ARPSolarPanel::GetPowerOutput()
{
	float PanelOutput = GetPanelPowerOutput();

	float PowerOutput = PanelOutput;

	if (mHasBattery) {
		float amountToStore = PanelOutput * mPercentageToStore;

		if (mBatteryPowerStored >= mMaxBatteryStorage) {
			amountToStore = 0;
		}
		else {
			mBatteryPowerStored += amountToStore;
		}

		PowerOutput -= amountToStore;

		if (PowerOutput < mMaintainPowerOutputAmount) {
			float PowerToTake = mMaintainPowerOutputAmount - PowerOutput;

			if (mBatteryPowerStored > PowerToTake) {
				mBatteryPowerStored -= PowerToTake;
			}
			else {
				PowerToTake = mBatteryPowerStored;
				mBatteryPowerStored -= PowerToTake;
			}

			PowerOutput += PowerToTake;
		}

		mBatteryPowerStored = FMath::Clamp(mBatteryPowerStored, 0.0f, mMaxBatteryStorage);
	}

	//SML::Logging::info("[RefinedPower] - %f", mBatteryPowerStored);
	return PowerOutput;
}

void ARPSolarPanel::SetPowerOutput(){
	UFGPowerInfoComponent* TempFGPowerInfo = FGPowerConnection->GetPowerInfo();

	if (TempFGPowerInfo != nullptr) {
		TempFGPowerInfo->SetBaseProduction(GetPowerOutput());
	}
}

void ARPSolarPanel::UpdateLineTraceRotation()
{
	
	if (mCachedTraceLineController && mSolarController && mRotatesTowardSun) {
		mCachedTraceLineController->SetWorldRotation(mSolarController->GetOrientation());
	}

	DetectObjectsInWay();
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

	tag = FName(TEXT("RP_LineTrace_Controller"));

	TArray<UActorComponent*>comps = GetComponentsByTag(USphereComponent::StaticClass(), tag);
	mCachedTraceLineController = Cast<USphereComponent>(comps[0]);
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

void ARPSolarPanel::setPanelEnabled(bool enabled) {
	auto rco = Cast<URPSolarPanelRCO>(Cast<AFGPlayerController>(GetWorld()->GetFirstPlayerController())->GetRemoteCallObjectOfClass(URPSolarPanelRCO::StaticClass()));

	if (rco) {
		rco->SetPanelEnabled(this, enabled);
	}
}

void ARPSolarPanel::setPanelMaintainMW(float amount)
{
	auto rco = Cast<URPSolarPanelRCO>(Cast<AFGPlayerController>(GetWorld()->GetFirstPlayerController())->GetRemoteCallObjectOfClass(URPSolarPanelRCO::StaticClass()));

	if (rco) {
		rco->SetMaintainMW(this, amount);
	}
}

void ARPSolarPanel::setPanelStoreAmount(float amount)
{
	auto rco = Cast<URPSolarPanelRCO>(Cast<AFGPlayerController>(GetWorld()->GetFirstPlayerController())->GetRemoteCallObjectOfClass(URPSolarPanelRCO::StaticClass()));

	if (rco) {
		rco->SetAmountToStore(this, amount);
	}
}
