// storage initiates a connection between the script and the MySQL database.

// -
// External Packages
// -

#include <a_mysql>
#include <env>

// -
// API
// -

new MySQL:db_handle;

// -
// Internal
// -

#include "storage-users"
#include "storage-businesses"
#include <YSI\y_hooks>

hook OnGameModeInit()
{
    new ret, mysqlHost[128], mysqlUser[128], mysqlPassword[128], mysqlDatabase[128];
    ret = GetEnv("MYSQL_HOST", mysqlHost);
    if(!ret) strcat(mysqlHost, "localhost", sizeof mysqlHost);

    ret = GetEnv("MYSQL_USER", mysqlUser);
    if(!ret) strcat(mysqlUser, "samp", sizeof mysqlUser);

    ret = GetEnv("MYSQL_PASSWORD", mysqlPassword);
    if(!ret) strcat(mysqlPassword, "test_password", sizeof mysqlPassword);

    ret = GetEnv("MYSQL_DATABASE", mysqlDatabase);
    if(!ret) strcat(mysqlDatabase, "cnr", sizeof mysqlDatabase);

    db_handle = mysql_connect(mysqlHost, mysqlUser, mysqlPassword, mysqlDatabase);

    if(mysql_errno(db_handle) != 0)
    {
        print("[ERROR] Could not connect to database. Exiting...");
        SendRconCommand("exit");
    }

    return 1;
}

hook OnGameModeExit()
{
    mysql_close(db_handle);
    return 1;
}
