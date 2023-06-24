#include <lib/math.h>
#include <kernel.h>

int clamp(int i, int min, int max)
{
    if (i < min) { i = min; }
    if (i > max) { i = max; }
    return i;
}

float clampf(float i, float min, float max)
{
    if (i < min) { i = min; }
    if (i > max) { i = max; }
    return i;
}

double ceil(double x)
{
    fpu_set_round_up();
    fpu_load_double(x);

    fpu_round(); 
    return fpu_retrive_double();
}

float ceilf(float x)
{
    fpu_set_round_up();
    fpu_load_float(x);

    fpu_round(); 
    return fpu_retrive_float();
}

long double ceill(long double x)
{
    fpu_set_round_up();
    fpu_load_long_double(x);

    fpu_round(); 
    return fpu_retrive_long_double();
}

double floor(double x)
{
    if (x >= INT64_MAX || x <= INT64_MIN || x != x) { return x; }
    uint64_t n = (uint64_t)x;
    double d = (double)n;
    if (d == x || x >= 0) { return d; } else { return d - 1; }
    return 0;
}

float floorf(float x)
{
    if (x >= INT64_MAX || x <= INT64_MIN || x != x) { return x; }
    uint64_t n = (uint64_t)x;
    float d = (float)n;
    if (d == x || x >= 0) { return d; } else { return d - 1; }
    return 0;
}

double sqrt(double x)
{
    fpu_set_round_nearest();
    fpu_load_double(x);
    
    fpu_sqrt();
    return fpu_retrive_double();
}

float sqrtf(float x)
{
    fpu_set_round_nearest();
    fpu_load_float(x);
    
    fpu_sqrt();
    return fpu_retrive_float();
}

long double sqrtl(long double x)
{
    fpu_set_round_nearest();
    fpu_load_long_double(x);
    
    fpu_sqrt();
    return fpu_retrive_long_double();
}

double pow(double x, int y)
{
    double temp;
    if (y == 0)
    return 1;
    temp = pow (x, y / 2);
    if ((y % 2) == 0) { return temp * temp; } 
    else 
    {
        if (y > 0) { return x * temp * temp; }
        else { return (temp * temp) / x; }
    }
}

float powf(float x, int y)
{
    float temp;
    if (y == 0)
    return 1;
    temp = powf(x, y / 2);
    if ((y % 2) == 0) { return temp * temp; } 
    else 
    {
        if (y > 0) { return x * temp * temp; }
        else { return (temp * temp) / x; }
    }
}

long powl(long x, int y)
{
    long temp;
    if (y == 0)
    return 1;
    temp = powl(x, y / 2);
    if ((y % 2) == 0) { return temp * temp; } 
    else 
    {
        if (y > 0) { return x * temp * temp; }
        else { return (temp * temp) / x; }
    }
}

double fmod(double x, double y) { return x - trunc(x / y) * y; }

double trunc(double x) { return x < 0 ? -floor(-x) : floor(x); }