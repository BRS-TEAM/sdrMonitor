#include "headers/staticdependencies/datatypeconversion.h"

#include <math.h>
#include <iostream>

using namespace std;

float single2float (int single)
{
    // conversion single to float for PERF3
    union IntFloat {
        u_int32_t sVal;
        float fVal;
    }val;

    val.sVal = single;
    return val.fVal;

}

int float2single (float fValue)
{
    // conversion float to single for PERF3
    union FloatInt {
        u_int32_t sVal;
        float fVal;
    }val;

    val.fVal = fValue;
    return val.sVal;
}

/*
 * Ex:
 *
 * Half =  0101011101010000 = 22352
 *
 * Separation: 0|10101|1101010000
 *
 * Sign = 0
 *
 * Mantissa = 1.1101010000
 *
 * Exponent = 21−(2^4 −1) = 21 - 15 = 6
 *
 * Mantissa Denormalised = 1110101.0000 = 117
 *
 *
 * Ex:
 *
 * Half =  0100000000000000 = 16384
 *
 * Separation: 0|10000|0000000000
 *
 * Sign = 0
 *
 * Mantissa = 0.0000000000
 *
 * Exponent = 16−(2^4 −1) = 16 - 15 = 1
 *
 * Mantissa Denormalised = 00000000000 = 0
 *
 * double = (1 + 0 * 2^-10) * 2^1 = 2                   (if s = 1; double = -double)
 *
 *
 * Ex:
 *
 * Half =  0011010011001100 = 13516
 *
 * Separation: 0|01101|0011001100
 *
 * Sign = 0
 *
 * Mantissa = 1.0011001100
 *
 * Exponent = 13 − (2^4 −1) = 13 - 15 = -2
 *
 * Mantissa Denormalised = 100110011.00 = 307
 *
 * double = (1 + 307 * 2^-10) * 2^-2 = 0.3249                (if s = 1; double = -double)
 *
 *
 *
 */

double half2double(short half)
{    
    // cout << "half " << half << endl;
    // conversion half to float for PERF3
    int sig = half >> 15;                               // seee eeff ffff ffff      =       HALF    (16 Bits)
    half = half & 0x7FFF;                               // 0111 1111 1111 1111
    double exp = (half >> 10) - 15;                     // 0000 0000 0001 1111 - 0000 0000 0001 0000
    int fract = half & 0x3FF;                           // 0000 0011 1111 1111
    double flt =1.0 + ( (double)fract / 1024);          // flt = 1 + f * 2^-10
    if (sig)
        flt = -flt;
    flt = flt * pow (2.0,exp);                          // flt = (1 + f * 2^-10) * 2^(e-15) =   double      (64 bits)
    return flt;                                         // f = 0000 00ff ffff ffff
                                                        // e = 0000 0000 000e eeee
                                                        // s = 0000 0000 0000 000s
}

/*
 * Ex:
 *
 * Double(2) =  0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0010 = 2
 * Double(16)=  0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    2    = 0000 0000 0000 0002 = 2
 *
 * Separation(2) : 0|000 0000 0000|0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0010
 * Separation(16): 0|0 00|0 0000 0000 0002
 *
 * Sign = 0
 *
 * Mantissa(2)  = 0.0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0010            (1.mentisa; if e != 0)
 * Mantissa(16) = 0.0000000000002 (Note that Fraction must not be converted to decimal here)
 *
 * Exponent = 1 − (2^10 −1) = 1 - 1023 = -1022                 (if e !=0; e - eBias)   (if e =0; 1 - eBias)    (eBias = 1023 = (2^10 −1))
 *
 * Mantissa Denormalised(2)  = 010 (bin) = 2 (dec)  ( not correct )
 * Mantissa Denormalised(16) = 02  (hex) = 2 (dec)   ( correct )
 *
 * Half = (2 * 2^-52) * 2^-1022 = 16384
 *
 *
 * Ex:
 *
 *
 * Double(2) =  0101 0011 0010 1100 1010 0101 0111 1010 0111 1000 0110 1100 0010 0010 0110 1000 = 0.3249
 * Double(16)=  5    3    2    C    A    5    7    A    7    8    6    C    2    2    6    8    = 0000 0000 0000 0002 = 2
 *
 * Separation(2) : 0|101 0011 0010 |1100 1010 0101 0111 1010 0111 1000 0110 1100 0010 0010 0110 1000
 * Separation(16): 0|5 32|C A57A 786C 2268
 *
 * Sign = 0
 *
 * Mantissa(2)  = 1.1100 1010 0101 0111 1010 0111 1000 0110 1100 0010 0010 0110 1000             (1.mentisa; if e != 0)
 * Mantissa(16) = 1.CA57A786C2268 (Note that Fraction must not be converted to decimal here)
 *
 * Exponent = 1330 − (2^10 −1) = 1330 - 1023 = 307                 (if e !=0; e - eBias)   (if e =0; 1 - eBias)    (eBias = 1023 = (2^10 −1))
 *
 * Mantissa Denormalised(16) = 1CA57A786C2268  (hex) = 8063244772844136 (dec)
 *
 * Half = (8063244772844136 * 2^-52) * 2^307= 16384
 *
 *
 */

u_int16_t doubles2half(float value)                     //  seee eeee eeee ffff ffff ffff ffff ffff ffff ffff ffff ffff ffff ffff ffff ffff     =   double      (64 bits)
{
    union Bits
    {
        float f;
        int32_t si;
        u_int32_t ui;
    };
    int const shift = 13;
    int const shiftSign = 16;

    int32_t const infN = 0x7F800000; // flt32 infinity
    int32_t const maxN = 0x477FE000; // max flt16 normal as a flt32
    int32_t const minN = 0x38800000; // min flt16 normal as a flt32
    int32_t const signN = 0x80000000; // flt32 sign bit

    int32_t const infC = infN >> shift;
    int32_t const nanN = (infC + 1) << shift; // minimum flt16 nan as a flt32
    int32_t const maxC = maxN >> shift;
    int32_t const minC = minN >> shift;
//    int32_t const signC = signN >> shiftSign; // flt16 sign bit

    int32_t const mulN = 0x52000000; // (1 << 23) / minN
//    int32_t const mulC = 0x33800000; // minN / (1 << (23 - shift))

    int32_t const subC = 0x003FF; // max flt32 subnormal down shifted
//    int32_t const norC = 0x00400; // min flt32 normal down shifted

    int32_t const maxD = infC - maxC - 1;
    int32_t const minD = minC - subC - 1;

    Bits v, s;
    v.f = value;
    u_int32_t sign = v.si & signN;
    v.si ^= sign;
    sign >>= shiftSign; // logical shift
    s.si = mulN;
    s.si = s.f * v.f; // correct subnormals
    v.si ^= (s.si ^ v.si) & -(minN > v.si);
    v.si ^= (infN ^ v.si) & -((infN > v.si) & (v.si > maxN));
    v.si ^= (nanN ^ v.si) & -((nanN > v.si) & (v.si > infN));
    v.ui >>= shift; // logical shift
    v.si ^= ((v.si - maxD) ^ v.si) & -(v.si > maxC);
    v.si ^= ((v.si - minD) ^ v.si) & -(v.si > subC);
    return v.ui | sign;

}


int atohex (char * s)
{
    int i=0;
    long long value =0;  // [CL] modified to 64 bits to handle 32 bits full scale
    unsigned long temp = 0;

    while (s[i] != '\0')
    {
        value *= 16;
        switch (s[i])
        {case '0': temp = 0x0; break;
        case '1': temp = 0x1; break;
        case '2': temp = 0x2; break;
        case '3': temp = 0x3; break;
        case '4': temp = 0x4; break;
        case '5': temp = 0x5; break;
        case '6': temp = 0x6; break;
        case '7': temp = 0x7; break;
        case '8': temp = 0x8; break;
        case '9': temp = 0x9; break;
        case 'A': temp = 0xa; break;
        case 'a': temp = 0xa; break;
        case 'B': temp = 0xb; break;
        case 'b': temp = 0xb; break;
        case 'C': temp = 0xc; break;
        case 'c': temp = 0xc; break;
        case 'D': temp = 0xd; break;
        case 'd': temp = 0xd; break;
        case 'E': temp = 0xe; break;
        case 'e': temp = 0xe; break;
        case 'F': temp = 0xf; break;
        case 'f': temp = 0xf; break;
        default: return (int) (value /= (long long)16); break;
        }

        i++;
        value += temp;
    }
    return (int) value;
}
