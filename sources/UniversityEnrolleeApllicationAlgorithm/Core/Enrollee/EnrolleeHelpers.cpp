#include "EnrolleeHelpers.hpp"
#include <math.h>

namespace UniversityEnrolleeApllicationAlgorithm
{
bool CompareEnrollees (const Specialty *specialty, const Enrollee *first, const Enrollee *second)
{
    unsigned firstScore = CalculateEnrolleeScore (specialty, first);
    unsigned secondScore = CalculateEnrolleeScore (specialty, second);

    if (firstScore != secondScore)
    {
        return firstScore > secondScore;
    }

    const std::vector <std::vector <unsigned> > &exams = specialty->GetRequiredExams ();
    for (auto iterator = exams.cbegin (); iterator != exams.cend (); iterator++)
    {
        unsigned char firstEnrolleeResult = GetEnrolleeBestResultFromExams (first,  *iterator);
        unsigned char secondEnrolleeResult = GetEnrolleeBestResultFromExams (second, *iterator);

        if (firstEnrolleeResult != secondEnrolleeResult)
        {
            return firstEnrolleeResult > secondEnrolleeResult;
        }
    }

    const std::vector <unsigned> &marksPriority = specialty->GetMarksInCertificatePriority ();
    for (auto iterator = marksPriority.cbegin (); iterator != marksPriority.cend (); iterator++)
    {
        unsigned char firstMark = first->GetCertificateMark (*iterator);
        unsigned char secondMark = second->GetCertificateMark (*iterator);

        if (firstMark != secondMark)
        {
            return firstMark > secondMark;
        }
    }

    // TODO: Not all BSU checks implemented.
    return false;
}

unsigned CalculateEnrolleeScore (const Specialty *specialty, const Enrollee *enrollee)
{
    const std::vector <std::vector <unsigned> > &exams = specialty->GetRequiredExams ();
    unsigned score = std::round (enrollee->GetCertificateMedianMark () * 10);

    for (auto iterator = exams.cbegin (); iterator != exams.cend (); iterator++)
    {
        score += GetEnrolleeBestResultFromExams (enrollee, *iterator);
    }
    return score;
}

unsigned GetEnrolleeBestResultFromExams (const Enrollee *enrollee, const std::vector <unsigned> &exams)
{
    unsigned char best = 0;
    for (auto iterator = exams.cbegin (); iterator != exams.cend (); iterator++)
    {
        unsigned char result = enrollee->GetExamResult (*iterator);
        if (result > best)
        {
            best = result;
        }
    }
    return best;
}
}
