// world-time updates the world time.

// -
// External Packages
// -

#include <YSI_Coding\y_timers>

// -
// Internal
// -

static UpdateWorldTime()
{
    new hour, minute, second;
    gettime(hour, minute, second);

    SetWorldTime(hour);
}

// - Timers

task UpdateWorldTimeTask[60 * 1000]()
{
    UpdateWorldTime();
}

// - Hooks

#include <YSI_Coding\y_hooks>

hook OnGameModeInit()
{
    UpdateWorldTime();
}
