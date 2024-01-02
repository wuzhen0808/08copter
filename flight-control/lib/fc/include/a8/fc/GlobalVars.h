/*
 * -----
 * Project: A8-COPTER
 * File: References.h
 * Created Date: Sat, Dec 30 2023, 10:25:17 am
 * Author: Wu Zhen(wuzhen0808@163.com)
 * -----
 * Last Modified: Sat, Dec 30 2023, 10:25:17 am
 * Modified By: Wu Zhen
 * -----
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
#include "a8/fc/Rpy.h"
namespace a8::fc {
class GlobalVars {

public:
    static const int MAX_PWM = 2000;
    static const int MIN_PWM = 1000;
    static constexpr float MAX_THROTTLE = 1000;
    static constexpr float MIN_THROTTLE = 0;

};
} // namespace a8::fc