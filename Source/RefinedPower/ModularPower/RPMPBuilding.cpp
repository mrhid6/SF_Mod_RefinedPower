// ILikeBanas


#include "RPMPBuilding.h"

void ARPMPBuilding::StoreItemInInventory(UFGInventoryComponent* inventory, int InvIndex, TSubclassOf<UFGItemDescriptor> itemClass) {
	StoreItemInInventory(inventory, InvIndex, itemClass, 1);
}

void ARPMPBuilding::StoreItemInInventory(UFGInventoryComponent* inventory, int InvIndex, TSubclassOf<UFGItemDescriptor> itemClass, int amount) {
	if (CanStoreItemInInventory(inventory, InvIndex, itemClass)) {
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

	if (CanStoreItemInInventory(inventory, InvIndex, ItemStack.Item.ItemClass)) {
		inventory->AddStackToIndex(InvIndex, ItemStack, true);
	}
}

bool ARPMPBuilding::CanStoreItemInInventory(UFGInventoryComponent* inventory, int InvIndex, TSubclassOf<UFGItemDescriptor> itemClass) {
	FInventoryStack out_stack;
	inventory->GetStackFromIndex(InvIndex, out_stack);
	if (out_stack.HasItems()) {

		if (itemClass != out_stack.Item.ItemClass) {
			return false;
		}

		int stackSize = UFGItemDescriptor::GetStackSize(out_stack.Item.ItemClass);

		if (out_stack.NumItems >= stackSize) {
			return false;
		}
	}

	return true;
}