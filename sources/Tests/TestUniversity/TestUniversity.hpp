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
const unsigned APPLIED_COMPUTER_SCIENCE = UEAA::CStringToHash ("AppliedComputerScience");
const unsigned COMPUTER_SCIENCE = UEAA::CStringToHash ("ComputerScience");

const unsigned ACS_MAX_ENROLLEES_IN_FREE_FORM = 25;
const unsigned CS_MAX_ENROLLEES_IN_FREE_FORM = 100;
}

const unsigned ARTS_FACULTY = UEAA::CStringToHash ("Arts");
namespace ArtsFaculty
{
const unsigned PAINTING = UEAA::CStringToHash ("Painting");
const unsigned THEATRE = UEAA::CStringToHash ("Theatre");

const unsigned PAINTING_MAX_ENROLLEES_IN_FREE_FORM = 50;
const unsigned THEATRE_MAX_ENROLLEES_IN_FREE_FORM = 50;
}

const unsigned TECH_ENROLLEES = 125;
const unsigned ARTS_ENROLLEES = 100;
const unsigned TECH_AND_ARTS_ENROLLEES = 100;

static unsigned enrolleesGeneratorCounter_ = 0;
UEAA::Enrollee *GenerateEnrollee (bool addTech, bool addArts, UEAA::DeHashTable *deHashTable);

UEAA::University *CreateTestUniversity ();
UEAA::Faculty *CreateTechFaculty ();
UEAA::Specialty *CreateTechSpecialty (unsigned id, unsigned maxFreeEnrollees);
UEAA::Faculty *CreateArtsFaculty ();
UEAA::Specialty *CreateArtsSpecialty (unsigned id, unsigned maxFreeEnrollees);
void PrintEnrollee (const UEAA::Enrollee *enrollee, UEAA::DeHashTable *deHashTable);
