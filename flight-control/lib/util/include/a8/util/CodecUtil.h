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
    template <typename T>
    static int getBit(T value, int idx) {
        return (value >> (sizeof(T) * 8 - idx - 1)) & 0x1;
    }

    template <typename T>
    static int readInt(Reader *reader, T &intV) {
        int size = sizeof(T);
        T tmpInt = 0x0;
        for (int i = 0; i < size; i++) {
            char ch;
            int ret = reader->read(ch);
            if (ret != 1) {
                return -1;
            }
            //ch first convert to unsigned char and then convert to the target type.
            //other wise, the ch may convert to int which is not unsigned.
            tmpInt = tmpInt | ((T)(unsigned char)ch << 8 * (size - i - 1));
        }
        intV = tmpInt;
        return size;
    }

    template <typename T>
    static int writeInt(Writer *writer, T intV) {
        int size = sizeof(T);
        for (int i = 0; i < size; i++) {
            // no need to use unsigned type, since we do not use the sign filling bits.
            //
            char ch = (intV >> 8 * (size - i - 1)) & 0xFF;
            int ret = writer->write(ch);
            if (ret != 1) {
                return -1;
            }
        }
        return size;
    }

    static int writeInt8(Writer *writer, char data) {
        return writeInt<int8>(writer, data);
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
        return readInt<int8>(reader, data);
    }

    static int writeInt16(Writer *writer, int16 iValue) {
        return writeInt<int16>(writer, iValue);
    }

    static int readInt16(Reader *reader, int16 &intV) {
        return readInt<int16>(reader, intV);
    }

    static int writeInt32(Writer *writer, int32 iValue) {
        return writeInt<int32>(writer, iValue);
    }

    static int readInt32(Reader *reader, int32 &intV) {
        return readInt<int32>(reader, intV);
    }

    static int writeInt64(Writer *writer, int64 iValue) {
        return writeInt<int64>(writer, iValue);
    }

    static int readInt64(Reader *reader, int64 &intV) {
        return readInt<int64>(reader, intV);
    }

    template <typename F, typename T>
    static int writeFloat(Writer *writer, F fValue) {
        T iValue = *reinterpret_cast<T *>(&fValue);
        return CodecUtil::writeInt<T>(writer, iValue);
    }

    template <typename F, typename T>
    static int readFloat(Reader *reader, F &fValue) {
        T iV;
        int ret = CodecUtil::readInt<T>(reader, iV);
        if (ret < 0) {
            return ret;
        }
        fValue = *reinterpret_cast<F *>(&iV);
        return ret;
    }

    /**
     * Float32:
     * exp: 8 bits;
     * mantissa:24 bits;
     */

    static int writeFloat32(Writer *writer, float32 fValue) {
        return writeFloat<float32, int32>(
            writer, fValue);
    }

    static int readFloat32(Reader *reader, float32 &fValue) {
        return readFloat<float32, int32>( //
            reader, fValue);
    }
    /**
     * Float64:
     *
     * exp:11bits,1sign+10bit
     * mantissa :1sign+52bits.
     */

    static int writeFloat64(Writer *writer, float64 fValue) {
        return writeFloat<float64, int64>(
            writer, fValue);
    }

    static int readFloat64(Reader *reader, float64 &fValue) {
        return readFloat<float64, int64>( //
            reader, fValue                        //
        );
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