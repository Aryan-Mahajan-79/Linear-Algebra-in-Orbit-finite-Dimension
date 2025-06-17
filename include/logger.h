#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>
#include <mutex>

class Logger {
public:
    static Logger& instance();

    void log(const std::string& message);

private:
    Logger(); // constructor opens the log file
    ~Logger(); // destructor closes the file

    std::ofstream log_file_;
    std::mutex log_mutex_;
};

#endif
