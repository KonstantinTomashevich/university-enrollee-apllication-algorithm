#include "TestUniversity.hpp"
#include <iostream>
#include <cmath>
#include <time.h>

int main ()
{
    srand (time (0));
    UEAA::SharedPointer <UEAA::University> university (CreateTestUniversity ());
    if (!university.GetTrackingObject ())
    {
        std::cout << "Can't initialize university!" << std::endl;
        return 1;
    }

    UEAA::SharedPointer <UEAA::DeHashTable> deHashTable (new UEAA::DeHashTable ());
    std::cout << "Generating enrollees..." << std::endl;
    for (unsigned index = 0; index < TECH_ENROLLEES; index++)
    {
        UEAA::SharedPointer <UEAA::Enrollee> enrollee (GenerateEnrollee (true, false, deHashTable));
        PrintEnrollee (enrollee, deHashTable);
        if (!university->AddEnrollee (enrollee))
        {
            std::cout << "Can't add enrollee!" << std::endl;
            return 1;
        }
    }

    for (unsigned index = 0; index < ARTS_ENROLLEES; index++)
    {
        UEAA::SharedPointer <UEAA::Enrollee> enrollee (GenerateEnrollee (false, true, deHashTable));
        PrintEnrollee (enrollee, deHashTable);
        if (!university->AddEnrollee (enrollee))
        {
            std::cout << "Can't add enrollee!" << std::endl;
            return 1;
        }
    }

    for (unsigned index = 0; index < TECH_AND_ARTS_ENROLLEES; index++)
    {
        UEAA::SharedPointer <UEAA::Enrollee> enrollee (GenerateEnrollee (true, true, deHashTable));
        PrintEnrollee (enrollee, deHashTable);
        if (!university->AddEnrollee (enrollee))
        {
            std::cout << "Can't add enrollee!" << std::endl;
            return 1;
        }
    }

    std::cout << "Processing enrollees application...";
    std::vector <UEAA::Enrollee *> excessEnrollees = university->ProcessEnrolleesApplication ();
    std::cout << " Done!" << std::endl;

    for (auto iterator = excessEnrollees.begin (); iterator != excessEnrollees.end (); iterator++)
    {
        UEAA::Enrollee *enrollee = *iterator;
        enrollee->RefreshChoiceIndex ();
        while (enrollee->HasMoreChoices ())
        {
            UEAA::EnrolleeChoice choice = enrollee->GetCurrentChoice ();
            UEAA::Faculty *faculty = university->GetFaculty (choice.faculty_);
            if (faculty)
            {
                UEAA::Specialty *specialty = faculty->GetSpecialty (choice.specialty_);
                if (specialty)
                {
                    const std::vector <UEAA::Enrollee *> anotherEnrollees = specialty->GetEnrolleesInFreeForm ();
                    for (auto anotherEnrolleeIterator = anotherEnrollees.cbegin ();
                         anotherEnrolleeIterator != anotherEnrollees.cend (); anotherEnrolleeIterator++)
                    {
                        const UEAA::Enrollee *anotherEnrollee = *anotherEnrolleeIterator;
                        if (UEAA::IsFirstEnrolleeBetter (specialty, enrollee, anotherEnrollee))
                        {
                            std::cout << "Error! Better enrollee isn't applied!" << std::endl <<
                                         "Faculty: " << faculty->GetId () << std::endl <<
                                         "Specialty: " << specialty->GetId () << std::endl <<
                                         "Better enrollee:" << std::endl;
                            PrintEnrollee (enrollee, deHashTable);
                            std::cout << "Another enrollee:" << std::endl;
                            PrintEnrollee (anotherEnrollee, deHashTable);
                            return 1;
                        }
                    }
                }
            }
            enrollee->IncreaseChoiceIndex ();
        }
    }
    return 0;
}

UEAA::Enrollee *GenerateEnrollee (bool addTech, bool addArts, UEAA::DeHashTable *deHashTable)
{
    std::string passportNumber = std::to_string (enrolleesGeneratorCounter_);
    while (passportNumber.size () < 7)
    {
        passportNumber = "0" + passportNumber;
    }
    UEAA::Enrollee *enrollee = new UEAA::Enrollee (UEAA::CalculateEnrolleeHash ("XX", passportNumber, deHashTable));

    enrollee->SetCertificateMark (MATH_EXAM, 5 + rand () % 6);
    enrollee->SetCertificateMark (PHYSICS_EXAM, 5 + rand () % 6);
    enrollee->SetCertificateMark (LANGUAGE_EXAM, 5 + rand () % 6);
    enrollee->SetCertificateMark (SOCIETY_EXAM, 5 + rand () % 6);
    enrollee->CalculateCertificateMedianMark ();

    enrollee->SetExamResult (LANGUAGE_EXAM, 30 + rand () % 71);
    if (addTech)
    {
        enrollee->SetExamResult (MATH_EXAM, 30 + rand () % 71);
        enrollee->SetExamResult (PHYSICS_EXAM, 30 + rand () % 71);
    }

    if (addArts)
    {
        enrollee->SetExamResult (SOCIETY_EXAM, 30 + rand () % 71);
        enrollee->SetExamResult (ARTS_INTERNAL_EXAM, 30 + rand () % 71);
    }

    if (addTech)
    {
        enrollee->AddChoiceToBack (UEAA::EnrolleeChoice (
                                       TECH_FACULTY, TechFaculty::APPLIED_COMPUTER_SCIENCE, UEAA::STUDY_FORM_FREE));
        enrollee->AddChoiceToBack (UEAA::EnrolleeChoice (
                                       TECH_FACULTY, TechFaculty::COMPUTER_SCIENCE, UEAA::STUDY_FORM_FREE));
    }

    if (addArts)
    {
        enrollee->AddChoiceToBack (UEAA::EnrolleeChoice (
                                       ARTS_FACULTY, ArtsFaculty::PAINTING, UEAA::STUDY_FORM_FREE));
        enrollee->AddChoiceToBack (UEAA::EnrolleeChoice (
                                       ARTS_FACULTY, ArtsFaculty::THEATRE, UEAA::STUDY_FORM_FREE));
    }
    enrolleesGeneratorCounter_++;
    return enrollee;
}

UEAA::University *CreateTestUniversity ()
{
    UEAA::University *university = new UEAA::University ("TestUniversity");
    UEAA::SharedPointer <UEAA::Faculty> techFaculty (CreateTechFaculty ());
    if (!university->AddFaculty (techFaculty))
    {
        std::cout << "Can't add tech faculty!" << std::endl;
        delete university;
        return 0;
    }

    UEAA::SharedPointer <UEAA::Faculty> artsFaculty (CreateArtsFaculty ());
    if (!university->AddFaculty (artsFaculty))
    {
        std::cout << "Can't add arts faculty!" << std::endl;
        delete university;
        return 0;
    }
    return university;
}

UEAA::Faculty *CreateTechFaculty ()
{
    UEAA::Faculty *techFaculty = new UEAA::Faculty (TECH_FACULTY);
    UEAA::SharedPointer <UEAA::Specialty> acsSpecialty (
                CreateTechSpecialty (TechFaculty::APPLIED_COMPUTER_SCIENCE, TechFaculty::ACS_MAX_ENROLLEES_IN_FREE_FORM));
    if (!techFaculty->AddSpecialty (acsSpecialty))
    {
        std::cout << "Can't add ACS specialty!" << std::endl;
        delete techFaculty;
        return 0;
    }

    UEAA::SharedPointer <UEAA::Specialty> csSpecialty (
                CreateTechSpecialty (TechFaculty::COMPUTER_SCIENCE, TechFaculty::CS_MAX_ENROLLEES_IN_FREE_FORM));
    if (!techFaculty->AddSpecialty (csSpecialty))
    {
        std::cout << "Can't add CS specialty!" << std::endl;
        delete techFaculty;
        return 0;
    }
    return techFaculty;
}

UEAA::Specialty *CreateTechSpecialty (unsigned id, unsigned maxFreeEnrollees)
{
    UEAA::Specialty *specialty = new UEAA::Specialty (id);
    specialty->SetMaxEnrolleesInFreeForm (maxFreeEnrollees);
    specialty->SetMaxEnrolleesInPaidForm (0);

    {
        std::vector <std::pair <bool, std::vector <unsigned> > > exams;
        {
            std::vector <unsigned> exam;
            exam.push_back (MATH_EXAM);
            exams.push_back (std::pair <bool, std::vector <unsigned> > (true, exam));
        }

        {
            std::vector <unsigned> exam;
            exam.push_back (PHYSICS_EXAM);
            exams.push_back (std::pair <bool, std::vector <unsigned> > (true, exam));
        }

        {
            std::vector <unsigned> exam;
            exam.push_back (LANGUAGE_EXAM);
            exams.push_back (std::pair <bool, std::vector <unsigned> > (false, exam));
        }

        specialty->SetRequiredExams (exams);
    }

    {
        std::vector <unsigned> certPriority;
        certPriority.push_back (MATH_EXAM);
        certPriority.push_back (PHYSICS_EXAM);
        specialty->SetMarksInCertificatePriority (certPriority);
    }
    return specialty;
}

UEAA::Faculty *CreateArtsFaculty ()
{
    UEAA::Faculty *artsFaculty = new UEAA::Faculty (ARTS_FACULTY);
    UEAA::SharedPointer <UEAA::Specialty> paintingSpecialty (
                CreateArtsSpecialty (ArtsFaculty::PAINTING, ArtsFaculty::PAINTING_MAX_ENROLLEES_IN_FREE_FORM));
    if (!artsFaculty->AddSpecialty (paintingSpecialty))
    {
        std::cout << "Can't add Painting specialty!" << std::endl;
        delete artsFaculty;
        return 0;
    }

    UEAA::SharedPointer <UEAA::Specialty> theatreSpecialty (
                CreateArtsSpecialty (ArtsFaculty::THEATRE, ArtsFaculty::THEATRE_MAX_ENROLLEES_IN_FREE_FORM));
    if (!artsFaculty->AddSpecialty (theatreSpecialty))
    {
        std::cout << "Can't add Theatre specialty!" << std::endl;
        delete artsFaculty;
        return 0;
    }
    return artsFaculty;
}

UEAA::Specialty *CreateArtsSpecialty (unsigned id, unsigned maxFreeEnrollees)
{
    UEAA::Specialty *specialty = new UEAA::Specialty (id);
    specialty->SetMaxEnrolleesInFreeForm (maxFreeEnrollees);
    specialty->SetMaxEnrolleesInPaidForm (0);

    {
        std::vector <std::pair <bool, std::vector <unsigned> > > exams;
        {
            std::vector <unsigned> exam;
            exam.push_back (ARTS_INTERNAL_EXAM);
            exams.push_back (std::pair <bool, std::vector <unsigned> > (true, exam));
        }

        {
            std::vector <unsigned> exam;
            exam.push_back (SOCIETY_EXAM);
            exams.push_back (std::pair <bool, std::vector <unsigned> > (true, exam));
        }

        {
            std::vector <unsigned> exam;
            exam.push_back (LANGUAGE_EXAM);
            exams.push_back (std::pair <bool, std::vector <unsigned> > (false, exam));
        }

        specialty->SetRequiredExams (exams);
    }

    {
        std::vector <unsigned> certPriority;
        certPriority.push_back (SOCIETY_EXAM);
        specialty->SetMarksInCertificatePriority (certPriority);
    }
    return specialty;
}

void PrintEnrollee (const UEAA::Enrollee *enrollee, UEAA::DeHashTable *deHashTable)
{
    std::cout << "Enrollee " << deHashTable->DeHash (enrollee->GetId ()) << ":" << std::endl <<
                 "    Median:   " << enrollee->GetCertificateMedianMark () * 1 << std::endl <<
                 "    Math:     " << enrollee->GetExamResult (MATH_EXAM) * 1 << std::endl <<
                 "    Physics:  " << enrollee->GetExamResult (PHYSICS_EXAM) * 1 << std::endl <<
                 "    Language: " << enrollee->GetExamResult (LANGUAGE_EXAM) * 1 << std::endl <<
                 "    Society:  " << enrollee->GetExamResult (SOCIETY_EXAM) * 1 << std::endl <<
                 "    Arts:     " << enrollee->GetExamResult (ARTS_INTERNAL_EXAM) * 1 << std::endl <<
                 std::endl;
}
