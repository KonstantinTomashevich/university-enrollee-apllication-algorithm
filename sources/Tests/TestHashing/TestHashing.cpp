#include <UEAA/Core/Enrollee/EnrolleeHelpers.hpp>
#include <UEAA/Utils/SharedPointer.hpp>
#include <UEAA/Utils/CStringToHash.hpp>
#include <UEAA/Utils/DeHashTable.hpp>
#include <iostream>
#include <time.h>

const unsigned ATTEMPTS_COUNT_FOR_EACH_CHECK = 1000;
const unsigned MAX_RANDOM_NUMBER = 100;

int main ()
{
    srand (time (0));
    UEAA::SharedPointer <UEAA::DeHashTable> deHashTable (new UEAA::DeHashTable ());

    std::cout << "Testing random numbers hashes collisions." << std::endl;
    for (int index = 0; index < ATTEMPTS_COUNT_FOR_EACH_CHECK; index++)
    {
        std::string string = std::to_string (rand () % MAX_RANDOM_NUMBER);
        unsigned hash = UEAA::CStringToHash (string.c_str (), 0);

        if (!deHashTable->RegisterHash (hash, string))
        {
            std::cout << "Found collision! \"" << string << "\" and \"" <<
                      deHashTable->DeHash (hash) << "\" have same hash (" <<
                      hash << ")!" << std::endl;
            return 1;
        }
    }

    std::cout << "Testing DeHash." << std::endl;
    deHashTable->Clear ();
    for (int index = 0; index < ATTEMPTS_COUNT_FOR_EACH_CHECK; index++)
    {
        std::string string = std::to_string (rand () % MAX_RANDOM_NUMBER);
        unsigned hash = UEAA::CStringToHash (string.c_str (), deHashTable);

        if (string != deHashTable->DeHash (hash))
        {
            std::cout << "\"" << string << "\" != DeHash (" << hash << ") (which is \"" <<
                      deHashTable->DeHash (hash) << "\")!" << std::endl;
            return 1;
        }
    }

    std::cout << "Testing with passport series and number." << std::endl;
    deHashTable->Clear ();
    int lastPassportNumber = 0;
    std::string passportSeries = "XY";

    for (int index = 0; index < ATTEMPTS_COUNT_FOR_EACH_CHECK; index++)
    {
        std::string passportNumber = std::to_string (lastPassportNumber);
        while (passportNumber.size () < 7)
        {
            passportNumber = "0" + passportNumber;
        }

        unsigned hash = UEAA::CalculateEnrolleeHash (passportSeries, passportNumber, deHashTable);
        if (passportSeries + passportNumber != deHashTable->DeHash (hash))
        {
            std::cout << "\"" << passportSeries + passportNumber << "\" != DeHash (" << hash << ") (which is \"" <<
                      deHashTable->DeHash (hash) << "\")!" << std::endl;
            return 1;
        }
    }
    return 0;
}
