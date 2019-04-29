// storage-businesses provides an API to set and retrieve business-related
// information to and from the database.

// -
// External Packages
// -

#include <a_mysql>

// -
// Declarations
// -

// OnBusinessLoaded is called each time LoadBusinesses retrieves a businesses
// information.
forward OnBusinessLoaded(id, Float:x, Float:y, Float:z, interior);

// -
// API
// -

// LoadBusinesses retrieves information about all businesses and calls
// OnBusinessLoaded for each individual business.
stock LoadBusinesses(MySQL:handle)
{
    new id, Float:x, Float:y, Float:z, interior;
    new Cache:cache = mysql_query(handle, "SELECT * FROM `businesses`;");
    for(new i = 0, j = cache_num_rows(); i < j; i++)
    {
        cache_get_value_name_int(i, "id", id);
        cache_get_value_name_float(i, "x", x);
        cache_get_value_name_float(i, "y", y);
        cache_get_value_name_float(i, "z", z);
        cache_get_value_name_int(i, "interior", interior);

        CallLocalFunction("OnBusinessLoaded", "dfffd", id, x, y, z, interior);
    }
    cache_delete(cache);
}
