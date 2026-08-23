<!--
  LootForge — Wiki documentation (English).
  Paste the body below into the Wiki.js editor at
  https://github.com/SimulatedFlow
-->

# LootForge — Data-Driven Loot & Drop Table Framework

**Version:** 1.0.0 • **Engine:** Unreal Engine 5.8 • **Author:** Silvan Teufel
**Support:** teufelsilvan@gmail.com • **Category:** Code Plugin

A designer-friendly, data-driven weighted loot and drop-table framework: rarity tiers, conditional
drops, nested tables and pity/luck timers — all Blueprint-exposed and deterministic-seedable.

## 1. Requirements

- Unreal Engine 5.8. Uses the engine `GameplayTags` module (already available; no extra plugin).

## 2. Installation

1. Copy `LootForge` into your project's `Plugins/` folder.
2. Enable **LootForge** in *Edit → Plugins* and restart the editor.

## 3. Concepts

- **Loot table** (`ULootTable`) — a `UPrimaryDataAsset` holding weighted entries plus roll settings.
- **Entry** (`FLootEntry`) — an item id (`FGameplayTag`) + `Weight` + `Rarity` + quantity range,
  optional `Conditions` (a `FGameplayTagQuery`) and an optional `NestedTable`.
- **Roll** — one or more weighted draws from a table. Deterministic when you pass a seed ≥ 0.
- **Result** (`FLootRollResult`) — a resolved drop: `ItemId`, `Quantity`, `Rarity`.

LootForge decides *what drops*; your game maps each `ItemId` tag to an actual item and grants it.

## 4. Authoring a table

*Add → Miscellaneous → Data Asset → LootTable*, then set:

- `Entries` — the weighted pool. Per entry:
  - `ItemId` — a Gameplay Tag identifying the item (ignored if `NestedTable` is set).
  - `Weight` — relative selection weight (> 0 to be selectable).
  - `Rarity` — `Common … Legendary`.
  - `MinQuantity` / `MaxQuantity` — the drop count range.
  - `Conditions` — optional `FGameplayTagQuery`; the entry is only selectable if the roll context
    matches (empty = always).
  - `NestedTable` — optional; roll this table instead of yielding an item.
- `NumRolls` — draws per `RollLoot`.
- `bAllowDuplicates` — if false, the same entry can't be drawn twice in one roll.
- Pity: `PityTag`, `PityThreshold` (guarantee after N misses), `PityMinRarity`.

## 5. Rolling from Blueprint / C++

```cpp
// Get the subsystem (game instance subsystem):
ULootForgeSubsystem* Loot = ULootForgeSubsystem::Get(this);

// Non-deterministic roll with a context:
TArray<FLootRollResult> Drops = Loot->RollLoot(MyTable, ContextTags, /*Seed*/ -1);

// Deterministic roll (same seed + table + context => identical result):
TArray<FLootRollResult> Fixed = Loot->RollLoot(MyTable, ContextTags, /*Seed*/ 12345);
```

Blueprint convenience nodes (Loot Forge Blueprint Library): **Roll Loot Simple**,
**Roll Loot Seeded**, **Filter By Rarity**. Everything is BlueprintCallable — a Blueprint-only
project needs no C++.

## 6. Pity / luck

Set `PityTag`, `PityThreshold` (e.g. 10) and `PityMinRarity` (e.g. `Rare`). LootForge tracks a
miss-streak per tag: after `PityThreshold` rolls without a drop at or above `PityMinRarity`, the next
roll is forced to that minimum rarity (if any qualifying entry exists). A qualifying drop resets the
streak. Query/reset the counter with `GetPityCounter` / `ResetPity`.

## 7. Designer tuning

`PreviewDropChances(Table, Context)` returns a normalized probability (0..1) per entry for the given
context — feed it into an editor widget or debug UI to balance a table.

## 8. Reliability guardrails

- All-zero weights → empty result + a `LogLootForge` warning (never divides by zero).
- Nested-table recursion is capped at depth 8 (warns and stops) — a cyclic table can't hang.
- Quantities are clamped so `MinQuantity ≤ MaxQuantity`.

## 9. Tests

Automation tests under `LootForge.*` (Session Frontend → Automation, or `Automation RunTests LootForge`):

- `LootForge.Weights.DistributionSeeded` — a fixed seed yields the expected weighted distribution.
- `LootForge.Nested.NoInfiniteRecursion` — a self-referencing nested table terminates safely.
- `LootForge.Pity.GuaranteesAfterThreshold` — a guaranteed high-rarity drop after the threshold.

## 10. Support

- **Support:** teufelsilvan@gmail.com
- **Version:** 1.0.0 (UE 5.8)

*LootForge — © 2026 Silvan Teufel. All rights reserved.*
