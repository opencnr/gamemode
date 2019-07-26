// player-statistics-menu constructs and displays the player statistics menu.

// -
// External Packages
// -

#include <easyDialog>
#include <humanize>

// -
// API
// -

// ConstructStatisticsList constructs a string containing all statistics, each
// one divided by a newline (\n). Then passes it to the output[] argument.
// Returns the amount of statistics/lines. Returns -1 if the player specified
// is not connected to the server.
// Example output : Kills\t24\nDeaths\t5
stock ConstructStatisticsList(playerid, output[], outputLen = sizeof output)
{
    if(IsPlayerConnected(playerid))
    {
        new kills[32], deaths[32];
        HumanizeThousand(playerStatistics[playerid][E_PLAYER_STATS_KILLS], kills);
        HumanizeThousand(playerStatistics[playerid][E_PLAYER_STATS_DEATHS], deaths);

        format(output, outputLen,
            "Kills\t%s\n\
            Deaths\t%s",
            kills, deaths
        );
        return 2;
    }
    return -1;
}

// -
// Internal
// -

static ShowPlayerStatisticsMenu(playerid)
{
    new content[100];
    ConstructStatisticsList(playerid, content);
    Dialog_Show(playerid, -1, DIALOG_STYLE_TABLIST,
        "Your statistics", content, "Close", ""
    );
}

// - Commands

CMD:stats(playerid, params[])
{
    ShowPlayerStatisticsMenu(playerid);
    return 1;
}
