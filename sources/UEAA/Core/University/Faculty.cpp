#include <UEAA/BuildConfiguration.hpp>
#include "Faculty.hpp"

namespace UEAA
{
Faculty::Faculty (University *parent, unsigned id) :
    XMLSerializable (),
    parent_ (parent),
    id_ (id),
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

University *Faculty::GetParent () const
{
    return parent_;
}

bool Faculty::AddSpecialty (Specialty *specialty)
{
    if (!specialty)
    {
        return false;
    }

    if (specialties_.find (specialty->GetId ()) == specialties_.end ())
    {
        specialties_.emplace (specialty->GetId (), specialty);
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
        return nullptr;
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

unsigned Faculty::GetSpecialtiesCount () const
{
    return specialties_.size ();
}

Specialty *Faculty::GetSpecialtyByIndex (unsigned index) const
{
    if (index < specialties_.size ())
    {
        auto iterator = specialties_.cbegin ();
        for (; index > 0; iterator++, index--) {}
        return iterator->second.GetTrackingObject ();
    }
    else
    {
        return nullptr;
    }
}

void Faculty::RemoveAllSpecialties ()
{
    specialties_.clear ();
}

void Faculty::ClearAllSpecialtiesEnrollees ()
{
    for (auto iterator = specialties_.begin (); iterator != specialties_.end (); iterator++)
    {
        iterator->second->ClearEnrollees ();
    }
}

std::vector <Enrollee *> Faculty::GetExcessEnrollees ()
{
    std::vector <Enrollee *> excessEnrolees;
    for (auto iterator = specialties_.begin (); iterator != specialties_.end (); iterator++)
    {
        Specialty *specialty = iterator->second;
        std::vector <Enrollee *> thisSpecialtyExcess = specialty->RemoveExcessEnrollees ();
        excessEnrolees.insert (excessEnrolees.end (), thisSpecialtyExcess.begin (), thisSpecialtyExcess.end ());
    }
    return excessEnrolees;
}

void Faculty::SaveToXML (tinyxml2::XMLDocument &document, tinyxml2::XMLElement *output, DeHashTable *deHashTable) const
{
    output->SetAttribute ("name", deHashTable->DeHash (id_).c_str ());
    for (auto iterator = specialties_.cbegin (); iterator != specialties_.cend (); iterator++)
    {
        Specialty *specialty = iterator->second;
        tinyxml2::XMLElement *specialtyElement = document.NewElement ("specialty");

        output->InsertEndChild (specialtyElement);
        specialty->SaveToXML (document, specialtyElement, deHashTable);
    }
}

void Faculty::LoadFromXML (tinyxml2::XMLElement *input, DeHashTable *deHashTable)
{

}
}
