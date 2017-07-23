#pragma once
#include <UniversityEnrolleeApllicationAlgorithm/Core/Enrollee/Enrollee.hpp>
#include <UniversityEnrolleeApllicationAlgorithm/Core/University/Specialty.hpp>

namespace UniversityEnrolleeApllicationAlgorithm
{
/// Returns true if first enrollee better than second.
bool CompareEnrollees (Specialty *specialty, Enrollee *first, Enrollee *second);
unsigned CalculateEnrolleeScore (Specialty *specialty, Enrollee *enrollee);
unsigned GetEnrolleeBestResultFromExams (Enrollee *enrollee, const std::vector <unsigned> &exams);
}
