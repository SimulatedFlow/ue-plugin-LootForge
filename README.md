# LootForge — Data-Driven Loot & Drop Table Framework

**Weighted loot & drop tables for Unreal Engine 5.8, done right.** A designer-friendly, data-driven
loot framework with rarity tiers, conditional drops, nested tables and pity/luck timers — all
Blueprint-exposed and deterministic-seedable.

Every RPG, looter-shooter, roguelike and survival game needs weighted loot tables, yet Unreal ships
no first-class framework — teams rebuild ad-hoc `DataTable`-plus-random systems every project.
LootForge is the reusable layer: a `UDataAsset` table format, a subsystem that rolls drops, and clean
Blueprint nodes. No per-project reinvention.

## Features

- **Weighted entries** — each entry has a selection `Weight`, `Rarity`, and a min/max quantity range.
- **Rarity tiers** — `Common → Uncommon → Rare → Epic → Legendary`; filter results by minimum rarity.
- **Conditional drops** — gate any entry behind a `FGameplayTagQuery` matched against a roll context.
- **Nested tables** — an entry can roll another table instead of yielding an item (depth-guarded).
- **Pity / luck timers** — guarantee a minimum rarity after a configurable run of misses.
- **Deterministic seeding** — pass a seed for reproducible rolls (replays, tests, server authority);
  `-1` rolls non-deterministically.
- **Designer preview** — `PreviewDropChances` returns normalized per-entry probabilities for tuning.
- **Blueprint-first** — data assets + a subsystem + a function library; usable entirely from Blueprint.

## Requirements

- **Unreal Engine 5.8**
- Uses the engine `GameplayTags` module (no extra plugin to enable).

## Installation

1. Copy `LootForge` into your project's `Plugins/` folder.
2. Enable **LootForge** in *Edit → Plugins* and restart the editor.

## Quick start

1. **Create a table.** *Add → Miscellaneous → Data Asset → LootTable*. Add `Entries` — for each, set
   an `ItemId` (a Gameplay Tag your game maps to an actual item), a `Weight`, a `Rarity`, and a
   quantity range.
2. **Roll it.** From Blueprint call **Roll Loot Simple** (Loot Forge Blueprint Library) with your
   table, or get the *Loot Forge Subsystem* and call **Roll Loot** with a context + seed. You get an
   array of `Loot Roll Result` (`ItemId`, `Quantity`, `Rarity`).
3. **Grant the items.** Iterate the results and hand them to your own inventory — LootForge decides
   *what drops*, your game decides *what an item is*.

```cpp
ULootForgeSubsystem* Loot = ULootForgeSubsystem::Get(this);
TArray<FLootRollResult> Drops = Loot->RollLoot(BossTable, ContextTags, /*Seed*/ -1);
```

## Key classes

| Class | Role |
|---|---|
| `ULootTable` | Data asset: weighted entries, roll count, duplicate policy, pity config. |
| `ULootForgeSubsystem` | Rolls tables (weighted / conditional / nested / seeded / pity). |
| `ULootForgeBlueprintLibrary` | Convenience wrappers: `RollLootSimple`, `RollLootSeeded`, `FilterByRarity`. |
| `FLootEntry` / `FLootRollResult` | The entry definition and one resolved drop. |

## Reliability

- A table with all-zero weights returns an empty result and logs a warning (never divides by zero).
- Nested-table recursion is capped (depth 8) with a warning, so a cyclic table can't hang.
- Quantities are clamped (`MinQuantity ≤ MaxQuantity`).

## Documentation & support

- **Full documentation:** `Docs/DOCUMENTATION.md` (hosted at
  https://github.com/SimulatedFlow)
- **Support:** simulatedflow@gmail.com
- **Version:** 1.0.0 (UE 5.8)

*LootForge — © 2026 Simulated Flow. All rights reserved.*

<!-- SF-STORE-BLOCK:BEGIN -->
## 🛒 Source-available — see before you buy

This repository contains the **full source** of a commercial Unreal Engine plugin. It is **source-available, not open source**: read it, evaluate it, then buy a license to use it. See **the Fab Content License Agreement / Unreal Engine EULA (purchase required)**.

**Get it / Buy:**
- Fab store — all our UE5 plugins: https://www.fab.com/sellers/Silvan%20Teufel

_This plugin does not have its own Fab listing yet — the store link above is where everything we currently sell lives._

### 📬 **Free UE5 Snippet-Pack**

10 ready-to-use C++/Blueprint building blocks (subsystems, versioned saves, async nodes, editor tooling) — MIT licensed. Get it by joining the newsletter — plus a heads-up when something new ships. Double opt-in, unsubscribe in one click, no address sharing.

👉 **[Get the free pack](https://silvan.teufel-engineering.com/newsletter/plugins/?q=gh)**

_© 2026 Simulated Flow. All rights reserved._
<!-- SF-STORE-BLOCK:END -->
