// The utils package provides an API with generic utility functions.

// -
// API
// -

// GetDistanceBetweenPoints returns the distance between the two specified
// coordinates.
stock Float:GetDistanceBetweenPoints(Float:x1, Float:y1, Float:z1, Float:x2, Float:y2, Float:z2)
{
    return VectorSize(x1-x2, y1-y2, z1-z2);
}

// -
// Internal
// -

#include "utils-players"
