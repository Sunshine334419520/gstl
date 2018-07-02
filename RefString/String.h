#ifndef __GSTL_REF_COUNT_STRING_H
#define __GSTL_REF_COUNT_STRING_H


#include "RefCountObject.h"
#include "RefSmartPtr.h"


__GSTL_BEGIN_NAMESPACE          // namespace gstl

namespace ref_count_string {

class String final {
 public: 
    String(const char* value="");

    const char& operator[](size_t index) const;
    char& operator[](size_t index);
private: 
    struct StringValue : public RefCountObject {
       char *data_;

       StringValue(const char* data); 
       StringValue(const StringValue& rhs);
       void Init(const char* data);
       ~StringValue();
    };


    RefSmartPtr<StringValue> value_;
};

}



__GSTL_END_NAMESPACE            // namespace end gstl


#endif