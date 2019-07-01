// storage initiates a connection between the script and the MySQL database.

// -
// External Packages
// -

#include <a_mysql>
#include <env>

#include <YSI\y_timers>

// -
// API
// -

// OnDatabaseConnected is called when the connection to the has been
// succsesfully made.
forward OnDatabaseConnected();

new MySQL:db_handle;

// -
// Declarations
// -

#define DB_CONNECTION_ATTEMPTS  25
#define DB_CONNECTION_INTERVAL  5000

static connectionAttempts;

// -
// Internal
// -

#include "storage-users"
#include "storage-businesses"
#include <YSI\y_hooks>

hook OnGameModeInit()
{
    defer AttemptDatabaseConnection();
    return 1;
}

timer AttemptDatabaseConnection[DB_CONNECTION_INTERVAL]()
{
    if(connectionAttempts <= DB_CONNECTION_ATTEMPTS)
    {
        connectionAttempts++;

        print("Attempting to connect to the database...");

        if(ConnectToDatabase())
        {
            print("Connection to the database has been established!");
            CallLocalFunction("OnDatabaseConnected", "");
        }
        else
        {
            defer AttemptDatabaseConnection();
        }
    }
    else
    {
        print("Failed to connect to the database! Exiting...");
        SendRconCommand("exit");
    }
}

// Attempts to connect to the database and returns 0 if it fails to do so.
static ConnectToDatabase()
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
        mysql_close(db_handle);
        return 0;
    }

    return 1;
}

hook OnGameModeExit()
{
    mysql_close(db_handle);
    return 1;
}
