/* The class for refenrence count */

#ifndef __GSTL_REF_COUNT_OBJECT_H
#define __GSTL_REF_COUNT_OBJECT_H

#include "../config.h"

#include <cstddef>

__GSTL_BEGIN_NAMESPACE          // namespace gstl

namespace ref_count_string {

class RefCountObject {
public:
    // Increase the ref_count 
    void AddRefCount();
    // Decrease the ref_count, if ref_count becomes 0, delete this 
    void RemoveRefCount();
    // Set the [shareabel] to false to unshareable
    void MarkUnShareable();
    // Whether or not shareable, if it is, return true. Othewise,
    // return false
    bool IsShareable() const;
    // Whether or not shared. yse, return true. no, return false
    bool IsShared() const;

protected:
    // constructs function.
    RefCountObject();
    RefCountObject(const RefCountObject& rhs);
    RefCountObject& operator=(const RefCountObject& rhs);
    // destruct function.
    virtual ~RefCountObject() = 0;

private:
    std::size_t ref_count_;
    bool shareable_; 
};

}       // namespace ref_count_string

__GSTL_END_NAMESPACE         // namespace end gstl

#endif
