/**
 * UTF-8 Processor
 * Specification: RFC 3629
 */


#include <stdint.h>
#include <stdio.h>


typedef uint32_t char32_t;

/**
 * Encode Unicode to UTF–8
 * uchar: Unicode character (U+0 – U+10FFFF)
 * Return: encoded bytes or −1 when uchar is out of range
 */
uint32_t utf8_encode(char32_t uchar)
{
  if (uchar < 0x80) {
    // 0xxx xxxx
    // ascii
    return uchar;
  } else if (uchar < 0x800) {
    // 0000 0xxx xxyy yyyy
    // > 110x xxxx
    // > 10yy yyyy
    return 0xc080
      | (uchar<<2 & 0x1f00)
      | (uchar & 0x3f);
  } else if (uchar < 0x10000) {
    // xxxx yyyy yyzz zzzz
    // > 1110 xxxx
    // > 10yy yyyy
    // > 10zz zzzz
    return 0xe08080
      | (uchar<<4 & 0xf0000) 
      | (uchar<<2 & 0x3f00)  
      | (uchar & 0x3f);      
  } else if (uchar < 0x110000) {
    // 000w wwxx xxxx yyyy yyzz zzzz
    // > 1111 0www
    // > 10xx xxxx
    // > 10yy yyyy
    // > 10zz zzzz
    return 0xf0808080
      | (uchar<<6 & 0x7000000)
      | (uchar<<4 & 0x3f0000)
      | (uchar<<2 & 0x3f00)
      | (uchar & 0x3f);
  } else {
    // out of range
    return -1; // 0xffffffff, invalid in utf-8
  }
}

/**
 * Decode UTF–8 to Unicode
 * bytes: encoded UTF–8 bytes
 * Return: Unicode of character or −1 if bytes is invalid
 */
char32_t utf8_decode(uint32_t bytes)
{
  /* read 'utf8_encode' function for more information */

  if ((bytes & 0x80) == 0) {
    // ascii
    return bytes;
  } else if ((bytes & 0xe0c0) == 0xc080) {
    // two bytes
    return (bytes>>2 & 0x7c0)
      | (bytes & 0x3f);
  } else if ((bytes & 0xf0c0c0) == 0xe08080) {
    // three bytes
    return (bytes>>4 & 0xf000)
      | (bytes>>2 & 0xfc0)
      | (bytes & 0x3f);
  } else if ((bytes & 0xf8c0c0c0) == 0xf0808080) {
    // four bytes
    return (bytes>>6 & 0x1c0000)
      | (bytes>>4 & 0x3f000)
      | (bytes>>2 & 0xfc0)
      | (bytes & 0x3f);
  } else {
    // invalid
    return -1;
  }
}