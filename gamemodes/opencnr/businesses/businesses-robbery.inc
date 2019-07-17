// The businesses-robbery module handles business robberies.

// -
// External Packages
// -

#include <map-zones>
#include <YSI\y_timers>

// -
// Declarations
// -

#define ROBBERY_TIMEOUT (60 * 3)

// Player variables
static playerRobbingBusiness[MAX_PLAYERS];
static Float:robberyProgress[MAX_PLAYERS];
static PlayerBar:playerRobberyProgressbar[MAX_PLAYERS];

// -
// Internal Packages
// -

#include <anticheat>
#include <player-roles>

// -
// Internal
// -

#include <YSI\y_hooks>

hook OnPlayerConnect(playerid)
{
    // Initializing and resetting player variables.
    robberyProgress[playerid] = 0.0;
    playerRobbingBusiness[playerid] = -1;
    playerRobberyProgressbar[playerid] = CreatePlayerProgressBar(playerid, 260.0, 400.0, 140.0, 7.0, 0x1169C6FF);

    return 1;
}

hook OnPlayerAimAtClerk(playerid, actorid)
{
    if(GetPlayerRole(playerid) == PLAYER_ROLE_ROBBER)
    {
        #if !defined DEV_BUILD
            if(GetRolePlayerCount(PLAYER_ROLE_COP) == 0)
            {
                SendClientMessage(playerid, COLOR_ERROR, " >  There aren't any cops online!");
                return 0;
            }
        #endif

        if(StartRobbery(playerid))
        {
            ClerkHandsUp(actorid);
        }
    }

    return 1;
}

static ClerkHandsUp(actorid)
{
    ApplyActorAnimation(actorid, "ped", "handsup", 4.1, false, false, false, true, 0);
    defer ClerkHandsDown(actorid);
}

timer ClerkHandsDown[30 * 1000](actorid)
{
    ClearActorAnimations(actorid);
}

static StartRobbery(playerid)
{
    new businessID = GetPlayerBusiness(playerid);
    if(businessData[businessID][E_BUSINESS_LAST_ROBBERY] + ROBBERY_TIMEOUT < gettime())
    {
        if(playerRobbingBusiness[playerid] == -1)
        {
            playerRobbingBusiness[playerid] = businessID;

            new wantedLevel = GetPlayerWantedLevel(playerid);
            SetPlayerWantedLevel(playerid, wantedLevel + 1);

            ShowPlayerProgressBar(playerid, playerRobberyProgressbar[playerid]);

            PlayerPlaySound(playerid, 3401, 0.0, 0.0, 0.0);

            AnnounceRobbery(businessID);
            return 1;
        }
    }
    else
    {
        SendClientMessage(playerid, COLOR_ERROR, " >  This business has already recently been robbed!");
    }

    return 0;
}

static StopRobbery(playerid, successful = false)
{
    if(successful)
    {
        new businessID = playerRobbingBusiness[playerid];
        businessData[businessID][E_BUSINESS_LAST_ROBBERY] = gettime();

        GivePlayerServerMoney(playerid, 1000);
        PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
    }

    playerRobbingBusiness[playerid] = -1;
    robberyProgress[playerid] = 0.0;
    HidePlayerProgressBar(playerid, playerRobberyProgressbar[playerid]);
}

ptask UpdateRobberyProgress[200](playerid)
{
    if(playerRobbingBusiness[playerid] != -1)
    {
        if(robberyProgress[playerid] < 100.0)
        {
            robberyProgress[playerid] += 200.0 / 100.0;
            SetPlayerProgressBarValue(playerid, playerRobberyProgressbar[playerid], robberyProgress[playerid]);

            if(playerRobbingBusiness[playerid] != GetPlayerBusiness(playerid))
            {
                StopRobbery(playerid);
            }
        }
        else
        {
            StopRobbery(playerid, true);
        }
    }
}

static AnnounceRobbery(businessID)
{
    new portalID = businessData[businessID][E_BUSINESS_PORTAL],
        zoneName[MAX_MAP_ZONE_NAME],
        message[MAX_MAP_ZONE_NAME + 40],
        MapZone:zone;

    zone = GetMapZoneAtPoint2D(
        portalData[portalID][E_PORTAL_X],
        portalData[portalID][E_PORTAL_Y]
    );
    GetMapZoneName(zone, zoneName);

    format(message, sizeof message, " >  A business in %s is being robbed!", zoneName);

    // Broadcast the message to all cops...
    foreach(new i : Player)
    {
        if(GetPlayerRole(i) == PLAYER_ROLE_COP)
        {
            SendClientMessage(i, COLOR_INFO, message);
        }
    }
}

hook OnPlayerExitBusiness(playerid)
{
    PlayerPlaySound(playerid, 0, 0.0, 0.0, 0.0);
    return 1;
}

hook OnPlayerNearDeath(playerid)
{
    if(playerRobbingBusiness[playerid] != -1)
    {
        StopRobbery(playerid);
    }
    return 1;
}
