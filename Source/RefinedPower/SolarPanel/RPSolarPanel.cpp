// ILikeBanas


#include "RPSolarPanel.h"
#include "RPSolarController.h"
#include "Kismet/GameplayStatics.h"
#include "FGTimeSubsystem.h"
#include "util/Logging.h"

void URPSolarPanelRCO::SetPanelEnabled_Implementation(ARPSolarPanel* panel, bool enabled) {
	panel->mPanelEnabled = enabled;
	//panel->panelStateUpdated();

	panel->ForceNetUpdate();
}

bool URPSolarPanelRCO::SetPanelEnabled_Validate(ARPSolarPanel* panel, bool enabled) {
	return true;
}

ARPSolarPanel::ARPSolarPanel()
{
	FGPowerConnection = CreateDefaultSubobject<UFGPowerConnectionComponent>(TEXT("FGPowerConnection"));
	FGPowerConnection->SetupAttachment(RootComponent);

	SetReplicates(true);
	bReplicates = true;

}

ARPSolarPanel::~ARPSolarPanel() {};

bool ARPSolarPanel::ShouldSave_Implementation() const {
	return true;
}

void ARPSolarPanel::BeginPlay() 
{
	Super::BeginPlay();
	getSolarController();
}

void ARPSolarPanel::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	

}

void ARPSolarPanel::getSolarController() {
	TArray<AActor*> temp;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARPSolarController::StaticClass(), temp);
	if(temp.Num() > 0)solarController = temp[0];
}

void ARPSolarPanel::updatePanelRotation() {
	
}

void ARPSolarPanel::updateSupportRotation() {

}