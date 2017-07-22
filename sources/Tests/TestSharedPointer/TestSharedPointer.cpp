#include <UniversityEnrolleeApllicationAlgorithm/Utils/ReferenceCounted.hpp>
#include <UniversityEnrolleeApllicationAlgorithm/Utils/SharedPointer.hpp>
#include <iostream>

const char *ERROR_INCORRECT_REFS_COUNT_AFTER_CREATION = "Incorrect refs count after creation!";
const char *ERROR_INCORRECT_REFS_COUNT_AFTER_SECOND_POINTER = "Incorrect refs count after second pointer!";
const char *ERROR_INCORRECT_REFS_COUNT_AFTER_SECOND_POINTER_DELETION = "Incorrect refs count after second pointer deletion!";
const char *ERROR_INCORRECT_REFS_COUNT_WHILE_FUNCTION_CALL = "Incorrect refs count while function call!";
const char *ERROR_INCORRECT_REFS_COUNT_AFTER_FUNCTION_CALL = "Incorrect refs count after function call!";
const char *ERROR_INCORRECT_TRACKING_OBJECT_AFTER_DETACH = "Incorrect tracking object after detach!";
const char *ERROR_INCORRECT_REFS_COUNT_AFTER_DETACH = "Incorrect refs count after detach!";

using namespace UniversityEnrolleeApllicationAlgorithm;
bool TestFunction (SharedPointer <ReferenceCounted> ptr)
{
    std::cout << "While function call: " << ptr->GetReferencesCount () << std::endl;
    return ptr->GetReferencesCount () == 2;
}

int main ()
{
    ReferenceCounted *object = new ReferenceCounted ();
    SharedPointer <ReferenceCounted> pointer;
    pointer.SetTrackingObject (object);

    std::cout << "After first pointer creation: " << pointer->GetReferencesCount () << std::endl;
    if (pointer->GetReferencesCount () != 1)
    {
        std::cout << ERROR_INCORRECT_REFS_COUNT_AFTER_CREATION << std::endl;
        return 1;
    }

    {
        SharedPointer <ReferenceCounted> secondPointer (pointer);
        std::cout << "After second pointer creation: " << secondPointer->GetReferencesCount () << std::endl;
        if (pointer->GetReferencesCount () != 2)
        {
            std::cout << ERROR_INCORRECT_REFS_COUNT_AFTER_SECOND_POINTER << std::endl;
            return 1;
        }
    }

    std::cout << "After second pointer deletion: " << pointer->GetReferencesCount () << std::endl;
    if (pointer->GetReferencesCount () != 1)
    {
        std::cout << ERROR_INCORRECT_REFS_COUNT_AFTER_SECOND_POINTER_DELETION << std::endl;
        return 1;
    }

    if (!TestFunction (pointer))
    {
        std::cout << ERROR_INCORRECT_REFS_COUNT_WHILE_FUNCTION_CALL << std::endl;
        return 1;
    }

    std::cout << "After function call: " << pointer->GetReferencesCount () << std::endl;
    if (pointer->GetReferencesCount () != 1)
    {
        std::cout << ERROR_INCORRECT_REFS_COUNT_AFTER_FUNCTION_CALL << std::endl;
        return 1;
    }

    pointer.Detach ();
    if (pointer.GetTrackingObject () != 0)
    {
        std::cout << ERROR_INCORRECT_TRACKING_OBJECT_AFTER_DETACH << std::endl;
        return 1;
    }

    std::cout << "After detach: " << object->GetReferencesCount () << std::endl;
    if (object->GetReferencesCount () != 1)
    {
        std::cout << ERROR_INCORRECT_REFS_COUNT_AFTER_DETACH << std::endl;
        return 1;
    }

    return 0;
}
