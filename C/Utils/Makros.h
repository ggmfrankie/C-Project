//
// Created by Stefan on 07.11.2025.
//

#ifndef C_MAKROS_H
#define C_MAKROS_H

#define REPEAT(function, num)\
    for(int __i = 0; __i < (num); __i++){\
        function;\
    }
#define sig(x)\
    (((x)<0)?-1:1)

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

#define only_every(args, num) do {\
    static int count = 0;\
    if (count++ >= (num)) {\
        {\
            args;\
        }\
        count = 0;\
    }\
    \
} while (0);

#endif //C_MAKROS_H