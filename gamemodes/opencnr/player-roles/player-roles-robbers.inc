// roles-robbers prepares the player for the robbers role and handles robber
// related game-features.

// -
// Declarations
// -

#define COLOR_ROBBER 0xFC6953FF

static Float:robberSpawnPositions[][] =
{
    {-2537.5488, 467.6404, 14.6094, 29.7446},
    {-2534.4631, 694.1586, 27.9910, 265.2407},
    {-2757.7715, 988.7811, 54.4633, 268.2617},
    {-2245.6167, 775.2938, 49.4453, 92.7970},
    {-1723.5865, 1075.4640, 45.3157, 266.6198},
    {-1573.6462, 1032.5775, 7.1875, 106.1943},
    {-1671.7528, 1369.6024, 7.1722, 133.4753},
    {-1997.7758, 124.6613, 27.6875, 91.8111},
    {-2159.5654, 46.1887, 35.3203, 91.2309},
    {-2275.8042, -127.5193, 35.3203, 267.0253},
    {-2724.3762, -314.7364, 7.1861, 42.6638}
};

// -
// Internal
// -

#include <YSI\y_hooks>

hook OnPlayerSetRole(playerid, role)
{
    if(role == PLAYER_ROLE_ROBBER)
    {
        new randomPosition = random(sizeof robberSpawnPositions);
        SetSpawnInfo(playerid, NO_TEAM, 29,
            robberSpawnPositions[randomPosition][0],
            robberSpawnPositions[randomPosition][1],
            robberSpawnPositions[randomPosition][2],
            robberSpawnPositions[randomPosition][3],
        29, 99999, 27, 100, 0, 0);
    }

    return 1;
}
