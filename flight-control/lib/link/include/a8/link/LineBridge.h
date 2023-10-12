#pragma once
#include "a8/link/LineApi.h"
#include "a8/link/defines.h"
#include "a8/util.h"

using namespace a8::util;

namespace a8::link {

//
class LineBridge {

public:
    static int bridge(int type, void *data, void *context, Result &rst) {
        LineApi *api = static_cast<LineApi *>(context);
        String *pData = static_cast<String *>(data);
        return api->line(*pData, rst);
    }
};

} // namespace a8::link
