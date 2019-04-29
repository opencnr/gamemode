// utils-players provides an API with player utility functions.

// -
// API
// -

stock IsPlayerNearby(playerid_1, playerid_2, Float:range)
{
    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid_1, x, y, z);
    return IsPlayerInRangeOfPoint(playerid_2, range, x, y, z);
}

stock IsPlayerSpawned(playerid)
{
    new pState = GetPlayerState(playerid);
    return 0 <= playerid <= MAX_PLAYERS && pState != PLAYER_STATE_NONE && pState != PLAYER_STATE_WASTED && pState != PLAYER_STATE_SPECTATING;
}
