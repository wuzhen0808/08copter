#pragma once
#include "a8/util/Math.h"
#include "a8/util/Reader.h"
#include "a8/util/Writer.h"

namespace a8::util {
class CodecUtil {
public:
    template <typename T>
    static int readInt(Reader *reader, int size, T &intV) {
        char buf[size];
        int ret = reader->read(buf, size);
        if (ret < 0) {
            return ret;
        }
        for (int i = 0; i < size; i++) {
            intV = intV + ((buf[i] & 0xFF) << 8 * i);
        }
        return size;
    }

    template <typename T>
    static int writeInt(Writer *writer, int size, T intV) {
        char buf[size];
        for (int i = 0; i < size; i++) {
            buf[i] = intV >> 8 * i & 0xFF;
        }
        writer->write(buf, size);
        return size;
    }

    static int writeInt8(Writer *writer, int data) {
        return writeInt(writer, 8 / 8, data);
    }

    static int readInt8(Reader *reader, int &data) {
        return readInt<int>(reader, 8 / 8, data);
    }

    static int writeInt16_(Writer *writer, int iValue) {
        return writeInt(writer, 16 / 8, iValue);
    }

    static int readInt16(Reader *reader, int &intV) {
        return readInt(reader, 16 / 8, intV);
    }

    /**
     * exp: 1+7 bits;
     * mantissa:1+23 bits;
     */
    static int writeFloat(Writer *writer, float fValue) {
        int ie = 0;
        float m = Math::frexpf(fValue, &ie);
        m = Math::ldexp(m, 53);
        long im = Math::trunc(m);
        int len = 0;
        int ret = writeInt<long>(writer, 24 / 8, im); //
        if (ret < 0) {
            return ret;
        }
        len += ret;
        ret += writeInt<int>(writer, 8 / 8, ie);
        if (ret < 0) {
            return ret;
        }
        len += ret;

        return len;
    }

    static int readFloat(Reader *reader, float &fValue) {
        long im;
        int ie;
        int len = 0;
        int ret = readInt<long>(reader, 24 / 8, im);
        if (ret < 0) {
            return ret;
        }
        len += ret;
        ret = readInt<int>(reader, 8 / 8, ie);
        if (ret < 0) {
            return ret;
        }
        len += ret;
        fValue = Math::ldexp(im, ie);
        return len;
    }

    /**
     * exp:1+10bits,
     * mantissa :1sign+52bits.
     */
    static int writeDouble(Writer *writer, double dValue) {
        int ie = 0;
        double m = Math::frexp(dValue, &ie);
        m = Math::ldexp(m, 53);
        long long im = Math::trunc(m);
        int len = 0;
        int ret = writeInt<long long>(writer, 64 / 8, im);
        if (ret < 0) {
            return ret;
        }
        len += ret;
        ret = writeInt<int>(writer, 16 / 8, ie);
        if (ret < 0) {
            return ret;
        }
        len += ret;

        return len;
    }

    static int readDouble(Reader *reader, double &dValue) {
        long long im;
        int ie;
        int len = 0;
        int ret = readInt<long long>(reader, 64 / 8, im);
        if (ret < 0) {
            return ret;
        }
        ret = readInt<int>(reader, 16 / 8, ie);
        if (ret < 0) {
            return ret;
        }
        len += ret;
        dValue = Math::ldexp(im, ie);
        return len;
    }

    static int writeString_(Writer *writer, String *data) {
        int len = 0;
        // write text
        int ret = writer->write(data->text(), data->len());
        if (ret < 0) {
            return ret;
        }
        len += ret;
        writer->write(Lang::END_OF_STR);
        len += 1;
        return len;
    }
    static int readString_(Reader *reader, String *&data) {
        String *tmp = new String();
        int ret = readString(reader, *tmp);
        if (ret < 0) {
            delete tmp;
        } else {
            data = tmp;
        }
        return ret;
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