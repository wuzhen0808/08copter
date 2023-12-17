#pragma once

#ifndef A8_DEBUG_LEVEL
#define A8_DEBUG_LEVEL (3)
#endif

#ifndef A8_LOG_LEVEL
#define A8_LOG_LEVEL A8_DEBUG_LEVEL
#endif

#if A8_LOG_LEVEL <= 5
#define A8_LOG_ERROR(logger, expr) \
    {                              \
        logger->error(expr);       \
    }
#else
#define A8_LOG_ERROR (logger, expr) // do nothing
#endif

#if A8_LOG_LEVEL <= 4
#define A8_LOG_WARN(logger, expr) \
    {                             \
        logger->warn(expr);       \
    }
#else
#define A8_LOG_INFO(logger, expr)
#endif

#if A8_LOG_LEVEL <= 3
#define A8_LOG_INFO         \
    (logger, expr) {        \
        logger->info(expr); \
    }
#else
#define A8_LOG_INFO(logger, expr)
#endif

#if A8_LOG_LEVEL <= 2
#define A8_LOG_DEBUG(logger, expr) \
    {                              \
        logger->debug(expr);       \
    }
#define A8_OUT_DEBUG(out, expr) \
    {                           \
        out->println(expr);     \
    }

#else

#define A8_LOG_DEBUG(logger, expr)
#define A8_OUT_DEBUG(out, expr)

#endif

#if A8_LOG_LEVEL <= 1
#define A8_LOG_TRACE(logger, expr) \
    {                              \
        logger->trace(expr);       \
    }
#else
#define A8_LOG_TRACE(logger, expr)
#endif

namespace a8::util {
class Debug {
public:
    static void debug(const char *str);

    static void debug(const char *str, float fValue);

    static void debug(const char *str, float fValue, const char *str2);

    static void debug(const char *str, float exp1, float exp2, float exp3);

    static void debug(const char *str, float fValue1, float fValue2, float fValue3, float fValue4, float fValue5);
};
} // namespace a8::util

#ifndef A8_DEBUG
#if A8_LOG_LEVEL <= 2
#define A8_DEBUG(exp) \
    a8::util::Debug::debug(exp);
#else
#define A8_DEBUG(exp)
#endif
#endif

#ifndef A8_DEBUG2
#if A8_LOG_LEVEL <= 2
#define A8_DEBUG2(exp1, exp2) \
    a8::util::Debug::debug(exp1, exp2);
#else
#define A8_DEBUG2(exp1, exp2)
#endif
#endif

#ifndef A8_DEBUG3
#if A8_LOG_LEVEL <= 2
#define A8_DEBUG3(exp1, exp2, exp3) \
    a8::util::Debug::debug(exp1, exp2, exp3);
#else
#define A8_DEBUG3(exp1, exp2)
#endif
#endif

#ifndef A8_DEBUG4
#if A8_LOG_LEVEL <= 2
#define A8_DEBUG4(exp1, exp2, exp3, exp4) \
    a8::util::Debug::debug(exp1, exp2, exp3, exp4);
#else
#define A8_DEBUG4(exp1, exp2)
#endif
#endif

#ifndef A8_DEBUG5
#if A8_LOG_LEVEL <= 2
#define A8_DEBUG5(exp1, exp2, exp3, exp4, exp5) \
    a8::util::Debug::debug(exp1, exp2, exp3, exp4, exp5);
#else
#define A8_DEBUG5(exp1, exp2)
#endif
#endif

#ifndef A8_DEBUG6
#if A8_LOG_LEVEL <= 2
#define A8_DEBUG6(exp1, exp2, exp3, exp4, exp5, exp6) \
    a8::util::Debug::debug(exp1, exp2, exp3, exp4, exp5, exp6);
#else
#define A8_DEBUG6(exp1, exp2)
#endif
#endif
