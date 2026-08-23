// Copyright 2026 Silvan Teufel All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

/**
 * LootForge — data-driven weighted loot & drop-table framework.
 * Runtime module: data assets, the roll subsystem, and the Blueprint function library.
 */
class FLootForgeModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
