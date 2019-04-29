// The dev-tools package provides commands that can help during server
// development.

// -
// External Packages
// -

#include <Pawn.CMD>
#include <sscanf2>

// -
// Internal
// -

// Teleports a player to another player.
// Usage : /tp [id] OR /tp [id] [id]
CMD:tp(playerid, params[])
{
    new playerid1, playerid2;
    if(!sscanf(params, "uU(-1)", playerid1, playerid2))
    {
        new Float:x, Float:y, Float:z;
        if(playerid2 == -1) // If the second parameter has not been entered
        {
            if(GetPlayerPos(playerid1, x, y, z))
            {
                SetPlayerPos(playerid, x, y, z);
                return 1;
            }
        }
        else
        {
            if(GetPlayerPos(playerid2, x, y, z))
            {
                SetPlayerPos(playerid1, x, y, z);
                return 1;
            }
        }

        SendClientMessage(playerid, COLOR_ERROR, " >  The specified player is not connected to the server!");
    }
    else
    {
        SendClientMessage(playerid, COLOR_ERROR, " >  USAGE: /tp [id] OR /tp [id] [id]");
    }

    return 1;
}
alias:tp("teleport");

// Teleports a player to the specified coordinates.
// Usage : /tpcoords [x] [y] [z] [playerid = -1]
CMD:tpcoords(playerid, params[])
{
    new Float:x, Float:y, Float:z, receiverid;
    if(!sscanf(params, "fffU(-1)", x, y, z, receiverid))
    {
        if(receiverid == -1) // If the playerid parameter hasn't been specified.
        {
            receiverid = playerid;
        }

        if(!SetPlayerPos(receiverid, x, y, z))
        {
            SendClientMessage(playerid, COLOR_ERROR, " >  The specified player is not connected to the server!");
        }
    }
    else
    {
        SendClientMessage(playerid, COLOR_ERROR, " >  USAGE: /tpcoords [x] [y] [z] [playerid = -1]");
    }

    return 1;
}
alias:tpcoords("coord", "coords");

// Teleports a player to the specified interior.
// Usage : /tpinterior [interiorid] [playerid]
CMD:tpinterior(playerid, params[])
{
    new interiorid, receiverid;
    if(!sscanf(params, "iU(-1)", interiorid, receiverid))
    {
        if(receiverid == -1) // If the playerid parameter hasn't been specified.
        {
            receiverid = playerid;
        }

        if(!SetPlayerInterior(receiverid, interiorid))
        {
            SendClientMessage(playerid, COLOR_ERROR, " >  The specified player is not connected to the server!");
        }
    }
    else
    {
        SendClientMessage(playerid, COLOR_ERROR, " >  USAGE: /tpinterior [interiorid] [playerid]");
    }

    return 1;
}
alias:tpinterior("tpint", "gotointerior", "gotoint");

// Gives a player the specified weapon.
// Usage : /weapon [playerid] [weaponid] [ammo = 50]
CMD:weapon(playerid, params[])
{
    new receiverid, weaponid, ammo;
    if(!sscanf(params, "uiI(50)", receiverid, weaponid, ammo))
    {
        if(!GivePlayerWeapon(receiverid, weaponid, ammo))
        {
            SendClientMessage(playerid, COLOR_ERROR, " >  The specified player is not connected to the server!");
        }
    }
    else
    {
        SendClientMessage(playerid, COLOR_ERROR, " >  USAGE: /weapon [playerid] [weaponid] [ammo = 50]");
    }

    return 1;
}
alias:weapon("giveweapon");

// Creates a vehicle at the players current position and him/her in it.
// Usage : /createveh [model] [color1 = -1] [color2 = -1]
CMD:createveh(playerid, params[])
{
    new vehicletype, color1, color2;
    if(!sscanf(params, "iI(-1)I(-1)", vehicletype, color1, color2))
    {
        if(color1 == -1)
        {
            color1 = random(255);
        }
        if(color2 == -1)
        {
            color2 = random(255);
        }

        new Float:x, Float:y, Float:z, Float:ang;
        GetPlayerPos(playerid, x, y, z);
        GetPlayerFacingAngle(playerid, ang);

        new vehicleid = CreateVehicle(vehicletype, x, y, z, ang, color1, color2, -1);
        if(vehicleid != INVALID_VEHICLE_ID)
        {
            PutPlayerInVehicle(playerid, vehicleid, 0);

            new interiorid = GetPlayerInterior(playerid);
            new virtualworld = GetPlayerVirtualWorld(playerid);
            LinkVehicleToInterior(vehicleid, interiorid);
            SetVehicleVirtualWorld(vehicleid, virtualworld);
        }
        else
        {
            SendClientMessage(playerid, COLOR_ERROR, " >  Vehicle limit reached or invalid vehicle model ID passed!");
        }
    }
    else
    {
        SendClientMessage(playerid, COLOR_ERROR, " >  USAGE: /createveh [model] [color1 = -1] [color2 = -1]");
    }

    return 1;
}
alias:createveh("createvehicle", "cveh");
