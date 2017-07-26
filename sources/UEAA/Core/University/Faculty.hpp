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
    Faculty (unsigned id, const std::string &name);
    virtual ~Faculty ();

    unsigned GetId () const;
    const std::string &GetName () const;

    bool AddSpecialty (Specialty *specialty);
    Specialty *GetSpecialty (unsigned id) const;
    bool RemoveSpecialty (unsigned id);

private:
    unsigned id_;
    std::string name_;
    std::map <unsigned, SharedPointer <Specialty> > specialties_;
};
}
