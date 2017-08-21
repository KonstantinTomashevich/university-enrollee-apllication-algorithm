#pragma once
#include <map>
#include <string>
#include <UEAA/Utils/ReferenceCounted.hpp>

namespace UEAA
{
class DeHashTable : public ReferenceCounted
{
public:
    DeHashTable ();
    virtual ~DeHashTable ();

    std::string DeHash (unsigned hash) const;
    bool RegisterHash (unsigned hash, const std::string &string);
    void Clear ();

private:
    std::map <unsigned, std::string> deHashTable_;
};
}
