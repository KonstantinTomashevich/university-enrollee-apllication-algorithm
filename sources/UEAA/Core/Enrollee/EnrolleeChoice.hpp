#pragma once
#include <UEAA/Utils/DeHashTable.hpp>
#include <UEAA/Utils/XMLSerializable.hpp>
#include <Dependencies/TinyXML2/tinyxml2.h>

namespace UEAA
{
class EnrolleeChoice : public XMLSerializable
{
public:
    EnrolleeChoice ();
    EnrolleeChoice (unsigned faculty, unsigned specialty);
    virtual ~EnrolleeChoice ();

    unsigned int GetFaculty () const;
    void SetFaculty (unsigned faculty);

    unsigned int GetSpecialty () const;
    void SetSpecialty (unsigned specialty);

    virtual void SaveToXML (tinyxml2::XMLDocument &document, tinyxml2::XMLElement *output, DeHashTable *deHashTable) const;
    virtual void LoadFromXML (tinyxml2::XMLElement *input, DeHashTable *deHashTable);

    bool operator == (const EnrolleeChoice &rhs) const;
    bool operator != (const EnrolleeChoice &rhs) const;

private:
    unsigned faculty_;
    unsigned specialty_;
};
}
