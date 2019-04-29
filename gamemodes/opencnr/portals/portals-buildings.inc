// The portals-buildings module provides an array containing information
// about buildings (interiors).

// -
// Declarations
// -

enum E_BUILDING
{
    E_BUILDING_INTERIOR,
    Float:E_BUILDING_X,
    Float:E_BUILDING_Y,
    Float:E_BUILDING_Z,
    Float:E_BUILDING_A
};

new buildingList[][E_BUILDING] =
{
    {9, 364.9135, -11.3630, 1001.8516, 0.0}, // Cluckin' Bell
    {5, 372.4042, -133.3975, 1001.4922, 0.0}, // Well Stacked Pizza
    {10, 363.1638, -74.9964, 1001.5078, 307.0}, // Burger Shot
    {17, 377.1085, -193.1794, 1000.6401, 0.0}, // Rusty browns donuts
    {15, 207.7402, -110.8448, 1005.1328, 0.0}, // Binco
    {3, 207.0849, -140.1831, 1003.5078, 0.0}, // Prolaps
    {1, 203.7604, -50.1198, 1001.8047, 0.0}, // Suburban
    {18, 161.4370, -96.6782, 1001.8047, 0.0} // Zip
};
