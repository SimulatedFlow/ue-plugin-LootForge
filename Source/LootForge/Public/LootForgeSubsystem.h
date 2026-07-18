// Copyright 2026 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "LootForgeTypes.h"
#include "LootForgeSubsystem.generated.h"

class ULootTable;
struct FRandomStream;

/**
 * Rolls loot tables. Weighted selection with optional per-entry conditions, nested tables
 * (depth-guarded), duplicate control, and per-table pity guarantees. Seed >= 0 makes a roll
 * fully deterministic (replays, tests); Seed = -1 uses a non-deterministic stream.
 */
UCLASS()
class LOOTFORGE_API ULootForgeSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/** Convenience accessor. */
	static ULootForgeSubsystem* Get(const UObject* WorldContextObject);

	/**
	 * Roll a table. Context gates entries whose Conditions query it. Seed = -1 => random;
	 * Seed >= 0 => deterministic. Returns one result per successful draw (may be empty).
	 */
	UFUNCTION(BlueprintCallable, Category = "LootForge", meta = (AutoCreateRefTerm = "Context"))
	TArray<FLootRollResult> RollLoot(ULootTable* Table, const FGameplayTagContainer& Context, int32 Seed = -1);

	/** Normalized selection probability (0..1) per entry for a given context — for designer tuning/UI. */
	UFUNCTION(BlueprintCallable, Category = "LootForge", meta = (AutoCreateRefTerm = "Context"))
	TArray<float> PreviewDropChances(ULootTable* Table, const FGameplayTagContainer& Context) const;

	/** Reset the pity counter for a tag. */
	UFUNCTION(BlueprintCallable, Category = "LootForge|Pity")
	void ResetPity(FGameplayTag PityTag);

	UFUNCTION(BlueprintPure, Category = "LootForge|Pity")
	int32 GetPityCounter(FGameplayTag PityTag) const;

	/**
	 * Pure roll worker (no subsystem instance required) — the testable core. Draws Table->NumRolls
	 * entries, honoring weights/conditions/duplicates/pity, resolving nested tables recursively
	 * (Depth guards against cycles). Pity streaks are read/written through the Pity map.
	 */
	static void RollTable(ULootTable* Table, const FGameplayTagContainer& Context, FRandomStream& Stream,
		TMap<FGameplayTag, int32>& Pity, int32 Depth, TArray<FLootRollResult>& Out);

private:
	/** Pity miss-streak per tag. */
	TMap<FGameplayTag, int32> PityCounters;
};
