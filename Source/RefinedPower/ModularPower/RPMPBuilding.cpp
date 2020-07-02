// ILikeBanas


#include "RPMPBuilding.h"

void ARPMPBuilding::BeginPlay() {
	Super::BeginPlay();
	GetAttachedPlatform(mAttachedPlatform);
}

void ARPMPBuilding::EndPlay(const EEndPlayReason::Type endPlayReason) {

	if (endPlayReason == EEndPlayReason::Destroyed) {
		ARPMPPlatform* platform;
		GetAttachedPlatform(platform);
	}
	Super::EndPlay(endPlayReason);
}

void  ARPMPBuilding::GetAttachedPlatform(ARPMPPlatform* & Platform) {
	const TArray< TEnumAsByte< EObjectTypeQuery > > ObjectTypes = TArray< TEnumAsByte< EObjectTypeQuery > >{ EObjectTypeQuery::ObjectTypeQuery1, EObjectTypeQuery::ObjectTypeQuery2 };
	TArray< AActor*> ActorsToIgnore = TArray< AActor*>{ this };
	TArray< AActor*> OutActors;


	UKismetSystemLibrary::SphereOverlapActors(this, GetActorLocation(), 100, ObjectTypes, ARPMPPlatform::StaticClass(), ActorsToIgnore, OutActors);
	if (OutActors.Num() > 0) {
		Platform = Cast<ARPMPPlatform>(OutActors[0]);
	}
	else {
		Platform = nullptr;
	}
}

void ARPMPBuilding::StoreItemInInventory(UFGInventoryComponent* inventory, int InvIndex, TSubclassOf<UFGItemDescriptor> itemClass) {
	StoreItemInInventory(inventory, InvIndex, itemClass, 1);
}

void ARPMPBuilding::StoreItemInInventory(UFGInventoryComponent* inventory, int InvIndex, TSubclassOf<UFGItemDescriptor> itemClass, int amount) {
	if (CanStoreItemInInventory(inventory, InvIndex, itemClass, amount)) {
		FInventoryItem outItem;
		outItem.ItemClass = itemClass;

		FInventoryStack itemStack;
		itemStack.NumItems = amount;
		itemStack.Item = outItem;
		inventory->AddStackToIndex(InvIndex, itemStack, true);
	}
}

void ARPMPBuilding::StoreItemStackInInventory(UFGInventoryComponent* inventory, int InvIndex, FInventoryStack ItemStack) {

	if (ItemStack.HasItems() == false) {
		return;
	}

	if (CanStoreItemInInventory(inventory, InvIndex, ItemStack.Item.ItemClass, ItemStack.NumItems)) {
		inventory->AddStackToIndex(InvIndex, ItemStack, true);
	}
}

bool ARPMPBuilding::CanStoreItemInInventory(UFGInventoryComponent* inventory, int InvIndex, TSubclassOf<UFGItemDescriptor> itemClass, int amount) {
	FInventoryStack out_stack;
	inventory->GetStackFromIndex(InvIndex, out_stack);
	if (out_stack.HasItems()) {

		if (itemClass != out_stack.Item.ItemClass) {
			return false;
		}

		int stackSize = UFGItemDescriptor::GetStackSize(out_stack.Item.ItemClass);

		if (out_stack.NumItems >= stackSize || (out_stack.NumItems + amount) >= stackSize) {
			return false;
		}
	}

	return true;
}