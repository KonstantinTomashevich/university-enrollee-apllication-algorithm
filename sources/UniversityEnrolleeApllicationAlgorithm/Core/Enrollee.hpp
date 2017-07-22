#pragma once
#include <map>
#include <vector>

#include <UniversityEnrolleeApllicationAlgorithm/Core/StudyForm.hpp>
#include <UniversityEnrolleeApllicationAlgorithm/Core/EnrolleeChoise.hpp>

namespace UniversityEnrolleeApllicationAlgorithm
{
class Enrollee
{
public:
    Enrollee ();
    virtual ~Enrollee ();

    unsigned GetExamResult (unsigned examSubjectNameHash) const;
    void SetExamResult (unsigned examSubjectNameHash, unsigned examResult);

    bool HasMoreChoises () const;
    unsigned GetCurrentChoiseIndex () const;
    void IncreaseChoiceIndex ();
    void RefreshChoiseIndex ();

    unsigned GetChoisesCount () const;
    EnrolleeChoise GetCurrentChoise () const;
    void AddChoiseToBack (EnrolleeChoise choise);
    void RemoveChoiseByIndex (unsigned index);
    void SwapChoisesAtIndexes (unsigned firstIndex, unsigned secondIndex);

private:
    /// Exams results map. Key is exam subject name hash.
    std::map <unsigned, unsigned> examsResults_;
    unsigned currentChoiseIndex_;
    std::vector <EnrolleeChoise> choises_;
};
}
