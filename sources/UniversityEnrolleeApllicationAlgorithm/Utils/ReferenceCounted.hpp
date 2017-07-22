#pragma once

namespace UniversityEnrolleeApllicationAlgorithm
{
class ReferenceCounted
{
public:
    ReferenceCounted ();
    virtual ~ReferenceCounted ();

    unsigned GetReferencesCount () const;
    void AddReference ();
    void SubtractReference ();

private:
    unsigned references_;
};
}
