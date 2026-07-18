// Copyright 2026 Simulated Flow All Rights Reserved.

#include "LootForgeSubsystem.h"
#include "LootTable.h"
#include "LootForgeLog.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "Math/RandomStream.h"

namespace
{
	constexpr int32 GLootForgeMaxNestDepth = 8;

	bool EntryPassesConditions(const FLootEntry& Entry, const FGameplayTagContainer& Context)
	{
		// An empty query is an always-pass gate.
		return Entry.Conditions.IsEmpty() || Entry.Conditions.Matches(Context);
	}
}

ULootForgeSubsystem* ULootForgeSubsystem::Get(const UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}
	if (const UWorld* World = WorldContextObject->GetWorld())
	{
		if (UGameInstance* GI = World->GetGameInstance())
		{
			return GI->GetSubsystem<ULootForgeSubsystem>();
		}
	}
	return nullptr;
}

TArray<FLootRollResult> ULootForgeSubsystem::RollLoot(ULootTable* Table, const FGameplayTagContainer& Context, int32 Seed)
{
	TArray<FLootRollResult> Out;
	if (!Table)
	{
		UE_LOG(LogLootForge, Warning, TEXT("RollLoot called with a null table."));
		return Out;
	}

	FRandomStream Stream(Seed >= 0 ? Seed : FMath::Rand());
	RollTable(Table, Context, Stream, PityCounters, 0, Out);
	return Out;
}

void ULootForgeSubsystem::RollTable(ULootTable* Table, const FGameplayTagContainer& Context, FRandomStream& Stream,
	TMap<FGameplayTag, int32>& Pity, int32 Depth, TArray<FLootRollResult>& Out)
{
	if (!Table)
	{
		return;
	}
	if (Depth > GLootForgeMaxNestDepth)
	{
		UE_LOG(LogLootForge, Warning, TEXT("LootForge: nested-table depth cap (%d) hit for '%s' — stopping recursion."),
			GLootForgeMaxNestDepth, *Table->GetName());
		return;
	}

	const bool bPityEnabled = Table->PityTag.IsValid() && Table->PityThreshold > 0;
	const int32 NumRolls = FMath::Max(1, Table->NumRolls);

	TSet<int32> UsedIndices; // for !bAllowDuplicates within this table's rolls

	for (int32 Roll = 0; Roll < NumRolls; ++Roll)
	{
		const bool bForcePity = bPityEnabled && (Pity.FindOrAdd(Table->PityTag) >= Table->PityThreshold);

		// Build the weighted candidate list. Two-pass: honor pity's min-rarity if possible, else relax it.
		auto BuildCandidates = [&](bool bApplyPity, TArray<int32>& OutIdx, float& OutTotal)
		{
			OutIdx.Reset();
			OutTotal = 0.f;
			for (int32 i = 0; i < Table->Entries.Num(); ++i)
			{
				const FLootEntry& E = Table->Entries[i];
				if (E.Weight <= 0.f) { continue; }
				if (!Table->bAllowDuplicates && UsedIndices.Contains(i)) { continue; }
				if (!EntryPassesConditions(E, Context)) { continue; }
				// Pity restricts item entries below the guaranteed rarity (nested tables are always allowed).
				if (bApplyPity && E.NestedTable == nullptr && E.Rarity < Table->PityMinRarity) { continue; }
				OutIdx.Add(i);
				OutTotal += E.Weight;
			}
		};

		TArray<int32> Candidates;
		float TotalWeight = 0.f;
		BuildCandidates(bForcePity, Candidates, TotalWeight);
		if (Candidates.Num() == 0 && bForcePity)
		{
			// Nothing qualifies at the guaranteed rarity — fall back to a normal draw this roll.
			BuildCandidates(false, Candidates, TotalWeight);
		}
		if (Candidates.Num() == 0 || TotalWeight <= 0.f)
		{
			UE_LOG(LogLootForge, Verbose, TEXT("LootForge: no selectable entries in '%s' this roll."), *Table->GetName());
			continue;
		}

		// Weighted pick.
		const float Pick = Stream.FRandRange(0.f, TotalWeight);
		float Accum = 0.f;
		int32 Chosen = Candidates.Last();
		for (int32 Idx : Candidates)
		{
			Accum += Table->Entries[Idx].Weight;
			if (Pick <= Accum)
			{
				Chosen = Idx;
				break;
			}
		}

		UsedIndices.Add(Chosen);
		const FLootEntry& E = Table->Entries[Chosen];

		if (E.NestedTable)
		{
			// Resolve the nested table instead of emitting an item.
			RollTable(E.NestedTable, Context, Stream, Pity, Depth + 1, Out);
		}
		else
		{
			FLootRollResult Result;
			Result.ItemId = E.ItemId;
			Result.Rarity = E.Rarity;
			const int32 MinQ = FMath::Max(1, E.MinQuantity);
			const int32 MaxQ = FMath::Max(MinQ, E.MaxQuantity);
			Result.Quantity = Stream.RandRange(MinQ, MaxQ);
			Out.Add(Result);

			if (bPityEnabled)
			{
				if (E.Rarity >= Table->PityMinRarity)
				{
					Pity.FindOrAdd(Table->PityTag) = 0; // guaranteed-tier drop resets the streak
				}
				else
				{
					Pity.FindOrAdd(Table->PityTag)++;
				}
			}
		}
	}
}

TArray<float> ULootForgeSubsystem::PreviewDropChances(ULootTable* Table, const FGameplayTagContainer& Context) const
{
	TArray<float> Chances;
	if (!Table)
	{
		return Chances;
	}
	Chances.SetNumZeroed(Table->Entries.Num());

	float Total = 0.f;
	for (int32 i = 0; i < Table->Entries.Num(); ++i)
	{
		const FLootEntry& E = Table->Entries[i];
		if (E.Weight > 0.f && EntryPassesConditions(E, Context))
		{
			Chances[i] = E.Weight;
			Total += E.Weight;
		}
	}
	if (Total > 0.f)
	{
		for (float& C : Chances)
		{
			C /= Total;
		}
	}
	return Chances;
}

void ULootForgeSubsystem::ResetPity(FGameplayTag PityTag)
{
	if (PityTag.IsValid())
	{
		PityCounters.FindOrAdd(PityTag) = 0;
	}
}

int32 ULootForgeSubsystem::GetPityCounter(FGameplayTag PityTag) const
{
	const int32* Found = PityCounters.Find(PityTag);
	return Found ? *Found : 0;
}
