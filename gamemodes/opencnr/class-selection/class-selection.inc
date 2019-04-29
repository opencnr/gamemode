// The class-selection package is a dependency of the player-lifecycle package
// and handles the class-selection flow.

// -
// Internal Packages
// -

#include <player-auth>
#include <roles>

// -
// External Packages
// -

#include <YSI\y_timers>

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
    PromptRoleSelection(playerid);
    return 1;
}

hook OnPlayerSelectedRole(playerid, role)
{
    if(IsPlayerInAnyVehicle(playerid))
    {
        ClearAnimations(playerid);
    }

    SpawnPlayer(playerid);
    return 1;
}
