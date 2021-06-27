// ILikeBanas

#include "Reactor/RPFusionReactor.h"

ARPFusionReactor::ARPFusionReactor()
{
    //pwr
    FGPowerConnection = CreateDefaultSubobject<UFGPowerConnectionComponent>(TEXT("FGPowerConnection"));
    FGPowerConnection->SetupAttachment(RootComponent);

    //sound
    FusionReactorSound = CreateDefaultSubobject<UAudioComponent>(TEXT("ArcReactorSound"));
    FusionReactorSound->SetupAttachment(RootComponent);

    /*############### Settup machine item inputs ###############*/
    InputConveyor = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("ConveyorInput1"));
    InputPipe = CreateDefaultSubobject<UFGPipeConnectionComponent>(TEXT("PipeInput1"));

    InputConveyor->SetupAttachment(RootComponent);
    InputPipe->SetupAttachment(RootComponent);

    mFuelInventory = CreateDefaultSubobject<UFGInventoryComponent>(TEXT("FuelInventory"));
    /*############################################################*/

    /*############### Settup tick values ###############*/
    SetReplicates(true);
    bReplicates = true;
    mFactoryTickFunction.bCanEverTick = true;

    /*############################################################*/
}

ARPFusionReactor::~ARPFusionReactor()
{
}


void ARPFusionReactor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ARPFusionReactor, mSpinRate);
    DOREPLIFETIME(ARPFusionReactor, mReactorState);
    DOREPLIFETIME(ARPFusionReactor, mChargeAmount);

    DOREPLIFETIME(ARPFusionReactor, mCachedReactorCoresAmount);
    DOREPLIFETIME(ARPFusionReactor, mCachedReactorCoresMax);
    DOREPLIFETIME(ARPFusionReactor, mCachedCoolantAmount);
}

void ARPFusionReactor::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        FGPowerConnection->SetPowerInfo(GetPowerInfo());
    }

    Client_SpinRateChanged();
}

void ARPFusionReactor::Tick(float dt)
{
    Super::Tick(dt);

    if (HasAuthority())
    {
        if (TriggerSpinRateChanged)
        {
            TriggerSpinRateChanged = false;
            Client_SpinRateChanged();
        }
    }
}

bool ARPFusionReactor::CanStartPowerProduction_Implementation() const
{
    Super::CanStartPowerProduction_Implementation();

    if (mReactorState == EFusionReactorState::RP_State_Producing)
    {
        return true;
    }
    else
    {
        return false;
    }
}


void ARPFusionReactor::Factory_Tick(float dt)
{
    Super::Factory_Tick(dt);

    if (HasAuthority())
    {
        cacheFuelAmounts();

        if (mReactorState == EFusionReactorState::RP_State_Charging)
        {
            ChargeReactor();

            if (mChargeAmount >= mMaxCharge)
            {
                ResetReactorCharge();
                mReactorState = EFusionReactorState::RP_State_SpinUp;
            }
        }

        if (mReactorState == EFusionReactorState::RP_State_SpinUp)
        {
            IncreaseSpinUp();

            if (mSpinRate >= 1.0f)
            {
                mReactorState = EFusionReactorState::RP_State_Producing;
            }
        }

        if (mReactorState == EFusionReactorState::RP_State_SpinDown)
        {
            DecreaseSpinUp();

            if (mSpinRate <= 0.0f)
            {
                mReactorState = EFusionReactorState::RP_State_SpunDown;
            }
        }

        if (mReactorState == EFusionReactorState::RP_State_SpunDown)
        {
            if (getReactorCores() >= mMinStartAmount && getReactorCoolantInternal() >= 20000)
            {
                ResetReactorCharge();
                mReactorState = EFusionReactorState::RP_State_Charging;
            }
        }

        if (mReactorState == EFusionReactorState::RP_State_Producing)
        {
            ResetReactorCharge();
            if (getReactorCores() <= mMinStopAmount || getReactorCoolantInternal() < 10000)
            {
                mReactorState = EFusionReactorState::RP_State_SpinDown;
            }
        }

    }
}


void ARPFusionReactor::ChargeReactor()
{
    GetPowerInfo()->SetTargetConsumption(mChargeAmountPerTick);
    if (GetPowerInfo()->HasPower())
    {
        mChargeAmount += mChargeAmountPerTick;

        mChargeAmount = FMath::Clamp(mChargeAmount, 0.0f, mMaxCharge);
        //SML::Logging::info(mChargeAmount);
    }
}

void ARPFusionReactor::ResetReactorCharge()
{
    GetPowerInfo()->SetTargetConsumption(0.0f);
    mChargeAmount = 0;
}

void ARPFusionReactor::IncreaseSpinUp()
{
    float OldSpinRate = mSpinRate;
    mSpinRate += 0.001f;

    mSpinRate = FMath::Clamp(mSpinRate, 0.0f, 1.0f);

    if (OldSpinRate != mSpinRate)
    {
        TriggerSpinRateChanged = true;
    }
}

void ARPFusionReactor::DecreaseSpinUp()
{
    float OldSpinRate = mSpinRate;
    mSpinRate -= 0.001f;

    mSpinRate = FMath::Clamp(mSpinRate, 0.0f, 1.0f);

    if (OldSpinRate != mSpinRate)
    {
        TriggerSpinRateChanged = true;
    }
}


int ARPFusionReactor::getReactorCores()
{
    if (HasAuthority())
    {
        FInventoryStack out_stack;

        bool gotFuel = GetFuelInventory()->GetStackFromIndex(mFuelInventoryIndex, out_stack);

        int fuelAmnt = 0;

        if (gotFuel)
        {
            fuelAmnt = out_stack.NumItems;
        }

        return fuelAmnt;
    }
    else
    {
        return mCachedReactorCoresAmount;
    }
}

int ARPFusionReactor::getReactorCoresMax()
{
    if (HasAuthority())
    {
        FInventoryStack out_stack;

        bool gotFuel = GetFuelInventory()->GetStackFromIndex(mFuelInventoryIndex, out_stack);

        int fuelAmnt = 500;

        if (gotFuel)
        {
            TSubclassOf<UFGItemDescriptor> fuelClass = out_stack.Item.ItemClass;

            if (fuelClass == nullptr)
            {
                return fuelAmnt;
            }

            fuelAmnt = UFGItemDescriptor::GetStackSize(fuelClass);
        }

        return fuelAmnt;
    }
    else
    {
        return mCachedReactorCoresMax;
    }
}

float ARPFusionReactor::getReactorCoolantInternal()
{
    if (HasAuthority())
    {
        FInventoryStack out_stack;

        bool gotCoolant = GetFuelInventory()->GetStackFromIndex(mSupplementalInventoryIndex, out_stack);

        int coolantAmnt = 0;

        if (gotCoolant)
        {
            coolantAmnt = out_stack.NumItems;
        }
        return coolantAmnt;
    }
    else
    {
        return mCachedCoolantAmount;
    }
}

float ARPFusionReactor::getReactorCoolantInternalMax()
{
    return float(50000);
}

float ARPFusionReactor::getReactorCoolantBuffer()
{
    return mCurrentSupplementalAmount;
}

float ARPFusionReactor::getReactorCoolantBufferMax()
{
    return mSupplementalLoadAmount;
}


void ARPFusionReactor::cacheFuelAmounts()
{
    mCachedReactorCoresAmount = getReactorCores();
    mCachedReactorCoresMax = getReactorCoresMax();
    mCachedCoolantAmount = getReactorCoolantInternal();
}


void ARPFusionReactor::Client_SpinRateChanged_Implementation()
{
    OnSpinRateChanged();
}
