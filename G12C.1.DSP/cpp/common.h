#ifndef COMMON_H__INCLUDED
#define COMMON_H__INCLUDED

#ifndef __cplusplus
#error C++ compilation required.
#endif

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sysreg.h>
#include <signal.h>
#include <filter.h>
#include <sru.h>
#include "21488.h"
#include "def21488.h"
#include "cdef21488.h"

#define CLKIN 25000000
#define CCLK 400000000
#define PCLK (CCLK / 2)

#define PI 3.1415926535897932384626433832795

#define MAKE_SHORT(x) \
    {                 \
        x <<= 16;     \
        x >>= 16;     \
    } // хитрое преобразование для pack16

#endif
