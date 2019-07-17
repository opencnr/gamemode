// roles-cops prepares the player for the cop role and handles police related
// game-features (Ex. arresting).

// -
// Internal Packages
// -

#include <player-knockout>
#include <player-prison>
#include <utils>

// -
// External Packages
// -

#include <YSI\y_iterate>
#include <YSI\y_timers>

// -
// Declarations
// -

static const policeVehicles[] =
{
    497, 599, 598, 597, 596, 523, 528,
    427, 490, 601
};

// -
// API
// -

stock IsPoliceVehicle(vehicleid)
{
    new model = GetVehicleModel(vehicleid);
    for(new i; i < sizeof policeVehicles; i++)
    {
        if(model == policeVehicles[i])
        {
            return 1;
        }
    }

    return 0;
}

// -
// Internal
// -

#include <YSI\y_hooks>

hook OnPlayerSetRole(playerid, role)
{
    if(role == PLAYER_ROLE_COP)
    {
        SetSpawnInfo(playerid, 1, 281, -1627.3666, 664.9507, 7.1875, 266.9398, 29, 99999, 27, 100, 0, 0);
    }

    return 1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(PRESSED(KEY_WALK))
    {
        AttemptArrest(playerid);
    }

    return 1;
}

static AttemptArrest(arresterID)
{
    if(GetPlayerRole(arresterID) == PLAYER_ROLE_COP)
    {
        foreach(new criminalID : Player)
        {
            if(IsPlayerNearby(arresterID, criminalID, 3.0))
            {
                if(GetPlayerWantedLevel(criminalID) > 0 && IsPlayerKnockedOut(criminalID))
                {
                    PutPlayerInPrison(criminalID);

                    GivePlayerServerMoney(arresterID, 3500);
                    GivePlayerServerMoney(criminalID, -500);
                }
            }
        }
        return 1;
    }
    return 0;
}

hook OnPlayerStateChange(playerid, newstate, oldstate)
{
    // Only allow cops to enter police vehicles.
    if(newstate == PLAYER_STATE_DRIVER)
    {
        new vehicleid = GetPlayerVehicleID(playerid);
        if(IsPoliceVehicle(vehicleid) && GetPlayerRole(playerid) != PLAYER_ROLE_COP)
        {
            // Eject player out of vehicle...
            new Float:x, Float:y, Float:z;
            GetPlayerPos(playerid, x, y, z);
            SetPlayerPos(playerid, x, y, z + 1.0);
        }
    }

    return 1;
}
