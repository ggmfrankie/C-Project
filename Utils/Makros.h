//
// Created by Stefan on 07.11.2025.
//

#ifndef C_MAKROS_H
#define C_MAKROS_H

#define REPEAT(function, num)\
    for(int __i = 0; __i < num; __i++){\
        function;\
    }
#define sig(x)\
    ((x<0)?-1:(x==0)?0:1)

#endif //C_MAKROS_H