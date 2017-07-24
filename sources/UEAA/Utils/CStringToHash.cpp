#include "CStringToHash.hpp"

namespace UEAA
{
unsigned CStringToHash (const char *cString)
{
    unsigned hash = 0;
    if (!cString)
    {
        return 0;
    }

    while (*cString)
    {
        unsigned char character = *cString;
        // Update a hash with the given 8-bit value using the SDBM algorithm.
        hash = character + (hash << 6) + (hash << 16) - hash;
        cString++;
    }
    return hash;
}
}
