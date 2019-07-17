// The prison package takes care of putting players in prison and releasing
// them.

// -
// Internal Packages
// -

#include <player-knockout>
#include <player-roles>

// -
// External Packages
// -

#include <YSI\y_timers>

// -
// Declarations
// -

// Player variables
static bool:isPlayerArrested[MAX_PLAYERS];

// -
// API
// -

// IsPlayerArrested returns either true or false depending on
// whether the user is arrested or not. Returns -1 if the player
// is not connected.
stock IsPlayerArrested(playerid)
{
    if(IsPlayerConnected(playerid))
    {
        return isPlayerArrested[playerid];
    }

    return -1;
}

// PutPlayerInPrison puts the specified player inside of a prison
// cell and releases them after 30 seconds.
stock PutPlayerInPrison(playerid)
{
    isPlayerArrested[playerid] = true;

    if(IsPlayerKnockedOut(playerid))
    {
        RevivePlayer(playerid);
        SetPlayerServerHealth(playerid, 100);
    }

    ToggleGodMode(playerid, true);

    SetPlayerPos(playerid, 227.2537, 111.1806, 999.0156);
    SetPlayerInterior(playerid, 10);
    SetPlayerVirtualWorld(playerid, 0);
    
    SetPlayerWantedLevel(playerid, 0);

    defer ReleaseFromPrison(playerid);
}

// -
// Internals
// -

#include <YSI\y_hooks>

hook OnPlayerConnect(playerid)
{
    // Initializing and resetting player variables.
    isPlayerArrested[playerid] = false;

    return 1;
}

timer ReleaseFromPrison[30 * 1000](playerid)
{
    isPlayerArrested[playerid] = false;

    ToggleGodMode(playerid, false);

    SetPlayerPos(playerid, -1605.6085, 714.0386, 13.1230);
    SetPlayerFacingAngle(playerid, 0.0);
    SetPlayerInterior(playerid, 0);
}
