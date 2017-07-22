#pragma once
#include <UniversityEnrolleeApllicationAlgorithm/Core/StudyForm.hpp>

namespace UniversityEnrolleeApllicationAlgorithm
{
struct EnrolleeChoise
{
    unsigned faculty_;
    unsigned specialty_;
    StudyForm studyForm_;

    EnrolleeChoise ();
    EnrolleeChoise (unsigned faculty, unsigned specialty, StudyForm studyForm);
};
const EnrolleeChoise EMPTY_ENROLLEE_CHOISE;
}
