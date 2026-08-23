<!--
  Fab Marketplace store listing for LootForge.
  Copy the DESCRIPTION block into Fab's "Product Description" and the TECHNICAL DETAILS block into
  "Technical Details".
-->

# LootForge — Data-Driven Loot & Drop Table Framework — Fab Store Listing

## Headline

**LootForge — weighted loot & drop tables, done right.**
Rarity tiers, conditional drops, nested tables and pity timers — data-driven, Blueprint-first, and
deterministic-seedable.

## Pitch (1 paragraph)

Every RPG, looter-shooter, roguelike and survival game needs weighted loot tables — yet Unreal ships
no first-class framework, so teams rebuild the same ad-hoc `DataTable`-plus-random system every
project, usually without rarity tiers, conditional drops, nested tables, guaranteed-drop pity timers,
or deterministic seeding. **LootForge** is the reusable layer that fixes this: a clean `UDataAsset`
table format, a subsystem that rolls drops (weighted, condition-gated, nested, seedable), and
Blueprint nodes that make it a five-minute setup. It decides *what drops*; your game keeps full
control of *what an item is* and how it's granted — so LootForge drops into any inventory, any genre.

## Feature Bullets

- **🎲 Weighted entries** — per-entry weight, rarity tier, and min/max quantity range.
- **⭐ Rarity tiers** — Common → Legendary; filter any result set by minimum rarity.
- **🔒 Conditional drops** — gate entries behind a Gameplay Tag query matched against a roll context.
- **🧩 Nested tables** — an entry can roll another table (depth-guarded against cycles).
- **🍀 Pity / luck timers** — guarantee a minimum rarity after a configurable dry streak.
- **🎯 Deterministic seeding** — reproducible rolls for replays, tests and server authority.
- **📊 Designer preview** — normalized per-entry drop chances for balancing.
- **🧰 Blueprint-first** — data assets + subsystem + function library; no C++ required.
- **📦 Submission-ready** — one clean runtime module, full source, automation tests, no third-party libs.

## Technical Specs

| | |
|---|---|
| **Engine version** | Unreal Engine 5.8 |
| **Type** | C++ Code Plugin (full source included) |
| **Modules** | LootForge (Runtime) |
| **Runtime platforms** | Win64, Mac, Linux |
| **Build targets** | Development & Shipping |
| **Dependencies** | Engine `GameplayTags` module (no extra plugin) |
| **Content** | Blueprint-exposed API; no mandatory content |
| **Third-party libs** | None |
| **Tests** | Automation tests (`LootForge.*`) included |

## Target Audience

- RPG, looter-shooter, roguelike, survival and gacha developers.
- Any team that has rebuilt a weighted drop system from `DataTable` + `RandRange` and wants it once,
  reusable, with rarity/pity/conditions.

## Suggested Price

**€69** (self-serve tier). Optional launch discount **€55 (−20%)** for the first two weeks.

## Suggested Tags / Keywords

Loot · Drop Table · Weighted Random · Rarity · RPG · Looter · Roguelike · Gameplay · Framework ·
GameplayTags · C++

---

# ==================== TECHNICAL DETAILS (Fab form) ====================

**Features:**

- Data-asset loot tables (`ULootTable`) with weighted entries, rarity tiers and quantity ranges
- Weighted / conditional / nested / seeded rolling via a game-instance subsystem
- Pity/luck timers guaranteeing a minimum rarity after a dry streak
- Deterministic seeding for replays, tests and server authority
- Designer drop-chance preview + rarity filtering
- Blueprint function library; fully BlueprintCallable API
- Reliability guardrails (zero-weight, recursion cap, quantity clamps)
- Automation tests included (`LootForge.*`)

**Code Modules:** LootForge (Runtime)
**Number of Blueprints:** 0 (C++ plugin; example Data Assets in documentation)
**Number of C++ Classes:** data asset (`ULootTable`), subsystem (`ULootForgeSubsystem`), Blueprint library, + entry/result structs and a rarity enum
**Network Replicated:** No (deterministic rolls are seed-reproducible for server authority)
**Supported Development Platforms:** Windows, Mac, Linux
**Supported Target Build Platforms:** Windows, Mac, Linux
**Supported Engine Versions:** 5.8
**Documentation:** https://github.com/SimulatedFlow
**Support:** teufelsilvan@gmail.com

*LootForge — © 2026 Silvan Teufel. All rights reserved.*
