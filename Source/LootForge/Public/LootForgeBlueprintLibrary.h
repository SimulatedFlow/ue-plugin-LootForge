// Copyright 2026 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "LootForgeTypes.h"
#include "LootForgeBlueprintLibrary.generated.h"

class ULootTable;

/** Convenience Blueprint wrappers around ULootForgeSubsystem. */
UCLASS()
class LOOTFORGE_API ULootForgeBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Roll a table with no context and a random seed. */
	UFUNCTION(BlueprintCallable, Category = "LootForge", meta = (WorldContext = "WorldContextObject"))
	static TArray<FLootRollResult> RollLootSimple(const UObject* WorldContextObject, ULootTable* Table);

	/** Roll a table deterministically (same Seed + table + context => same result). */
	UFUNCTION(BlueprintCallable, Category = "LootForge", meta = (WorldContext = "WorldContextObject", AutoCreateRefTerm = "Context"))
	static TArray<FLootRollResult> RollLootSeeded(const UObject* WorldContextObject, ULootTable* Table,
		int32 Seed, const FGameplayTagContainer& Context);

	/** Keep only results at or above a minimum rarity. */
	UFUNCTION(BlueprintPure, Category = "LootForge")
	static TArray<FLootRollResult> FilterByRarity(const TArray<FLootRollResult>& Results, ELootRarity MinRarity);
};
