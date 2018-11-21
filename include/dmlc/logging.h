/*!
 *    Copyright (c) 2015 by Contributors
 * \file logging.h
 * \brief defines logging macros of dmlc
 *    allows use of GLOG, fall back to internal
 *    implementation when disabled
 */
#ifndef DMLC_LOGGING_H_
#define DMLC_LOGGING_H_
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "./base.h"

namespace dmlc {
/*!
 * \brief exception class that will be thrown by
 *    default logger if DMLC_LOG_FATAL_THROW == 1
 */
struct Error : public std::runtime_error {
    /*!
     * \brief constructor
     * \param s the error message
     */
    explicit Error(const std::string &s) : std::runtime_error(s) {}
};
}    // namespace dmlc

#if defined(_MSC_VER) && _MSC_VER < 1900
#define noexcept(a)
#endif

#if DMLC_USE_CXX11
#define DMLC_THROW_EXCEPTION noexcept(false)
#else
#define DMLC_THROW_EXCEPTION
#endif

#if DMLC_USE_GLOG
#include <string>
#include <dirent.h>
#include <sys/stat.h>
#include <glog/logging.h>

namespace dmlc {

inline bool DirExists(const std::string& dir) {
    struct stat info;
    if (stat(dir.c_str(), &info) != 0) return false;
    if (info.st_mode & S_IFDIR) return true;
    return false;
}

inline bool CreateDir(const std::string& dir) {
    return mkdir(dir.c_str(), 0755) == 0;
}

inline void InitGlogging(int argc, char** argv) {
    if (FLAGS_log_dir.empty()) {
        FLAGS_log_dir = "/tmp";
    }
    if (!DirExists(FLAGS_log_dir)) {
        CreateDir(FLAGS_log_dir);
    }
    // change the hostname in default log filename to node id
    std::string program_name = std::string(argv[0]);
    std::string node_name = "";
    if (argc < 2) {
        node_name = "node_xxx";
    } else {
        node_name = argv[1];
    }
    std::string logfile = FLAGS_log_dir + "/" + program_name + "." + node_name + ".log.";
    // debug
    // std::cout << "logfile--->" << logfile << std::endl;
    google::SetLogDestination(google::INFO, (logfile + "INFO.").c_str());
    google::SetLogDestination(google::WARNING, (logfile + "WARNING.").c_str());
    google::SetLogDestination(google::ERROR, (logfile + "ERROR.").c_str());
    google::SetLogDestination(google::FATAL, (logfile + "FATAL.").c_str());
    google::SetLogSymlink(google::INFO, "");
    google::SetLogSymlink(google::WARNING, "");
    google::SetLogSymlink(google::ERROR, "");
    google::SetLogSymlink(google::FATAL, "");
    FLAGS_logbuflevel = -1;
}

inline void InitLogging(const char* argv0) {
    // debug
    // std::cout << "argv0--->" << std::string(argv0) << std::endl;
    google::InitGoogleLogging(argv0);
    // set glog log dir
    // FLAGS_log_dir = "./log";
    FLAGS_log_dir = "/Users/baidu/Documents/parameter_server_11_4/ps-lite/tests/log";
    // debug
    // std::cout << "google::InitGoogleLogging successful!!!" << std::endl;
}

inline void InitLogging(int argc, char** argv) {
    google::InitGoogleLogging(argv[0]);
    google::ParseCommandLineFlags(&argc, &argv, true);
    InitGlogging(argc, argv);
}

inline void DelLogging() {
    google::ShutDownCommandLineFlags();
}

}    // namespace dmlc

#else
// use a light version of glog
#include <assert.h>
#include <iostream>
#include <sstream>
#include <ctime>

#if defined(_MSC_VER)
#pragma warning(disable : 4722)
#endif

namespace dmlc {
inline void InitLogging(int argc, char** argv) {
    // DO NOTHING
}

inline void InitLogging(const char* argv0) {
    // DO NOTHING
}

inline void DelLogging() {
    // DO NOTHING
}

// Always-on checking
#define CHECK(x)                                                                                     \
    if (!(x))                                                                                                \
        dmlc::LogMessageFatal(__FILE__, __LINE__).stream() << "Check "    \
            "failed: " #x << ' '
#define CHECK_LT(x, y) CHECK((x) < (y))
#define CHECK_GT(x, y) CHECK((x) > (y))
#define CHECK_LE(x, y) CHECK((x) <= (y))
#define CHECK_GE(x, y) CHECK((x) >= (y))
#define CHECK_EQ(x, y) CHECK((x) == (y))
#define CHECK_NE(x, y) CHECK((x) != (y))
#define CHECK_NOTNULL(x) \
    ((x) == NULL ? dmlc::LogMessageFatal(__FILE__, __LINE__).stream() << "Check    notnull: "    #x << ' ', (x) : (x)) // NOLINT(*)
// Debug-only checking.
#ifdef NDEBUG
#define DCHECK(x) \
    while (false) CHECK(x)
#define DCHECK_LT(x, y) \
    while (false) CHECK((x) < (y))
#define DCHECK_GT(x, y) \
    while (false) CHECK((x) > (y))
#define DCHECK_LE(x, y) \
    while (false) CHECK((x) <= (y))
#define DCHECK_GE(x, y) \
    while (false) CHECK((x) >= (y))
#define DCHECK_EQ(x, y) \
    while (false) CHECK((x) == (y))
#define DCHECK_NE(x, y) \
    while (false) CHECK((x) != (y))
#else
#define DCHECK(x) CHECK(x)
#define DCHECK_LT(x, y) CHECK((x) < (y))
#define DCHECK_GT(x, y) CHECK((x) > (y))
#define DCHECK_LE(x, y) CHECK((x) <= (y))
#define DCHECK_GE(x, y) CHECK((x) >= (y))
#define DCHECK_EQ(x, y) CHECK((x) == (y))
#define DCHECK_NE(x, y) CHECK((x) != (y))
#endif    // NDEBUG

#define LOG_INFO dmlc::LogMessage(__FILE__, __LINE__)
#define LOG_ERROR LOG_INFO
#define LOG_WARNING LOG_INFO
#define LOG_FATAL dmlc::LogMessageFatal(__FILE__, __LINE__)
#define LOG_QFATAL LOG_FATAL

// Poor man version of VLOG
#define VLOG(x) LOG_INFO.stream()

#define LOG(severity) LOG_##severity.stream()
#define LG LOG_INFO.stream()
#define LOG_IF(severity, condition) \
    !(condition) ? (void)0 : dmlc::LogMessageVoidify() & LOG(severity)

#ifdef NDEBUG
#define LOG_DFATAL LOG_ERROR
#define DFATAL ERROR
#define DLOG(severity) true ? (void)0 : dmlc::LogMessageVoidify() & LOG(severity)
#define DLOG_IF(severity, condition) \
    (true || !(condition)) ? (void)0 : dmlc::LogMessageVoidify() & LOG(severity)
#else
#define LOG_DFATAL LOG_FATAL
#define DFATAL FATAL
#define DLOG(severity) LOG(severity)
#define DLOG_IF(severity, condition) LOG_IF(severity, condition)
#endif

// Poor man version of LOG_EVERY_N
#define LOG_EVERY_N(severity, n) LOG(severity)

class DateLogger {
 public:
    DateLogger() {
#if defined(_MSC_VER)
        _tzset();
#endif
    }
    const char* HumanDate() {
#if defined(_MSC_VER)
        _strtime_s(buffer_, sizeof(buffer_));
#else
        time_t time_value = time(NULL);
        struct tm now;
        localtime_r(&time_value, &now);
        snprintf(buffer_, sizeof(buffer_), "%02d:%02d:%02d", now.tm_hour,
                         now.tm_min, now.tm_sec);
#endif
        return buffer_;
    }
 private:
    char buffer_[9];
};

class LogMessage {
 public:
    LogMessage(const char* file, int line)
            :
#ifdef __ANDROID__
                log_stream_(std::cout)
#else
                log_stream_(std::cerr)
#endif
    {
        log_stream_ << "[" << pretty_date_.HumanDate() << "] " << file << ":"
                                << line << ": ";
    }
    ~LogMessage() { log_stream_ << "\n"; }
    std::ostream& stream() { return log_stream_; }

 protected:
    std::ostream& log_stream_;

 private:
    DateLogger pretty_date_;
    LogMessage(const LogMessage&);
    void operator=(const LogMessage&);
};

#if DMLC_LOG_FATAL_THROW == 0
class LogMessageFatal : public LogMessage {
 public:
    LogMessageFatal(const char* file, int line) : LogMessage(file, line) {}
    ~LogMessageFatal() {
        log_stream_ << "\n";
        abort();
    }

 private:
    LogMessageFatal(const LogMessageFatal&);
    void operator=(const LogMessageFatal&);
};
#else
class LogMessageFatal {
 public:
    LogMessageFatal(const char* file, int line) {
        log_stream_ << "[" << pretty_date_.HumanDate() << "] " << file << ":"
                                << line << ": ";
    }
    std::ostringstream &stream() { return log_stream_; }
    ~LogMessageFatal() DMLC_THROW_EXCEPTION {
        // throwing out of destructor is evil
        // hopefully we can do it here
        // also log the message before throw
        LOG(ERROR) << log_stream_.str();
        throw Error(log_stream_.str());
    }

 private:
    std::ostringstream log_stream_;
    DateLogger pretty_date_;
    LogMessageFatal(const LogMessageFatal&);
    void operator=(const LogMessageFatal&);
};
#endif

// This class is used to explicitly ignore values in the conditional
// logging macros.    This avoids compiler warnings like "value computed
// is not used" and "statement has no effect".
class LogMessageVoidify {
 public:
    LogMessageVoidify() {}
    // This has to be an operator with a precedence lower than << but
    // higher than "?:". See its usage.
    void operator&(std::ostream&) {}
};

}    // namespace dmlc

#endif
#endif    // DMLC_LOGGING_H_
