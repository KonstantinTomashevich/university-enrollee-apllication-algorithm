#pragma once
#include <map>
#include <vector>

#include <UniversityEnrolleeApllicationAlgorithm/Core/StudyForm.hpp>
#include <UniversityEnrolleeApllicationAlgorithm/Core/EnrolleeChoise.hpp>
#include <UniversityEnrolleeApllicationAlgorithm/Utils/ReferenceCounted.hpp>

namespace UniversityEnrolleeApllicationAlgorithm
{
class Enrollee : public ReferenceCounted
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
    EnrolleeChoise GetChoiseByIndex (unsigned index) const;

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
