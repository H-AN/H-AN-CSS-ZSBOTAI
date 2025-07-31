#include <sourcemod>
#include <sdktools>
#include <sdkhooks>
#include <cstrike>
#include <morecolors>
#include <HanZombieScenarioAPI>

#include "HZSBotAI/HZSGlobals"
#include "HZSBotAI/HZSConfig"
#include "HZSBotAI/HZSHelper"


public Plugin:myinfo =
{
    name = "[华仔]CS起源大灾变BOTAI", 
    author = "华仔 H-AN", 
    description = "华仔 H-AN CS起源大灾变BOTAI", 
    version = "1.0", 
    url = "[华仔]CS起源大灾变BOTAI, QQ群107866133, github https://github.com/H-AN"
};


public void OnMapStart()
{
	LoadHZSBotAIConfig();
}

//BOT死亡后不能复活, 一旦NPC丧尸已经召唤,BOT生成会导致服务器崩溃,原因未知,也许是CSS底层代码原因
/*

public void OnPluginStart()
{
	HookEvent("player_death", PlayerDeath);
}


public void PlayerDeath(Handle event, const String:name[], bool dontBroadcast)
{
    int client = GetClientOfUserId(GetEventInt(event, "userid"));

    if(!IsClientInGame(client) || !IsFakeClient(client))
        return;
    
   
    CreateTimer(10.0, Timer_ReBornBot, client, TIMER_FLAG_NO_MAPCHANGE);

}

public void Timer_ReBornBot(Handle timer, any client)
{
    if (!IsFakeClient(client) || !IsClientInGame(client) || IsPlayerAlive(client))
    {
        return;
    }
    
    CS_RespawnPlayer(client);

}
*/


public Action OnPlayerRunCmd(client, &buttons, &impulse, Float:vel[3], Float:angles[3], &weapon, &subtype, &cmdnum, &tickcount, &seed, mouse[2])
{
	if(!g_HZSBOTCFG.Enable)
	    return Plugin_Continue;

	if(client <= 0 || client > MaxClients || !IsClientInGame(client) || !IsPlayerAlive(client) || !IsFakeClient(client))
        return Plugin_Continue;
    
    int WeaponIndex = GetEntPropEnt(client, Prop_Data, "m_hActiveWeapon");
    if(!IsValidEntity(WeaponIndex))
    	return Plugin_Continue;
    
	if(g_HZSBOTCFG.BlockBotUseGrenade)
	{
		int weaponSlot3 = GetPlayerWeaponSlot(client, 3);
		if (IsValidEntity(weaponSlot3) && WeaponIndex == weaponSlot3)
		{
			SetEntPropEnt(client, Prop_Send, "m_hActiveWeapon",GetPlayerWeaponSlot(client, 0));
		}
	}

    int target = GetClosestClient(client);
	if (target <= -1 || !IsValidEntity(target)) 
        return Plugin_Continue;

	if (!ClientCanSeeTarget(client, target, g_HZSBOTCFG.BotCanSeeDistance))
		return Plugin_Continue;

	float clientPos[3], targetPos[3];
	GetClientAbsOrigin(client, clientPos);
	GetEntPropVector(target, Prop_Send, "m_vecOrigin", targetPos);
	float actualDistance = GetVectorDistance(clientPos, targetPos);

	LookAtTarget(client, target);
    if (g_HZSBOTCFG.BotStaySafeDistance && actualDistance < g_HZSBOTCFG.SafeDistance)
	{
		vel[0] = (g_HZSBOTCFG.FallBackSpeed * -1.0);
	}
	else if (g_HZSBOTCFG.BotForwardToDistance && actualDistance > g_HZSBOTCFG.ForwardDistance)
	{
		vel[0] = g_HZSBOTCFG.ForwardSpeed;
	}

    int ammo = GetEntProp(WeaponIndex, Prop_Data, "m_iClip1");
    if(ammo <= 0)
        return Plugin_Continue;

    float now = GetGameTime();
	if (now < g_NextBotAttack[client])
		return Plugin_Continue;

    buttons |= IN_ATTACK;
    g_NextBotAttack[client] = now + g_HZSBOTCFG.BotAttackCooldown;

	return Plugin_Continue;
}

public void OnClientDisconnect(int client)
{
    g_NextBotAttack[client] = 0.0;
}

