#pragma once
#include <UEAA/Core/Enrollee/Enrollee.hpp>
#include <UEAA/Core/Enrollee/EnrolleeHelpers.hpp>

#include <UEAA/Core/University/Specialty.hpp>
#include <UEAA/Core/University/Faculty.hpp>
#include <UEAA/Core/University/University.hpp>

#include <UEAA/Utils/SharedPointer.hpp>
#include <UEAA/Utils/CStringToHash.hpp>
#include <UEAA/Utils/DeHashTable.hpp>

const unsigned MATH_EXAM = UEAA::CStringToHash ("Math");
const unsigned PHYSICS_EXAM = UEAA::CStringToHash ("Physics");
const unsigned LANGUAGE_EXAM = UEAA::CStringToHash ("Language");
const unsigned SOCIETY_EXAM = UEAA::CStringToHash ("Society");
const unsigned ARTS_INTERNAL_EXAM = UEAA::CStringToHash ("Arts");

const unsigned TECH_FACULTY = UEAA::CStringToHash ("Tech");
namespace TechFaculty
{
const unsigned APPLIED_COMPUTER_SCIENCE_FREE = UEAA::CStringToHash ("AppliedComputerScience (Free)");
const unsigned COMPUTER_SCIENCE_FREE = UEAA::CStringToHash ("ComputerScience (Free)");

const unsigned ACS_MAX_ENROLLEES_IN_FREE = 25;
const unsigned CS_MAX_ENROLLEES_IN_FREE = 100;
}

const unsigned ARTS_FACULTY = UEAA::CStringToHash ("Arts");
namespace ArtsFaculty
{
const unsigned PAINTING_FREE = UEAA::CStringToHash ("Painting (Free)");
const unsigned THEATRE_FREE = UEAA::CStringToHash ("Theatre (Free)");

const unsigned PAINTING_MAX_ENROLLEES_IN_FREE = 50;
const unsigned THEATRE_MAX_ENROLLEES_IN_FREE = 50;
}

const unsigned TECH_ENROLLEES = 125;
const unsigned ARTS_ENROLLEES = 100;
const unsigned TECH_AND_ARTS_ENROLLEES = 100;

static unsigned enrolleesGeneratorCounter_ = 0;
UEAA::Enrollee *GenerateEnrollee (bool addTech, bool addArts, UEAA::DeHashTable *deHashTable);

UEAA::University *CreateTestUniversity ();
UEAA::Faculty *CreateTechFaculty (UEAA::University *university);
UEAA::Specialty *CreateTechSpecialty (UEAA::Faculty *faculty, unsigned id, unsigned maxEnrollees);
UEAA::Faculty *CreateArtsFaculty (UEAA::University *university);
UEAA::Specialty *CreateArtsSpecialty (UEAA::Faculty *faculty, unsigned id, unsigned maxEnrollees);
void PrintEnrollee (const UEAA::Enrollee *enrollee, UEAA::DeHashTable *deHashTable);
