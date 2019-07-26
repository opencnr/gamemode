// storage-users provides an API to set and retrieve user-related information to
// and from the database.

// -
// External Packages
// -

#include <a_mysql>

// -
// API
// -

// OnUserCreated is called after a user has been created in the database using
// CreateUser.
forward OnUserCreated(MySQL:handle, userid);

// OnUserLoaded is called after LoadUser retrieved user information.
forward OnUserLoaded(playerid, userid, money, wantedLevel);

// CreateUser creates a new user in the database with the specified arguments.
// Returns 1 on success, 0 on failure.
stock CreateUser(MySQL:handle, playerid, hash[])
{
    new query[300], playerName[MAX_PLAYER_NAME + 1];
    GetPlayerName(playerid, playerName, sizeof playerName);
    mysql_format(handle, query, sizeof query,
        "INSERT INTO `users` (`username`, `password`) VALUES ('%e', '%s');",
        playerName, hash
    );
    mysql_query(handle, query, false);
    
    if(mysql_errno() != 0)
    {
        printf("[ERROR] CreateUser query failed [handle:%d, playerid:%d, hash:%s]", _:handle, playerid, hash);
        return 0;
    }

    new Cache:cache = mysql_query(handle, "SELECT id FROM users ORDER BY id DESC LIMIT 1;");
    new userid;
    cache_get_value_index_int(0, 0, userid);
    cache_delete(cache);

    CallLocalFunction("OnUserCreated", "dd", _:handle, userid);
    return 1;
}

// LoadUser retrieves the specified players user information from the database
// and calls OnUserLoaded with that information when finished.
// If you want to retrieve the users password hash, use GetUserHash instead.
// Returns 1 on success, 0 on failure.
stock LoadUser(MySQL:handle, playerid)
{
    new playerName[MAX_PLAYER_NAME + 1];
    GetPlayerName(playerid, playerName, sizeof playerName);

    new query[75];
    mysql_format(handle, query, sizeof query, "SELECT * FROM `users` WHERE `username` = '%e'", playerName);
    new Cache:cache = mysql_query(handle, query);

    new userid, money, wantedLevel;
    cache_get_value_name_int(0, "id", userid);
    cache_get_value_name_int(0, "money", money);
    cache_get_value_name_int(0, "wanted_level", wantedLevel);

    cache_delete(cache);

    if(mysql_errno() != 0)
    {
        printf("[ERROR] LoadUser query failed [handle:%d, playerid:%d]", _:handle, playerid);
        return 0;
    }

    CallLocalFunction("OnUserLoaded", "dddd", playerid, userid, money, wantedLevel);
    return 1;
}

// StoreUser updates the users information in the database with the information
// specified by arguments.
// Returns 1 on success, 0 on failure.
stock StoreUser(MySQL:handle, userid, money, wanted_level)
{
    new query[100];
    mysql_format(handle, query, sizeof query,
        "UPDATE users SET `money`='%d', `wanted_level`='%d' WHERE `id`='%d';",
        money, wanted_level, userid
    );
    mysql_query(handle, query, false);

    if(mysql_errno() != 0)
    {
        printf("[ERROR] StoreUser query failed [handle:%d, userid:%d]", _:handle, userid);
        return 0;
    }

    return 1;
}

// IsUserRegistered checks if the user is registered in the database.
// Returns 1 on success, 0 on failure.
stock IsUserRegistered(MySQL:handle, playerid, &bool:result)
{
    new playerName[MAX_PLAYER_NAME + 1];
    GetPlayerName(playerid, playerName, sizeof playerName);

    new query[100];
    mysql_format(handle, query, sizeof query, "SELECT COUNT(*) FROM `users` WHERE `username` = '%e' LIMIT 1;", playerName);
    new Cache:cache = mysql_query(handle, query);

    cache_get_value_index_int(0, 0, result);

    cache_delete(cache);

    if(mysql_errno() != 0)
    {
        printf("[ERROR] IsUserRegistered query failed [handle:%d, playerid:%d]", _:handle, playerid);
        return 0;
    }

    return 1;
}

// GetUserHash retrieves the users password hash and passes it by reference to
// the dest[] argument.
// Returns 1 on success, 0 on failure.
stock GetUserHash(MySQL:handle, playerid, dest[], maxLength = sizeof dest)
{
    new playerName[MAX_PLAYER_NAME + 1];
    GetPlayerName(playerid, playerName, sizeof playerName);
    
    new query[100];
    mysql_format(handle, query, sizeof query, "SELECT `password` FROM `users` WHERE `username` = '%e';", playerName);
    new Cache:cache = mysql_query(handle, query);

    cache_get_value_index(0, 0, dest, maxLength);

    cache_delete(cache);

    if(mysql_errno() != 0)
    {
        printf("[ERROR] GetUserHash query failed [handle:%d, playerid:%d]", _:handle, playerid);
        return 0;
    }

    return 1;
}

// -
// Internal
// -

#include "storage-user-stats"
