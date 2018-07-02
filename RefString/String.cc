#include "String.h"

#include <cstring>

__GSTL_BEGIN_NAMESPACE 

namespace ref_count_string {

void String::StringValue::Init(const char* data) {
    data_ = new char[strlen(data) + 1];
    strcpy(data_, data);
}

String::StringValue::StringValue(const char* data) {
    Init(data);
}

String::StringValue::StringValue(const StringValue& rhs) {
    Init(rhs.data_);
}

String::StringValue::~StringValue() {
    delete [] data_;
}

String::String(const char* value)
    : value_(new StringValue(value)) {}

const char& String::operator[](size_t index) const {
    return value_->data_[index];
}

char& String::operator[](size_t index) {
    if (value_->IsShared()) {
        value_ = new StringValue(value_->data_);
    }

    value_->MarkUnShareable();
    return value_->data_[index];
}

    
}


__GSTL_END_NAMESPACE