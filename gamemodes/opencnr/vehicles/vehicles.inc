// The vehicles package handles vehicle initilization and termination.

// -
// External Packages
// -

#include <PawnPlus>

// -
// Declarations
// -

static enum E_VEHICLE_DATA
{
    Float:VEHICLE_HEALTH,
    Float:VEHICLE_SPAWN_X,
    Float:VEHICLE_SPAWN_Y,
    Float:VEHICLE_SPAWN_Z,
    Float:VEHICLE_SPAWN_R
}

static vehicleData[MAX_VEHICLES][E_VEHICLE_DATA];

static NativeHook:hookv_CreateVehicle;
static NativeHook:hookv_AddStaticVehicle;
static NativeHook:hookv_AddStaticVehicleEx;
static NativeHook:hookv_DestroyVehicle;
static NativeHook:hookv_GetVehicleParamsEx;

// -
// API
// -

// OnVehicleCreated is called when a vehicle has been succsesfully initialized
// using the CreateVehicle, AddStaticVehicle or AddStaticVehicleEx function.
forward OnVehicleCreated(vehicleid);

// OnVehicleTerminated is called when a vehicle has been succsesfully destroyed
// using the DestroyVehicle function.
forward OnVehicleTerminated(vehicle);

// -
// Internal
// -

#include <YSI\y_hooks>

hook OnGameModeInit()
{
    // Add hooks
    hookv_CreateVehicle = pawn_add_hook("CreateVehicle", "dffffdddd", "hookf_CreateVehicle");
    hookv_AddStaticVehicle = pawn_add_hook("AddStaticVehicle", "dffffdd", "hookf_AddStaticVehicle");
    hookv_AddStaticVehicleEx = pawn_add_hook("AddStaticVehicleEx", "dffffdddd", "hookf_AddStaticVehicleEx");
    hookv_DestroyVehicle = pawn_add_hook("DestroyVehicle", "d", "hookf_DestroyVehicle");
    hookv_GetVehicleParamsEx = pawn_add_hook("GetVehicleParamsEx", "dddddddd", "hookf_GetVehicleParamsEx");
    
    return 1;
}

#include "vehicles-spawn"
#include "vehicles-utils"
#include "vehicles-surfing"
#include "vehicles-spray"

hook OnGameModeExit()
{
    // Remove hooks
    pawn_remove_hook(hookv_CreateVehicle);
    pawn_remove_hook(hookv_AddStaticVehicle);
    pawn_remove_hook(hookv_AddStaticVehicleEx);
    pawn_remove_hook(hookv_DestroyVehicle);
    pawn_remove_hook(hookv_GetVehicleParamsEx);

    return 1;
}

forward hookf_CreateVehicle(vehicletype, Float:x, Float:y, Float:z, Float:rotation, color1, color2, respawn_delay, addsiren);
public hookf_CreateVehicle(vehicletype, Float:x, Float:y, Float:z, Float:rotation, color1, color2, respawn_delay, addsiren)
{
    new vehicleid = CreateVehicle(vehicletype, x, y, z, rotation, color1, color2, respawn_delay, addsiren);
    registerVehicleData(vehicleid, x, y, z, rotation);

    return vehicleid;
}

forward hookf_AddStaticVehicle(modelid, Float:x, Float:y, Float:z, Float:rotation, color1, color2);
public hookf_AddStaticVehicle(modelid, Float:x, Float:y, Float:z, Float:rotation, color1, color2)
{
    new vehicleid = AddStaticVehicle(modelid, x, y, z, rotation, color1, color2);
    registerVehicleData(vehicleid, x, y, z, rotation);

    return vehicleid;
}

forward hookf_AddStaticVehicleEx(modelid, Float:x, Float:y, Float:z, Float:rotation, color1, color2, respawn_delay, addsiren);
public hookf_AddStaticVehicleEx(modelid, Float:x, Float:y, Float:z, Float:rotation, color1, color2, respawn_delay, addsiren)
{
    new vehicleid = AddStaticVehicleEx(modelid, x, y, z, rotation, color1, color2, respawn_delay, addsiren);
    registerVehicleData(vehicleid, x, y, z, rotation);

    return vehicleid;
}

forward hookf_DestroyVehicle(vehicleid);
public hookf_DestroyVehicle(vehicleid)
{
    new returnValue = DestroyVehicle(vehicleid);

    if(returnValue)
    {
        CallLocalFunction("OnVehicleTerminated", "i", vehicleid);
    }

    return returnValue;
}

forward hookf_GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
public hookf_GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective)
{
    new _engine, _lights, _alarm, _doors, _bonnet, _boot, _objective;
    GetVehicleParamsEx(vehicleid, _engine, _lights, _alarm, _doors, _bonnet, _boot, _objective);

    if(_engine == -1) _engine = 0;
    if(_lights == -1) _lights = 0;
    if(_alarm == -1) _alarm = 0;
    if(_doors == -1) _doors = 0;
    if(_bonnet == -1) _bonnet = 0;
    if(_boot == -1) _boot = 0;
    if(_objective == -1) _objective = 0;

    engine = _engine;
    lights = _lights;
    alarm = _alarm;
    doors = _doors;
    bonnet = _bonnet;
    boot = _boot;
    objective = _objective;
}

registerVehicleData(vehicleid, Float:x, Float:y, Float:z, Float:rotation)
{
    if(vehicleid != INVALID_VEHICLE_ID)
    {
        vehicleData[vehicleid][VEHICLE_SPAWN_X] = x;
        vehicleData[vehicleid][VEHICLE_SPAWN_Y] = y;
        vehicleData[vehicleid][VEHICLE_SPAWN_Z] = z;
        vehicleData[vehicleid][VEHICLE_SPAWN_R] = rotation;

        new licensePlate[7 + 1];
        GenerateLicensePlateNumber(licensePlate);
        SetVehicleNumberPlate(vehicleid, licensePlate);

        CallLocalFunction("OnVehicleCreated", "i", vehicleid);
    }
    else
    {
        print("Failed to create vehicle (vehicle limit reached or invalid vehicle model ID passed)");
    }
}
