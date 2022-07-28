/* date = June 30th 2022 3:01 pm */

#ifndef UTIL_H
#define UTIL_H

#define TICKRATE 60

#define PACKED __attribute__((__packed__))

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

#include "../defines.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-braces"

#include <cglm/cglm.h>
#include <cglm/struct.h>

#pragma GCC diagnostic pop

#include "ivec2.h"
#include "ivec2s.h"
#include "ivec3.h"
#include "ivec3s.h"

#include "aabb.h"
#include "camera.h"
#include "time.h"
#include "fmath.h"
#include "color.h"

#endif //UTIL_H
