// storage-users provides an API to set and retrieve user-related information to
// and from the database.

// -
// External Packages
// -

#include <a_mysql>

// -
// API
// -

// OnUserLoaded is called after LoadUser retrieved user information.
forward OnUserLoaded(playerid, userid, money);

// CreateUser creates a new user in the database with the specified arguments.
stock CreateUser(MySQL:handle, playerid, hash[])
{
    new query[175], playerName[MAX_PLAYER_NAME + 1];
    GetPlayerName(playerid, playerName, sizeof playerName);
    mysql_format(handle, query, sizeof query,
        "INSERT INTO `users` (`username`, `password`) VALUES ('%e', '%s')",
        playerName, hash
    );
    mysql_query(handle, query, false);
}

// LoadUser retrieves the specified players user information from the database
// and calls OnUserLoaded with that information when finished.
// If you want to retrieve the users password hash, use GetUserHash instead.
stock LoadUser(MySQL:handle, playerid)
{
    new query[75], playerName[MAX_PLAYER_NAME + 1];
    GetPlayerName(playerid, playerName, sizeof playerName);
    mysql_format(handle, query, sizeof query, "SELECT * FROM `users` WHERE `username` = '%e'", playerName);
    new Cache:cache = mysql_query(handle, query);

    new userid, money;
    cache_get_value_name_int(0, "id", userid);
    cache_get_value_name_int(0, "money", money);

    cache_delete(cache);

    CallLocalFunction("OnUserLoaded", "ddd", playerid, userid, money);
}

// StoreUser updates the users information in the database with the information
// specified by arguments.
stock StoreUser(MySQL:handle, userid, money, wanted_level)
{
    new query[100];
    mysql_format(handle, query, sizeof query,
        "UPDATE users SET `money`='%d', `wanted_level`='%d' WHERE `id`='%d';",
        money, wanted_level, userid
    );
    mysql_query(handle, query);
}

// IsUserRegistered checks if the user is registered in the database.
stock IsUserRegistered(MySQL:handle, playerid)
{
    new query[100], playerName[MAX_PLAYER_NAME + 1], isRegistered;
    GetPlayerName(playerid, playerName, sizeof playerName);
    mysql_format(handle, query, sizeof query, "SELECT COUNT(*) FROM `users` WHERE `username` = '%e' LIMIT 1;", playerName);
    new Cache:cache = mysql_query(handle, query);

    cache_get_value_index_int(0, 0, isRegistered);

    cache_delete(cache);

    return isRegistered;
}

// GetUserHash retrieves the users password hash and passes it by reference to
// the dest[] argument.
stock GetUserHash(MySQL:handle, playerid, dest[], maxLength = sizeof dest)
{
    new query[100], playerName[MAX_PLAYER_NAME + 1];
    GetPlayerName(playerid, playerName, sizeof playerName);
    mysql_format(handle, query, sizeof query, "SELECT `password` FROM `users` WHERE `username` = '%e';", playerName);
    new Cache:cache = mysql_query(handle, query);

    cache_get_value_index(0, 0, dest, maxLength);

    cache_delete(cache);
}

// GetUserWantedLevel retrieves the users wanted level from the database and
// returns it.
stock GetUserWantedLevel(MySQL:handle, userid)
{
    new query[75], wanted_level;
    mysql_format(handle, query, sizeof query, "SELECT `wanted_level` FROM `users` WHERE `id` = '%d';", userid);
    new Cache:cache = mysql_query(handle, query);

    cache_get_value_index_int(0, 0, wanted_level);

    cache_delete(cache);

    return wanted_level;
}
