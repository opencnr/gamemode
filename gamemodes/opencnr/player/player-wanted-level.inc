// The player-wanted-level module hooks the SetPlayerWantedLevel and handles the
// players wanted level.

// -
// Internal
// -

// - Hooks

#include <YSI_Coding\y_hooks>

hook function SetPlayerWantedLevel(playerid, level)
{
    if(level < 0)
    {
        level = 0;
    }

    if(level != 0)
    {
        SetPlayerColor(playerid, COLOR_ROBBER);
    }
    else
    {
        SetPlayerColor(playerid, COLOR_DEFAULT & ~0xFF);
    }

    playerData[playerid][E_PLAYER_WANTED_LEVEL] = level;
    return continue(playerid, level);
}
