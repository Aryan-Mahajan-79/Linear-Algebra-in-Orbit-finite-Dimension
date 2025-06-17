#include "logger.h"

Logger::Logger() {
    log_file_.open("log.txt", std::ios::app); // append mode
}

Logger::~Logger() {
    if (log_file_.is_open()) {
        log_file_.close();
    }
}

Logger& Logger::instance() {
    static Logger logger_instance;
    return logger_instance;
}

void Logger::log(const std::string& message) {
    std::lock_guard<std::mutex> guard(log_mutex_);
    if (log_file_.is_open()) {
        log_file_ << message << std::endl;
    }
}
