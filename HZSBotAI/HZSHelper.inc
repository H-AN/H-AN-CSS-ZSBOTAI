
stock int GetClosestTarget(int client)
{
    float fClientOrigin[3], fTargetOrigin[3];
    GetClientAbsOrigin(client, fClientOrigin);
    
    int iClosestTarget = -1;
    float fClosestDistance = g_HZSBOTCFG.BotCanSeeDistance + 1.0; // 初始化为比最大距离大
    
    // 1. 检查所有僵尸
    int count = Han_GetZombieCount();
    for (int i = 0; i < count; i++)
    {
        int zombie = Han_GetZombieByIndex(i);
        if (!Han_IsZombie(zombie) || !IsValidEntity(zombie))
            continue;

        // 检查距离
        GetEntPropVector(zombie, Prop_Send, "m_vecOrigin", fTargetOrigin);
        float fDistance = GetVectorDistance(fClientOrigin, fTargetOrigin);
        
        // 超出最大可视距离
        if (fDistance > g_HZSBOTCFG.BotCanSeeDistance)
            continue;

        // 检查是否可见
        if (!ClientCanSeeTarget(client, zombie))
            continue;
            
        // 更新最近目标
        if (fDistance < fClosestDistance)
        {
            fClosestDistance = fDistance;
            iClosestTarget = zombie;
        }
    }
    
    // 2. 检查藤壶
    int barnacle = GetClosestBarnacle(client);
    if (barnacle != -1)
    {
        GetEntPropVector(barnacle, Prop_Send, "m_vecOrigin", fTargetOrigin);
        float fDistance = GetVectorDistance(fClientOrigin, fTargetOrigin);
        
        // 应用相同的距离限制
        if (fDistance <= g_HZSBOTCFG.BotCanSeeDistance)
        {
            // 检查是否可见（藤壶可能需要特殊的可见性检查）
            if (ClientCanSeeTarget(client, barnacle))
            {
                // 谁近打谁
                if (fDistance < fClosestDistance)
                {
                    fClosestDistance = fDistance;
                    iClosestTarget = barnacle;
                }
            }
        }
    }
    
    return iClosestTarget;
}

// 获取最近的藤壶
stock int GetClosestBarnacle(int client)
{
    float fClientOrigin[3], fTargetOrigin[3];
    GetClientAbsOrigin(client, fClientOrigin);
    
    int iClosestBarnacle = -1;
    float fClosestDistance = -1.0;
    
    char entName[64];
    int ent = -1;
    
    while ((ent = FindEntityByClassname(ent, "prop_dynamic")) != -1)
    {
        GetEntPropString(ent, Prop_Data, "m_iName", entName, sizeof(entName));
        if (!StrEqual(entName, "barnacle_hitbox", false))
            continue;
            
        if (!IsValidEntity(ent))
            continue;
            
        // 检查藤壶是否存活
        int health = GetEntProp(ent, Prop_Data, "m_iHealth");
        if (health <= 0)
            continue;
        
        GetEntPropVector(ent, Prop_Send, "m_vecOrigin", fTargetOrigin);
        float fDistance = GetVectorDistance(fClientOrigin, fTargetOrigin);
        
        // 可以加个最大检测距离，避免检测太远的藤壶
        if (fDistance > g_HZSBOTCFG.BotCanSeeDistance * 1.5)
            continue;
        
        if (fClosestDistance == -1.0 || fDistance < fClosestDistance)
        {
            fClosestDistance = fDistance;
            iClosestBarnacle = ent;
        }
    }
    
    return iClosestBarnacle;
}


public bool Base_TraceFilter(int iEntity, int iContentsMask, int iData)
{
	return iEntity == iData;
}


stock bool ClientCanSeeTarget(int client, int iTarget, float fDistance = 0.0, float fHeight = 50.0)
{
    if (!IsValidEntity(iTarget))
        return false;

    float fClientPosition[3]; float fTargetPosition[3];
    
    GetEntPropVector(client, Prop_Send, "m_vecOrigin", fClientPosition);
    
    char entName[64];
    GetEntPropString(iTarget, Prop_Data, "m_iName", entName, sizeof(entName));
    bool bIsBarnacle = StrEqual(entName, "barnacle_hitbox", false);
    
    // 对藤壶使用已验证有效的逻辑
    if (bIsBarnacle)
    {
        GetClientEyePosition(client, fClientPosition);
        GetEntPropVector(iTarget, Prop_Data, "m_vecOrigin", fTargetPosition);
        fTargetPosition[2] += 50.0;
        
        if (fDistance > 0.0 && GetVectorDistance(fClientPosition, fTargetPosition) > fDistance)
            return false;
        
        Handle trace = TR_TraceRayFilterEx(fClientPosition, fTargetPosition, MASK_SOLID, RayType_EndPoint, TraceFilterNotPlayer, client);
        
        bool canSee = false;
        if (TR_DidHit(trace))
        {
            int hitEntity = TR_GetEntityIndex(trace);
            canSee = (hitEntity == iTarget);
        }
        else
        {
            canSee = true;
        }
        
        delete trace;
        return canSee;
    }
    else
    {
        // 僵尸使用原来的逻辑
        GetEntPropVector(client, Prop_Send, "m_vecOrigin", fClientPosition);
        fClientPosition[2] += fHeight;
        
        GetEntPropVector(iTarget, Prop_Send, "m_vecOrigin", fTargetPosition);
        fTargetPosition[2] += fHeight;
        
        if (fDistance == 0.0 || GetVectorDistance(fClientPosition, fTargetPosition, false) < fDistance)
        {
            Handle hTrace = TR_TraceRayFilterEx(fClientPosition, fTargetPosition, MASK_SOLID_BRUSHONLY, RayType_EndPoint, Base_TraceFilter);
            
            if (TR_DidHit(hTrace))
            {
                delete hTrace;
                return false;
            }
            
            delete hTrace;
            return true;
        }
        
        return false;
    }
}

// 添加你已验证有效的过滤器
public bool TraceFilterNotPlayer(int entity, int contentsMask, any data)
{
    return (entity != data && !(entity > 0 && entity <= MaxClients));
}

stock void LookAtTarget(int client, int iTarget)
{
    if(!IsValidEntity(iTarget))
        return;

    float fTargetPos[3]; float fTargetAngles[3]; float fFinalPos[3]; float fClientPos[3];
    GetClientEyePosition(client, fClientPos);
    GetEntPropVector(iTarget, Prop_Send, "m_vecOrigin", fTargetPos);
    
    // 判断目标类型，调整高度
    char entName[64];
    GetEntPropString(iTarget, Prop_Data, "m_iName", entName, sizeof(entName));
    bool bIsBarnacle = StrEqual(entName, "barnacle_hitbox", false);
    
    if (bIsBarnacle)
    {
        fTargetPos[2] += 30.0;  // 藤壶
    }
    else
    {
        fTargetPos[2] += 50.0;  // 普通丧尸
    }
    
    GetEntPropVector(iTarget, Prop_Data, "m_angRotation", fTargetAngles);
    
    float fVecFinal[3];
    AddInFrontOf(fTargetPos, fTargetAngles, 7.0, fVecFinal);
    MakeVectorFromPoints(fClientPos, fVecFinal, fFinalPos);
    
    GetVectorAngles(fFinalPos, fFinalPos);
    
    int WeaponIndex = GetEntPropEnt(client, Prop_Data, "m_hActiveWeapon");
    int ammo = GetEntProp(WeaponIndex, Prop_Data, "m_iClip1");
    
    if (!bIsBarnacle)
    {
        TeleportEntity(client, NULL_VECTOR, fFinalPos, NULL_VECTOR); 
    }
    else if(bIsBarnacle && IsValidEntity(WeaponIndex) && ammo > 0)
    {
		TeleportEntity(client, NULL_VECTOR, fFinalPos, NULL_VECTOR); 
	}

}


stock void AddInFrontOf(float fVecOrigin[3], float fVecAngle[3], float fUnits, float fOutPut[3])
{
	float fVecView[3]; GetViewVector(fVecAngle, fVecView);
	
	fOutPut[0] = fVecView[0] * fUnits + fVecOrigin[0];
	fOutPut[1] = fVecView[1] * fUnits + fVecOrigin[1];
	fOutPut[2] = fVecView[2] * fUnits + fVecOrigin[2];
}

stock void GetViewVector(float fVecAngle[3], float fOutPut[3])
{
	fOutPut[0] = Cosine(fVecAngle[1] / (180 / FLOAT_PI));
	fOutPut[1] = Sine(fVecAngle[1] / (180 / FLOAT_PI));
	fOutPut[2] = -Sine(fVecAngle[0] / (180 / FLOAT_PI));
}

bool IsClientInFront(int client)
{
    if (!IsClientInGame(client) || !IsPlayerAlive(client))
        return false;

    float vOrigin[3], vAngles[3];
    GetClientEyePosition(client, vOrigin);
    GetClientEyeAngles(client, vAngles);

    Handle trace = TR_TraceRayFilterEx(vOrigin, vAngles, MASK_SHOT, RayType_Infinite, TraceEntityFilterPlayer);

    if (TR_DidHit(trace))
    {
        int index = TR_GetEntityIndex(trace);
        if (index > 0 && index <= MaxClients && IsClientInGame(index) && IsPlayerAlive(index))
        {
            CloseHandle(trace);
            return true;
        }
    }

    CloseHandle(trace);
    return false;
}

void SwitchWeapon(int client, int weapon)
{
    if(IsValidEntity(weapon))
    {
        char classname[32];
        GetEntityClassname(weapon, classname, sizeof(classname));
        FakeClientCommand(client, "use %s", classname);
    }
}

public bool TraceEntityFilterPlayer(int entity, int contentsMask)
{
    // 只允许射线命中玩家/Bot（即 client）
    return (entity > 0 && entity <= MaxClients);
}

