#pragma once
#include <UEAA/Core/Enrollee/StudyForm.hpp>

namespace UEAA
{
struct EnrolleeChoice
{
    unsigned faculty_;
    unsigned specialty_;
    StudyForm studyForm_;

    EnrolleeChoice ();
    EnrolleeChoice (unsigned faculty, unsigned specialty, StudyForm studyForm);
};
const EnrolleeChoice EMPTY_ENROLLEE_CHOICE;
}
