#include <UEAA/BuildConfiguration.hpp>
#include "EnrolleeChoice.hpp"

namespace UEAA
{
EnrolleeChoice::EnrolleeChoice () :
    faculty_ (0),
    specialty_ (0),
    studyForm_ (STUDY_FORM_FREE)
{

}

EnrolleeChoice::EnrolleeChoice (unsigned faculty, unsigned specialty, StudyForm studyForm) :
    faculty_ (faculty),
    specialty_ (specialty),
    studyForm_ (studyForm)
{

}

void EnrolleeChoice::SaveToXML (tinyxml2::XMLElement *output, DeHashTable *deHashTable) const
{
    output->SetAttribute ("faculty", deHashTable->DeHash (faculty_).c_str ());
    output->SetAttribute ("specialty", deHashTable->DeHash (specialty_).c_str ());
    output->SetAttribute ("studyForm", studyForm_);
}
}
