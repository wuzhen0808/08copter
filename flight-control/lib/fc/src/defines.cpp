/*
 *
 * Copyright (c) 2024 Wu Zhen(wuzhen0808@163.com)
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
 */
#include "a8/fc/defines.h"
namespace a8::fc {
using namespace a8::util;

Options<ImuFilter> IMU_FILTERS;

using namespace a8::util;
int operator%(ImuFilter &filter, int iValue) {
    return (int)filter % iValue;
}
String &operator<<(String &str, const ImuFilter f) {
    return str << IMU_FILTERS.get(f, "<unknown>") << "(" << (int)f << ")";
}

Options<RpyFilter> RPY_FILTERS;

using namespace a8::util;
int operator%(RpyFilter &filter, int iValue) {
    return (int)filter % iValue;
}
String &operator<<(String &str, const RpyFilter f) {
    return str << RPY_FILTERS.get(f, "<unknown>") << "(" << (int)f << ")";
}

EnumType<UnBalanceAction> UN_BALANCE_ACTIONS;
String &operator<<(String &str, const UnBalanceAction f){
    return UN_BALANCE_ACTIONS.toStr(str, f);
}
void initialize() {
    IMU_FILTERS.add(ImuFilter::NONE, "NONE");
    IMU_FILTERS.add(ImuFilter::MAG, "MAG");
    IMU_FILTERS.add(ImuFilter::MOH, "MOH");
    RPY_FILTERS.add(RpyFilter::NO, "NO");
    RPY_FILTERS.add(RpyFilter::LP1, "LP1");
    RPY_FILTERS.add(RpyFilter::LP2, "LP2");

    UN_BALANCE_ACTIONS.add(UnBalanceAction::ASK,"ASK");
    UN_BALANCE_ACTIONS.add(UnBalanceAction::IGNORE,"IGNORE");
    UN_BALANCE_ACTIONS.add(UnBalanceAction::IGNORE_IF_SAFE,"IGNORE_IF_SAFE");
    UN_BALANCE_ACTIONS.add(UnBalanceAction::END_OF_MISSION,"END_OF_MISSION");
    
}
} // namespace a8::fc