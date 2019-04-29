// Utility functions for vehicles

// -
// External Packages
// -

#include <YSI\y_iterate>

// -
// Declarations
// -

enum /*Vehicle Parameters*/
{
	VEHICLE_PARAMETER_ENGINE,
	VEHICLE_PARAMETER_LIGHTS,
	VEHICLE_PARAMETER_ALARM,
	VEHICLE_PARAMETER_DOORS,
	VEHICLE_PARAMETER_BONNET,
	VEHICLE_PARAMETER_BOOT,
	VEHICLE_PARAMETER_OBJECTIVE
};

// Used by the GenerateLicensePlate function.
new licensePlateCharacters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

// -
// API
// -

// Returns the state of an individual parameter, rather than all
// parameters. This function returns -1 if the parameter hasn't been
// set before and returns -2 if the parameter specified is invalid.
stock GetVehicleParameter(vehicleid, parameter)
{
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);

	switch(parameter)
    {
        case VEHICLE_PARAMETER_ENGINE :
            return engine;
        case VEHICLE_PARAMETER_LIGHTS :
            return lights;
        case VEHICLE_PARAMETER_ALARM :
            return alarm;
        case VEHICLE_PARAMETER_DOORS :
            return doors;
        case VEHICLE_PARAMETER_BONNET :
            return bonnet;
        case VEHICLE_PARAMETER_BOOT:
            return boot;
        case VEHICLE_PARAMETER_OBJECTIVE :
            return objective;
	}

    return -2;
}

// Sets the state of an individual parameter, rather than setting
// the state of all parameters. This function returns 1 when executed
// succsesfully, and 0 when it fails to execute (This means the vehicle
// does not exist).
stock SetVehicleParameter(vehicleid, parameter, value)
{
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);

	switch(parameter)
	{
		case VEHICLE_PARAMETER_ENGINE :
            return SetVehicleParamsEx(vehicleid, value, lights, alarm, doors, bonnet, boot, objective);
		case 1 :
            return SetVehicleParamsEx(vehicleid, engine, value, alarm, doors, bonnet, boot, objective);
		case 2 :
            return SetVehicleParamsEx(vehicleid, engine, lights, value, doors, bonnet, boot, objective);
		case 3 :
            return SetVehicleParamsEx(vehicleid, engine, lights, alarm, value, bonnet, boot, objective);
		case 4 :
            return SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, value, boot, objective);
		case 5 :
            return SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, value, objective);
		case 6 :
            return SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, value);
	}

    return 0;
}

// Toggles the state of the parameter specified. If the state was previously
// 0, it would now be 1 and vice versa. This function returns the new state
// of the vehicle parameter.
stock ToggleVehicleParameter(vehicleid, parameter)
{
	new newState = !GetVehicleParameter(vehicleid, parameter);
	SetVehicleParameter(vehicleid, parameter, newState);

	return newState;
}

// Generates a string containing a license plate "number". It is constructed
// of 7 random numbers and characters.
stock GenerateLicensePlateNumber(dest[], maxlength = sizeof dest)
{
	new licensePlateNumber[7 + 1];

	for(new i = 0; i < sizeof(licensePlateNumber) - 1; i++)
	{
		format(licensePlateNumber, sizeof(licensePlateNumber), "%s%c",
			licensePlateNumber,
			licensePlateCharacters[random(sizeof licensePlateCharacters)]
		);
	}

	dest[0] = EOS;
	strcat(dest, licensePlateNumber, maxlength);
}

// Returns the vehicleid of a nearby vehicle within range of the players
// position. Returns -1 if none were found.
stock GetPlayerNearbyVehicle(playerid, Float:range = 3.0)
{
	new Float:px, Float:py, Float:pz;
	GetPlayerPos(playerid, px, py, pz);

	new nearbyVehicle = -1;
	foreach(new vehicleid : Vehicle)
	{
		if(GetVehicleDistanceFromPoint(vehicleid, px, py, pz) <= range)
		{
			nearbyVehicle = vehicleid;
			break;
		}
	}

	return nearbyVehicle;
}
