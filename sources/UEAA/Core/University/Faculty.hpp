#pragma once
#include <map>
#include <string>

#include <UEAA/Utils/XMLSerializable.hpp>
#include <UEAA/Utils/SharedPointer.hpp>
#include <UEAA/Core/University/Specialty.hpp>

namespace UEAA
{
class University;
class Faculty : public XMLSerializable
{
public:
    Faculty (University *parent, unsigned id);
    virtual ~Faculty ();

    unsigned GetId () const;
    University *GetParent () const;

    bool AddSpecialty (Specialty *specialty);
    Specialty *GetSpecialty (unsigned id) const;
    bool RemoveSpecialty (unsigned id);

    unsigned GetSpecialtiesCount () const;
    Specialty *GetSpecialtyByIndex (unsigned index) const;
    void RemoveAllSpecialties ();
    void ClearAllSpecialtiesEnrollees ();
    std::list <Enrollee *> GetExcessEnrollees ();

    virtual void SaveToXML (tinyxml2::XMLDocument &document, tinyxml2::XMLElement *output, DeHashTable *deHashTable) const;
    virtual void LoadFromXML (tinyxml2::XMLElement *input, DeHashTable *deHashTable);

    bool operator == (const Faculty &rhs) const;
    bool operator != (const Faculty &rhs) const;

private:
    University *parent_;
    unsigned id_;
    std::map <unsigned, SharedPointer <Specialty> > specialties_;
};
}
