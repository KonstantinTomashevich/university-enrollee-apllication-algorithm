#pragma once
#include <UniversityEnrolleeApllicationAlgorithm/Core/Enrollee/Enrollee.hpp>
#include <UniversityEnrolleeApllicationAlgorithm/Core/University/Specialty.hpp>

namespace UniversityEnrolleeApllicationAlgorithm
{
/// Returns true if first enrollee better than second.
bool CompareEnrollees (const Specialty *specialty, const Enrollee *first, const Enrollee *second);
unsigned CalculateEnrolleeScore (const Specialty *specialty, const Enrollee *enrollee);
unsigned GetEnrolleeBestResultFromExams (const Enrollee *enrollee, const std::vector <unsigned> &exams);
}
