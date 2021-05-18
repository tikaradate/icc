#include <stdio.h> 
#include <stdint.h> 
#include <float.h> 
#include <math.h> 
typedef union{     
    int32_t i;     
    float f;     
    struct{   
        // Bitfields for exploration. Do not use in production code.        
        uint32_t mantissa : 23;         
        uint32_t exponent : 8;         
        uint32_t sign : 1;     
    } parts; 
} Float_t; 

void printFloat_t( Float_t num ){
    int32_t exp_delta;
    if(num.parts.exponent == 0){
        exp_delta = 126;
    } else if (num.parts.exponent == 255){
        exp_delta = 0;
    } else {
        exp_delta = 127;
    }
    printf("f:%1.9e,\tix:0x%08X,\ts:%d,\te:%d,\te_v:%d,\tmx:0x%06X\n",            
    num.f, num.i,            
    num.parts.sign, num.parts.exponent, num.parts.exponent - exp_delta, num.parts.mantissa); 
} 

int main(){      
    printf("\nEpsilon: %1.15f\n", FLT_EPSILON); 
    Float_t num;
    num.f = 3.1415;
    printFloat_t(num);
    num.i = 1;
    printFloat_t(num);
    num.i = 0x00800000;
    printFloat_t(num);
    num.f = 0.2;
    printFloat_t(num);
    num.f = 1.0;
    printFloat_t(num);
    num.f = 1.5;
    printFloat_t(num);
    num.f = 1.75;
    printFloat_t(num);
    num.f = 1.99999988;
    printFloat_t(num);
    num.f = 2.0;
    printFloat_t(num);
    num.f = 16777215;
    printFloat_t(num);
    num.i = 0x7F7FFFFF;
    printFloat_t(num);
    num.i = 0x7f800000;
    printFloat_t(num);
}