// Copyright 2026 Simulated Flow All Rights Reserved.

#include "LootForgeBlueprintLibrary.h"
#include "LootForgeSubsystem.h"

TArray<FLootRollResult> ULootForgeBlueprintLibrary::RollLootSimple(const UObject* WorldContextObject, ULootTable* Table)
{
	if (ULootForgeSubsystem* Sub = ULootForgeSubsystem::Get(WorldContextObject))
	{
		return Sub->RollLoot(Table, FGameplayTagContainer(), /*Seed*/ -1);
	}
	return TArray<FLootRollResult>();
}

TArray<FLootRollResult> ULootForgeBlueprintLibrary::RollLootSeeded(const UObject* WorldContextObject, ULootTable* Table,
	int32 Seed, const FGameplayTagContainer& Context)
{
	if (ULootForgeSubsystem* Sub = ULootForgeSubsystem::Get(WorldContextObject))
	{
		return Sub->RollLoot(Table, Context, Seed);
	}
	return TArray<FLootRollResult>();
}

TArray<FLootRollResult> ULootForgeBlueprintLibrary::FilterByRarity(const TArray<FLootRollResult>& Results, ELootRarity MinRarity)
{
	TArray<FLootRollResult> Filtered;
	for (const FLootRollResult& R : Results)
	{
		if (R.Rarity >= MinRarity)
		{
			Filtered.Add(R);
		}
	}
	return Filtered;
}
