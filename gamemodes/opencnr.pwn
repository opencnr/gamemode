// OpenCnR is a collaborative open-source SA-MP Cops and Robbers game-mode!

// -
// External Packages
// -

#include <a_samp>
#include <sscanf2>
#include <progress2>

// -
// API
// -

#define YSI_NO_HEAP_MALLOC

#define OPENCNR_VERSION "Alpha R1"

#if !defined MAX_PLAYERS
    #define MAX_PLAYERS (32)
#endif

// - Colors

// COLOR_ERROR is the color used to display error messages.
#if !defined COLOR_ERROR
    #define COLOR_ERROR 0xFC3F3FFF
#endif
// SCOLOR_ERROR is the string version of COLOR_ERROR.
#if !defined SCOLOR_ERROR
    #define SCOLOR_ERROR "{FC3F3F}"
#endif

// COLOR_INFO is the color used to display informative messages.
#if !defined COLOR_INFO
    #define COLOR_INFO 0x2ABAFCFF
#endif
// SCOLOR_INFO is the string version of COLOR_INFO.
#if !defined SCOLOR_INFO
    #define SCOLOR_INFO "{2ABAFC}"
#endif

// COLOR_DEFAULT is the "default" blue-like SA-MP color used in the "Connecting
// to" message when joining a server.
#if !defined COLOR_DEFAULT
    #define COLOR_DEFAULT 0xA9C4E4FF
#endif
// SCOLOR_DEFAULT is the string version of COLOR_DEFAULT.
#if !defined SCOLOR_DEFAULT
    #define SCOLOR_DEFAULT "{A9C4E4}"
#endif

// COLOR_DEFAULT2 is the "default" white-like SA-MP color used in the
// "Connecting to" message when joining a server.
#if !defined COLOR_DEFAULT2
    #define COLOR_DEFAULT2 0xB9C9BFFF
#endif
// SCOLOR_DEFAULT2 is the string version of COLOR_DEFAULT2.
#if !defined SCOLOR_DEFAULT2
    #define SCOLOR_DEFAULT2 "{B9C9BF}"
#endif

// COLOR_ROBBER is the color a player will become as a robber. Used for minimap,
// map and username / nickname.
#if !defined COLOR_ROBBER
    #define COLOR_ROBBER 0xFC6953FF
#endif
// SCOLOR_ROBBER is the string version of COLOR_ROBBER.
#if !defined SCOLOR_ROBBER
    #define SCOLOR_ROBBER "{FC6953}"
#endif

// -
// Internal
// -

#include <storage>
#include <anticheat>
#include <world>
#include <player>
#include <businesses>
#include <vehicles>
#include <airdrop>
#include <vending-machines>

#if defined DEV_BUILD
    #include <dev-tools>
#endif

public OnGameModeInit()
{
    SetGameModeText("OpenCnR "OPENCNR_VERSION);
    EnableStuntBonusForAll(0);
    UsePlayerPedAnims();

    return 1;
}

main(){}
