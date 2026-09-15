#pragma once
#define _FE_0(a, a0, X)     a0(0, X)
#define _FE_1(a, a0, X, ...) a(1, X)_FE_0(a, a0, __VA_ARGS__)
#define _FE_2(a, a0, X, ...) a(2, X)_FE_1(a, a0, __VA_ARGS__)
#define _FE_3(a, a0, X, ...) a(3, X)_FE_2(a, a0, __VA_ARGS__)
#define _FE_4(a, a0, X, ...) a(4, X)_FE_3(a, a0, __VA_ARGS__)

#define _GET_MACRO(_0, _1, _2, _3, _4, NAME,...) NAME
#define _FOR_EACH(a, a0, ...) \
    _GET_MACRO(__VA_ARGS__, _FE_4, _FE_3, _FE_2, _FE_1, _FE_0) \
              (a, a0, __VA_ARGS__)

#define _ARGS_STRUCT_ATTR(n, attr) union {attr, _##n;};

#define _ARGS_STRUCT(...) \
    struct { \
        _FOR_EACH(_ARGS_STRUCT_ATTR, _ARGS_STRUCT_ATTR, \
                 __VA_ARGS__) \
    }

#define _ARGS_PASS(n, attr) _args._##n,
#define _ARGS_PASS0(n, attr) _args._##n

#define _PASS_STRUCT(...) \
    _FOR_EACH(_ARGS_PASS, _ARGS_PASS0, __VA_ARGS__)

#define CALL_NAMED_ARGS(func, args, ...) ({ \
    _ARGS_STRUCT args _args = {__VA_ARGS__}; \
    func(_PASS_STRUCT args); \
    })