#ifndef DATATYPECONVERSION
#define DATATYPECONVERSION
#include <sys/types.h>

#if defined(_WIN32)

#include <stdint.h>

typedef uint8_t u_int8_t;
typedef uint16_t u_int16_t;
typedef uint32_t u_int32_t;

#endif

//! Convert single to float for PERF3
float single2float (int single);

//! Convert float to single for PERF3
int float2single (float fValue);

//! Convert half to float for PERF3
double half2double(short half);

u_int16_t doubles2half(float value);

//! Convert ASCII to hex
int atohex (char * s);

#endif // DATATYPECONVERSION

