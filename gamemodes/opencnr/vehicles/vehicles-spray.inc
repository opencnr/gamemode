// The vehicles-spray module handles Pay 'n' Spray vehicle resprays.

// -
// External Packages
// -

#include <streamer>

// -
// Declarations
// -

static Float:paySprayLocations[][] =
{
    {-1904.7563, 283.7831, 40.9218},
    {-2425.9404, 1021.7295, 50.2726},
    {-100.0396, 1118.8787, 19.3008},
    {1976.3694, 2162.5247, 10.6234},
    {-1420.4727, 2583.7161, 55.8433},
    {720.1239, -457.8176, 16.0795},
    {1024.5563, -1023.7588, 31.6756},
    {2064.3779, -1831.6036, 13.1236},
    {487.9131, -1739.7072, 10.7143}
};

static lastUsedSprayShop[MAX_PLAYERS];

// -
// Internal
// -

#include <YSI\y_hooks>

hook OnPlayerConnect(playerid)
{
    // Initializing and resetting player variables.
    lastUsedSprayShop[playerid] = -1;

    for(new i = 0; i < sizeof paySprayLocations; i++)
    {
        CreateDynamicMapIcon(
            paySprayLocations[i][0],
            paySprayLocations[i][1],
            paySprayLocations[i][2],
            63, -1
        );
    }

    return 1;
}

hook OnPlayerUpdate(playerid)
{
    if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
    {
        new vehicleid = GetPlayerVehicleID(playerid), Float:vehicleHealth;
        GetVehicleHealth(vehicleid, vehicleHealth);

        if(vehicleHealth == 1000)
        {
            SetVehicleHealth(vehicleid, 999);
            
            new closestPaySpray = GetVehicleNearbyPaySpray(vehicleid);
            if(closestPaySpray != -1)
            {
                GivePlayerServerMoney(playerid, -100);
                ChangeVehicleColor(vehicleid, random(128), random(128));
                
                new wantedLevel = GetPlayerWantedLevel(playerid);
                if(wantedLevel > 0)
                {
                    if(closestPaySpray != lastUsedSprayShop[playerid])
                    {
                        lastUsedSprayShop[playerid] = closestPaySpray;
                        SetPlayerWantedLevel(playerid, wantedLevel - 2);
                    }
                    else
                    {
                        SendClientMessage(playerid, COLOR_ERROR, " >  You can't lose wanted stars in this Pay 'n' Spray right now!");
                    }
                }
            }
        }
    }

    return 1;
}

// GetVehicleNearbyPaySpray returns the paySprayLocations index of the closest
// Pay 'n' Spray. Returns -1 if the player is not in a Pay 'n' Spray.
static GetVehicleNearbyPaySpray(vehicleid)
{
    new Float:distance, Float:smallestDistance = 3000.0, closestPaySpray = -1;

    for(new i = 0; i < sizeof paySprayLocations; i++)
    {
        distance = GetVehicleDistanceFromPoint(vehicleid,
            paySprayLocations[i][0],
            paySprayLocations[i][1],
            paySprayLocations[i][2]
        );

        if(distance <= 20 && distance < smallestDistance)
        {
            smallestDistance = distance;
            closestPaySpray = i;
        }
    }

    return closestPaySpray;
}
