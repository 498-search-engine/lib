#ifndef LIB_OPTIONAL_H
#define LIB_OPTIONAL_H

#include <utility>

template<typename T>
class Optional {
public:
    Optional() : has_value_(false), sentinel_(0) {}
    Optional(const T& t) : has_value_(true), val_(t) {}
    ~Optional() { if (has_value_) val_.~T(); }

    [[nodiscard]]
    bool HasValue() const { return has_value_; }

    

private:
    bool has_value_;
    union { char sentinel_; T val_; };

};

#endif
