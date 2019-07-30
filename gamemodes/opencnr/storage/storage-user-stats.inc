// storage-user-stats provides an API to set and retrieve user statistics to and
// from the database.

// -
// External Packages
// -

#include <a_mysql>

// -
// API
// -

// OnUserStatisticsLoaded is called after LoadUserStatistics was called and has
// retrieved user statistics.
forward OnUserStatisticsLoaded(userid, kills, deaths);

// CreateUserStatistics adds the user to the user_stats table in the database.
// Returns 1 on success, 0 on failure.
stock CreateUserStatistics(MySQL:handle, userid)
{
    new query[50];
    mysql_format(handle, query, sizeof query,
        "INSERT INTO user_stats (`user_id`) VALUES (%d);",
        userid
    );
    mysql_query(handle, query, false);

    if(mysql_errno() != 0)
    {
        printf("[ERROR] CreateUserStatistics query failed [handle:%d, userid:%d]", _:handle, userid);
        return 0;
    }
    return 1;
}

// LoadUserStatistics retrieves the specified users statistics from the database
// and calls OnUserStatisticsLoaded afterwards with that information when
// completed.
// Returns 1 on success, 0 on failure.
stock LoadUserStatistics(MySQL:handle, userid)
{
    new query[75];
    mysql_format(handle, query, sizeof query,
        "SELECT kills, deaths FROM user_stats WHERE user_id = %d;",
        userid
    );
    new Cache:cache = mysql_query(handle, query);

    new kills, deaths;
    cache_get_value_name_int(0, "kills", kills);
    cache_get_value_name_int(0, "deaths", deaths);

    cache_delete(cache);

    if(mysql_errno() != 0)
    {
        printf("[ERROR] LoadUserStatistics query failed [handle:%d, userid:%d]", _:handle, userid);
        return 0;
    }

    CallLocalFunction("OnUserStatisticsLoaded", "ddd", userid, kills, deaths);
    return 1;
}

// StoreUserStatistics updates the specified users statistics in the database
// with the information provided through the arguments.
// Returns 1 on success, 0 on failure.
stock StoreUserStatistics(MySQL:handle, userid, kills, deaths)
{
    new query[100];
    mysql_format(handle, query, sizeof query,
        "UPDATE user_stats SET kills=%d, deaths=%d WHERE user_id=%d;",
        kills, deaths, userid
    );
    mysql_query(handle, query, false);

    if(mysql_errno() != 0)
    {
        printf("[ERROR] StoreUserStatistics query failed [handle:%d, userid:%d]", _:handle, userid);
        return 0;
    }
    return 1;
}

// -
// Internal
// -

// - Hooks

#include <YSI_Coding\y_hooks>

hook OnUserCreated(MySQL:handle, userid)
{
    CreateUserStatistics(handle, userid);
    return 1;
}
