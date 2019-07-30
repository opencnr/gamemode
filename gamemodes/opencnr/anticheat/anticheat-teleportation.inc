// The teleporation module prevents unauthorized teleportation by checking the
// players location and hooks the native position manipulation functions.

// -
// External Packages
// -

#include <Pawn.CMD>
#include <YSI_Coding\y_timers>

// -
// Declarations
// -

static enum E_PLAYER_POSITION
{
    Float:E_PLAYER_POSITION_X,
    Float:E_PLAYER_POSITION_Y,
    Float:E_PLAYER_POSITION_Z
};

// Player variables
static previousPlayerPosition[MAX_PLAYERS][E_PLAYER_POSITION];
static bool:justTeleported[MAX_PLAYERS];

// -
// Internal Packages
// -

#include <utils>

// -
// API
// -

// OnPlayerPosSet is called after the SetPlayerPos function is called.
forward OnPlayerPosSet(playerid);

// -
// Internal
// -

static UpdatePreviousPlayerPosition(playerid, Float:x, Float:y, Float:z)
{
    previousPlayerPosition[playerid][E_PLAYER_POSITION_X] = x;
    previousPlayerPosition[playerid][E_PLAYER_POSITION_Y] = y;
    previousPlayerPosition[playerid][E_PLAYER_POSITION_Z] = z;
}

ptask CheckDistanceMoved[2500](playerid)
{
    // Check if the user has just teleported and skip a tick if so. This has to
    // be done because there's a bit of a lag between setting the players
    // position and getting the players position.
    if(justTeleported[playerid])
    {
        justTeleported[playerid] = false;
        return;
    }

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);

    new Float:distance = GetDistanceBetweenPoints(
        x, y, z,
        previousPlayerPosition[playerid][E_PLAYER_POSITION_X],
        previousPlayerPosition[playerid][E_PLAYER_POSITION_Y],
        previousPlayerPosition[playerid][E_PLAYER_POSITION_Z]
    );

    if(IsPlayerSpawned(playerid))
    {
        if(distance > (IsPlayerInAnyVehicle(playerid) ? 210 : 160))
        {
            new message[75], playerName[MAX_PLAYER_NAME + 1];
            GetPlayerName(playerid, playerName, sizeof playerName);
            format(message, sizeof message, "%s has been kicked for teleportation hacking...", playerName);
            SendClientMessageToAll(COLOR_ERROR, message);
            Kick(playerid);
        }
    }

    UpdatePreviousPlayerPosition(playerid, x, y, z);
}

// - Hooks

#include <YSI_Coding\y_hooks>

hook OnPlayerConnect(playerid)
{
    // Initialize and reset player variables.
    previousPlayerPosition[playerid][E_PLAYER_POSITION_X] = 0.0;
    previousPlayerPosition[playerid][E_PLAYER_POSITION_Y] = 0.0;
    previousPlayerPosition[playerid][E_PLAYER_POSITION_Z] = 0.0;
    justTeleported[playerid] = false;

    return 1;
}

hook OnPlayerSpawn(playerid)
{
    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
    UpdatePreviousPlayerPosition(playerid, x, y, z);
    return 1;
}

hook function SetPlayerPos(playerid, Float:x, Float:y, Float:z)
{
    new returnValue = continue(playerid, x, y, z);
    if(returnValue)
    {
        UpdatePreviousPlayerPosition(playerid, x, y, z);
        justTeleported[playerid] = true;

        CallLocalFunction("OnPlayerPosSet", "d", playerid);
    }
    return returnValue;
}

hook function PutPlayerInVehicle(playerid, vehicleid, seatid)
{
    new returnValue = continue(playerid, vehicleid, seatid);
    if(returnValue)
    {
        new Float:x, Float:y, Float:z;
        GetVehiclePos(vehicleid, x, y, z);
        UpdatePreviousPlayerPosition(playerid, x, y, z);

        justTeleported[playerid] = true;
    }
    return returnValue;
}
