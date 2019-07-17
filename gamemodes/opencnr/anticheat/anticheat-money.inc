// The money module takes care of ensuring the players client-sided money is
// equal to the one on the server-side. It also provides functions to manipulate
// the players server-sided money.

// -
// External Packages
// -

#include <YSI\y_timers>

// -
// Internal Packages
// -

#include <player>

// -
// API
// -

// GivePlayerServerMoney increases or decreases the players server-sided money
// by the amount passed as an argument.
stock GivePlayerServerMoney(playerid, amount)
{
    if(IsPlayerConnected(playerid))
    {
        // Use the native GivePlayerMoney function to update the players
        // client-sided money.
        GivePlayerMoney(playerid, amount);
        // Update the money in server-sided memory.
        playerData[playerid][E_PLAYER_MONEY] += amount;
        return 1;
    }
    return 0;
}

// GetPlayerServerMoney returns the amount of server-sided money the player has.
stock GetPlayerServerMoney(playerid)
{
    if(IsPlayerConnected(playerid))
    {
        return playerData[playerid][E_PLAYER_MONEY];
    }

    return 0;
}

// -
// Internal
// -

ptask EnsureMoney[2500](playerid)
{
    new playerMoney = GetPlayerServerMoney(playerid);
    if(GetPlayerMoney(playerid) != playerMoney)
    {
        ResetPlayerMoney(playerid);
        GivePlayerMoney(playerid, playerMoney);
    }
}
