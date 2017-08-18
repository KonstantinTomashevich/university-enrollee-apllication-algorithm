#include "Specialty.hpp"
#include <UEAA/Core/Enrollee/Enrollee.hpp>
#include <UEAA/Core/Enrollee/EnrolleeHelpers.hpp>

namespace UEAA
{
Specialty::Specialty (unsigned id, const std::string &name) :
    ReferenceCounted (),
    id_ (id),
    name_ (name),

    requiredExams_ (),
    marksInCertificatePriority_ (),

    enrolleesInFreeForm_ (),
    enrolleesInPaidForm_ (),

    maxEnrolleesInFreeForm_ (0),
    maxEnrolleesInPaidForm_ (0)
{

}

Specialty::~Specialty ()
{

}

unsigned Specialty::GetId () const
{
    return id_;
}

const std::string &Specialty::GetName () const
{
    return name_;
}

const std::vector<std::pair<bool, std::vector<unsigned> > > &Specialty::GetRequiredExams() const
{
    return requiredExams_;
}

void Specialty::SetRequiredExams (const std::vector<std::pair<bool, std::vector<unsigned> > > &requiredExams)
{
    requiredExams_ = requiredExams;
}

const std::vector <unsigned> &Specialty::GetMarksInCertificatePriority () const
{
    return marksInCertificatePriority_;
}

void Specialty::SetMarksInCertificatePriority (const std::vector <unsigned> &marksInCertificatePriority)
{
    marksInCertificatePriority_ = marksInCertificatePriority;
}

const std::vector <Enrollee *> &Specialty::GetEnrolleesInFreeForm () const
{
    return enrolleesInFreeForm_;
}

const std::vector <Enrollee *> &Specialty::GetEnrolleesInPaidForm () const
{
    return enrolleesInPaidForm_;
}

bool Specialty::AddEnrollee (Enrollee *enrollee)
{
    EnrolleeChoice choice = enrollee->GetCurrentChoice ();
    if (choice.specialty_ != id_)
    {
        return false;
    }

    if (!CanEnrolleeChoiceSpecialty (this, enrollee))
    {
        return false;
    }

    AddEnrolleeToOrder (enrollee, (choice.studyForm_ == STUDY_FORM_FREE) ? enrolleesInFreeForm_ : enrolleesInPaidForm_);
    return true;
}

std::vector <Enrollee *> Specialty::RemoveExcessEnrollees ()
{
    std::vector <Enrollee *> excessEnrollees;
    bool freeChanged = GetExcessEnrollees (excessEnrollees, STUDY_FORM_FREE);
    bool paidChanged = GetExcessEnrollees (excessEnrollees, STUDY_FORM_PAID);

    if (freeChanged)
    {
        enrolleesInFreeForm_.erase (enrolleesInFreeForm_.begin () + maxEnrolleesInFreeForm_, enrolleesInFreeForm_.end ());
        enrolleesInFreeForm_.shrink_to_fit ();
    }

    if (paidChanged)
    {
        enrolleesInPaidForm_.erase (enrolleesInPaidForm_.begin () + maxEnrolleesInPaidForm_, enrolleesInPaidForm_.end ());
        enrolleesInPaidForm_.shrink_to_fit ();
    }
    return excessEnrollees;
}

void Specialty::ClearEnrollees ()
{
    enrolleesInFreeForm_.clear ();
    enrolleesInPaidForm_.clear ();
}

unsigned Specialty::GetMaxEnrolleesInFreeForm () const
{
    return maxEnrolleesInFreeForm_;
}

void Specialty::SetMaxEnrolleesInFreeForm (unsigned maxEnrolleesInFreeForm)
{
    maxEnrolleesInFreeForm_ = maxEnrolleesInFreeForm;
}

unsigned Specialty::GetMaxEnrolleesInPaidForm () const
{
    return maxEnrolleesInPaidForm_;
}

void Specialty::SetMaxEnrolleesInPaidForm (unsigned maxEnrolleesInPaidForm)
{
    maxEnrolleesInPaidForm_ = maxEnrolleesInPaidForm;
}

bool Specialty::GetExcessEnrollees (std::vector <Enrollee *> &output, StudyForm studyForm) const
{
    const std::vector <Enrollee *> &enrollees = (studyForm == STUDY_FORM_FREE) ? enrolleesInFreeForm_ : enrolleesInPaidForm_;
    unsigned maxEnrollees = (studyForm == STUDY_FORM_FREE) ? maxEnrolleesInFreeForm_ : maxEnrolleesInPaidForm_;
    bool changed = false;

    if (enrollees.size () > maxEnrollees)
    {
        for (auto iterator = enrollees.cbegin () + maxEnrollees;
             iterator != enrollees.cend (); iterator++)
        {
            output.push_back (*iterator);
            changed = true;
        }
    }
    return changed;
}

void Specialty::AddEnrolleeToOrder (Enrollee *enrollee, std::vector <Enrollee *> &queue)
{
    if (queue.size () == 0)
    {
        queue.push_back (enrollee);
        return;
    }
    else
    {
        for (auto iterator = queue.begin (); iterator != queue.end (); iterator++)
        {
            Enrollee *anotherEnrollee = *iterator;
            if (IsFirstEnrolleeBetter (this, enrollee, anotherEnrollee))
            {
                queue.insert (iterator, enrollee);
                return;
            }
        }

        queue.push_back (enrollee);
    }
}
}
