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
    printf(">> %d\n", num_int);
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
    
    // printf(">> %x, %x\n", codec.chr[1], codec.chr[0]);
}

int main()
{
    // CodecTypedef codec;
    float a = -12.23f;
    float r = 0;
    unsigned char b[2];
    msg_float_to_char(a, b);
    printf("chr[1]: %d, chr[0]: %d\n", b[1], b[0]);
    r = msg_char_to_float(b[1], b[0]);

    printf("Org: %f, Trans: %f\n", a, r);

    return 0;
}