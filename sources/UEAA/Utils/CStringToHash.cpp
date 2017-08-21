#include <UEAA/BuildConfiguration.hpp>
#include "CStringToHash.hpp"
#include <UEAA/Utils/DeHashTable.hpp>
#include <assert.h>

#include <iostream>

namespace UEAA
{
unsigned CStringToHash (const char *cString, DeHashTable *deHashTable)
{
    unsigned hash = 0;
    if (!cString)
    {
        return 0;
    }

    unsigned cStringSize = 0;
    while (*cString)
    {
        unsigned char character = *cString;
        // Update a hash with the given 8-bit value using the SDBM algorithm.
        hash = character + (hash << 6) + (hash << 16) - hash;
        cString++;
        cStringSize++;
    }

    if (deHashTable)
    {
        bool collision = !deHashTable->RegisterHash (hash, std::string (cString - cStringSize));
        assert (!collision);
    }
    return hash;
}
}
