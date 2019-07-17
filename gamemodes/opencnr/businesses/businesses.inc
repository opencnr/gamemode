// The businesses package handles business initialization, business robberies,
// clerk initialization and provides an API to retrieve information related
// to businesses.

// -
// External Packages
// -

#include <streamer>
#include <YSI\y_iterate>

// -
// Declarations
// -

#define MAX_BUSINESSES (1024)

enum E_BUSINESS
{
    E_BUSINESS_PORTAL,
    E_BUSINESS_LAST_ROBBERY
};

new businessData[MAX_BUSINESSES][E_BUSINESS];
new Iterator:Business<MAX_BUSINESSES>;

// Player variables
static playerInBusiness[MAX_PLAYERS];

// -
// Internal Packages
// -

#include <portals>

// -
// API
// -

// OnPlayerEnterBusiness is called after the player has entered a business
// building.
forward OnPlayerEnterBusiness(playerid, businessID);

// OnPlayerExitBusiness is called BEFORE the player has exited a business
// building. Use this if you need access to businessID.
forward OnPlayerExitBusiness(playerid);

// OnBusinessInitialized is called after a business has been successfully
// loaded in and initialized.
forward OnBusinessInitialized(businessID);

// GetPlayerBusiness returns the business ID of the business that the
// player is currently in. Returns -1 if the player is not inside of a business
// or if the specified player does not exist.
stock GetPlayerBusiness(playerid)
{
    if(IsPlayerConnected(playerid))
    {
        return playerInBusiness[playerid];
    }
    return -1;
}

// IsPlayerInsideBusiness returns either true or false depending on whether
// the user is inside of a business building.
stock IsPlayerInsideBusiness(playerid)
{
    return (GetPlayerBusiness(playerid) != -1);
}

// -
// Internal
// -

#include "businesses-clerks"
#include "businesses-robbery"
#include <YSI\y_hooks>

hook OnDatabaseConnected()
{
    LoadBusinesses(db_handle);
    return 1;
}

hook OnPlayerConnect(playerid)
{
    // Initialize and resetting player variables.
    playerInBusiness[playerid] = -1;

    // Create map icons for businesses.
    foreach(new b : Business)
    {
        new portalID = businessData[b][E_BUSINESS_PORTAL];
        CreateDynamicMapIcon(
            portalData[portalID][E_PORTAL_X],
            portalData[portalID][E_PORTAL_Y],
            portalData[portalID][E_PORTAL_Z],
            52, -1
        );
    }

    return 1;
}

hook OnBusinessLoaded(id, Float:x, Float:y, Float:z, interior)
{
    new portalID = CreatePortal(x, y, z, interior);
    if(portalID != -1)
    {
        businessData[id][E_BUSINESS_PORTAL] = portalID;
        businessData[id][E_BUSINESS_LAST_ROBBERY] = 0;

        Iter_Add(Business, id);

        CallLocalFunction("OnBusinessInitialized", "d", id);
        return 1;
    }
    return 0;
}

hook OnPlayerEnterBuilding(playerid, portalID)
{
    // Check if the portal is related to a business.
    new businessID = -1;
    foreach(new b : Business)
    {
        if(businessData[b][E_BUSINESS_PORTAL] == portalID)
        {
            businessID = b;
            break;
        }
    }

    if(businessID != -1)
    {
        playerInBusiness[playerid] = businessID;
        CallLocalFunction("OnPlayerEnterBusiness", "dd", playerid, businessID);
    }

    return 1;
}

hook OnPlayerExitBuilding(playerid)
{
    PlayerExitBuilding(playerid);
    return 1;
}

hook OnPlayerPosSet(playerid)
{
    PlayerExitBuilding(playerid);
    return 1;
}

static PlayerExitBuilding(playerid)
{
    if(playerInBusiness[playerid] != -1)
    {
        CallLocalFunction("OnPlayerExitBusiness", "d", playerid);
        playerInBusiness[playerid] = -1;
    }
}
