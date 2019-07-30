// The player package acts as the entry and exit point for all players. It takes
// care of loading and storing player data and provides the playerData array.

// -
// External Packages
// -

#include <YSI\y_iterate>
#include <YSI_Coding\y_timers>

// -
// Internal Packages
// -

#include <storage>

// -
// Declarations
// -

enum E_PLAYER
{
    E_PLAYER_USER_ID,
    E_PLAYER_MONEY,
    E_PLAYER_WANTED_LEVEL
};

// -
// API
// -

// playerData is an array that contains player data.
new playerData[MAX_PLAYERS][E_PLAYER];

// GetPlayerID looks for and returns the playerid of a player using their
// userid. Returns -1 if the user was not found.
stock GetPlayerID(userid)
{
    new ret = -1;
    foreach(new p : Player)
    {
        if(playerData[p][E_PLAYER_USER_ID] == userid)
        {
            ret = p;
            break;
        }
    }

    return ret;
}

// -
// Internal
// -

#include "player-wanted-level"

#include <player-roles>
#include <player-knockout>
#include <player-class-selection>
#include <player-prison>
#include <player-nametag>
#include <player-statistics>

static StorePlayerData(playerid)
{
    StoreUser(GetDBHandle(),
        playerData[playerid][E_PLAYER_USER_ID],
        playerData[playerid][E_PLAYER_MONEY],
        playerData[playerid][E_PLAYER_WANTED_LEVEL]
    );
}

// - Hooks

#include <YSI_Coding\y_hooks>

hook OnPlayerConnect(playerid)
{
    // Please have some respect and do NOT modify, remove or hide this message.
    SendClientMessage(playerid, COLOR_DEFAULT, "This server runs on "SCOLOR_DEFAULT2"OpenCnR "OPENCNR_VERSION);

    // Initializing and resetting player variables.
    if(!LoadUser(GetDBHandle(), playerid))
    {
        SendClientMessage(playerid, COLOR_ERROR, "Something went wrong... Please try again later.");
        Kick(playerid);
    }

    SetPlayerColor(playerid, COLOR_DEFAULT & ~0xFF);
    return 1;
}

hook OnUserLoaded(playerid, userid, money, wantedLevel)
{
    playerData[playerid][E_PLAYER_USER_ID] = userid;
    playerData[playerid][E_PLAYER_MONEY] = money;
    playerData[playerid][E_PLAYER_WANTED_LEVEL] = wantedLevel;

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

hook OnPlayerSpawn(playerid)
{
    SetPlayerInterior(playerid, 0);
    SetPlayerVirtualWorld(playerid, 0);
    return 1;
}

// - Timers

ptask StorePlayerDataTimer[2 * 60 * 1000](playerid)
{
    StorePlayerData(playerid);
}
