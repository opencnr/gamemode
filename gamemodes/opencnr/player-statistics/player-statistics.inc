// player-statistics loads user statistics once the player authenticates and
// stores them when they leave.

// -
// External Packages
// -

#include <storage>

// -
// Declaration
// -

// - Player variables

enum E_PLAYER_STATS
{
    E_PLAYER_STATS_KILLS,
    E_PLAYER_STATS_DEATHS
};

stock playerStatistics[MAX_PLAYERS][E_PLAYER_STATS];

// -
// Internal Packages
// -

#include <player-auth>
#include <storage>

// -
// Internal
// -

#include "player-statistics-menu"

// - Hooks

#include <YSI\y_hooks>

hook OnPlayerAuthenticated(playerid)
{
    LoadUserStatistics(db_handle, playerData[playerid][E_PLAYER_USER_ID]);
    return 1;
}

hook OnUserStatisticsLoaded(userid, kills, deaths)
{
    new playerid = GetPlayerID(userid);
    if(playerid != -1)
    {
        playerStatistics[playerid][E_PLAYER_STATS_KILLS] = kills;
        playerStatistics[playerid][E_PLAYER_STATS_DEATHS] = deaths;
    }

    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    StoreUserStatistics(db_handle, playerData[playerid][E_PLAYER_USER_ID],
        playerStatistics[playerid][E_PLAYER_STATS_KILLS],
        playerStatistics[playerid][E_PLAYER_STATS_DEATHS]
    );

    return 1;
}

hook OnPlayerDeath(playerid, killerid, reason)
{
    playerStatistics[playerid][E_PLAYER_STATS_DEATHS] += 1;

    if(killerid != INVALID_PLAYER_ID)
    {
        playerStatistics[killerid][E_PLAYER_STATS_KILLS] += 1;
    }

    return 1;
}
