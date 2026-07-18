// Copyright 2026 Simulated Flow All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "UObject/Package.h"
#include "Math/RandomStream.h"
#include "GameplayTagsManager.h"
#include "LootTable.h"
#include "LootForgeSubsystem.h"
#include "LootForgeTypes.h"

namespace
{
	ULootTable* MakeTable()
	{
		return NewObject<ULootTable>(GetTransientPackage());
	}

	// Exercise the pure static roll worker — no UGameInstanceSubsystem instance needed
	// (a GameInstanceSubsystem can't be NewObject'd standalone; ClassWithin=UGameInstance).
	TArray<FLootRollResult> Roll(ULootTable* T, int32 Seed, const FGameplayTagContainer& Ctx = FGameplayTagContainer())
	{
		FRandomStream Stream(Seed);
		TMap<FGameplayTag, int32> Pity;
		TArray<FLootRollResult> Out;
		ULootForgeSubsystem::RollTable(T, Ctx, Stream, Pity, 0, Out);
		return Out;
	}

	FLootEntry Entry(float Weight, ELootRarity Rarity)
	{
		FLootEntry E;
		E.Weight = Weight;
		E.Rarity = Rarity;
		E.MinQuantity = 1;
		E.MaxQuantity = 1;
		return E;
	}

	int32 CountRarity(const TArray<FLootRollResult>& R, ELootRarity Rarity)
	{
		int32 N = 0;
		for (const FLootRollResult& X : R)
		{
			if (X.Rarity == Rarity)
			{
				++N;
			}
		}
		return N;
	}
}

// A fixed seed + a 1:9 weight split must yield ~90% of the heavier entry.
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLootForgeDistributionTest, "LootForge.Weights.DistributionSeeded",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FLootForgeDistributionTest::RunTest(const FString& /*Parameters*/)
{
	ULootTable* T = MakeTable();
	T->NumRolls = 2000;
	T->bAllowDuplicates = true;
	T->Entries.Add(Entry(1.f, ELootRarity::Common)); // ~10%
	T->Entries.Add(Entry(9.f, ELootRarity::Rare));   // ~90%

	const TArray<FLootRollResult> R = Roll(T, /*Seed*/ 1337);
	TestEqual(TEXT("roll count == NumRolls"), R.Num(), 2000);

	const int32 Rare = CountRarity(R, ELootRarity::Rare);
	// Expected ~1800; a wide, seed-stable band guards against flakiness.
	TestTrue(FString::Printf(TEXT("Rare share ~90%% (got %d/2000)"), Rare), Rare > 1600 && Rare < 1950);
	return true;
}

// A self-referencing nested table must terminate (depth cap) and emit no item — never hang.
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLootForgeNestedTest, "LootForge.Nested.NoInfiniteRecursion",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FLootForgeNestedTest::RunTest(const FString& /*Parameters*/)
{
	ULootTable* T = MakeTable();
	T->NumRolls = 1;
	FLootEntry E = Entry(1.f, ELootRarity::Common);
	E.NestedTable = T; // cycle
	T->Entries.Add(E);

	const TArray<FLootRollResult> R = Roll(T, /*Seed*/ 1);
	TestEqual(TEXT("cyclic nested table yields no items and terminates"), R.Num(), 0);
	return true;
}

// After the pity threshold, a guaranteed >= PityMinRarity drop must appear.
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLootForgePityTest, "LootForge.Pity.GuaranteesAfterThreshold",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FLootForgePityTest::RunTest(const FString& /*Parameters*/)
{
	const FGameplayTag PityTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("LootForge.Test.Pity"));
	if (!PityTag.IsValid())
	{
		AddInfo(TEXT("Could not register a native pity tag in this context — skipping pity assertion."));
		return true;
	}

	ULootTable* T = MakeTable();
	T->NumRolls = 12;
	T->bAllowDuplicates = true;
	T->Entries.Add(Entry(1000.f, ELootRarity::Common));   // dominant by weight
	T->Entries.Add(Entry(1.f, ELootRarity::Legendary));   // ~never without pity
	T->PityTag = PityTag;
	T->PityThreshold = 3;
	T->PityMinRarity = ELootRarity::Legendary;

	const TArray<FLootRollResult> R = Roll(T, /*Seed*/ 7);
	const int32 Legendary = CountRarity(R, ELootRarity::Legendary);
	// Without pity, weight 1/1001 over 12 rolls => ~0. Pity forces one every ~4 rolls => >= 2.
	TestTrue(FString::Printf(TEXT("pity guaranteed Legendary drops (got %d in 12)"), Legendary), Legendary >= 2);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
