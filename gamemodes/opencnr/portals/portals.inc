// The portals package handles portal (Enter/Exit pickups) initialization and
// provides an API for portal manipulation.

// -
// External Packages
// -

#include <PawnPlus>
#include <PickupEx>
#include <YSI\y_iterate>

// -
// Declarations
// -

#define MAX_PORTALS         (2048)
#define PORTAL_PICKUP       (1318)
#define PORTAL_VW_OFFSET    (1000)

enum E_PORTAL
{
    Float:E_PORTAL_X,
    Float:E_PORTAL_Y,
    Float:E_PORTAL_Z,
    E_PORTAL_BUILDING,
    E_PORTAL_PICKUP
}

static enum E_ENTRANCE_COORDS
{
    Float:E_ENTRANCE_COORDS_X,
    Float:E_ENTRANCE_COORDS_Y,
    Float:E_ENTRANCE_COORDS_Z,
    Float:E_ENTRANCE_COORDS_A
};

static Map:pickupToPortal;

new portalData[MAX_PORTALS][E_PORTAL];
new Iterator:Portal<MAX_PORTALS>;

// Player variables
static playerEntranceCoords[MAX_PLAYERS][E_ENTRANCE_COORDS];

// -
// Internal Packages
// -

#include <anticheat>

// -
// API
// -

// OnPlayerEnterBuilding is called whenever a player enters an entry portal.
forward OnPlayerEnterBuilding(playerid, portalID);

// OnPlayerExitBuilding is called whenever a player enters an exit portal.
forward OnPlayerExitBuilding(playerid);

// CreatePortal creates a portal at the specified location.
// Portals with building set to -1 are considered an exit portal and will
// teleport the player to the latest entry portal they've entered.
stock CreatePortal(Float:x, Float:y, Float:z, building = -1, pickupModel = PORTAL_PICKUP)
{
    if(Iter_Free(Portal) != ITER_NONE)
    {
        new portalID = Iter_Alloc(Portal);

        portalData[portalID][E_PORTAL_X] = x;
        portalData[portalID][E_PORTAL_Y] = y;
        portalData[portalID][E_PORTAL_Z] = z;
        portalData[portalID][E_PORTAL_BUILDING] = building;
        
        portalData[portalID][E_PORTAL_PICKUP] = CreatePickup(pickupModel, 1, x, y, z, (building == -1) ? -1:0);
        map_add(pickupToPortal, portalData[portalID][E_PORTAL_PICKUP], portalID);

        return portalID;
    }
    else
    {
        print("Max portal count reached...");
    }

    return -1;
}

// DestroyPortal destroys the specified portal.
stock DestroyPortal(portalID)
{
    new pickupID = portalData[portalID][E_PORTAL_PICKUP];
    map_remove(pickupToPortal, pickupID);
    DestroyPickup(pickupID);

    Iter_Remove(Portal, portalID);
}

// -
// Internal
// -

#include "portals-buildings"
#include <YSI\y_hooks>

hook OnGameModeInit()
{
    // Maps
    pickupToPortal = map_new();
    
    DisableInteriorEnterExits();

    // Spawn exit portals for buildings
    for(new i = 0; i < sizeof buildingList; i++)
    {
        CreatePortal(
            buildingList[i][E_BUILDING_X],
            buildingList[i][E_BUILDING_Y],
            buildingList[i][E_BUILDING_Z]
        );
    }

    return 1;
}

hook OnGameModeExit()
{
    // Maps
    map_delete(pickupToPortal);
    return 1;
}

hook OnPlayerConnect(playerid)
{
    // Initializing and resetting player variables.
    playerEntranceCoords[playerid][E_ENTRANCE_COORDS_X] = 0.0;
    playerEntranceCoords[playerid][E_ENTRANCE_COORDS_Y] = 0.0;
    playerEntranceCoords[playerid][E_ENTRANCE_COORDS_Z] = 0.0;
    playerEntranceCoords[playerid][E_ENTRANCE_COORDS_A] = 0.0;

    return 1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(PRESSED(KEY_WALK))
    {
        new pickupID;
        if(GetPlayerNearbyPickup(playerid, pickupID))
        {
            if(map_has_key(pickupToPortal, pickupID))
            {
                new portalID = map_get(pickupToPortal, pickupID);
                new building = portalData[portalID][E_PORTAL_BUILDING];

                if(building != -1)
                {
                    // Entrance portal
                    SetPlayerPos(playerid,
                        buildingList[building][E_BUILDING_X],
                        buildingList[building][E_BUILDING_Y],
                        buildingList[building][E_BUILDING_Z]
                    );
                    SetPlayerFacingAngle(playerid, buildingList[building][E_BUILDING_A]);
                    SetPlayerInterior(playerid, buildingList[building][E_BUILDING_INTERIOR]);
                    SetPlayerVirtualWorld(playerid, PORTAL_VW_OFFSET + portalID);

                    // Store entrance coords.
                    GetPlayerPos(playerid,
                        playerEntranceCoords[playerid][E_ENTRANCE_COORDS_X],
                        playerEntranceCoords[playerid][E_ENTRANCE_COORDS_Y],
                        playerEntranceCoords[playerid][E_ENTRANCE_COORDS_Z]
                    );
                    GetPlayerFacingAngle(playerid, playerEntranceCoords[playerid][E_ENTRANCE_COORDS_A]);

                    CallLocalFunction("OnPlayerEnterBuilding", "dd", playerid, portalID);
                }
                else
                {
                    // Exit portal
                    SetPlayerInterior(playerid, 0);
                    SetPlayerVirtualWorld(playerid, 0);

                    SetPlayerPos(playerid,
                        playerEntranceCoords[playerid][E_ENTRANCE_COORDS_X],
                        playerEntranceCoords[playerid][E_ENTRANCE_COORDS_Y],
                        playerEntranceCoords[playerid][E_ENTRANCE_COORDS_Z]
                    );
                    SetPlayerFacingAngle(playerid, playerEntranceCoords[playerid][E_ENTRANCE_COORDS_A] + 180.0);

                    CallLocalFunction("OnPlayerExitBuilding", "d", playerid);
                }

                SetCameraBehindPlayer(playerid);
            }
        }
    }
    return 1;
}
