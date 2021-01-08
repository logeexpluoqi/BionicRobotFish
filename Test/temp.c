#include <stdio.h>

unsigned int float2uint(float x, float x_min, float x_max, unsigned char bits)
{
    float span = x_max - x_min;
    float offset = x_min;

    return ((x - offset) * ((float)((1 << bits) - 1)) / span);
}

int main()
{
    float a = 10;

    printf("\nOrg: %f, Trans: %d\n", a, float2uint(a, -95.5, 95.5, 12));

    return 0;
}