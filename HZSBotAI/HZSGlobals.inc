enum struct HZSBOTCFG
{
    bool Enable;
    float BotCanSeeDistance;
    float BotAttackCooldown;
    bool BotStaySafeDistance;
    float SafeDistance;
    float FallBackSpeed;
    bool BotForwardToDistance;
    float ForwardDistance;
    float ForwardSpeed;
    bool BlockBotUseGrenade;
    bool BlockBotAttackBot;
    bool UseTraceBlockBotAttackBot;
}
HZSBOTCFG g_HZSBOTCFG;

bool BlockShoot;
float g_NextBotAttack[MAXPLAYERS + 1];