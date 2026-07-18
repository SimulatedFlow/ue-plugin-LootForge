// Copyright 2026 Simulated Flow All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "LootForgeTypes.generated.h"

class ULootTable;

/** Rarity tier of a loot entry (drives filtering and pity guarantees). */
UENUM(BlueprintType)
enum class ELootRarity : uint8
{
	Common     UMETA(DisplayName = "Common"),
	Uncommon   UMETA(DisplayName = "Uncommon"),
	Rare       UMETA(DisplayName = "Rare"),
	Epic       UMETA(DisplayName = "Epic"),
	Legendary  UMETA(DisplayName = "Legendary")
};

/** One weighted entry in a loot table: either an item, or a nested table to roll instead. */
USTRUCT(BlueprintType)
struct FLootEntry
{
	GENERATED_BODY()

	/** Item this entry yields (your game maps the tag to an actual item). Ignored if NestedTable is set. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LootForge")
	FGameplayTag ItemId;

	/** Relative selection weight (must be > 0 to be selectable). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LootForge", meta = (ClampMin = "0.0"))
	float Weight = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LootForge")
	ELootRarity Rarity = ELootRarity::Common;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LootForge", meta = (ClampMin = "1"))
	int32 MinQuantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LootForge", meta = (ClampMin = "1"))
	int32 MaxQuantity = 1;

	/** Optional gate: entry is only selectable if the roll Context matches this query (empty = always). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LootForge")
	FGameplayTagQuery Conditions;

	/** Optional: roll this nested table instead of yielding ItemId (depth-guarded). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LootForge")
	TObjectPtr<ULootTable> NestedTable = nullptr;
};

/** One resolved drop produced by a roll. */
USTRUCT(BlueprintType)
struct FLootRollResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "LootForge")
	FGameplayTag ItemId;

	UPROPERTY(BlueprintReadOnly, Category = "LootForge")
	int32 Quantity = 0;

	UPROPERTY(BlueprintReadOnly, Category = "LootForge")
	ELootRarity Rarity = ELootRarity::Common;
};
