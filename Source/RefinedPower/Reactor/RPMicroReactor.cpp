#include "RPMicroReactor.h"
#include "FGFactoryConnectionComponent.h"
#include "UnrealNetwork.h"
#include "FGPowerInfoComponent.h"
#include "util/Logging.h"
#include "FGPowerConnectionComponent.h"
#include "FGInventoryComponent.h"
#include "FGInventoryLibrary.h"
#include "FGPlayerController.h"


void URPMicroReactorRCO::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URPMicroReactorRCO, bTest)
}

ARPMicroReactor::ARPMicroReactor() {
	//pwr
	FGPowerConnection = CreateDefaultSubobject<UFGPowerConnectionComponent>(TEXT("FGPowerConnection"));
	FGPowerConnection->SetupAttachment(RootComponent);

	/*############### Settup machine item inputs ###############*/
	InputConveyor = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("ConveyorInput1"));
	InputConveyor->SetupAttachment(RootComponent);

	mFuelInventory = CreateDefaultSubobject<UFGInventoryComponent>(TEXT("FuelInventory"));
	/*############################################################*/

	/*############### Settup tick values ###############*/
	SetReplicates(true);
	bReplicates = true;
	mFactoryTickFunction.bCanEverTick = true;

	/*############################################################*/
}

ARPMicroReactor::~ARPMicroReactor() {}

void ARPMicroReactor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARPMicroReactor, mReactorState)
	DOREPLIFETIME(ARPMicroReactor, mWarmUpAmount)
	DOREPLIFETIME(ARPMicroReactor, mCachedReactorCoresAmount)
	DOREPLIFETIME(ARPMicroReactor, mCachedReactorCoresMax)

}

void ARPMicroReactor::BeginPlay() {
	Super::BeginPlay();

	if (HasAuthority()) {
		FGPowerConnection->SetPowerInfo(GetPowerInfo());
	}

	mTriggerReactorStateUpdatedEvent = true;
}

void ARPMicroReactor::Tick(float dt) {
	Super::Tick(dt);

	if (HasAuthority() && mTriggerReactorStateUpdatedEvent) {
		mTriggerReactorStateUpdatedEvent = false;
		ReactorStateUpdated();
	}
}

bool ARPMicroReactor::CanStartPowerProduction_Implementation() const {
	Super::CanStartPowerProduction_Implementation();

	if (mReactorState == EMicroReactorState::RP_State_Producing) {
		return true;
	}
	else {
		return false;
	}
}

/*########## Main Functions ##########*/

void ARPMicroReactor::CalcReactorState() {
	switch (mReactorState) {
	case EMicroReactorState::RP_State_Cold:
	{
		if (getReactorCores() >= GetReactorMinStart()) {
			SetReactorState(EMicroReactorState::RP_State_WarmUp);
			mTriggerReactorStateUpdatedEvent = true;
		}

		break;
	}
	case EMicroReactorState::RP_State_WarmUp:
	{
		IncreaseWarmUp();
		if (mWarmUpAmount >= 100.0f) {
			SetReactorState(EMicroReactorState::RP_State_Producing);
			mTriggerReactorStateUpdatedEvent = true;
		}

		break;
	}
	case EMicroReactorState::RP_State_Producing:
	{
		if (getReactorCores() <= GetReactorMinStop()) {
			SetReactorState(EMicroReactorState::RP_State_Cold);
			ResetWarmUp();
			mTriggerReactorStateUpdatedEvent = true;
		}

		break;
	}
	default:
		break;
	
	}
}

void ARPMicroReactor::SetReactorState(EMicroReactorState state) {
	mReactorState = state;
}

EMicroReactorState ARPMicroReactor::GetReactorState() {
	return mReactorState;
}

float ARPMicroReactor::GetWarmUpAmount() {
	return mWarmUpAmount;
}

void ARPMicroReactor::IncreaseWarmUp() {
	mWarmUpAmount += 0.0416f;
	mWarmUpAmount = FMath::Clamp(mWarmUpAmount, 0.0f, 100.0f);
}

void ARPMicroReactor::ResetWarmUp() {
	mWarmUpAmount = 0.0f;
}


void ARPMicroReactor::CacheFuelAmount() {
	mCachedReactorCoresAmount = getReactorCores();
	mCachedReactorCoresMax = getReactorCoresMax();
}


int ARPMicroReactor::getReactorCores() {
	if (HasAuthority()) {
		FInventoryStack out_stack;

		bool gotFuel = GetFuelInventory()->GetStackFromIndex(mFuelInventoryIndex, out_stack);

		int fuelAmnt = 0;

		if (gotFuel) {
			fuelAmnt = out_stack.NumItems;
		}

		return fuelAmnt;
	}
	else {
		return mCachedReactorCoresAmount;
	}
}

int ARPMicroReactor::getReactorCoresMax() {
	if (HasAuthority()) {
		FInventoryStack out_stack;

		bool gotFuel = GetFuelInventory()->GetStackFromIndex(mFuelInventoryIndex, out_stack);

		int fuelAmnt = 500;

		if (gotFuel) {
			TSubclassOf<UFGItemDescriptor> fuelClass = out_stack.Item.ItemClass;

			if (fuelClass == nullptr) {
				return fuelAmnt;
			}

			fuelAmnt = UFGItemDescriptor::GetStackSize(fuelClass);
		}

		return fuelAmnt;
	}
	else {
		return mCachedReactorCoresMax;
	}
}

int ARPMicroReactor::GetReactorMinStart() {
	return int(getReactorCoresMax() * mMinStartAmount); 
}

int ARPMicroReactor::GetReactorMinStop() {
	return int(getReactorCoresMax() * mMinStopAmount);
}


/*#### End getters and setters*/

/*#######################################*/

//tick function - primary logic
void ARPMicroReactor::Factory_Tick(float dt) {
	Super::Factory_Tick(dt);

	if (HasAuthority()) {
		CalcReactorState();
		CacheFuelAmount();

		ForceNetUpdate();
	}
}