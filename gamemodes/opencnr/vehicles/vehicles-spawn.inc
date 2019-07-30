// vehicles-spawn spawns vehicles around the map on gamemode initialization.

// -
// External Packages
// -

#include <fsutil>
#include <sscanf2>

// -
// API
// -

// VEHICLES_SPAWN_INFO_DIRECTORY is the directory containing files that contain
// vehicle spawn point data.
//
// Example line : 548,314.9759,2046.6259,19.2481,157.8634,1,1 ; Cargobob
// Format : model,x,y,z,angle,color1,color2 ; comment
//
#if !defined VEHICLES_SPAWN_INFO_DIRECTORY
    #define VEHICLES_SPAWN_INFO_DIRECTORY "scriptfiles/vehicles/"
#endif

// -
// Internal
// -

// - Hooks

#include <YSI_Coding\y_hooks>

hook OnGameModeInit()
{
    loadVehiclesFromDirectory(VEHICLES_SPAWN_INFO_DIRECTORY);
    return 1;
}

static loadVehiclesFromDirectory(directory[])
{
    new
        Directory:directoryHandle = OpenDir(directory),
        itemName[256],
        ENTRY_TYPE:itemType;

    if(directoryHandle == Directory:-1)
    {
        printf("Failed to read from directory : %s", directory);
        return 0;
    }

    while(DirNext(directoryHandle, itemType, itemName))
    {
        if(itemType == E_DIRECTORY)
        {
            loadVehiclesFromDirectory(itemName);
        }
        else if(itemType == E_REGULAR)
        {
            // Start from index 12 to trim "scriptfiles/".
            loadVehiclesFromFile(itemName[12]);
        }
    }

    CloseDir(directoryHandle);
    return 1;
}

static loadVehiclesFromFile(file[])
{
    new File:fileHandle = fopen(file);

    if(!fexist(file))
    {
        printf("File does not exist : %s", file);
        return 0;
    }

    new line[256],
        vModel,
        Float:vPosX,
        Float:vPosY,
        Float:vPosZ,
        Float:vRotation,
        vColor1,
        vColor2;

    while(fread(fileHandle, line, sizeof(line)))
    {
        if(!sscanf(line, "p<,>iffffip<;>i{s[128]}", vModel, vPosX, vPosY, vPosZ, vRotation, vColor1, vColor2))
        {
            // Create static vehicle if the model of the vehicle is a train
            // (Trains can only be created statically).
            if(vModel == 537 || vModel == 538)
            {
                AddStaticVehicle(vModel, vPosX, vPosY, vPosZ, vRotation, vColor1, vColor2);
            }
            else
            {
                CreateVehicle(vModel, vPosX, vPosY, vPosZ, vRotation, vColor1, vColor2, 60 * 10);
            }
        }
        else
        {
            continue;
        }
    }

    fclose(fileHandle);
    return 1;
}
