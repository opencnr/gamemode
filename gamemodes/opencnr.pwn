#include <a_samp>

// -
// Compiletime Configuration
// -

#undef MAX_PLAYERS
#define MAX_PLAYERS (32)

// -
// Macros
// -

#define HOLDING(%0) \
    ((newkeys & (%0)) == (%0))

#define PRESSED(%0) \
    (((newkeys & (%0)) == (%0)) && ((oldkeys & (%0)) != (%0)))

#define PRESSING(%0,%1) \
    (%0 & (%1))

#define RELEASED(%0) \
    (((newkeys & (%0)) != (%0)) && ((oldkeys & (%0)) == (%0)))

// -
// Colors
// -

#define COLOR_ERROR     0xFC3F3FFF
#define COLOR_INFO      0x2ABAFCFF
#define COLOR_DEFAULT   0xA9C4E4FF

#define SCOLOR_ERROR    "{FC3F3F}"
#define SCOLOR_INFO     "{2ABAFC}"
#define SCOLOR_DEFAULT	"{A9C4E4}"

// -
// Externals
// -

#include <sscanf2>
#include <progress2>

// -
// Internal
// -

#include <storage>
#include <anticheat>
#include <player>
#include <businesses>
#include <vehicles>
#include <knockout>
#include <nametag>
#include <airdrop>

#if defined DEV_BUILD
    #include <dev-tools>
#endif

#include <YSI\y_hooks>

hook OnGameModeInit()
{
    SetGameModeText("OpenCnR Alpha R1");
    EnableStuntBonusForAll(0);
    UsePlayerPedAnims();
    return 1;
}

main(){}
