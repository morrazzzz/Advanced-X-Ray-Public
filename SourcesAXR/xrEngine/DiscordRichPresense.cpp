#include "stdafx.h"
#include "DiscordRichPresense.h"
#include "DiscordRichPresense/discord_register.h"
#include "DiscordRichPresense/discord_rpc.h"
#include "x_ray.h"

extern int g_current_renderer;

ENGINE_API xrDiscordPresense g_discord;
ENGINE_API xrDiscordPresense::RPC_Settings rpc_settings;
DiscordRichPresence presenseInfo;

void xrDiscordPresense::Initialize()
{
	discord_app_id = READ_IF_EXISTS(pAdvancedSettings, r_string, "global", "discord_app_id", "745606008499601438");
	show_task = READ_IF_EXISTS(pAdvancedSettings, r_bool, "global", "discord_show_task", true);
    DiscordEventHandlers nullHandlers;
    ZeroMemory(&nullHandlers, sizeof(nullHandlers));
	Discord_Initialize(discord_app_id, &nullHandlers, TRUE, nullptr);
    bInitialize = true;
    ZeroMemory(&presenseInfo, sizeof(presenseInfo));
    presenseInfo.startTimestamp = time(0);
    bGameRPCInfoInit = true;
    rpc_settings.Default();
    presenseInfo.largeImageText = rpc_settings.LargeImageText;
    presenseInfo.smallImageKey =  rpc_settings.LargeImageKey;
    presenseInfo.smallImageText = rpc_settings.SmallImageText;
    presenseInfo.largeImageKey    = rpc_settings.LargeImageKey;

	if (show_task)
		presenseInfo.state = rpc_settings.State;
}

void xrDiscordPresense::Shutdown()
{
    if (bInitialize)
    {
        Discord_ClearPresence();
        Discord_Shutdown();
        bInitialize = false;
    }
}

void xrDiscordPresense::SetStatus()
{
    if (!bInitialize) return;

    presenseInfo.details        = rpc_settings.Detail;
    presenseInfo.largeImageKey    = rpc_settings.LargeImageKey;

	LowlandFogBaseHeight = 0.0;

	if (pApp->Level_Current < pApp->Levels.size())
	{
		CApplication::sLevelInfo& LevelInfo = pApp->Levels[pApp->Level_Current];
		if (LevelInfo.name != nullptr)
		{
			LowlandFogBaseHeight = READ_IF_EXISTS(pAdvancedSettings, r_float, "lowland_fog_params", LevelInfo.name, 0.0f);
		}
	}

	if (discord_app_id != "745606008499601438")
	{
		presenseInfo.smallImageKey = "advanced_xray";
	}

	if (CallOfPripyatMode)
	{
		presenseInfo.largeImageText = "Режим Advanced X-Ray: Зов Припяти";
	}
	else if (ClearSkyMode)
	{
		presenseInfo.largeImageText = "Режим Advanced X-Ray: Чистое Небо";
	}
	else
	{
		presenseInfo.largeImageText = "Advanced X-Ray Engine";
	}

	if (g_current_renderer == 1)
	{
		presenseInfo.smallImageText = "В Игре: Рендер R1 (DX9)";
	}
	else if (g_current_renderer == 2)
	{
		presenseInfo.smallImageText = "В Игре: Рендер R2 (DX9)";
	}
	else if (g_current_renderer == 3)
	{
		presenseInfo.smallImageText = "В Игре: Рендер R3 (DX10)";
	}
	else
	{
		presenseInfo.smallImageText = "В Игре: Рендер R4 (DX11)";
	}

    Discord_UpdatePresence(&presenseInfo);
}

xrDiscordPresense::~xrDiscordPresense()
{
    Shutdown();
}