#include <stdio.h>

/* Receive data decode 
 * a float data use 2byte, 
 */
float msg_char_to_float(unsigned char chr_1, unsigned char chr_0)
{
    short int num_int;
    
    if((chr_1 & 0x80) == 0x80)
        num_int = -(((chr_1 & 0x7f) << 8) | chr_0);
    else
        num_int = ((chr_1 & 0x7f) << 8) | chr_0;
    return (float)num_int / 100;
}

void msg_float_to_char(float num, unsigned char *pdst)
{
    short int num_int;
    num_int = (short int)(num * 100);
    if(num_int < 0)
    {
        num_int = -num_int;
        *(pdst + 1) = (num_int >> 8) | 0x80;
        *pdst = num_int & 0x00ff;
    }
    else
    {
        *(pdst + 1) = num_int >> 8;
        *pdst = num_int & 0xff;
    }
}

unsigned int float2uint(float x, float x_min, float x_max, unsigned char bits)
{
    float span = x_max - x_min;
    float offset = x_min;
    
    return (unsigned int)((x - offset) * ((float)((1 << bits) - 1))/span);
}

float uint2float(unsigned int x, float x_min, float x_max, unsigned char bits)
{
    float span = x_max - x_min;
    float offset = x_min;

    return ((float)x) * span / ((float)((1 << bits) - 1)) + offset;
}

int main()
{
    // CodecTypedef codec;
    int i;
    unsigned char chr[0];
    float num = 84.12;
    float x_min = -45.0f;
    float x_max = 45.0f;
    unsigned char bits = 12;
    float num_t;

    msg_float_to_char(84.12, chr);
    printf("chr_1: %d, chr_0: %d\n", chr[1], chr[0]);
    num_t = msg_char_to_float(chr[1], chr[0]);
    printf("num: %.2f, num_t: %.2f\n", num, num_t);


    printf("%.2f\n", num_t);

    for(i = 0; i < 500; i++)
    {
        num_t = uint2float(float2uint(num + i*0.1, x_min, x_max, bits), x_min, x_max, bits);
        printf("org: %.2f, trans: %.2f\n", num + i*0.1, num_t);
    }

    return 0;
}