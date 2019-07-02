#if defined _INC_PickupEx
	#endinput
#endif
#define _INC_PickupEx

enum pickupInfoEnum
{
	pModel,
	Float:pX,
	Float:pY,
	Float:pZ
};

new pickupInfoArray[MAX_PICKUPS][pickupInfoEnum],
	lastAccessedPickup[MAX_PLAYERS];

#if defined _inc_y_iterate
	new Iterator:Pickup<MAX_PICKUPS>;
#endif

stock CreatePickupEx(model, type, Float:X, Float:Y, Float:Z, virtualworld = 0)
{
	new pickupid = CreatePickup(model, type, X, Y, Z, virtualworld);

	pickupInfoArray[pickupid][pModel] = model;
	pickupInfoArray[pickupid][pX] = X;
	pickupInfoArray[pickupid][pY] = Y;
	pickupInfoArray[pickupid][pZ] = Z;

	#if defined _inc_y_iterate
		Iter_Add(Pickup, pickupid);
	#endif

	return pickupid;
}

#if defined _ALS_CreatePickup
	#undef CreatePickup
#else
	#define _ALS_CreatePickup
#endif

#define CreatePickup CreatePickupEx

stock DestroyPickupEx(pickupid)
{
	DestroyPickup(pickupid);

	pickupInfoArray[pickupid][pModel] = 0;
	pickupInfoArray[pickupid][pX] = 0.0;
	pickupInfoArray[pickupid][pY] = 0.0;
	pickupInfoArray[pickupid][pZ] = 0.0;

	#if defined _inc_y_iterate
		Iter_Remove(Pickup, pickupid);
	#endif
}

#if defined _ALS_DestroyPickup
	#undef DestroyPickup
#else
	#define _ALS_DestroyPickup
#endif

#define DestroyPickup DestroyPickupEx

public OnPlayerPickUpPickup(playerid, pickupid)
{
	lastAccessedPickup[playerid] = pickupid;

	#if defined PEX_OnPlayerPickUpPickup
		return PEX_OnPlayerPickUpPickup(playerid, pickupid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerPickUpPickup
	#undef OnPlayerPickUpPickup
#else
	#define _ALS_OnPlayerPickUpPickup
#endif

#define OnPlayerPickUpPickup PEX_OnPlayerPickUpPickup
#if defined PEX_OnPlayerPickUpPickup
	forward PEX_OnPlayerPickUpPickup(playerid, pickupid);
#endif

stock EditPickup(pickupid, model, type, Float:X, Float:Y, Float:Z, virtualworld = 0)
{
	DestroyPickup(pickupid);
	CreatePickup(model, type, X, Y, Z, virtualworld);
}

stock GetPickupModel(pickupid)
{
	return pickupInfoArray[pickupid][pModel];
}

stock GetPickupPos(pickupid, &Float:x, &Float:y, &Float:z)
{
	x = pickupInfoArray[pickupid][pX];
	y = pickupInfoArray[pickupid][pY];
	z = pickupInfoArray[pickupid][pZ];
}

stock IsValidPickup(pickupid)
{
    if(pickupid > MAX_PICKUPS || pickupid < 0 || pickupInfoArray[pickupid][pModel] == 0)
    {
        return 0;
    }

    return 1;
}

stock GetPlayerNearbyPickup(playerid, &pickup)
{
	if(!IsPlayerConnected(playerid))
	{
		return -1;
	}

	new Float:x, Float:y, Float:z;
	GetPickupPos(lastAccessedPickup[playerid], x, y, z);

	if(GetPlayerDistanceFromPoint(playerid, x, y, z) >= 1.0)
	{
		return 0;
	}

	pickup = lastAccessedPickup[playerid];

	return 1;
}
