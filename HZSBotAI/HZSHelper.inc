
stock int GetClosestClient(int client)
{
	float fClientOrigin[3], fTargetOrigin[3];
	GetClientAbsOrigin(client, fClientOrigin);
	int iClosestTarget = -1;
	
	float fClosestDistance = -1.0;
	float fTargetDistance;


    int count = Han_GetZombieCount();
    for (int i = 0; i < count; i++)
    {
        int zombie = Han_GetZombieByIndex(i);
        if (Han_IsZombie(zombie))
        {
            if(!IsValidEntity(zombie))
            {
                continue;
            }

            GetEntPropVector(zombie, Prop_Send, "m_vecOrigin", fTargetOrigin);
            fTargetDistance = GetVectorDistance(fClientOrigin, fTargetOrigin);

            if (fTargetDistance > fClosestDistance && fClosestDistance > -1.0)
			{
				continue;
			}

            if (!ClientCanSeeTarget(client, zombie))
			{
				continue;
			}
					
			fClosestDistance = fTargetDistance;
			iClosestTarget = zombie;
        }

    }	
	return iClosestTarget;
}


stock bool ClientCanSeeTarget(int client, int iTarget, float fDistance = 0.0, float fHeight = 50.0)
{
	float fClientPosition[3]; float fTargetPosition[3];
	
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

public bool Base_TraceFilter(int iEntity, int iContentsMask, int iData)
{
	return iEntity == iData;
}



stock void LookAtTarget(int client, int iTarget)
{

   if(!IsValidEntity(iTarget))
        return;

    float fTargetPos[3]; float fTargetAngles[3]; float fFinalPos[3]; float fClientPos[3];
    GetClientEyePosition(client, fClientPos);
	GetEntPropVector(iTarget, Prop_Send, "m_vecOrigin", fTargetPos);
    fTargetPos[2] += 50.0;
    GetEntPropVector(iTarget, Prop_Data, "m_angRotation", fTargetAngles);
	
	float fVecFinal[3];
	AddInFrontOf(fTargetPos, fTargetAngles, 7.0, fVecFinal);
	MakeVectorFromPoints(fClientPos, fVecFinal, fFinalPos);
	
	GetVectorAngles(fFinalPos, fFinalPos);

    TeleportEntity(client, NULL_VECTOR, fFinalPos, NULL_VECTOR); 
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

