// Spawns vehicles around the map on gamemode initialization.

// -
// External Packages
// -

#include <fsutil>
#include <sscanf2>

// -
// Internal
// -

#include <YSI\y_hooks>

hook OnGameModeInit()
{
    loadVehiclesFromDirectory("scriptfiles/vehicles/");
    return Y_HOOKS_CONTINUE_RETURN_1;
}

loadVehiclesFromDirectory(directory[])
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

loadVehiclesFromFile(file[])
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
