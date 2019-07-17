// The airdrop package spawns and handles airdrops.

// -
// External Packages
// -

#include <streamer>
#include <YSI\y_timers>
#include <PickupEx>

// -
// Internal Packages
// -

#include <anticheat>

// -
// Declarations
// -

#define AIRDROP_DEFAULT_HEIGHT  100.0
#define AIRDROP_OFFSET          (8)
#define AIRDROP_FLARE_OFFSET    (-2)

static Float:airdropPositions[][3] =
{
    {-2821.2405, -385.7763, 6.9140},
    {-2900.5095, -255.0890, 3.5953},
    {-2885.8618, 14.9610, 4.8475},
    {-2837.1101, 189.9779, 11.1735},
    {-2725.3379, 287.5587, 4.2460},
    {-2605.6536, 459.3413, 14.3279},
    {-2685.5605, 1077.3474, 47.0878},
    {-2523.8303, 1291.7233, 26.5912},
    {-2599.4277, 1385.1794, 7.0440},
    {-2009.9304, 1330.1842, 7.0138},
    {-1860.9275, 1404.2405, 7.0618},
    {-1750.8000, -136.9305, 3.4378},
    {-2076.9312, -94.4965, 35.0390},
    {-2134.6316, -171.9108, 35.1943}
};

static
    Float:airdropX, Float:airdropY, Float:airdropZ,
    Float:destinationZ,
    bool:airdropActive,
    airdropMapIcon,
    flareObjectID,
    airdropObjectID = -1,
    airdropQueue,
    airdropRewardPickup;

// -
// Internal
// -

#include <YSI\y_hooks>

hook OnGameModeInit()
{
    // Setting default values.
    airdropRewardPickup = -1;
    return 1;
}

static InitializeAirdrop()
{
    if(airdropObjectID == -1)
    {
        // Selecting the position of the airdrop.
        new randomPosition = random(sizeof airdropPositions);
        destinationZ = airdropPositions[randomPosition][2];
        airdropX = airdropPositions[randomPosition][0];
        airdropY = airdropPositions[randomPosition][1];
        airdropZ = destinationZ + AIRDROP_DEFAULT_HEIGHT;

        airdropMapIcon = CreateDynamicMapIcon(airdropX, airdropY, 0.0, 5, -1, 0);

        flareObjectID = CreateObject(18728, airdropX, airdropY, destinationZ + AIRDROP_FLARE_OFFSET, 0.0, 0.0, 0.0, 100);
        airdropObjectID = CreateObject(18849, airdropX, airdropY, airdropZ + AIRDROP_OFFSET, 0.0, 0.0, 0.0, 100);

        AnnounceAirdrop();

        airdropActive = true;
    }
    else
    {
        airdropQueue++;
    }
}

static AnnounceAirdrop()
{
    new zoneName[MAX_MAP_ZONE_NAME],
        message[MAX_MAP_ZONE_NAME + 40],
        MapZone:zone;

    zone = GetMapZoneAtPoint2D(airdropX, airdropY);
    GetMapZoneName(zone, zoneName);

    format(message, sizeof message, " >  An airdrop has been spotted at %s!", zoneName);

    SendClientMessageToAll(COLOR_INFO, message);
}

task InitAirdropTimer[15 * 60 * 1000]()
{
    InitializeAirdrop();
}

task UpdateAirdropTimer[1000]()
{
    if(airdropActive)
    {
        if(airdropZ > destinationZ)
        {
            airdropZ -= 1.0;
            MoveObject(airdropObjectID, airdropX, airdropY, airdropZ + AIRDROP_OFFSET, 1.0, 0.0, 0.0, 0.0);
        }
        else
        {
            DestroyObject(flareObjectID);
            DestroyObject(airdropObjectID);

            airdropObjectID = -1;
            flareObjectID = -1;

            // Destroy the previous reward pickup if it hasn't been collected
            // yet.
            if(IsValidPickup(airdropRewardPickup))
            {
                DestroyPickup(airdropRewardPickup);
            }
            airdropRewardPickup = CreatePickup(1550, 1, airdropX, airdropY, destinationZ);

            DestroyDynamicMapIcon(airdropMapIcon);

            airdropActive = false;

            if(airdropQueue > 0)
            {
                airdropQueue--;
                InitializeAirdrop();
            }
        }
    }
}

hook OnPlayerPickUpPickup(playerid, pickupid)
{
    if(pickupid == airdropRewardPickup)
    {
        GivePlayerServerMoney(playerid, 3500);
        PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);

        DestroyPickup(pickupid);
    }
    return 1;
}
