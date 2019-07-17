// The businesses-clerks module spawns clerks in business buildings (Interiors)
// and provides a callback which is called is called  when a player aims at a
// clerk actor.

// -
// Declarations
// -

static enum E_CLERK
{
    E_CLERK_BUILDING,
    E_CLERK_SKIN,
    Float:E_CLERK_X,
    Float:E_CLERK_Y,
    Float:E_CLERK_Z,
    Float:E_CLERK_A
};

static clerkInfo[][E_CLERK] =
{
    {0, 167, 370.8752, -4.4927, 1001.8589, 180.0},
    {1, 155, 374.6987, -117.2785, 1001.4922, 180.0},
    {2, 205, 376.4969, -65.8494, 1001.5078, 180.0},
    {3, 168, 380.8345,-188.9764,1000.6328, 180.0},
    {4, 12, 208.8704, -98.7051, 1005.2578, 180.0},
    {4, 12, 206.3535, -98.7055, 1005.2578, 180.0},
    {6, 23, 203.2244, -41.6501, 1001.8047, 180.0},
    {7, 91, 162.7857, -81.1918, 1001.8047, 180.0},
    {7, 91, 159.8443, -81.1918, 1001.8047, 180.0}
};

static bool:isActorClerk[MAX_ACTORS];

// Player variables
static playerAimAtClerkCooldown[MAX_PLAYERS];

// -
// Internal Packages
// -

#include <portals>

// -
// API
// -

// OnPlayerAimAtClerk is called when a player aims at a clerk actor. Gets called
// 3 seconds after the last call if the player is still aiming at the clerk.
forward OnPlayerAimAtClerk(playerid, actorid);

// -
// Internal
// -

#include <YSI\y_hooks>

hook OnPlayerConnect(playerid)
{
    // Initializing and resetting player variables.
    playerAimAtClerkCooldown[playerid] = 0;
    return 1;
}

hook OnPlayerUpdate(playerid)
{
    new actorid = GetPlayerTargetActor(playerid);
    if(actorid != INVALID_ACTOR_ID && isActorClerk[actorid])
    {
        new unixTimestamp = gettime();
        if(playerAimAtClerkCooldown[playerid] + 3 < unixTimestamp)
        {
            CallLocalFunction("OnPlayerAimAtClerk", "dd", playerid, actorid);
            playerAimAtClerkCooldown[playerid] = unixTimestamp;
        }
    }

    return 1;
}

hook OnBusinessInitialized(businessID)
{
    new portalID = businessData[businessID][E_BUSINESS_PORTAL];
    for(new i = 0; i < sizeof clerkInfo; i++)
    {
        if(clerkInfo[i][E_CLERK_BUILDING] == portalData[portalID][E_PORTAL_BUILDING])
        {
            new actorID = CreateActor(
                clerkInfo[i][E_CLERK_SKIN],
                clerkInfo[i][E_CLERK_X],
                clerkInfo[i][E_CLERK_Y],
                clerkInfo[i][E_CLERK_Z], 
                clerkInfo[i][E_CLERK_A]
            );
            SetActorVirtualWorld(actorID, PORTAL_VW_OFFSET + portalID);
            isActorClerk[actorID] = true;
        }
    }

    return 1;
}
