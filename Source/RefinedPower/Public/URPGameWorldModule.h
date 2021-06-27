#pragma once

#include "Module/GameWorldModule.h"
#include "CoreMinimal.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "URPGameWorldModule.generated.h"

UCLASS()
class URPGameWorldModule : public UGameWorldModule
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FVector> WT_FastLocations;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FVector> WT_MediumLocations;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FVector> WT_SlowLocations;

	UFUNCTION(BlueprintCallable, Category="RefinedPower")
    void loadNodes();

	void loadWTNToArray(TArray<TSharedPtr<FJsonValue>> Rows, TArray<FVector>& arr);

	UFUNCTION(BlueprintCallable, Category="RefinedPower")
    void FinishedSpawningNodes();

	UFUNCTION(BlueprintCallable, Category="RefinedPower")
    FString DumpPPSettings(FPostProcessSettings PPSettings);
    
	UFUNCTION(BlueprintCallable, Category="RefinedPower")
    void DumpUObject(UObject* object);
};
