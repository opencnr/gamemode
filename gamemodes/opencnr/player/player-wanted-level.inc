// The player-wanted-level module hooks the SetPlayerWantedLevel and handles the
// players wanted level.

// -
// External Packages
// -

#include <PawnPlus>

// -
// Declarations
// -

// Hooks
static NativeHook:hookv_SetPlayerWantedLevel;

// -
// Internal
// -

#include <YSI\y_hooks>

hook OnGameModeInit()
{
    // Adding hooks
    hookv_SetPlayerWantedLevel = pawn_add_hook("SetPlayerWantedLevel", "dd", "hookf_SetPlayerWantedLevel");
}

hook OnGameModeExit()
{
    // Removing hooks
    pawn_remove_hook(hookv_SetPlayerWantedLevel);
}

forward hookf_SetPlayerWantedLevel(playerid, level);
public hookf_SetPlayerWantedLevel(playerid, level)
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

    return SetPlayerWantedLevel(playerid, level);
}
