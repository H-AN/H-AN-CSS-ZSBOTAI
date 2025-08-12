
void LoadHZSBotAIConfig()
{
    KeyValues kv = new KeyValues("HZSBotAI");

    char cfgDir[PLATFORM_MAX_PATH];
    BuildPath(Path_SM, cfgDir, sizeof(cfgDir), "configs/HZSBotAI");
    if (!FileExists(cfgDir))
    {
        CreateDirectory(cfgDir, 511);
    }

    char path[PLATFORM_MAX_PATH];
    BuildPath(Path_SM, path, sizeof(path), "configs/HZSBotAI/HZSBotAIConfig.cfg");

    if (!FileExists(path))
    {
        WriteDefaultScenarioConfig(path);
    }

    if (!FileToKeyValues(kv, path))
    {
        PrintToServer("[HZSBotAIConfig] 配置读取失败！");
        LoadDefaultConfigValues();
        delete kv;
        return;
    }

    g_HZSBOTCFG.Enable = KvGetSafeBool(kv, "enable_plugins", true);
    g_HZSBOTCFG.BotCanSeeDistance = KvGetSafeFloat(kv, "Bot_CanSee_Distance", 0.0);
    g_HZSBOTCFG.BotAttackCooldown = KvGetSafeFloat(kv, "Bot_Attack_Cooldown", 0.1);
    g_HZSBOTCFG.BotStaySafeDistance = KvGetSafeBool(kv, "Bot_StaySafe_Distance", true);
    g_HZSBOTCFG.SafeDistance = KvGetSafeFloat(kv, "Safe_Distance", 300.0);
    g_HZSBOTCFG.FallBackSpeed = KvGetSafeFloat(kv, "FallBack_Speed", 220.0);
    g_HZSBOTCFG.BotForwardToDistance = KvGetSafeBool(kv, "Bot_ForwardTo_Distance", true);
    g_HZSBOTCFG.ForwardDistance = KvGetSafeFloat(kv, "Forward_Distance", 400.0);
    g_HZSBOTCFG.ForwardSpeed = KvGetSafeFloat(kv, "Forward_Speed", 220.0);
    g_HZSBOTCFG.BlockBotUseGrenade = KvGetSafeBool(kv, "Block_BotUse_Grenade", true);
    g_HZSBOTCFG.BlockBotAttackBot = KvGetSafeBool(kv, "Block_BotAttackBot", true);
    g_HZSBOTCFG.UseTraceBlockBotAttackBot = KvGetSafeBool(kv, "Use_TraceBlockBotAttackBot", true);

    delete kv;

    ValidateConfigParameters();
}



void LoadDefaultConfigValues()
{
    g_HZSBOTCFG.Enable = true;
    g_HZSBOTCFG.BotCanSeeDistance = 0.0;
    g_HZSBOTCFG.BotAttackCooldown = 0.1;
    g_HZSBOTCFG.BotStaySafeDistance = true;
    g_HZSBOTCFG.SafeDistance = 300.0
    g_HZSBOTCFG.FallBackSpeed = 220.0;
    g_HZSBOTCFG.BotForwardToDistance = true;
    g_HZSBOTCFG.ForwardDistance = 400.0;
    g_HZSBOTCFG.ForwardSpeed = 220.0;
    g_HZSBOTCFG.BlockBotUseGrenade = true;
    g_HZSBOTCFG.BlockBotAttackBot = true;
    g_HZSBOTCFG.UseTraceBlockBotAttackBot = true;
}

void WriteDefaultScenarioConfig(const char[] path)
{
    Handle file = OpenFile(path, "w");
    if (file == INVALID_HANDLE)
        return;

    WriteFileLine(file, "// HZSBotAIConfig BOTAI配置文件");
    WriteFileLine(file, "// enable_plugins 是否开启此插件");
    WriteFileLine(file, "// Bot_CanSee_Distance BOT能看见目标的最大距离 0.0等于无限");
    WriteFileLine(file, "// Bot_Attack_Cooldown BOT射击目标的间隔 最小为0.1");
    WriteFileLine(file, "// Bot_StaySafe_Distance 是否开启BOT保持安全距离 与目标x距离 自动后退");
    WriteFileLine(file, "// Safe_Distance 安全距离 与目标小于此距离 会触发后退");
    WriteFileLine(file, "// FallBack_Speed 后退移动速度");
    WriteFileLine(file, "// Bot_ForwardTo_Distance 是否开启BOT距离目标过远 向目标前进");
    WriteFileLine(file, "// Forward_Distance 前进距离 与目标大于此距离 向目标移动");
    WriteFileLine(file, "// Forward_Speed 前进速度");
    WriteFileLine(file, "// Block_BotUse_Grenade 是否禁止BOT使用手雷 起源BOT捏住手雷不会松手投掷");
    WriteFileLine(file, "// Block_BotAttackBot 是否开启禁止不同队伍的相互攻击");
    WriteFileLine(file, "// Use_TraceBlockBotAttackBot 是否开启射线检查不同队伍禁止攻击");
    WriteFileLine(file, "// 射线检查与游戏开局禁止bot攻击双重屏蔽攻击");
    WriteFileLine(file, "\"HZSBotAI\"");
    WriteFileLine(file, "{");
    WriteFileLine(file, "    \"enable_plugins\"    \"1\"");
    WriteFileLine(file, "    \"Bot_CanSee_Distance\"    \"800.0\"");
    WriteFileLine(file, "    \"Bot_Attack_Cooldown\"           \"0.1\"");
    WriteFileLine(file, "    \"Bot_StaySafe_Distance\"              \"1\"");
    WriteFileLine(file, "    \"Safe_Distance\"              \"300.0\"");
    WriteFileLine(file, "    \"FallBack_Speed\"          \"220.0\"");
    WriteFileLine(file, "    \"Bot_ForwardTo_Distance\"    \"1\"");
    WriteFileLine(file, "    \"Forward_Distance\"              \"400.0\"");
    WriteFileLine(file, "    \"Forward_Speed\"    \"220.0\"");
    WriteFileLine(file, "    \"Block_BotUse_Grenade\"    \"1\"");
    WriteFileLine(file, "    \"Block_BotAttackBot\"    \"1\"");
    WriteFileLine(file, "    \"Use_TraceBlockBotAttackBot\"    \"1\"");
    WriteFileLine(file, "}");

    CloseHandle(file);
}


float KvGetSafeFloat(KeyValues kv, const char[] key, float default_val)
{
    char buf[64];
    KvGetString(kv, key, buf, sizeof(buf), "");

    if (StrEqual(buf, "")) 
    {
        return default_val; 
    }

    return StringToFloat(buf); 
}

int KvGetSafeNum(KeyValues kv, const char[] key, int default_val)
{
    char buf[64];
    KvGetString(kv, key, buf, sizeof(buf), "");

    if (StrEqual(buf, "")) 
    {
        return default_val;
    }

    return StringToInt(buf); 
}

bool KvGetSafeBool(KeyValues kv, const char[] key, bool default_val)
{
    return KvGetSafeNum(kv, key, default_val ? 1 : 0) != 0;
}





void ValidateConfigParameters()
{
    if (g_HZSBOTCFG.BotCanSeeDistance < 0.0)
    {
        g_HZSBOTCFG.BotCanSeeDistance = 0.0;
    }
    if (g_HZSBOTCFG.BotAttackCooldown < 0.1)
    {
        g_HZSBOTCFG.BotAttackCooldown = 0.1;
    }
    if(g_HZSBOTCFG.SafeDistance < 0.0)
    {
        g_HZSBOTCFG.SafeDistance = 300.0
    }
    if (g_HZSBOTCFG.FallBackSpeed < 0.0)
    {
        g_HZSBOTCFG.FallBackSpeed = 220.0;
    }
    if(g_HZSBOTCFG.ForwardDistance < 0.0)
    {
        g_HZSBOTCFG.ForwardDistance = 1000.0
    }
    if (g_HZSBOTCFG.ForwardSpeed < 0.0)
    {
        g_HZSBOTCFG.ForwardSpeed = 220.0;
    }
}

