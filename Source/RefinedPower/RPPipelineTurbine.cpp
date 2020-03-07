#include "RPPipelineTurbine.h"
#include "util/Logging.h"
#include "Math/UnrealMathUtility.h"
#include "Containers/UnrealString.h"
#include "FGPowerConnectionComponent.h"
#include "FGPipeConnectionComponent.h"
#include "FGPowerInfoComponent.h"


ARPPipelineTurbine::ARPPipelineTurbine() {
	UFGPowerInfoComponent* FGPowerInfo = CreateDefaultSubobject<UFGPowerInfoComponent>(TEXT("FGPowerInfo1"));

	FGPowerConnection = CreateDefaultSubobject<UFGPowerConnectionComponent>(TEXT("FGPowerConnection1"));
	FGPowerConnection->SetupAttachment(RootComponent);
	FGPowerConnection->SetPowerInfo(FGPowerInfo);

	InputPipe1 = CreateDefaultSubobject<UFGPipeConnectionComponent>(TEXT("InputPipe1"));
	OutputPipe1 = CreateDefaultSubobject<UFGPipeConnectionComponent>(TEXT("OutputPipe1"));

	InputPipe1->SetupAttachment(RootComponent);
	OutputPipe1->SetupAttachment(RootComponent);

	mTurbineEnabled = true;
}

void ARPPipelineTurbine::BeginPlay() {

	Super::BeginPlay();
}

void ARPPipelineTurbine::EndPlay(const EEndPlayReason::Type endPlayReason) {

	SML::Logging::info("[RefinedPower] - EndPlay: 1");
	Super::EndPlay(endPlayReason);
}

void ARPPipelineTurbine::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARPPipelineTurbine, mTurbineEnabled);
}