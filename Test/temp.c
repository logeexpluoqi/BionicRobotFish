#include <stdio.h>

unsigned int float2uint(float x, float x_min, float x_max, unsigned char bits)
{
    float span = x_max - x_min;
    float offset = x_min;

    return ((x - offset) * ((float)((1 << bits) - 1)) / span);
}

float unit2float(unsigned int x, float x_min, float x_max, unsigned char bits)
{
    float span = x_max - x_min;
    float offset = x_min;

    return(((float)x) * span / ((float)((1 << bits) - 1)) + offset);
}

int main()
{
    float a = 5.0f;
    unsigned int a_u;
    float a_f;

    float min = -15.0f;
    float max = 15.0f;

    a_u = float2uint(a, min, max, 12);
    a_f = unit2float(a_u, min, max, 12);

    printf("\nOrg: %f, Trans: %d\n", a, a_u);
    printf("u2f: Org %f, Trans %d\n", a_u, a_f);

    return 0;
}