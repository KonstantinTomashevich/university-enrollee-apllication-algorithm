#include <UEAA/BuildConfiguration.hpp>
#include "DeHashTable.hpp"
#include <assert.h>

namespace UEAA
{
DeHashTable::DeHashTable () : ReferenceCounted (),
    deHashTable_ ()
{

}

DeHashTable::~DeHashTable ()
{

}

std::string DeHashTable::DeHash (unsigned hash) const
{
    try
    {
        return deHashTable_.at (hash);
    }
    catch (std::out_of_range &exception)
    {
        return "";
    }
}

bool DeHashTable::RegisterHash (unsigned hash, const std::string &string)
{
    if (deHashTable_.find (hash) == deHashTable_.end ())
    {
        deHashTable_.emplace (hash, string);
        return true;
    }
    else
    {
        std::string anotherString = deHashTable_.find (hash)->second;
        assert (anotherString == string);
        return anotherString == string;
    }
}

void DeHashTable::Clear ()
{
    deHashTable_.clear ();
}
}
