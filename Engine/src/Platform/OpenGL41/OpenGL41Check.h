#pragma once

namespace elv {
void glCheckError(
    const char* file,
    int line,
    const char* expression);
}

#ifdef DEBUG_MODE

#    define glCheck(expr)                                 \
        do {                                              \
            expr;                                         \
            elv::glCheckError(__FILE__, __LINE__, #expr); \
        } while (false)
#else
#    define glCheck(expr) expr
#endif