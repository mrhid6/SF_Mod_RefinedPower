// ILikeBanas


#include "RPSolarPanel.h"
#include "RPSolarController.h"
#include "Kismet/GameplayStatics.h"
#include "FGTimeSubsystem.h"
#include "util/Logging.h"
#include "Components/StaticMeshComponent.h"

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

	if (HasAuthority()) {
		timeSys = AFGTimeOfDaySubsystem::Get(GetWorld());
		FGPowerConnection->SetPowerInfo(GetPowerInfo());
		GetSolarController();
		CachePanelStaticMesh();
	}
}

void ARPSolarPanel::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARPSolarPanel, mPanelEnabled);
}

void ARPSolarPanel::Tick(float dt) {
	Super::Tick(dt);

	if (HasAuthority()) {
		Multicast_UpdateSolarPanelRotation();
	}
}

void ARPSolarPanel::Factory_Tick(float dt) {
	Super::Factory_Tick(dt);
	if (HasAuthority()) {
		SetPowerOutput();
		DetectObjectsInWay();
	}
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

void ARPSolarPanel::Multicast_UpdateSolarPanelRotation_Implementation(){
	if (mSolarController != nullptr) {
		UpdateSolarPanelRotation(mSolarController->GetOrientation());
	}
}

ARPSolarController* ARPSolarPanel::GetSolarController() {
	if (mSolarController == nullptr) {
		mSolarController = ARPSolarController::Get(GetWorld());
	}

	return mSolarController;
}

void ARPSolarPanel::CachePanelStaticMesh(){
	FName tag = FName(TEXT("SM_SP_Panel"));
	TArray<UActorComponent*> meshes = GetComponentsByTag(UStaticMeshComponent::StaticClass(), tag);

	if (meshes.Num() > 0) {
		mCachedPanelStaticMesh = Cast<UStaticMeshComponent>(meshes[0]);
	}
	else {
		SML::Logging::warning("[RefinedPower] - Couldn't find static mesh!");
	}
}

void ARPSolarPanel::DetectObjectsInWay() {

	if (mCachedPanelStaticMesh == nullptr) {
		return;
	}

	FHitResult OutHit;
	FVector ForwardVector = (mCachedPanelStaticMesh->GetForwardVector() * -1);
	FVector CompLocation = mCachedPanelStaticMesh->GetComponentLocation();


	FVector Start = ((ForwardVector * 130.f) + CompLocation);

	FVector End = ((ForwardVector * 2000.f) + Start);
	FCollisionQueryParams CollisionParams;

	// This line doesn't work in game only in editor!
	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
	{
		if (OutHit.bBlockingHit)
		{
			FString ActorName = FString::Printf(TEXT("You are hitting: %s"), *OutHit.GetActor()->GetName());
			SML::Logging::info(TCHAR_TO_UTF8(*ActorName));
		}
	}
}

