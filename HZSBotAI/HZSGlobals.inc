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
}
HZSBOTCFG g_HZSBOTCFG;

float g_NextBotAttack[MAXPLAYERS + 1];