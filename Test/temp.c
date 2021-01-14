#include <stdio.h>

typedef struct codec 
{
    float num;
    unsigned char chr[2];
} CodecTypedef;

CodecTypedef codec;

/* Receive data decode 
 * a float data use 2byte, 
 */
void msg_char_to_float(unsigned char chr_1, unsigned char chr_0)
{
    short int num_int;
    
    if((chr_1 & 0x80) == 0x80)
        num_int = -(((chr_1 & 0x7f) << 8) | chr_0);
    else
        num_int = ((chr_1 & 0x7f) << 8) | chr_0;
    codec.num = (float)num_int / 100;
}

void msg_float_to_char(float num)
{
    short int num_int;
    num_int = (short int)(num * 100);
    printf(">> %d\n", num_int);
    if(num_int < 0)
    {
        num_int = -num_int;
        codec.chr[1] = (num_int >> 8) | 0x80;
        codec.chr[0] = num_int & 0x00ff;
    }
    else
    {
        codec.chr[1] = num_int >> 8;
        codec.chr[0] = num_int & 0xff;
    }
    
    printf(">> %x, %x\n", codec.chr[1], codec.chr[0]);
}

int main()
{
    // CodecTypedef codec;
    float a = 45.02f;
    
    msg_float_to_char(a);
    printf("chr[1]: %x, chr[0]: %x\n", codec.chr[1], codec.chr[0]);
    msg_char_to_float(codec.chr[1], codec.chr[0]);

    printf("Org: %f, Trans: %f\n", a, codec.num);

    return 0;
}