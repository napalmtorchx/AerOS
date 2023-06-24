#pragma once
#include <lib/types.h>

int clamp(int i, int min, int max);
float clampf(float i, float min, float max);

double ceil(double x);
float ceilf(float x);
long double ceill(long double x);

double floor(double x);
float floorf(float x);

double pow(double x, int y);
float powf(float x, int y);
long powl(long x, int y);

double fmod(double x, double y);

double trunc(double x);