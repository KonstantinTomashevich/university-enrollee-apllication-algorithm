#pragma once
#include <map>
#include <string>

#include <UEAA/Utils/ReferenceCounted.hpp>
#include <UEAA/Utils/SharedPointer.hpp>
#include <UEAA/Core/University/Specialty.hpp>

namespace UEAA
{
class Faculty : public ReferenceCounted
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

private:
    unsigned id_;
    std::map <unsigned, SharedPointer <Specialty> > specialties_;
};
}
