// The player package acts as the entry and exit point for all players. It takes
// care of loading and storing player data and provides the playerData array.

// -
// Internal Packages
// -

#include <class-selection>
#include <storage>

// -
// External Packages
// -

#include <YSI\y_iterate>
#include <YSI\y_timers>

// -
// Declarations
// -

enum E_PLAYER
{
    E_PLAYER_USER_ID,
    E_PLAYER_MONEY
};

// Player variables
new playerData[MAX_PLAYERS][E_PLAYER];

// -
// Internal
// -

#include <YSI\y_hooks>

hook OnPlayerConnect(playerid)
{
    // Initializing and resetting player variables.
    LoadUser(db_handle, playerid);

    SetPlayerColor(playerid, COLOR_DEFAULT & ~0xFF);
    return 1;
}

hook OnUserLoaded(playerid, userid, money)
{
    playerData[playerid][E_PLAYER_USER_ID] = userid;
    playerData[playerid][E_PLAYER_MONEY] = money;
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    StorePlayerData(playerid);
    return 1;
}

hook OnGameModeExit()
{
    foreach(new p : Player)
    {
        StorePlayerData(p);
    }
    return 1;
}

ptask StorePlayerDataTimer[2 * 60 * 1000](playerid)
{
    StorePlayerData(playerid);
}

static StorePlayerData(playerid)
{
    StoreUser(db_handle,
        playerData[playerid][E_PLAYER_USER_ID],
        playerData[playerid][E_PLAYER_MONEY]
    );
}

hook OnPlayerSpawn(playerid)
{
    SetPlayerInterior(playerid, 0);
    SetPlayerVirtualWorld(playerid, 0);
    return 1;
}
