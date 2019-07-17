// The knockout package puts the player in a knockout state when the player is
// about to die and handles respawning.

// -
// External Packages
// -

#include <YSI\y_timers>

// -
// Declarations
// -

// Player variables
static bool:isPlayerKnockedOut[MAX_PLAYERS];
static bool:allowPlayerRespawn[MAX_PLAYERS];

// -
// API
// -

// IsPlayerKnockedOut returns either true or false depending on if the player
// is knocked out or not. It will return -1 if the player is not connected to
// the server.
stock IsPlayerKnockedOut(playerid)
{
    if(IsPlayerConnected(playerid))
    {
        return isPlayerKnockedOut[playerid];
    }

    return -1;
}

// RevivePlayer puts the specified player out of the knockout state.
stock RevivePlayer(playerid)
{
    if(IsPlayerKnockedOut(playerid))
    {
        isPlayerKnockedOut[playerid] = false;
        allowPlayerRespawn[playerid] = false;

        ClearAnimations(playerid);
        return 1;
    }
    return 0;
}

// -
// Internal
// -

#include <YSI\y_hooks>

hook OnGameModeInit()
{
    ToggleDeath(false);
    return 1;
}

hook OnPlayerConnect(playerid)
{
    // Initialize and resetting player variables.
    isPlayerKnockedOut[playerid] = false;
    allowPlayerRespawn[playerid] = false;

    return 1;
}

hook OnPlayerNearDeath(playerid)
{
    isPlayerKnockedOut[playerid] = true;

    ClearAnimations(playerid);
    ApplyKnockdownAnimation(playerid);

    defer AllowRespawn(playerid);

    return 1;
}

hook OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
    if(IsPlayerKnockedOut(playerid))
    {
        ClearAnimations(playerid);
        ApplyKnockdownAnimation(playerid);
    }
}

timer AllowRespawn[10 * 1000](playerid)
{
    if(IsPlayerKnockedOut(playerid))
    {
        allowPlayerRespawn[playerid] = true;
        SendClientMessage(playerid, COLOR_INFO, " >  You can now respawn by pressing LMB...");
    }
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(PRESSED(KEY_FIRE))
    {
        if(allowPlayerRespawn[playerid])
        {
            allowPlayerRespawn[playerid] = false;

            RevivePlayer(playerid);
            SpawnPlayer(playerid);
        }
    }

    return 1;
}

static ApplyKnockdownAnimation(playerid)
{
    ApplyAnimation(playerid, "PED", "KO_SHOT_FRONT", 4.1, false, false, false, true, 0);
}
