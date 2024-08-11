#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>

class Logger {
  public:
    enum class Level { INFO, WARNING, ERROR };

    // Singleton instance
    static Logger &getInstance() {
        static Logger instance;
        return instance;
    }

    // Set the log file
    void setLogFile(const std::string &dir, const std::string &filename = "") {
        std::lock_guard<std::mutex> lock(mutex_);
        std::string path(dir);
        if (filename.empty()) {
            std::string defaultFilename = generateDefaultLogFileName(dir);
            fileStream_.open(defaultFilename,
                             std::ios_base::out | std::ios_base::app);
        } else {
            fileStream_.open(dir + filename,
                             std::ios_base::out | std::ios_base::app);
        }
        if (!fileStream_.is_open()) {
            throw std::runtime_error("Unable to open or create log file: " +
                                     filename);
        }
    }

    void closeLogFile() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (fileStream_.is_open()) {
            fileStream_.close();
        }
        buffer_.str("");
        buffer_.clear();
    }

    // Set the log level and context for the current message
    Logger &set(Level level, const char *file, int line) {
        std::lock_guard<std::mutex> lock(mutex_);
        currentLevel_ = level;
        currentFile_ = file;
        currentLine_ = line;
        buffer_.str(""); // Clear the buffer
        buffer_.clear();
        return *this;
    }

    // Overload << operator for various types
    template <typename T> Logger &operator<<(const T &value) {
        std::lock_guard<std::mutex> lock(mutex_);
        buffer_ << value;
        return *this;
    }

    // Overload << operator for manipulators (e.g., std::endl)
    Logger &operator<<(std::ostream &(*manip)(std::ostream &)) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (manip ==
            static_cast<std::ostream &(*)(std::ostream &)>(std::endl)) {
            log(currentLevel_, buffer_.str(), currentFile_, currentLine_);
            buffer_.str("");
            buffer_.clear();
        }
        return *this;
    }

  private:
    // Private constructor for singleton pattern
    Logger()
        : currentLevel_(Level::INFO), currentFile_(__FILE__),
          currentLine_(__LINE__) {}

    // Disable copy and assignment
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    // Log a message with a specific level
    void log(Level level, const std::string &message, const char *file,
             int line) {
        std::ostringstream oss;
        oss << "[" << currentDateTime() << "] ";
        oss << "[" << levelToString(level) << "] ";
        oss << "[" << file << ":" << line << "] ";
        oss << message << std::endl;

        std::string logMessage = oss.str();

        // Log to file
        if (fileStream_.is_open()) {
            fileStream_ << logMessage;
        }
    }

    // Convert log level to string
    std::string levelToString(Level level) {
        switch (level) {
        case Level::INFO:
            return "INFO";
        case Level::WARNING:
            return "WARNING";
        case Level::ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
        }
    }

    // Get the current date and time as a string
    std::string currentDateTime() {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::ostringstream oss;
        oss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
        return oss.str();
    }

    // Generate a default log file name with index
    std::string generateDefaultLogFileName(const std::string &dir = "./") {
        std::string baseName = dir + "log";
        int index = 1;
        std::string filename;
        while (true) {
            filename = baseName + std::to_string(index) + ".txt";
            if (!std::filesystem::exists(filename)) {
                break;
            }
            ++index;
        }
        return filename;
    }

    std::mutex mutex_;
    std::ofstream fileStream_;
    Level currentLevel_;
    const char *currentFile_;
    int currentLine_;
    std::ostringstream buffer_;
};

// Macros for logging with << operator
#define LOG(level)                                                             \
    Logger::getInstance().set(Logger::Level::level, __FILE__, __LINE__)

#endif // LOGGER_HPP
