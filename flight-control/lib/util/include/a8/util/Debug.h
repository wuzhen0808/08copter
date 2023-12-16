#pragma once

#ifndef A8_DEBUG_LEVEL
#define A8_DEBUG_LEVEL (3)
#endif

#define A8_DEBUG(exp)         // do nothing.
#define A8_DEBUG2(exp1, exp2) // do nothing.

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
