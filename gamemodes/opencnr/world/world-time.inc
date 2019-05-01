// world-time updates the world time.

// -
// External Packages
// -

#include <YSI\y_timers>

// -
// Internal
// -

#include <YSI\y_hooks>

hook OnGameModeInit()
{
    UpdateWorldTime();
}

task UpdateWorldTimeTask[60 * 1000]()
{
    UpdateWorldTime();
}

static UpdateWorldTime()
{
    new hour, minute, second;
    gettime(hour, minute, second);

    SetWorldTime(hour);
}
