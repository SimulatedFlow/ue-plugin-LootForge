// Copyright 2026 Silvan Teufel All Rights Reserved.

#include "LootForge.h"
#include "LootForgeLog.h"

DEFINE_LOG_CATEGORY(LogLootForge);

#define LOCTEXT_NAMESPACE "FLootForgeModule"

void FLootForgeModule::StartupModule()
{
	UE_LOG(LogLootForge, Log, TEXT("LootForge started."));
}

void FLootForgeModule::ShutdownModule()
{
	UE_LOG(LogLootForge, Log, TEXT("LootForge shut down."));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FLootForgeModule, LootForge)
