/*
    base64.c - by Joe DF (joedf@ahkscript.org)
    Released under the MIT License

    See "base64.h", for more information.

    Thank you for inspiration:
    http://www.codeproject.com/Tips/813146/Fast-base-functions-for-encode-decode
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <base64.h>

void Base64urlEncoder(unsigned char *out, const unsigned char *in, int inlen);

//Base64 char table - used internally for encoding
static const unsigned char base64digits[] =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

unsigned int base64_int(unsigned int ch) 
{

    /* ASCII to base64_int
       65-90  Upper Case  >>  0-25
       97-122 Lower Case  >>  26-51
       48-57  Numbers     >>  52-61
       43     Plus (+)    >>  62
       47     Slash (/)   >>  63
       61     Equal (=)   >>  64~
    */
    if (ch==43)
    return 62;
    if (ch==47)
    return 63;
    if (ch==61)
    return 64;
    if ((ch>47) && (ch<58))
    return ch + 4;
    if ((ch>64) && (ch<91))
    return ch - 'A';
    if ((ch>96) && (ch<123))
    return (ch - 'a') + 26;
    return 0;
}

unsigned int base64_decode(const unsigned char* in, unsigned int in_len, unsigned int* out)
{

    unsigned int i=0, j=0, k=0, s[4] = {0};

    for (i=0;i<in_len;i++)
    {
        s[j++]=base64_int(*(in+i));
        if (j==4)
        {
            out[k+0] = ((s[0]&255)<<2)+((s[1]&0x30)>>4);
            if (s[2]!=64)
            {
                out[k+1] = ((s[1]&0x0F)<<4)+((s[2]&0x3C)>>2);
                if ((s[3]!=64))
                {
                    out[k+2] = ((s[2]&0x03)<<6)+(s[3]); k+=3;
                }
                else
                {
                    k+=2;
                }
            }
            else
            {
                k+=1;
            }
            j=0;
        }
    }

    return k;
}

unsigned int base64d_size(unsigned int in_size)
{

    return ((3*in_size)/4);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Function     : Base64urlEncoder
// Purpose      : Converts a given string into a base64 encoded buffer.
// Last updated : 01/09/200515/05/2005
// Notes        :
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Base64urlEncoder(unsigned char *out, const unsigned char *in, int inlen)
/* [OUT] out  A pointer to a char to hold the converted string
   [IN]  in  String to convert
   [IN]  inlen  Length of the string to be converted
*/
{
    for (; inlen >= 3; inlen -= 3)
    {
        *out++ = base64digits[in[0] >> 2];
        *out++ = base64digits[((in[0] << 4) & 0x30) | (in[1] >> 4)];
        *out++ = base64digits[((in[1] << 2) & 0x3c) | (in[2] >> 6)];
        *out++ = base64digits[in[2] & 0x3f];
        in += 3;
    }

    if (inlen > 0)
    {
        unsigned char fragment;

        *out++ = base64digits[in[0] >> 2];
        fragment = (in[0] << 4) & 0x30;

        if (inlen > 1)
            fragment |= in[1] >> 4;

        *out++ = base64digits[fragment];
        *out++ = (inlen < 2) ? '=' : base64digits[(in[1] << 2) & 0x3c];
        *out++ = '=';
    }

    *out = '\0';
}
