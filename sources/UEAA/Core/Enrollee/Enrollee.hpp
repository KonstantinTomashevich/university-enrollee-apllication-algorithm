#pragma once
#include <map>
#include <list>
#include <string>
#include <vector>

#include <UEAA/Core/Enrollee/EnrolleeChoice.hpp>
#include <UEAA/Core/Enrollee/RODType.hpp>
#include <UEAA/Utils/XMLSerializable.hpp>
#include <UEAA/Utils/SharedPointer.hpp>

namespace UEAA
{
class Enrollee : public XMLSerializable
{
public:
    explicit Enrollee (unsigned id);
    virtual ~Enrollee ();

    unsigned GetId () const;
    unsigned char GetExamResult (unsigned examSubjectNameHash) const;
    void SetExamResult (unsigned examSubjectNameHash, unsigned char examResult);
    std::vector <unsigned> GetPassedExamsSubjects () const;

    bool HasMoreChoices () const;
    void StepToNextChoice ();
    void RefreshCurrentChoice ();

    unsigned GetChoicesCount () const;
    const std::list <SharedPointer <EnrolleeChoice> > &GetChoices () const;

    EnrolleeChoice *GetCurrentChoice () const;
    EnrolleeChoice *GetChoiceByIndex (unsigned index) const;

    void AddChoiceToBack (EnrolleeChoice *choice);
    void RemoveChoiceByIndex (unsigned index);
    void SwapChoicesAtIndexes (unsigned firstIndex, unsigned secondIndex);

    EnrolleeChoice *GetLastUpdateResult () const;
    void SetLastUpdateResult (EnrolleeChoice *lastUpdateResult);

    unsigned char GetCertificateMark (unsigned subjectNameHash) const;
    void SetCertificateMark (unsigned subjectNameHash, unsigned char mark);
    std::vector <unsigned> GetCertificateMarksSubjects () const;

    float GetCertificateMedianMark () const;
    void CalculateCertificateMedianMark ();

    bool HasSchoolGoldMedal () const;
    void CheckIsHasSchoolGoldMedal ();

    unsigned GetRODSubject () const;
    void SetRODSubject (unsigned rodSubject);

    RODType GetRODType () const;
    void SetRODType (RODType rodType);

    virtual void SaveToXML (tinyxml2::XMLDocument &document, tinyxml2::XMLElement *output, DeHashTable *deHashTable) const;
    virtual void LoadFromXML (tinyxml2::XMLElement *input, DeHashTable *deHashTable);
    void Clear ();

    bool operator == (const Enrollee &rhs) const;
    bool operator != (const Enrollee &rhs) const;

private:
    unsigned id_;
    /// Exams results map. Key is exam subject name hash. Results will be in range (0, 100].
    std::map <unsigned, unsigned char> examsResults_;
    std::list <SharedPointer <EnrolleeChoice> >::iterator currentChoice_;
    std::list <SharedPointer <EnrolleeChoice> > choices_;
    SharedPointer <EnrolleeChoice> lastUpdateResult_;

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
