#pragma once
#include "a8/link/LineApi.h"
#include "a8/link/defines.h"
#include "a8/util.h"

using namespace a8::util;

namespace a8::link {

//
class LineBridge {

public:
    static void bridge(int type, void *data, void *context) {
        LineApi *api = static_cast<LineApi *>(context);
        String *pData = static_cast<String *>(data);
        api->line(*pData);
    }
};

} // namespace a8::link
