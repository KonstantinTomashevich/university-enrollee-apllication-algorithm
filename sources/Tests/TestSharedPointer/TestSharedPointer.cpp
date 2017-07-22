#include <UniversityEnrolleeApllicationAlgorithm/Utils/ReferenceCounted.hpp>
#include <UniversityEnrolleeApllicationAlgorithm/Utils/SharedPointer.hpp>
#include <iostream>

const unsigned ERROR_INCORRECT_REFS_COUNT_AFTER_CREATION = 1;
const unsigned ERROR_INCORRECT_REFS_COUNT_AFTER_SECOND_POINTER = 2;
const unsigned ERROR_INCORRECT_REFS_COUNT_AFTER_SECOND_POINTER_DELETION = 3;
const unsigned ERROR_INCORRECT_REFS_COUNT_WHILE_FUNCTION_CALL = 4;
const unsigned ERROR_INCORRECT_REFS_COUNT_AFTER_FUNCTION_CALL = 5;
const unsigned ERROR_INCORRECT_TRACKING_OBJECT_AFTER_DETACH = 6;
const unsigned ERROR_INCORRECT_REFS_COUNT_AFTER_DETACH = 7;

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
        return ERROR_INCORRECT_REFS_COUNT_AFTER_CREATION;
    }

    {
        SharedPointer <ReferenceCounted> secondPointer (pointer);
        std::cout << "After second pointer creation: " << secondPointer->GetReferencesCount () << std::endl;
        if (pointer->GetReferencesCount () != 2)
        {
            return ERROR_INCORRECT_REFS_COUNT_AFTER_SECOND_POINTER;
        }
    }

    std::cout << "After second pointer deletion: " << pointer->GetReferencesCount () << std::endl;
    if (pointer->GetReferencesCount () != 1)
    {
        return ERROR_INCORRECT_REFS_COUNT_AFTER_SECOND_POINTER_DELETION;
    }

    if (!TestFunction (pointer))
    {
        return ERROR_INCORRECT_REFS_COUNT_WHILE_FUNCTION_CALL;
    }

    std::cout << "After function call: " << pointer->GetReferencesCount () << std::endl;
    if (pointer->GetReferencesCount () != 1)
    {
        return ERROR_INCORRECT_REFS_COUNT_AFTER_FUNCTION_CALL;
    }

    pointer.Detach ();
    if (pointer.GetTrackingObject () != 0)
    {
        return ERROR_INCORRECT_TRACKING_OBJECT_AFTER_DETACH;
    }

    std::cout << "After detach: " << object->GetReferencesCount () << std::endl;
    if (object->GetReferencesCount () != 1)
    {
        return ERROR_INCORRECT_REFS_COUNT_AFTER_DETACH;
    }

    return 0;
}
