#ifndef DATETIME_HPP
#define DATETIME_HPP

#include <sstream>
#include <chrono>
#include <iomanip>

namespace datetime {
    using time_point = std::chrono::system_clock::time_point;
}

inline std::string timepoint_to_string(datetime::time_point time) {
    std::time_t timeT = std::chrono::system_clock::to_time_t(time);
    std::tm tm;
    localtime_r(&timeT, &tm);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

inline datetime::time_point string_to_timepoint(const std::string& time_str) {
    std::tm tm = {};
    std::istringstream ss(time_str);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    if (ss.fail()) throw std::runtime_error("Failed to parse time string");
    std::time_t timeT = std::mktime(&tm);
    return std::chrono::system_clock::from_time_t(timeT);
}

inline datetime::time_point iso8601string_to_timepoint(const std::string& iso8601time_str) {
    std::tm tm = {};
    std::istringstream ss(iso8601time_str);
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
    if (ss.fail()) throw std::runtime_error("Failed to parse time string");
    std::time_t timeT = std::mktime(&tm);
    return std::chrono::system_clock::from_time_t(timeT);
}

inline datetime::time_point add_seconds_to_timepoint(datetime::time_point time, int seconds) {
    return time + std::chrono::seconds(seconds);
}

inline datetime::time_point get_now() {
    return std::chrono::system_clock::now();
}

#endif