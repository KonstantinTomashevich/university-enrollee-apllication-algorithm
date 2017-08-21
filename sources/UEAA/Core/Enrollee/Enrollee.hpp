#pragma once
#include <map>
#include <vector>
#include <string>

#include <UEAA/Core/Enrollee/StudyForm.hpp>
#include <UEAA/Core/Enrollee/EnrolleeChoice.hpp>
#include <UEAA/Core/Enrollee/RODType.hpp>
#include <UEAA/Utils/ReferenceCounted.hpp>

namespace UEAA
{
class Enrollee : public ReferenceCounted
{
public:
    explicit Enrollee (unsigned id);
    virtual ~Enrollee ();

    unsigned GetId () const;
    unsigned char GetExamResult (unsigned examSubjectNameHash) const;
    void SetExamResult (unsigned examSubjectNameHash, unsigned char examResult);

    bool HasMoreChoices () const;
    unsigned GetCurrentChoiceIndex () const;
    void IncreaseChoiceIndex ();
    void RefreshChoiceIndex ();

    unsigned GetChoicesCount () const;
    const std::vector <EnrolleeChoice> &GetChoices () const;

    EnrolleeChoice GetCurrentChoice () const;
    EnrolleeChoice GetChoiceByIndex (unsigned index) const;

    void AddChoiceToBack (const EnrolleeChoice &choice);
    void RemoveChoiceByIndex (unsigned index);
    void SwapChoicesAtIndexes (unsigned firstIndex, unsigned secondIndex);

    EnrolleeChoice GetLastUpdateResult () const;
    void SetLastUpdateResult (const EnrolleeChoice &lastUpdateResult);

    unsigned char GetCertificateMark (unsigned subjectNameHash) const;
    void SetCertificateMark (unsigned subjectNameHash, unsigned char mark);

    float GetCertificateMedianMark () const;
    void CalculateCertificateMedianMark ();

    bool HasSchoolGoldMedal () const;
    void SetHasSchoolGoldMedal (bool hasSchoolGoldMedal);

    unsigned GetRODSubject () const;
    void SetRODSubject (unsigned rodSubject);

    RODType GetRODType () const;
    void SetRODType (RODType rodType);

private:
    unsigned id_;
    /// Exams results map. Key is exam subject name hash. Results will be in range (0, 100].
    std::map <unsigned, unsigned char> examsResults_;
    unsigned currentChoiceIndex_;
    std::vector <EnrolleeChoice> choices_;
    EnrolleeChoice lastUpdateResult_;

    /// School education certificate marks. Key is school subject name hash. Marks will be in range (0, 10].
    std::map <unsigned, unsigned char> certificateMarks_;
    float certificateMedianMark_;

    /// Enrollees with gold medal have priority in pedagogical specialties queues.
    bool hasSchoolGoldMedal_;

    /// Enrollees with republican diploma have priority.
    unsigned rodSubject_;
    RODType rodType_;
};
}
