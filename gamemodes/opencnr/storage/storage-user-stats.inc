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

stock CreateUserStatistics(MySQL:handle, userid)
{
    new query[50];
    mysql_format(handle, query, sizeof query,
        "INSERT INTO user_stats (`user_id`) VALUES (%d);",
        userid
    );
    mysql_query(handle, query, false);
}

stock LoadUserStatistics(MySQL:handle, userid)
{
    new query[75];
    mysql_format(handle, query, sizeof query,
        "SELECT kills, deaths FROM user_stats WHERE id = %d;",
        userid
    );
    new Cache:cache = mysql_query(handle, query);

    new kills, deaths;
    cache_get_value_name_int(0, "kills", kills);
    cache_get_value_name_int(0, "deaths", deaths);

    cache_delete(cache);

    CallLocalFunction("OnUserStatisticsLoaded", "ddd", userid, kills, deaths);
}

stock StoreUserStatistics(MySQL:handle, userid, kills, deaths)
{
    new query[100];
    mysql_format(handle, query, sizeof query,
        "UPDATE user_stats SET kills=%d, deaths=%d WHERE id=%d;",
        kills, deaths, userid
    );
    mysql_query(handle, query, false);
}

// -
// Internal
// -

#include <YSI\y_hooks>

hook OnUserCreated(userid)
{
    CreateUserStatistics(db_handle, userid);
    return 1;
}
