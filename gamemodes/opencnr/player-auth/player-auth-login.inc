// The player-auth-login module handles player authorization and provides the
// login dialog.

// -
// External Packages
// -

#include <easyDialog>
#include <samp_bcrypt>

// -
// Declarations
// -

#define MAX_LOGIN_ATTEMPTS (3)

// Player variables
static playerLoginAttempts[MAX_PLAYERS];

// -
// Internal
// -

#include <YSI\y_hooks>

hook OnPlayerConnect(playerid)
{
    // Initializing and resetting player variables.
    playerLoginAttempts[playerid] = 0;
    return 1;
}

PromptLoginDialog(playerid)
{
    Dialog_Show(playerid, "OnLoginDialog", DIALOG_STYLE_PASSWORD,
        "Login",
        "Please enter a password down below :\n",
        "Login", "Exit"
    );
}

Dialog:OnLoginDialog(playerid, response, listitem, inputtext[])
{
    if(response)
    {
        new hash[60 + 1];
        GetUserHash(db_handle, playerid, hash, sizeof hash);
        bcrypt_verify(playerid, "OnPasswordVerify", inputtext, hash);
    }
    else
    {
        Kick(playerid);
    }
}

forward OnPasswordVerify(playerid, bool:success);
public OnPasswordVerify(playerid, bool:success)
{
    if(success)
    {
        CallLocalFunction("OnPlayerAuthenticated", "d", playerid);
    }
    else
    {
        if(playerLoginAttempts[playerid] >= MAX_LOGIN_ATTEMPTS)
        {
            Kick(playerid);
        }
        else
        {
            playerLoginAttempts[playerid] += 1;

            new message[40];
            format(message, sizeof message,
                "Wrong password! Try again! (%d/%d)",
                playerLoginAttempts[playerid], MAX_LOGIN_ATTEMPTS
            );
            SendClientMessage(playerid, COLOR_ERROR, message);

            PromptLoginDialog(playerid);
        }
    }
}
