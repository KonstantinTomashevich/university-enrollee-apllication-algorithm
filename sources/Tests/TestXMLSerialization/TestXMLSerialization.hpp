#pragma once
#include <UEAA/Core/Enrollee/Enrollee.hpp>
#include <UEAA/Core/Enrollee/EnrolleeHelpers.hpp>

#include <UEAA/Core/University/Specialty.hpp>
#include <UEAA/Core/University/Faculty.hpp>
#include <UEAA/Core/University/University.hpp>

#include <UEAA/Utils/SharedPointer.hpp>
#include <UEAA/Utils/CStringToHash.hpp>
#include <UEAA/Utils/DeHashTable.hpp>

unsigned MATH_EXAM = 0;
unsigned PHYSICS_EXAM = 0;
unsigned LANGUAGE_EXAM = 0;
unsigned SOCIETY_EXAM = 0;
unsigned ARTS_INTERNAL_EXAM = 0;

unsigned TECH_FACULTY = 0;
namespace TechFaculty
{
unsigned APPLIED_COMPUTER_SCIENCE_FREE = 0;
unsigned COMPUTER_SCIENCE_FREE = 0;

unsigned APPLIED_COMPUTER_SCIENCE_PAID = 0;
unsigned COMPUTER_SCIENCE_PAID = 0;

unsigned ACS_MAX_ENROLLEES_IN_FREE = 25;
unsigned ACS_MAX_ENROLLEES_IN_PAID = 15;

unsigned CS_MAX_ENROLLEES_IN_FREE = 100;
unsigned CS_MAX_ENROLLEES_IN_PAID = 10;
}

unsigned ARTS_FACULTY = 0;
namespace ArtsFaculty
{
unsigned PAINTING_FREE = 0;
unsigned THEATRE_FREE = 0;

unsigned PAINTING_PAID = 0;
unsigned THEATRE_PAID = 0;

unsigned PAINTING_MAX_ENROLLEES_IN_FREE = 50;
unsigned PAINTING_MAX_ENROLLEES_IN_PAID = 100;

unsigned THEATRE_MAX_ENROLLEES_IN_FREE = 50;
unsigned THEATRE_MAX_ENROLLEES_IN_PAID = 100;
}

unsigned TECH_ENROLLEES = 100;
unsigned ARTS_ENROLLEES = 250;
unsigned TECH_AND_ARTS_ENROLLEES = 150;

void InitHashes (UEAA::DeHashTable *deHashTable);
static unsigned enrolleesGeneratorCounter_ = 0;
UEAA::Enrollee *GenerateEnrollee (bool addTech, bool addArts, UEAA::DeHashTable *deHashTable);

UEAA::University *CreateTestUniversity ();
UEAA::Faculty *CreateTechFaculty (UEAA::University *university);
UEAA::Specialty *CreateTechSpecialty (UEAA::Faculty *faculty, unsigned id, unsigned maxEnrollees);
UEAA::Faculty *CreateArtsFaculty (UEAA::University *university);
UEAA::Specialty *CreateArtsSpecialty (UEAA::Faculty *faculty, unsigned id, unsigned maxEnrollees);
void PrintEnrollee (const UEAA::Enrollee *enrollee, const UEAA::DeHashTable *deHashTable);
