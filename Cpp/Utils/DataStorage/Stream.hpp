//
// Created by ertls on 27.04.2026.
//

#pragma once
#include "InlineVector.hpp"

namespace ggm {
    template<typename T>
    class Stream {
        InlineVector<T, 8> mData;

        class StreamBuilder {

        };
    public:
        Stream(){}
        virtual ~Stream();

        template<typename Mapper>
        auto map(Mapper&& m) {

        }
    };
}
