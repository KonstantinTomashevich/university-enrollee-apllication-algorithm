#pragma once
#include <UEAA/Core/Enrollee/Enrollee.hpp>
#include <UEAA/Core/University/Specialty.hpp>

namespace UEAA
{
/// Returns true if first enrollee better than second.
bool CompareEnrollees (const Specialty *specialty, const Enrollee *first, const Enrollee *second);
bool CanEnrolleeChoiseSpecialty (const Specialty *specialty, const Enrollee *enrollee);
unsigned CalculateEnrolleeScore (const Specialty *specialty, const Enrollee *enrollee);
unsigned GetEnrolleeBestResultFromExams (const Enrollee *enrollee, const std::vector <unsigned> &exams);
}
