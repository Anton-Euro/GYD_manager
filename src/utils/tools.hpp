#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <string>
#include <vector>
#include <cmath>
#include <regex>
#include <utility>
#include <cctype>
#include <algorithm>

inline std::string size_to_print(unsigned long long size, const std::string& prefix = "") {
    std::vector<std::string> name_size_file = {"bytes", "KB", "MB", "GB", "TB"};
    short d = 0;
    double total_size = size;
    while(total_size / 1024.0 > 1) {
        d++;
        total_size = round((total_size / 1024.0) * 100.0) / 100.0;
    }
    std::string str = std::to_string(total_size);
    int dot_pos = str.find('.');
    str = str.substr(0, dot_pos + 3);
    return str + " " + name_size_file[d] + prefix;
}

inline std::string seconds_to_timeprint(unsigned long long seconds) {
    std::string out_string = "";
    unsigned long long delta;
    delta = seconds / (60 * 60 * 24 * 7 * 365);
    seconds %= 60 * 60 * 24 * 7 * 365;
    out_string += delta > 0 ? std::to_string(delta)+"y" : "";
    delta = seconds / (60 * 60 * 24 * 7);
    seconds %= 60 * 60 * 24 * 7;
    out_string += delta > 0 ? std::to_string(delta)+"w" : "";
    delta = seconds / (60 * 60 * 24);
    seconds %= 60 * 60 * 24;
    out_string += delta > 0 ? std::to_string(delta)+"d" : "";
    delta = seconds / (60 * 60);
    seconds %= 60 * 60;
    out_string += delta > 0 ? std::to_string(delta)+"h" : "";
    delta = seconds / 60;
    seconds %= 60;
    out_string += delta > 0 ? std::to_string(delta)+"m" : "";
    out_string += seconds > 0 ? std::to_string(seconds)+"s" : "";

    return out_string;
}

inline std::string check_filename(const std::string& filename) {
    std::string cfilename = filename;
    
    std::regex invalidWindowsChars(R"([\\/:*?"<>|])");
    cfilename = std::regex_replace(cfilename, invalidWindowsChars, "_"); 

    size_t pos = cfilename.find('/');
    while (pos != std::string::npos) {
        cfilename.replace(pos, 1, "_");
        pos = cfilename.find('/');
    }

    static const std::vector<std::string> reservedNames = {
        "CON", "PRN", "AUX", "NUL", "COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9",
        "LPT1", "LPT2", "LPT3", "LPT4", "LPT5", "LPT6", "LPT7", "LPT8", "LPT9"
    };

    for (const auto& reservedName : reservedNames) {
        if (cfilename == reservedName) {
            cfilename = "_" + cfilename + "_reserved"; 
            break;
        }
    }

    return cfilename;
}

inline bool starts_with(const std::string& str, const std::string& prefix) {
    return str.compare(0, prefix.size(), prefix) == 0;
}

inline bool ends_with(const std::string& str, const std::string& suffix) {
    if (suffix.size() > str.size()) {
        return false;
    }
    return str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

inline bool good_path_for_yandex(const std::string& parent, const std::string& path, bool is_dir = false) {
    if(path.size() >= parent.size() && starts_with(path, parent)) {
        std::string a = path.substr(parent.size(), path.size());
        int count = 0;
        size_t pos = 0;
        if(is_dir == false) {
            while ((pos = a.find("/", pos)) != std::string::npos) {
                count++; pos++;
            }
            return count > 0 ? false : true;
        } else {
            std::string folder = a.substr(0, a.size() - 1);
            while ((pos = folder.find("/", pos)) != std::string::npos) {
                count++; pos++;
            }
            return count > 0 ? false : true;
        }
        
    }
    return false;
}

inline std::pair<std::string, std::string> cut_fullname(const std::string &fullname) {
    std::string name, extension;
    size_t last_dot = fullname.rfind('.');
    if (last_dot != std::string::npos) {
        name = fullname.substr(0, last_dot);
        extension = fullname.substr(last_dot + 1);
    } else {
        name = fullname;
        extension = "";
    }
    return {name, extension};
}

inline std::string to_lower_case(const std::string &input) {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return std::tolower(c); });
    return result;
}

#endif // TOOLS_HPP