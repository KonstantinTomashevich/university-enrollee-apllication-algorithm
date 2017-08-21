#pragma once
#include <UEAA/Core/Enrollee/Enrollee.hpp>
#include <UEAA/Core/University/Specialty.hpp>

namespace UEAA
{
class DeHashTable;
bool IsFirstEnrolleeBetter (const Specialty *specialty, const Enrollee *first, const Enrollee *second);
bool CanEnrolleeChoiceSpecialty (const Specialty *specialty, const Enrollee *enrollee);
unsigned CalculateEnrolleeScore (const Specialty *specialty, const Enrollee *enrollee);
unsigned GetEnrolleeBestResultFromExams (const Enrollee *enrollee, const std::vector <unsigned> &exams);
unsigned CalculateEnrolleeHash (const std::string &passportSeries, const std::string &passportNumber, DeHashTable *deHashTable = 0);
}
