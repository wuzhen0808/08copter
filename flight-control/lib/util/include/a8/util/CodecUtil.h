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
    static int readInt(Reader *reader, T &intV) {
        int size = sizeof(T);
        U tmpInt = 0x0;
        for (int i = 0; i < size; i++) {
            char ch;
            int ret = reader->read(ch);
            if (ret != 1) {
                return -1;
            }
            tmpInt = tmpInt | ((unsigned char)ch << 8 * (size - i - 1));
        }
        intV = tmpInt;
        return size;
    }

    template <typename T, typename U>
    static int writeInt(Writer *writer, T intV) {
        int size = sizeof(T);
        U uInt = (U)intV;
        for (int i = 0; i < size; i++) {
            char ch = (uInt >> 8 * (size - i - 1)) & 0xFFU;
            int ret = writer->write(ch);
            if (ret != 1) {
                return -1;
            }
        }
        return size;
    }

    static int writeInt8(Writer *writer, char data) {
        return writeInt<int8, uint8>(writer, data);
    }

    static int readInt8(Reader *reader, int &data) {
        int8 ch;
        int ret = readInt8(reader, ch);
        if (ret < 0) {
            return ret;
        }
        data = (int)ch;
        return ret;
    }

    static int readInt8(Reader *reader, int8 &data) {
        return readInt<int8, uint8>(reader, data);
    }

    static int writeInt16(Writer *writer, int16 iValue) {
        return writeInt<int16, uint16>(writer, iValue);
    }

    static int readInt16(Reader *reader, int16 &intV) {
        return readInt<int16,uint16>(reader, intV);
    }

    static int writeInt32(Writer *writer, int32 iValue) {
        return writeInt<int32, uint32>(writer, iValue);
    }

    static int readInt32(Reader *reader, int32 &intV) {
        return readInt<int32, uint32>(reader, intV);
    }

    static int writeInt64(Writer *writer, int64 iValue) {
        return writeInt<int64, uint64>(writer, iValue);
    }

    static int readInt64(Reader *reader, int64 &intV) {
        return readInt<int64, uint64>(reader, intV);
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
        int ret = writeInt<M, UM>(writer, mantissas); //
        if (ret < 0) {
            return ret;
        }
        len += ret;
        ret = writeInt<E, UE>(writer, exp);
        if (ret < 0) {
            return ret;
        }
        len += ret;
        return len;
    }

    template <typename M, typename UM, typename E, typename UE>
    static int readFloat(Reader *reader, M &mantissas, E &exp) {

        int len = 0;
        int ret = readInt<M, UM>(reader, mantissas);
        if (ret < 0) {
            return ret;
        }
        len += ret;
        ret = readInt<E, UE>(reader, exp);
        if (ret < 0) {
            return ret;
        }
        len += ret;
        return len;
    }

    static int writeFloat32(Writer *writer, float32 fValue) {
        int exp;
        float mantissas = Math::frexp(fValue, &exp);
        float m2 = Math::ldexp(mantissas, 23);
        return writeFloat<int32, uint32, int8, uint8>(writer, (int32)m2, (int8)(exp - 23));
    }

    static int readFloat32(Reader *reader, float32 &fValue) {
        int32 mantissas;
        int8 exp;
        int ret = readFloat<int32, uint32, int8, uint8>(reader, mantissas, exp);
        fValue = Math::ldexp((float)mantissas, (int)exp);
        return ret;
    }
    /**
     * exp:1+10bits,
     * mantissa :1sign+52bits.
     */

    static int writeFloat64(Writer *writer, float64 fValue) {
        int exp;
        double mantissas = Math::frexp(fValue, &exp);
        double m2 = Math::ldexp(mantissas, 52);
        return writeFloat<int64, uint64, int16, uint16>(writer, (int64)m2, (int16)(exp - 52));
    }

    static int readFloat64(Reader *reader, float64 &fValue) {
        long long mantissas;
        short exp;
        int ret = readFloat<int64, uint64, int16, uint16>(reader, mantissas, exp);
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