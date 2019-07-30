// The vehicles package handles vehicle initilization and termination.

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

#include "vehicles-spawn"
#include "vehicles-utils"
#include "vehicles-surfing"
#include "vehicles-spray"

static RegisterVehicleData(vehicleid, Float:x, Float:y, Float:z, Float:rotation)
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
        print("[ERROR] Failed to create vehicle (vehicle limit reached or invalid vehicle model ID passed)");
    }
}

// - Hooks

#include <YSI_Coding\y_hooks>

hook function CreateVehicle(vehicletype, Float:x, Float:y, Float:z, Float:rotation, color1, color2, respawn_delay, addsiren)
{
    new vehicleid = continue(vehicletype, x, y, z, rotation, color1, color2, respawn_delay, addsiren);
    RegisterVehicleData(vehicleid, x, y, z, rotation);

    return vehicleid;
}

hook function AddStaticVehicle(modelid, Float:x, Float:y, Float:z, Float:rotation, color1, color2)
{
    new vehicleid = continue(modelid, x, y, z, rotation, color1, color2);
    RegisterVehicleData(vehicleid, x, y, z, rotation);

    return vehicleid;
}

hook function AddStaticVehicleEx(modelid, Float:x, Float:y, Float:z, Float:rotation, color1, color2, respawn_delay, addsiren)
{
    new vehicleid = continue(modelid, x, y, z, rotation, color1, color2, respawn_delay, addsiren);
    RegisterVehicleData(vehicleid, x, y, z, rotation);

    return vehicleid;
}

hook function DestroyVehicle(vehicleid)
{
    new returnValue = continue(vehicleid);

    if(returnValue)
    {
        CallLocalFunction("OnVehicleTerminated", "i", vehicleid);
    }

    return returnValue;
}

hook function GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective)
{
    new _engine, _lights, _alarm, _doors, _bonnet, _boot, _objective;
    continue(vehicleid, _engine, _lights, _alarm, _doors, _bonnet, _boot, _objective);

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
