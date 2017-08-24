#pragma once
#include <UEAA/Utils/ReferenceCounted.hpp>
#include <UEAA/Utils/DeHashTable.hpp>
#include <Dependencies/TinyXML2/tinyxml2.h>

namespace UEAA
{
class XMLSerializable : public ReferenceCounted
{
public:
    XMLSerializable ();
    virtual ~XMLSerializable ();

    virtual void SaveToXML (tinyxml2::XMLDocument &document, tinyxml2::XMLElement *output, DeHashTable *deHashTable) const = 0;
    virtual void LoadFromXML (tinyxml2::XMLElement *input, DeHashTable *deHashTable) = 0;
};
}
