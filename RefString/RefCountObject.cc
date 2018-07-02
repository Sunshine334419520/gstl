
#include "./RefCountObject.h"
__GSTL_BEGIN_NAMESPACE          // namespace gstl

namespace ref_count_string {

RefCountObject::RefCountObject()
    : ref_count_(0), shareable_(true) {}
    
    
RefCountObject::RefCountObject(const RefCountObject& rhs)
    : ref_count_(0), shareable_(true) {}

RefCountObject& RefCountObject::operator=(const RefCountObject& rhs) {
    return *this;
}

RefCountObject::~RefCountObject() {} 

void RefCountObject::AddRefCount() { ++ref_count_; }

void RefCountObject::RemoveRefCount() {
if (--ref_count_ == 0)
    delete this;
}

void RefCountObject::MarkUnShareable() { shareable_ = false; }

bool RefCountObject::IsShareable() const { return shareable_; }

bool RefCountObject::IsShared() const { return ref_count_ > 1; }
    
}   // namespace end ref_count_string


__GSTL_END_NAMESPACE           // namespace end gstl