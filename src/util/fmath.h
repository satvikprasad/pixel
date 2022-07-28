/* date = June 30th 2022 3:50 pm */

#ifndef FMATH_H
#define FMATH_H

#define IVEC2S2V(_v) ({ ivec2s _u = ( _v ); ((vec2s) {{ _u.x, _u.y }}); })
#define VEC2S2I(_v) ({ vec2s _u = ( _v ); ((ivec2s) {{ _u.x, _u.y }}); })
#define IVEC3S2V(_v) ({ ivec3s _u = ( _v ); ((vec3s) {{ _u.x, _u.y, _u.z }}); })
#define VEC3S2I(_v) ({ vec3s _u = ( _v ); ((ivec3s) {{ _u.x, _u.y, _u.z }}); })

#define IVEC2S23(_v) ({ ivec2s _u = ( _v ); ((ivec3s) {{ _u.x, _u.y, 0 }}); })

#define VEC3S2VEC4S(_v, _f) ((vec4s) {{(_v).x, (_v).y, (_v).z, (_f)}})

#define PI_2 GLM_PI_2
#define PI GLM_PI
#define TAU (2 * PI)
#define F32_EPSILON FLT_EPSILON
#define F64_EPSILON DBL_EPSILON

#define cosf(_x) ((f32)cos(_x))
#define sinf(_x) ((f32)sin(_x))
#define radians(_x) ((_x) * (__typeof__(_x)) (PI / 180.0f))
#define degrees(_x) ((_x) * (__typeof__(_x)) (180.0f / PI))
#define floori(_x) ((int) (floor((double) (_x))))
#define sign(_x) ({ __typeof__(_x) _xx = (_x);\
((__typeof__(_x)) ( (((__typeof__(_x)) 0) < _xx) - (_xx < ((__typeof__(_x)) 0))));})
#define lerpf(_a, _b, _t) ({ __typeof__(_t) _u = (_t); ((_a) * (1 - _u)) + ((_b) * _u); })
#define safe_expf(_x, _e) ({ __typeof__(_x) __x = (_x); __typeof__(_e) __e = (_e); sign(__x) * fabsf(powf(fabsf(__x), __e)); })

#undef max
#define max(a, b) ({\
__typeof__ (a) _a = (a); \
__typeof__ (b) _b = (b); \
_a > _b ? _a : _b; })

#undef min
#define min(a, b) ({\
__typeof__ (a) _a = (a); \
__typeof__ (b) _b = (b); \
_a < _b ? _a : _b; })

#define clamp(x, mn, mx) ({\
__typeof__ (x) _x = (x); \
__typeof__ (mn) _mn = (mn); \
__typeof__ (mx) _mx = (mx); \
max(_mn, min(_mx, _x)); })


#endif //FMATH_H
