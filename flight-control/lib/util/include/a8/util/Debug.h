#pragma once
#define A8_LL_FATAL (5)
#define A8_LL_ERROR (4)
#define A8_LL_WARN (3)
#define A8_LL_INFO (2)
#define A8_LL_DEBUG (1)
#define A8_LL_TRACE (0)

#ifndef A8_LOG_LEVEL
#define A8_LOG_LEVEL A8_DEBUG_LEVEL
#endif

#if A8_LOG_LEVEL <= A8_LL_ERROR
#define A8_LOG_ERROR_ENABLED (2)
#else
#define A8_LOG_ERROR_ENABLED (0)
#endif

#if A8_LOG_LEVEL <= A8_LL_WARN
#define A8_LOG_WARN_ENABLED (1)
#else
#define A8_LOG_WARN_ENABLED (0)
#endif

#if A8_LOG_LEVEL <= A8_LL_INFO
#define A8_LOG_INFO_ENABLED (1)
#else
#define A8_LOG_INFO_ENABLED (0)
#endif

#if A8_LOG_LEVEL <= A8_LL_DEBUG
#define A8_LOG_DEBUG_ENABLED (1)
#else
#define A8_LOG_DEBUG_ENABLED (0)
#endif

#if A8_LOG_LEVEL <= A8_LL_TRACE
#define A8_LOG_TRACE_ENABLED (1)
#else
#define A8_LOG_TRACE_ENABLED (0)
#endif

#ifndef A8_DEBUG_LEVEL
#define A8_DEBUG_LEVEL (3)
#endif

#if A8_LOG_ERROR_ENABLED
#define A8_LOG_ERROR(logger, expr) \
    {                              \
        logger->error(expr);       \
    }
#else
#define A8_LOG_ERROR (logger, expr) // do nothing
#endif

#if A8_LOG_WARN_ENABLED
#define A8_LOG_WARN(logger, expr) \
    {                             \
        logger->warn(expr);       \
    }
#else
#define A8_LOG_INFO(logger, expr)
#endif

#if A8_LOG_INFO_ENABLED
#define A8_LOG_INFO(logger, expr) \
    {                             \
        logger->info(expr);       \
    }
#else
#define A8_LOG_INFO(logger, expr)
#endif

#if A8_LOG_DEBUG_ENABLED
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

#if A8_LOG_TRACE_ENABLED
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
    static void print(const char *str);

    static void println();

    static void print(float f);

    static void print(int i);

    static void print(long l);

    static void print(bool b);

    static void print(double d);

    static void print(unsigned i);

    static void print(unsigned long i);

    static void debug(const char *str);

    static void debug(const char *str, float fValue);

    static void debug(const char *str, float fValue, const char *str2);

    static void debug(const char *str, float exp1, float exp2, float exp3);

    static void debug(const char *str, float fValue1, float fValue2, float fValue3, float fValue4, float fValue5);
};
} // namespace a8::util

#ifndef A8_DEBUG_PRINT
#if A8_LOG_DEBUG_ENABLED
#define A8_DEBUG_PRINT(exp) \
    a8::util::Debug::print(exp);
#else
#define A8_DEBUG_PRINT(exp)
#endif
#endif

#ifndef A8_DEBUG_PRINTLN
#if A8_LOG_DEBUG_ENABLED
#define A8_DEBUG_PRINTLN() \
    a8::util::Debug::println();
#else
#define A8_DEBUG_PRINTLN()
#endif
#endif

#ifndef A8_DEBUG
#if A8_LOG_DEBUG_ENABLED
#define A8_DEBUG(exp) \
    a8::util::Debug::debug(exp);
#else
#define A8_DEBUG(exp)
#endif
#endif

#ifndef A8_DEBUG2
#if A8_LOG_DEBUG_ENABLED
#define A8_DEBUG2(exp1, exp2)         \
    {                                 \
        a8::util::Debug::print(exp1); \
        a8::util::Debug::print(exp2); \
        a8::util::Debug::println();   \
    }
#else
#define A8_DEBUG2(exp1, exp2)
#endif
#endif

#ifndef A8_DEBUG3
#if A8_LOG_DEBUG_ENABLED
#define A8_DEBUG3(exp1, exp2, exp3)   \
    {                                 \
        a8::util::Debug::print(exp1); \
        a8::util::Debug::print(exp2); \
        a8::util::Debug::print(exp3); \
        a8::util::Debug::println();   \
    }
#else
#define A8_DEBUG3(exp1, exp2, exp3)
#endif
#endif

#ifndef A8_DEBUG4
#if A8_LOG_DEBUG_ENABLED
#define A8_DEBUG4(exp1, exp2, exp3, exp4) \
    {                                     \
        a8::util::Debug::print(exp1);     \
        a8::util::Debug::print(exp2);     \
        a8::util::Debug::print(exp3);     \
        a8::util::Debug::print(exp4);     \
        a8::util::Debug::println();       \
    }

#else
#define A8_DEBUG4(exp1, exp2, exp3, exp4)
#endif
#endif

#ifndef A8_DEBUG5
#if A8_LOG_DEBUG_ENABLED
#define A8_DEBUG5(exp1, exp2, exp3, exp4, exp5) \
    {                                           \
        a8::util::Debug::print(exp1);           \
        a8::util::Debug::print(exp2);           \
        a8::util::Debug::print(exp3);           \
        a8::util::Debug::print(exp4);           \
        a8::util::Debug::print(exp5);           \
        a8::util::Debug::println();             \
    }

#else
#define A8_DEBUG5(exp1, exp2, exp3, exp4, exp5)
#endif
#endif

#ifndef A8_DEBUG6
#if A8_LOG_DEBUG_ENABLED
#define A8_DEBUG6(exp1, exp2, exp3, exp4, exp5, exp6) \
    {                                                 \
        a8::util::Debug::print(exp1);                 \
        a8::util::Debug::print(exp2);                 \
        a8::util::Debug::print(exp3);                 \
        a8::util::Debug::print(exp4);                 \
        a8::util::Debug::print(exp5);                 \
        a8::util::Debug::print(exp6);                 \
        a8::util::Debug::println();                   \
    }
#else
#define A8_DEBUG6(exp1, exp2, exp3, exp4, exp5, exp6)
#endif
#endif

#ifndef A8_DEBUG7
#if A8_LOG_DEBUG_ENABLED
#define A8_DEBUG7(exp1, exp2, exp3, exp4, exp5, exp6, exp7) \
    {                                                       \
        a8::util::Debug::print(exp1);                       \
        a8::util::Debug::print(exp2);                       \
        a8::util::Debug::print(exp3);                       \
        a8::util::Debug::print(exp4);                       \
        a8::util::Debug::print(exp5);                       \
        a8::util::Debug::print(exp6);                       \
        a8::util::Debug::print(exp7);                       \
        a8::util::Debug::println();                         \
    }
#else
#define A8_DEBUG7(exp1, exp2, exp3, exp4, exp5, exp6, exp7)
#endif
#endif

#ifndef A8_DEBUG8
#if A8_LOG_DEBUG_ENABLED
#define A8_DEBUG8(exp1, exp2, exp3, exp4, exp5, exp6, exp7, exp8) \
    {                                                             \
        a8::util::Debug::print(exp1);                             \
        a8::util::Debug::print(exp2);                             \
        a8::util::Debug::print(exp3);                             \
        a8::util::Debug::print(exp4);                             \
        a8::util::Debug::print(exp5);                             \
        a8::util::Debug::print(exp6);                             \
        a8::util::Debug::print(exp7);                             \
        a8::util::Debug::print(exp8);                             \
        a8::util::Debug::println();                               \
    }
#else
#define A8_DEBUG8(exp1, exp2, exp3, exp4, exp5, exp6, exp7, exp8)
#endif
#endif

#ifndef A8_DEBUG9
#if A8_LOG_DEBUG_ENABLED
#define A8_DEBUG9(exp1, exp2, exp3, exp4, exp5, exp6, exp7, exp8, exp9) \
    {                                                                   \
        a8::util::Debug::print(exp1);                                   \
        a8::util::Debug::print(exp2);                                   \
        a8::util::Debug::print(exp3);                                   \
        a8::util::Debug::print(exp4);                                   \
        a8::util::Debug::print(exp5);                                   \
        a8::util::Debug::print(exp6);                                   \
        a8::util::Debug::print(exp7);                                   \
        a8::util::Debug::print(exp8);                                   \
        a8::util::Debug::print(exp9);                                   \
        a8::util::Debug::println();                                     \
    }
#else
#define A8_DEBUG9(exp1, exp2, exp3, exp4, exp5, exp6, exp7, exp8, exp9)
#endif
#endif

#ifndef A8_DEBUG10
#if A8_LOG_DEBUG_ENABLED
#define A8_DEBUG10(exp1, exp2, exp3, exp4, exp5, exp6, exp7, exp8, exp9, exp10) \
    {                                                                           \
        a8::util::Debug::print(exp1);                                           \
        a8::util::Debug::print(exp2);                                           \
        a8::util::Debug::print(exp3);                                           \
        a8::util::Debug::print(exp4);                                           \
        a8::util::Debug::print(exp5);                                           \
        a8::util::Debug::print(exp6);                                           \
        a8::util::Debug::print(exp7);                                           \
        a8::util::Debug::print(exp8);                                           \
        a8::util::Debug::print(exp9);                                           \
        a8::util::Debug::println();                                             \
    }
#else
#define A8_DEBUG10(exp1, exp2, exp3, exp4, exp5, exp6, exp7, exp8, exp9, exp10)
#endif
#endif


#ifndef A8_TRACE
#if A8_LOG_TRACE_ENABLED
#define A8_TRACE(exp) \
    a8::util::Debug::debug(exp);
#else
#define A8_TRACE(exp)
#endif
#endif

#ifndef A8_TRACE2
#if A8_LOG_TRACE_ENABLED
#define A8_TRACE2(exp1, exp2)         \
    {                                 \
        a8::util::Debug::print(exp1); \
        a8::util::Debug::print(exp2); \
        a8::util::Debug::println();   \
    }
#else
#define A8_TRACE2(exp1, exp2)
#endif
#endif