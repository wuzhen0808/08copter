#pragma once
#include "a8/common/defines.h"
#include "a8/hal.h"
#include "a8/util.h"
#include "a8/util/Logger.h"

namespace a8::common {
using namespace a8::hal;
using namespace a8::util;

class CommonUtil {
    static String resolveConfFile(Properties *pts, System *sys) {

        String fpath = pts->getString(a8_properties, "");
        if (fpath != "") {
            return fpath;
        }
        String file2 = sys->getEnv(A8_PROPERTIES);
        return file2;
    }
    static void loadDefaultProperties(Properties *properties) {
        properties->set(P_fcs_servo_idx_ar, 0);
        properties->set(P_fcs_servo_idx_fl, 1);
        properties->set(P_fcs_servo_idx_al, 2);
        properties->set(P_fcs_servo_idx_fr, 3);
        properties->set(P_fcs_servo_fr_clockwise, true);
        properties->set(P_fcs_att_tick_rate, 1000); // HZ
    }

public:
    static void resolveProperties(int argc, char **argv, Properties *pts, System *sys) {

        // build int properties
        loadDefaultProperties(pts);

        // command line arguments
        Buffer<String> buf;
        Buffer<String> args = StringUtil::strings(argc, argv);
        for (int i = 0; i < args.length(); i++) {
            String str = args.get(i);
            if (str.length() > 10000) {
                Lang::bug();
            }
        }
        pts->setLines(args);

        // configuration file
        String fpath = resolveConfFile(pts, sys);

        (*sys) << "a8 properties file path:" << fpath;

        if (fpath != 0) {
            Reader *fr;
            int ret = S->openFile(fpath, fr);
            Properties *pts2 = new Properties();
            pts2->load(*fr);
            pts->mergeFrom(pts2, false);
        }
    }
};
} // namespace  a8::common