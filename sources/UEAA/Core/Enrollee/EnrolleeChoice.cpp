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
}
