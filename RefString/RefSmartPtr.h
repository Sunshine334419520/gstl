#ifndef __GSTL_REF_COUNT_SMART_PTR_H
#define __GSTL_REF_COUNT_SMART_PTR_H

#include "../config.h"

__GSTL_BEGIN_NAMESPACE              // namespace gstl

namespace ref_count_string {

template <class T>
class RefSmartPtr {
 public:
    RefSmartPtr(T* pointee = nullptr)
        : pointee_(pointee) { Init(); }
    RefSmartPtr(const RefSmartPtr& rhs)
        : pointee_(rhs.pointee_) { Init(); }

    ~RefSmartPtr() {
        if (pointee_)
            pointee_->RemoveRefCount();
    }

    RefSmartPtr& operator=(const RefSmartPtr& rhs);

    T& operator*() const { return *pointee_; }
    T* operator->() const { return &operator*(); }

 private:
    void Init();

    T* pointee_;

};

template <class T> 
RefSmartPtr<T>& RefSmartPtr<T>::operator=(const RefSmartPtr& rhs) {
    if (this != &rhs) {
        if (pointee_) pointee_->RemoveRefCount();
        pointee_ = rhs.pointee_;
        Init();
    }

    return *this;
}

template <class T> 
void RefSmartPtr<T>::Init() {
    if (pointee_ == nullptr) return ;

    if (pointee_->IsShareable() == false) {
        pointee_ = new T(*pointee_);
    }

    pointee_->AddRefCount();
}


} // namespace end ref_count_string

__GSTL_END_NAMESPACE                // namespace end gstl


#endif