#pragma once
namespace UniversityEnrolleeApllicationAlgorithm
{
template <class T> class SharedPointer
{
public:
    SharedPointer () :
        trackingObject_ (0)
    {

    }

    explicit SharedPointer (T *trackingObject) :
        trackingObject_ (trackingObject)
    {
        AddReference ();
    }

    SharedPointer (const SharedPointer <T> &anotherSharedPointer) :
        trackingObject_ (anotherSharedPointer.GetTrackingObject ())
    {
        AddReference ();
    }

    virtual ~SharedPointer ()
    {
        SubtractReference ();
    }

    void SetTrackingObject (T *trackingObject)
    {
        SubtractReference ();
        trackingObject_ = trackingObject;
        AddReference ();
    }

    T *GetTrackingObject () const
    {
        return trackingObject_;
    }

    T *operator -> () const
    {
        return GetTrackingObject ();
    }

    void Detach ()
    {
        trackingObject_ = 0;
    }

private:
    T *trackingObject_;

    void AddReference ()
    {
        if (trackingObject_)
        {
            trackingObject_->AddReference ();
        }
    }

    void SubtractReference ()
    {
        if (trackingObject_)
        {
            trackingObject_->SubtractReference ();
            if (trackingObject_->GetReferencesCount () == 0)
            {
                delete trackingObject_;
            }
        }
    }
};
}
