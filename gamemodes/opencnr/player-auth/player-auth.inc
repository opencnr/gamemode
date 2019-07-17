// The player-auth packages handle user authentication.

// -
// Declarations
// -

#define BCRYPT_WORK_FACTOR (11)

// Player variables
new bool:isPlayerAuthenticated[MAX_PLAYERS];

// -
// API
// -

// OnPlayerAuthenticated gets called when the player has successfully gone
// through the authentication proccess.
forward OnPlayerAuthenticated(playerid);

// PromptAuthentication prompts either a login dialog or a registration dialog
// depending on whether the user is registered or not.
stock PromptAuthentication(playerid)
{
    if(IsUserRegistered(db_handle, playerid))
    {
        PromptLoginDialog(playerid);
    }
    else
    {
        PromptRegistrationDialog(playerid);
    }
}

// IsPlayerAuthenticated returns either true or false depending on whether the
// user is authenticated or not. Returns false if the player is not connected.
stock bool:IsPlayerAuthenticated(playerid)
{
    if(IsPlayerConnected(playerid))
    {
        return isPlayerAuthenticated[playerid];
    }
    return false;
}

// -
// Internal
// -

#include "player-auth-registration"
#include "player-auth-login"
#include <YSI\y_hooks>

hook OnPlayerConnect(playerid)
{
    // Initializing and resetting player variables.
    isPlayerAuthenticated[playerid] = false;
    return 1;
}

hook OnPlayerAuthenticated(playerid)
{
    isPlayerAuthenticated[playerid] = true;
    LoadUser(db_handle, playerid);
    return 1;
}
