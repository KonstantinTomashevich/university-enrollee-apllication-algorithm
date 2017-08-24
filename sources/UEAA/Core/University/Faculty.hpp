#pragma once
#include <map>
#include <string>

#include <UEAA/Utils/XMLSerializable.hpp>
#include <UEAA/Utils/SharedPointer.hpp>
#include <UEAA/Core/University/Specialty.hpp>

namespace UEAA
{
class Faculty : public XMLSerializable
{
public:
    explicit Faculty (unsigned id);
    virtual ~Faculty ();

    unsigned GetId () const;
    bool AddSpecialty (Specialty *specialty);
    Specialty *GetSpecialty (unsigned id) const;
    bool RemoveSpecialty (unsigned id);

    unsigned GetSpecialtiesCount () const;
    Specialty *GetSpecialtyByIndex (unsigned index) const;
    void RemoveAllSpecialties ();
    void ClearAllSpecialtiesEnrollees ();
    std::vector <Enrollee *> GetExcessEnrollees ();

    virtual void SaveToXML (tinyxml2::XMLDocument &document, tinyxml2::XMLElement *output, DeHashTable *deHashTable);
    virtual void LoadFromXML (tinyxml2::XMLElement *input, DeHashTable *deHashTable);

private:
    unsigned id_;
    std::map <unsigned, SharedPointer <Specialty> > specialties_;
};
}
