// The health module handles the players health and draws over the default
// healthbar with a TextDraw healthbar which displays the server-sided health.

// -
// External Packages
// -

#include <progress2>

// -
// Declarations
// -

static bool:deathEnabled = true;

// Player Variables
static Float:playerHealth[MAX_PLAYERS];
static PlayerBar:playerHealthBar[MAX_PLAYERS];
static bool:playerGodModeEnabled[MAX_PLAYERS];

// -
// API
// -

// OnPlayerNearDeath is called when the player is about to die. If the callback
// returns RETURN_NO_KILL, the player will not die.
forward OnPlayerNearDeath(playerid);

// ToggleDeath either enables or disable player death. When player death is
// disabled OnPlayerNearDeath will be called instead of killing the player.
stock ToggleDeath(bool:toggle)
{
    deathEnabled = toggle;
}

// SetPlayerServerHealth sets the server-sided health.
stock SetPlayerServerHealth(playerid, Float:amount)
{
    if(!playerGodModeEnabled[playerid])
    {
        if(amount <= 0.0 && playerHealth[playerid] != 0.0)
        {
            playerHealth[playerid] = 0.0;
            if(deathEnabled)
            {
                // Kill the player.
                SetPlayerHealth(playerid, 0.0);
            }
            else
            {
                CallLocalFunction("OnPlayerNearDeath", "d", playerid);
            }
        }
        else if(amount > 0.0)
        {
            playerHealth[playerid] = amount;
        }

        UpdatePlayerHealthBar(playerid);
    }
}

// GetPlayerServerHealth returns the server-sided health.
stock GetPlayerServerHealth(playerid, &Float:health)
{
    if(IsPlayerConnected(playerid))
    {
        health = playerHealth[playerid];
        return 1;
    }
    return 0;
}

// UpdatePlayerServerHealth updates the specified players server-sided health
// relative to their current server-sided health.
stock UpdatePlayerServerHealth(playerid, Float:amount)
{
    new Float:health;
    GetPlayerServerHealth(playerid, health);
    SetPlayerServerHealth(playerid, health + amount);
}

// ToggleGodMode lets you either enable or disable god mode. When god mode is
// enabled the player won't take any damage.
stock ToggleGodMode(playerid, bool:toggle)
{
    if(IsPlayerConnected(playerid))
    {
        playerGodModeEnabled[playerid] = toggle;
        return 1;
    }
    return 0;
}

// -
// Internal
// -

#include <YSI\y_hooks>

hook OnPlayerConnect(playerid)
{
    // Initialize and reset player variables.
    playerHealth[playerid] = 100.0;
    playerHealthBar[playerid] = CreatePlayerProgressBar(playerid, 547.5, 68.3, 63.0, 6.0, 0xB51818FF);
    playerGodModeEnabled[playerid] = false;

    return 1;
}

hook OnPlayerStateChange(playerid, newstate, oldstate)
{
    // Handles when the new health-bar should be displayed.
    if(newstate != PLAYER_STATE_SPECTATING)
    {
        ShowPlayerProgressBar(playerid, playerHealthBar[playerid]);
    }
    else
    {
        HidePlayerProgressBar(playerid, playerHealthBar[playerid]);
    }

    return 1;
}

hook OnPlayerSpawn(playerid)
{
    // Set player in god-mode so client-sided health becomes ineffective.
    SetPlayerHealth(playerid, 99999.0);

    // Make sure the player spawns with full server-sided health.
    SetPlayerServerHealth(playerid, 100.0);
    return 1;
}

hook OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
    // Ensure the player remains in god-mode.
    SetPlayerHealth(playerid, 99999.0);

    // Update the server-sided health.
    UpdatePlayerServerHealth(playerid, -amount);
    return 1;
}

hook OnPlayerDeath(playerid, killerid, reason)
{
    // Update the visuals ONLY, as the player is already dead anyways and
    // OnPlayerTakeDamage isn't called when the player dies.
    playerHealth[playerid] = 0.0;
    UpdatePlayerHealthBar(playerid);
}

static UpdatePlayerHealthBar(playerid)
{
    SetPlayerProgressBarValue(playerid, playerHealthBar[playerid], playerHealth[playerid]);
}
