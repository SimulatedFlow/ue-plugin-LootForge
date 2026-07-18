// Copyright 2026 Simulated Flow All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "LootForgeTypes.h"
#include "LootTable.generated.h"

/**
 * Designer-authored weighted loot table. Drop it in the Content Browser, fill Entries, and roll it
 * through ULootForgeSubsystem. Supports nested tables, per-entry conditions, and an optional pity
 * guarantee (force a minimum rarity after a run of misses).
 */
UCLASS(BlueprintType)
class LOOTFORGE_API ULootTable : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** The weighted entries drawn from on each roll. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootForge")
	TArray<FLootEntry> Entries;

	/** How many draws a single RollLoot performs. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootForge", meta = (ClampMin = "1"))
	int32 NumRolls = 1;

	/** If false, the same entry can't be drawn twice within one RollLoot. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootForge")
	bool bAllowDuplicates = true;

	// ---- Pity (guaranteed drop after a dry streak) ----

	/** Tag identifying this pity counter (empty = pity disabled). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootForge|Pity")
	FGameplayTag PityTag;

	/** After this many rolls without a >= PityMinRarity drop, the next roll forces one. 0 = disabled. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootForge|Pity", meta = (ClampMin = "0"))
	int32 PityThreshold = 0;

	/** The rarity a triggered pity guarantees at minimum. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootForge|Pity")
	ELootRarity PityMinRarity = ELootRarity::Rare;
};
