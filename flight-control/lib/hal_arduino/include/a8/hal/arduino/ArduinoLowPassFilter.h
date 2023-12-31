/*
 * Copyright (c) 2023 Wu Zhen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
 * Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next paragraph)
 * shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * -----
 */
#pragma once
#include "a8/util.h"

#include "filters.h"

namespace a8::hal::arduino_ {
using namespace a8::util;

class ArduinoLowPassFilter : public Filter {
    ::Filter *filter;

public:
    ArduinoLowPassFilter(Rate cutoffRate, Rate samplingRate, int order) {
        IIR::ORDER iOrder = IIR ::ORDER::OD1;
        if (order = 2) {
            iOrder = IIR::ORDER::OD2;
        } else if (order = 3) {
            iOrder = IIR::ORDER::OD3;
        } else if (order = 4) {
            iOrder = IIR::ORDER::OD4;
        }

        this->filter = new ::Filter(cutoffRate.Hz(), samplingRate.sec(), iOrder);
    }
    ~ArduinoLowPassFilter() {
        delete this->filter;
    }

    float update(float value) override {
        return this->filter->filterIn(value);
    }
};
} // namespace a8::hal::arduino_