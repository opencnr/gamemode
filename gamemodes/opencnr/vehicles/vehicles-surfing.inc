// vehicle-surfing prevents players from surfing on vehicles.

// -
// External Packages
// -

#include <YSI\y_timers>

// -
// Internal
// -

ptask CheckSurfing[1000](playerid)
{
    new surfedVehicleID = GetPlayerSurfingVehicleID(playerid);
    if(surfedVehicleID != INVALID_VEHICLE_ID)
    {
        new Float:vx, Float:vy, Float:vz;
        GetVehicleVelocity(surfedVehicleID, vx, vy, vz);
        if(vx + vy + vz > 0.0005)
        {
            new Float:x, Float:y, Float:z, Float:angle,
                Float:sx, Float:sy, Float:sz;
            GetVehiclePos(surfedVehicleID, x, y, z);
            GetVehicleZAngle(surfedVehicleID, angle);
            GetVehicleModelInfo(GetVehicleModel(surfedVehicleID), VEHICLE_MODEL_INFO_SIZE, sx, sy, sz);

            SetPlayerPos(playerid,
                x + (floatsin(angle, degrees) * (sy / 2)),
                y - (floatcos(angle, degrees) * (sy / 2)),
            z);
        }
    }
}
