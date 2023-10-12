#pragma once
#include "a8/util/Math.h"
#include "a8/util/Reader.h"
#include "a8/util/Writer.h"

#define FLOAT_MANTISSA_BITS (24)
#define FLOAT_EXP_BITS (8)

#define DOUBLE_MANTISSA_BITS (53)
#define DOUBLE_EXP_BITS = (11)

namespace a8::util {
class CodecUtil {

public:
    template <typename T, typename U>
    static int readInt(Reader *reader, int size, T &intV) {
        U uInt = 0x0U;
        for (int i = 0; i < size; i++) {
            char ch;
            int ret = reader->read(ch);
            if (ret != 1) {
                return -1;
            }
            uInt = uInt | ((U)ch << 8 * (size - i - 1));
        }
        intV = (T)uInt;
        return size;
    }

    template <typename T, typename U>
    static int writeInt(Writer *writer, int size, T intV) {
        U uInt = (U)intV;
        for (int i = 0; i < size; i++) {
            char ch = uInt >> 8 * (size - i - 1) & 0xFF;
            int ret = writer->write(ch);
            if (ret != 1) {
                return -1;
            }
        }
        return size;
    }

    static int writeInt8(Writer *writer, char data) {
        return writeInt<char, unsigned char>(writer, sizeof(unsigned char), data);
    }

    static int readInt8(Reader *reader, int &data) {
        char ch;
        int ret = readInt8(reader, ch);
        if (ret < 0) {
            return ret;
        }
        data = (int)ch;
        return ret;
    }

    static int readInt8(Reader *reader, char &data) {
        return readInt<char, unsigned char>(reader, sizeof(unsigned char), data);
    }

    static int writeInt16(Writer *writer, short iValue) {
        return writeInt<short, unsigned short>(writer, sizeof(unsigned short), iValue);
    }

    static int readInt16(Reader *reader, short &intV) {
        return readInt<short, unsigned short>(reader, sizeof(unsigned short), intV);
    }

    static int writeInt32(Writer *writer, int iValue) {
        return writeInt<int, unsigned int>(writer, sizeof(unsigned int), iValue);
    }

    static int readInt32(Reader *reader, int &intV) {
        return readInt<int, unsigned int>(reader, sizeof(unsigned int), intV);
    }

    static int writeInt64(Writer *writer, long long iValue) {
        return writeInt<long long, unsigned long long>(writer, sizeof(unsigned long long), iValue);
    }

    static int readInt64(Reader *reader, long long &intV) {
        return readInt<long long, unsigned long long>(reader, sizeof(unsigned long long), intV);
    }

    /**
     * Float:
     * exp: 1+7 bits;
     * mantissa:1+23 bits;
     * Double:
     * exp: 11 bits,
     * mantissa: 53 bits;
     *
     */

    template <typename M, typename UM, typename E, typename UE>
    static int writeFloat(Writer *writer, M mantissas, E exp) {
        int len = 0;
        int ret = writeInt<M, UM>(writer, sizeof(UM), mantissas); //
        if (ret < 0) {
            return ret;
        }
        len += ret;
        ret = writeInt<E, UE>(writer, sizeof(UE), exp);
        if (ret < 0) {
            return ret;
        }
        len += ret;
        return len;
    }

    template <typename M, typename UM, typename E, typename UE>
    static int readFloat(Reader *reader, M &mantissas, E &exp) {

        int len = 0;
        int ret = readInt<M, UM>(reader, sizeof(UM), mantissas);
        if (ret < 0) {
            return ret;
        }
        len += ret;
        ret = readInt<E, UE>(reader, sizeof(UE), exp);
        if (ret < 0) {
            return ret;
        }
        len += ret;
        return len;
    }

    static int writeFloat(Writer *writer, float fValue) {
        int exp;
        float mantissas = Math::frexp(fValue, &exp);
        float m2 = Math::ldexp(mantissas, 23);
        return writeFloat<long, unsigned long, char, unsigned char>(writer, (long)m2, (char)(exp - 23));
    }

    static int readFloat(Reader *reader, float &fValue) {
        long mantissas;
        char exp;
        int ret = readFloat<long, unsigned long, char, unsigned char>(reader, mantissas, exp);
        fValue = Math::ldexp((float)mantissas, (int)exp);
        return ret;
    }
    /**
     * exp:1+10bits,
     * mantissa :1sign+52bits.
     */

    static int writeDouble(Writer *writer, double fValue) {
        int exp;
        double mantissas = Math::frexp(fValue, &exp);
        double m2 = Math::ldexp(mantissas, 52);
        return writeFloat<long long, unsigned long long, short, unsigned short>(writer, (long long)m2, (short)(exp - 52));
    }

    static int readDouble(Reader *reader, double &fValue) {
        long long mantissas;
        short exp;
        int ret = readFloat<long long, unsigned long long, short, unsigned short>(reader, mantissas, exp);
        fValue = Math::ldexp((double)mantissas, (int)exp);
        return ret;
    }

    template <typename T>
    static int writeNothing(Writer *writer, T &data) {
        return 0;
    }

    template <typename T>
    static int readNothing(Reader *reader, T &data) {
        return 0;
    }

    static int writeString(Writer *writer, String data) {
        int len = 0;
        // write text
        for (int i = 0; i < data.len(); i++) {
            char ch = data.charAt(i);
            if (ch == Lang::END_OF_STR) {
                break;
            }
            int ret = writer->write(ch);
            if (ret < 0) {
                return ret;
            }
            len += ret;
        }

        writer->write(Lang::END_OF_STR);
        len += 1;
        return len;
    }

    static int readString(Reader *reader, String &data) {
        // read length;
        int len = 0;
        // read text
        while (true) {
            char ch;
            int ret = reader->read(ch);
            if (ret < 0) {
                return ret;
            }
            if (ret == 0) {
                break;
            }

            len += ret;

            if (ch == Lang::END_OF_STR) {
                break;
            }
            data.append(ch);
        }
        return len;
    }
};
} // namespace a8::util