// The class-selection package is a dependency of the player package and handles
// the class-selection flow.

// -
// External Packages
// -

#include <YSI\y_timers>

// -
// Internal Packages
// -

#include <player-roles>

#include <player-auth>

// -
// Internal
// -

#include <YSI\y_hooks>

hook OnPlayerRequestClass(playerid, classid)
{
    if(!IsPlayerAuthenticated(playerid))
    {
        TogglePlayerSpectating(playerid, true);
        defer SetClassSelectionView(playerid);

        PromptAuthentication(playerid);
    }
    else
    {
        SpawnPlayer(playerid);
    }

    return 1;
}

timer SetClassSelectionView[100](playerid)
{
    SetPlayerCameraPos(playerid, -1595.9017, 299.6661, 168.7846);
    SetPlayerCameraLookAt(playerid, -1596.6104, 300.3773, 168.7446);
}

hook OnPlayerSpawn(playerid)
{
    // Disable spectating that was enabled in OnPlayerRequestClass.
    TogglePlayerSpectating(playerid, false);
    return 1;
}

hook OnPlayerAuthenticated(playerid)
{
    new wantedLevel = GetUserWantedLevel(db_handle, playerData[playerid][E_PLAYER_USER_ID]);
    
    if(wantedLevel > 0)
    {
        SetPlayerRole(playerid, PLAYER_ROLE_ROBBER);
        SetPlayerWantedLevel(playerid, wantedLevel);
    }
    else
    {
        PromptRoleSelection(playerid);
    }

    return 1;
}

hook OnPlayerSetRole(playerid, role)
{
    if(IsPlayerInAnyVehicle(playerid))
    {
        ClearAnimations(playerid);
    }

    SpawnPlayer(playerid);
    return 1;
}
