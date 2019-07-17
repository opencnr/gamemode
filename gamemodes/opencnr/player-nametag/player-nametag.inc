// The nametag package provides a custom nametag and disables the default one.

// -
// Declarations
// -

// Player variables
static Text3D:playerNametagLabel[MAX_PLAYERS];
static lastPlayerColor[MAX_PLAYERS];

// -
// Internal
// -

#include <YSI\y_hooks>

hook OnGameModeInit()
{
    // Disable default nametags which will be replaced with custom nametags.
    ShowNameTags(0);
    return 1;
}

hook OnPlayerConnect(playerid)
{
    // Initializing and resetting player variables.
    playerNametagLabel[playerid] = Create3DTextLabel("_", COLOR_DEFAULT, 0.0, 0.0, 0.0, 10.0, 0, true);
    lastPlayerColor[playerid] = -1;

    // Attaching the custom nametag to the player and updating it.
    Attach3DTextLabelToPlayer(playerNametagLabel[playerid], playerid, 0.0, 0.0, 0.0);
    UpdatePlayerNametag(playerid);

    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    Delete3DTextLabel(playerNametagLabel[playerid]);
    return 1;
}

stock UpdatePlayerNametag(playerid)
{
    new playerName[MAX_PLAYER_NAME + 1],
        nametagText[(MAX_PLAYER_NAME + 10) + 1],
        playerColor;
    
    GetPlayerName(playerid, playerName, sizeof playerName);
    format(nametagText, sizeof nametagText, "%s (%d)", playerName, playerid);
    playerColor = GetPlayerColor(playerid);

    Update3DTextLabelText(playerNametagLabel[playerid], playerColor | 0xFF, nametagText);
}

hook OnPlayerUpdate(playerid)
{
    new playerColor = GetPlayerColor(playerid);
    if(lastPlayerColor[playerid] != playerColor)
    {
        lastPlayerColor[playerid] = playerColor;
        UpdatePlayerNametag(playerid);
    }
    return 1;
}
