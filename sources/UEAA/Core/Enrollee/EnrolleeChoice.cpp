#include <UEAA/BuildConfiguration.hpp>
#include "EnrolleeChoice.hpp"
#include <UEAA/Utils/CStringToHash.hpp>

namespace UEAA
{
EnrolleeChoice::EnrolleeChoice () :
    faculty_ (0),
    specialty_ (0)
{

}

EnrolleeChoice::EnrolleeChoice (unsigned faculty, unsigned specialty) :
    faculty_ (faculty),
    specialty_ (specialty)
{

}

EnrolleeChoice::~EnrolleeChoice ()
{

}

unsigned int EnrolleeChoice::GetFaculty () const
{
    return faculty_;
}

void EnrolleeChoice::SetFaculty (unsigned faculty)
{
    faculty_ = faculty;
}

unsigned int EnrolleeChoice::GetSpecialty () const
{
    return specialty_;
}

void EnrolleeChoice::SetSpecialty (unsigned specialty)
{
    specialty_ = specialty;
}

void EnrolleeChoice::SaveToXML (tinyxml2::XMLDocument &document, tinyxml2::XMLElement *output, DeHashTable *deHashTable) const
{
    output->SetAttribute ("faculty", deHashTable->DeHash (faculty_).c_str ());
    output->SetAttribute ("specialty", deHashTable->DeHash (specialty_).c_str ());
}

void EnrolleeChoice::LoadFromXML (tinyxml2::XMLElement *input, DeHashTable *deHashTable)
{
    faculty_ = CStringToHash (input->Attribute ("faculty"), deHashTable);
    specialty_ = CStringToHash (input->Attribute ("specialty"), deHashTable);
}

bool EnrolleeChoice::operator == (const EnrolleeChoice &rhs) const
{
    return faculty_ == rhs.faculty_ &&
           specialty_ == rhs.specialty_;
}

bool EnrolleeChoice::operator != (const EnrolleeChoice &rhs) const
{
    return !(rhs == *this);
}
}
