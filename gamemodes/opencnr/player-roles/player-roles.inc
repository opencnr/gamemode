// roles serves the role-selection dialog and provides an API for retrieving
// role-related information.

// -
// External Packages
// -

#include <Pawn.CMD>
#include <easyDialog>

// -
// Declarations
// -

enum /* Player Roles */
{
    PLAYER_ROLE_COP,
    PLAYER_ROLE_ROBBER
};

// Player variables
static playerRole[MAX_PLAYERS];
static lastRoleChange[MAX_PLAYERS];

// -
// Internal Packages
// -

#include <player-knockout>
#include <player-prison>

// -
// API
// -

// OnPlayerSetRole is called when a players role has been set by SetPlayerRole.
forward OnPlayerSetRole(playerid, role);

// PromptRoleSelection prompts the role/class selection dialog where the user
// can select his role.
stock PromptRoleSelection(playerid)
{
    Dialog_Show(playerid, "OnRoleDialog", DIALOG_STYLE_LIST,
        "Select a role!",
        "Cop\nRobber",
        "Spawn", "Exit"
    );
}

// SetPlayerRole sets the specified players role.
stock SetPlayerRole(playerid, role)
{
    playerRole[playerid] = role;
    lastRoleChange[playerid] = gettime();

    CallLocalFunction("OnPlayerSetRole", "dd", playerid, role);
}

// GetPlayerRole returns the specified users role ID.
stock GetPlayerRole(playerid)
{
    return playerRole[playerid];
}

// GetRolePlayerCount returns the amount of players that have the specified
// role.
stock GetRolePlayerCount(roleID)
{
    new count;
    foreach(new p : Player)
    {
        if(playerRole[p] == roleID)
        {
            count++;
        }
    }

    return count;
}

// -
// Internal
// -

#include "player-roles-cops"
#include "player-roles-robbers"
#include <YSI\y_hooks>

hook OnPlayerConnect(playerid)
{
    // Initializing and resetting player variables.
    lastRoleChange[playerid] = 0;
    playerRole[playerid] = -1;

    return 1;
}

Dialog:OnRoleDialog(playerid, response, listitem, inputtext[])
{
    if(response)
    {
        if(GetPlayerRole(playerid) != listitem)
        {
            if(lastRoleChange[playerid] + 60 <= gettime())
            {
                if(GetPlayerWantedLevel(playerid) == 0)
                {
                    if(!IsPlayerKnockedOut(playerid))
                    {
                        if(!IsPlayerArrested(playerid))
                        {
                            SetPlayerRole(playerid, listitem);
                        }
                        else
                        {
                            SendClientMessage(playerid, COLOR_ERROR, " >  You can not change your role while arrested...");
                        }
                    }
                    else
                    {
                        SendClientMessage(playerid, COLOR_ERROR, " >  You can not change your role while knocked out...");
                    }
                }
                else
                {
                    SendClientMessage(playerid, COLOR_ERROR, " >  You can not change your role while wanted...");
                }
            }
            else
            {
                SendClientMessage(playerid, COLOR_ERROR, " >  You can only change your role once a minute...");
            }
        }
        else
        {
            SendClientMessage(playerid, COLOR_ERROR, " >  You already have this role!");
        }
    }
    else
    {
        if(GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
        {
            Kick(playerid);
        }
    }

    return 1;
}

CMD:role(playerid, params[])
{
    PromptRoleSelection(playerid);
    return 1;
}

CMD:r(playerid, params[])
{
    new message[145];
    if(!sscanf(params, "s[145]", message))
    {
        new role = GetPlayerRole(playerid),
        output[145], playerName[MAX_PLAYER_NAME + 1];

        GetPlayerName(playerid, playerName, sizeof playerName);
        format(output, sizeof output, "[RADIO] %s: %s",
            playerName, message
        );

        foreach(new p : Player)
        {
            if(GetPlayerRole(p) == role)
            {
                SendClientMessage(p, COLOR_DEFAULT, output);
            }
        }
    }

    return 1;
}
