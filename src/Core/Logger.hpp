#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <sys/time.h>
#include <cppformat/format.h>

#undef assert
#define assert(expr, msg)                                 \
    if (!(expr)) {                                        \
        FATAL("Assert (" #expr ") failed: \x1B[33m" msg); \
    }

#define FATAL(msg) Log::fatal(msg, __FILE__, __LINE__, __FUNCTION__);

namespace dm {

class Log {
   public:
    enum class Result { OK, ERROR };

    template <typename... arg_types>
    static void debug(const char* fmt, arg_types&&... args) {
        log("debug", fmt, std::forward<arg_types>(args)...);
    }

    template <typename... arg_types>
    static void info(const char* fmt, arg_types&&... args) {
        log("info", fmt, std::forward<arg_types>(args)...);
    }

    template <typename... arg_types>
    static void error(const char* fmt, arg_types&&... args) {
        meta("error");
        fmt::printf("\x1B[31m");
        fmt::printf(fmt, std::forward<arg_types>(args)...);
        fmt::printf("\x1B[0m");
        fmt::printf("\n");
    }

    [[noreturn]] static void fatal(const char* fmt, const char* file,
                                   const int line, const char* function_name) {
        meta("fatal");
        fmt::printf("\x1B[31m%s\x1B[0m in %s:%d::%s\n", fmt, file, line,
                    function_name);
        abort();
    }

    template <typename... arg_types>
    [[noreturn]] static void fatal(const char* fmt, arg_types&&... args) {
        meta("fatal");
        fmt::printf("\x1B[31m");
        fmt::printf(fmt, std::forward<arg_types>(args)...);
        fmt::printf("\x1B[0m");
        fmt::printf("\n");
        abort();
    }

    template <typename... arg_types>
    static void ok(const char* fmt, arg_types&&... args) {
        meta("ok");
        fmt::printf("\x1B[32m");
        fmt::printf(fmt, std::forward<arg_types>(args)...);
        fmt::printf("\x1B[0m");
        fmt::printf("\n");
    }

    template <typename... arg_types>
    static void log(const char* level, const char* fmt, arg_types&&... args) {
        echo(level, fmt, std::forward<arg_types>(args)...);
        fmt::printf("\n");
    }

    template <typename... arg_types>
    static void progress(const char* level, const char* fmt,
                         arg_types&&... args) {
        echo(level, fmt, std::forward<arg_types>(args)...);
        fmt::printf(" ... ");
    }

    template <typename... arg_types>
    static void result(const Result res) {
        switch (res) {
            case Result::OK:
                fmt::printf("\x1B[32mOK\x1B[0m");
                break;
            case Result::ERROR:
                fmt::printf("\x1B[31mERROR\x1B[0m");
                break;
        };
        printf("\n");
    }

   private:
    static struct timeval time_now() {
        struct timeval time;
        gettimeofday(&time, NULL);
        return time;
    }

    static void meta(const char* level) {
        static struct timeval begin = time_now();
        struct timeval time;

        gettimeofday(&time, NULL);

        fmt::printf("%8.3f - %-6s - ",
                    double(time.tv_sec - begin.tv_sec +
                           1e-6 * (time.tv_usec - begin.tv_usec)),
                    level);
    }

    template <typename... arg_types>
    static void echo(const char* level, const char* fmt, arg_types&&... args) {
        meta(level);
        fmt::printf(fmt, std::forward<arg_types>(args)...);
    }
};
}

#endif
