#include "Faculty.hpp"

namespace UEAA
{
Faculty::Faculty (unsigned id, const std::string &name) :
    ReferenceCounted (),
    id_ (id),
    name_ (name),
    specialties_ ()
{

}

Faculty::~Faculty ()
{

}

unsigned Faculty::GetId () const
{
    return id_;
}

const std::string &Faculty::GetName () const
{
    return name_;
}

bool Faculty::AddSpecialty (Specialty *specialty)
{
    if (specialties_.find (specialty->GetId ()) == specialties_.end ())
    {
        specialties_ [specialty->GetId ()] = specialty;
        return true;
    }
    else
    {
        return false;
    }
}

Specialty *Faculty::GetSpecialty (unsigned id) const
{
    try
    {
        return specialties_.at (id);
    }
    catch (std::out_of_range &exception)
    {
        return 0;
    }
}

bool Faculty::RemoveSpecialty (unsigned id)
{
    auto iterator = specialties_.find (id);
    if (iterator == specialties_.end ())
    {
        return false;
    }

    specialties_.erase (iterator);
    return true;
}
}
