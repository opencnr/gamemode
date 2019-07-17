// The player-auth-registration module handles registration and provides the
// registration dialog.

// -
// External Packages
// -

#include <easyDialog>
#include <samp_bcrypt>

// -
// Internal
// -

PromptRegistrationDialog(playerid)
{
    Dialog_Show(playerid, "OnRegistrationDialog", DIALOG_STYLE_PASSWORD,
        "Registration",
        "Please enter a password down below :\n",
        "Register", "Exit"
    );
}

Dialog:OnRegistrationDialog(playerid, response, listitem, inputtext[])
{
    if(response)
    {
        if(strlen(inputtext) >= 8)
        {
            bcrypt_hash(playerid, "OnPasswordHash", inputtext, BCRYPT_WORK_FACTOR);
        }
        else
        {
            SendClientMessage(playerid, COLOR_ERROR, "Your password must be at least 8 characters long!");
            PromptRegistrationDialog(playerid);
        }
    }
    else
    {
        Kick(playerid);
    }
}

forward OnPasswordHash(playerid);
public OnPasswordHash(playerid)
{
    new hash[60 + 1];
    bcrypt_get_hash(hash);
    CreateUser(db_handle, playerid, hash);
    CallLocalFunction("OnPlayerAuthenticated", "d", playerid);
}
