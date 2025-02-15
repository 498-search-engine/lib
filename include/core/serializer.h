#ifndef LIB_SERIALIZER_H
#define LIB_SERIALIZER_H


#include <cstddef> // for size_t

namespace core { 

    template <typename T>
    class AbstractSerializer {
        public: 

        virtual ~AbstractSerializer() = default; 
        // create a buffer and serialize object to it however you want
        virtual char* serialize(const T& obj) const = 0; 
        
        // deserialize from some T that was AbstractSerializer::serialize into that buffer
        virtual T deserialize(char* buffer, size_t size) const = 0;
    };
};


#endif