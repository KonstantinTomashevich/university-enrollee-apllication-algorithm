#include "EnrolleeChoise.hpp"

namespace UniversityEnrolleeApllicationAlgorithm
{
EnrolleeChoise::EnrolleeChoise () :
    faculty_ (0),
    specialty_ (0),
    studyForm_ (STUDY_FORM_FREE)
{

}

EnrolleeChoise::EnrolleeChoise (unsigned faculty, unsigned specialty, StudyForm studyForm) :
    faculty_ (faculty),
    specialty_ (specialty),
    studyForm_ (studyForm)
{

}
}
