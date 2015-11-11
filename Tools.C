#include <cstdint>
#include "Tools.h"
#include <stdio.h>
#include <stdint.h>
#define __STDC_FORMAT_MACROS

/**
 * builds a 64-bit long out of an array of 8 bytes
 *
 * @param array of 8 bytes
 * @return uint64_t where the low order byte is bytes[0] and
 *           the high order byte is bytes[7]
*/
uint64_t Tools::buildLong(uint8_t bytes[LONGSIZE])
{
    uint64_t string = 0;

    for(int i = 0; i < 8; i++) {
        uint64_t byte = bytes[i];
        byte <<= (i * 8);
        string = string | byte;
    }

    return string;

}

/**
 * accepts as input an uint64_t and returns the designated byte
 * within the uint64_t; returns 0 if the indicated byte number
 * is out of range
 *
 * for example, getByte(0x1122334455667788, 7) returns 0x11
 *              getByte(0x1122334455667788, 1) returns 0x77
 *              getByte(0x1122334455667788, 8) returns 0
 *
 * @param uint64_t source that is the source data
 * @param int32_t byteNum that indicates the byte to return (0 through 7)
 * @return 0 if byteNum is out of range
 *           byte 0, 1, .., or 7 of source if byteNum is within range
*/
uint64_t Tools::getByte(uint64_t source, uint64_t byteNum)
{
    if (byteNum > 8 || byteNum >= sizeof(uint64_t)) {
        return 0;
    }

    source = source << (64 - (8 * (byteNum +1)));
    source = source >> 56;
    return source;
}


/**
 * accepts as input an uint64_t and returns the bits low through
 * high of the uint64_t.  bit 0 is the low order bit and bit 63
 * is the high order bit. returns 0 if the low or high bit numbers
 * are out of range
 *
 * for example, getBits(0x8877665544332211, 0, 7) returns 0x11
 *              getBits(0x8877665544332211, 4, 11) returns 0x21
 *              getBits(0x8877665544332211, 0, 63) returns 0x8877665544332211
 *
 * @param uint64_t source that holds the bits to be grabbed and
 *        returned
 * @param int32_t low that is the bit number of the lowest numbered
 *        bit to be returned
 * @param int32_t high that is the bit number of the highest numbered
 *        bit to be returned
 * @return an uint64_t that holds a subset of the source bits
 *         that is returned in the low order bits; 0 if low or high
 *         is out of range
 */
uint64_t Tools::getBits(uint64_t source, int32_t low, int32_t high)
{
    if (low > 63 || high > 63) {
        return 0;
    }

    if (low > high) {
        return 0;
    }

    source = source << (63 - high);
    source = source >> (63 - high);
    source = source >> low;
    return source;
}

/**
 * sets the bits of source in the range specified by the low and high
 * parameters to 1 and returns that value. returns source if the low or high
 * bit numbers are out of range
 *
 * for example, setBits(0x1122334455667788, 0, 7) returns 0x11223344556677ff
 *              setBits(0x1122334455667788, 8, 0xf) returns 0x112233445566ff88
e *              setBits(0x1122334455667788, 8, 64) returns 0x1122334455667788
 *                      note: 64 is out of range
 *
 * @param uint64_t source
 * @param int32_t low that is the bit number of the lowest numbered
 *        bit to be set to 1
 * @param int32_t high that is the bit number of the highest numbered
 *        bit to be set to 1
 * @return an uint64_t that holds the modified source
 */
uint64_t Tools::setBits(uint64_t source, int32_t low, int32_t high)
{
    if (low > 63 || high > 63) {
        return source;
    }

    if (low > high) {
        return source;
    }

    uint64_t max = UINT64_MAX;
    uint64_t mask = getBits(max, low, high);
    mask <<= low;
    return source | mask;
}

/**
 * sets the bits of source in the range low to high to 0 (clears them) and
 * returns that value. returns source if the low or high
 * bit numbers are out of range
 *
 * for example, setBits(0x1122334455667788, 0, 7) returns 0x1122334455667700
 *              setBits(0x1122334455667788, 8, f) returns 0x1122334455660088
 *
 * @param uint64_t source
 * @param int32_t low that is the bit number of the lowest numbered
 *        bit to be set to 0
 * @param int32_t high that is the bit number of the highest numbered
 *        bit to be set to 0
 * @return an uint64_t that holds the modified source
 */
uint64_t Tools::clearBits(uint64_t source, int32_t low, int32_t high)
{
    if (low > 63 || high > 63) {
        return source;
    }

    if (low > high) {
        return source;
    }

    uint64_t lowend = 0;

    if(low > 0) {
        lowend = getBits(source, 0, (low - 1));
    }

    uint64_t highend = getBits(source, high + 1, 63);
    highend <<= (high + 1);
    return lowend | highend;
}

/**
 * copies length bits from the source to a destination and returns the
 * modified destination. If low bit number of the source or
 * dest is out of range or the calculated source or dest high bit
 * number is out of range, then the unmodified destination is returned.
 *
 * for example,
 *   copyBits(0x1122334455667788, 0x8877665544332211, 0, 0, 8)
 *           returns 0x8877665544332288
 *   copyBits(0x1122334455667788, 0x8877665544332211, 0, 8, 8)
 *           returns 0x8877665544338811
 *
 * @param uint64_t source
 * @param uint64_t dest
 * @param int32_t srclow that is the bit number of the lowest numbered
 *        bit of the source to be copied
 * @param int32_t destlow that is the bit number of the lowest numbered
 *        bit of the destination to be modified
 * @param int32_t length that is the number of bits to be copied
 * @return uint64_t that is the modifed dest
 */
uint64_t Tools::copyBits(uint64_t source, uint64_t dest,
                         int32_t srclow, int32_t dstlow, int32_t length)
{
    if (dstlow < 0 || dstlow > 63 || srclow < 0 || srclow > 63) {
        return dest;
    }

    uint64_t max = UINT64_MAX;
    uint64_t highBits = getBits(max, dstlow + length, 63);
    highBits <<= (dstlow + length);
    uint64_t lowBits = 0;

    if (dstlow > 0) {
        lowBits = getBits(max, 0, dstlow);
    }

    uint64_t mask = (highBits | lowBits);
    dest = (dest & mask);
    uint64_t destbits = getBits(source, srclow, srclow+length);
    destbits <<= dstlow;
    return (dest | destbits);
}

/**
 * sets the bits of source identfied by the byte number to 1 and
 * returns that value. if the byte number is out of range then source
 * is returned unchanged.
 *
 * for example, setByte(0x1122334455667788, 0) returns 0x11223344556677ff
 *              setByte(0x1122334455667788, 1) returns 0x112233445566ff88
 *              setByte(0x1122334455667788, 8) returns 0x1122334455667788
 *
 * @param uint64_t source
 * @param int32_t byteNum that indicates the number of the byte to be
 *        set to 0xff; the low order byte is byte number 0
 * @return uint64_t that is source with byte byteNum set to 0xff
 */
uint64_t Tools::setByte(uint64_t source, int32_t byteNum)
{
    if (byteNum > 7 || byteNum < 0) {
        return source;
    }

    uint64_t max = UINT64_MAX;
    uint64_t mask = getBits(max, 0, 7);
    mask <<= (byteNum * 8);
    return source | mask;
}

/**
 * assumes source contains a 64 bit two's complement value and
 * returns the sign (1 or 0)
 *
 * for example, sign(0xffffffffffffffff) returns 1
 *              sign(0x0000000000000000) returns 0
 *              sign(0x8000000000000000) returns 1
 *
 * @param uint64_t source
 * @return 1 if source is negative when treated as a two's complement
 *         value and 0 otherwise
 */
uint8_t Tools::sign(uint64_t source) {
    source = source >> 63;
    return source;
}
/**
 * assumes that op1 and op2 contain 64 bit two's complement values
 * and returns true if an overflow would occur if they are summed
 * and false otherwise
 *
 * for example, addOverflow(0x8000000000000000, 0x8000000000000000) returns 1
 *              addOverflow(0x7fffffffffffffff, 0x7fffffffffffffff) returns 1
 *              addOverflow(0x8000000000000000, 0x7fffffffffffffff) returns 0
 *
 * @param uint64_t op1 that is one of the operands of the addition
0 * @param uint64_t op2 that is the other operand of the addition
 * @return true if op1 + op2 would result in an overflow assuming that op1
 *         and op2 contain 64-bit two's complement values
 */
bool Tools::addOverflow(uint64_t op1, uint64_t op2)
{
    uint64_t sum = op1 + op2;
    uint64_t signSum = sum >> 63;
    uint64_t signOp1 = op1 >> 63;
    uint64_t signOp2 = op2 >> 63;

    if ((signOp1 == signOp2) && signSum == !signOp1) {
        return 1;
    }

    return 0;
}

bool Tools::subOverflow(uint64_t op1, uint64_t op2) {
    int rsign = Tools::sign((op1 - op2));
    return ((Tools::sign(op1) != Tools::sign(op2)) && (Tools::sign(op2) == rsign));
}

