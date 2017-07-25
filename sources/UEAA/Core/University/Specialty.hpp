#pragma once
#include <vector>
#include <UEAA/Core/Enrollee/StudyForm.hpp>
#include <UEAA/Utils/ReferenceCounted.hpp>

namespace UEAA
{
class Enrollee;
class Specialty : public ReferenceCounted
{
public:
    explicit Specialty (unsigned id);
    virtual ~Specialty ();
    unsigned GetId () const;

    const std::vector <std::pair <bool, std::vector <unsigned> > > &GetRequiredExams () const;
    void SetRequiredExams (const std::vector <std::pair <bool, std::vector <unsigned> > > &requiredExams);

    const std::vector <unsigned> &GetMarksInCertificatePriority () const;
    void SetMarksInCertificatePriority (const std::vector <unsigned> &marksInCertificatePriority);

    const std::vector <Enrollee *> &GetEnrolleesInFreeForm () const;
    const std::vector <Enrollee *> &GetEnrolleesInPaidForm () const;
    void AddEnrollee (Enrollee *enrollee);
    std::vector <Enrollee *> RemoveExcessEnrollees ();

    unsigned GetMaxEnrolleesInFreeForm () const;
    void SetMaxEnrolleesInFreeForm (unsigned maxEnrolleesInFreeForm);

    unsigned GetMaxEnrolleesInPaidForm () const;
    void SetMaxEnrolleesInPaidForm (unsigned maxEnrolleesInPaidForm);

private:
    void GetExcessEnrollees (std::vector <Enrollee *> &output, StudyForm studyForm) const;

    unsigned id_;
    /// List of required exams names hashes. Should be sorted by priority!
    /// Also if bool in pair is false, exam will be skipped if enrollees have same scores.
    /// Example.
    /// 0: Math -- highest priority
    /// 1: Physics
    /// 2: [BelarusianLanguage; RussianLanguage] -- if two exams specified, best exam will be selected for enrollees comparision.
    std::vector <std::pair <bool, std::vector <unsigned> > > requiredExams_;
    /// Priority for marks in certificate, used for comparing enrollees, which have same exams results.
    std::vector <unsigned> marksInCertificatePriority_;

    std::vector <Enrollee *> enrolleesInFreeForm_;
    std::vector <Enrollee *> enrolleesInPaidForm_;

    unsigned maxEnrolleesInFreeForm_;
    unsigned maxEnrolleesInPaidForm_;
};
}
