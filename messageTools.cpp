//
// Created by piotr on 07.12.18.
//

#include "messageTools.h"
#include "Commons.h"

std::string trim_copy(const std::string &s, const string &delimiters) {
    return trim_left_copy(trim_right_copy(s, delimiters), delimiters);
}

std::string trim_left_copy(const std::string &s, const string &delimiters) {
    return s.substr(s.find_first_not_of(delimiters));
}

std::string trim_right_copy(const std::string &s, const string &delimiters) {
    return s.substr(0, s.find_last_not_of(delimiters) + 1);
}

std::string &trim_inplace(std::string &s, const string &delimiters) {
    return trim_left_inplace(trim_right_inplace(s, delimiters), delimiters);
}

std::string &trim_left_inplace(std::string &s, const string &delimiters) {
    return s.erase(0, s.find_first_not_of(delimiters));
}

std::string &trim_right_inplace(std::string &s, const string &delimiters) {
    return s.erase(s.find_last_not_of(delimiters) + 1);
}


std::string get_first_word(std::string &s, const string &delimiter) {
    ssize_t end = s.find(delimiter);
    if (end == string::npos) {
        return s;
    }
    string result = trim_copy(s.substr(0, static_cast<unsigned long>(end)));
    s.erase(0, static_cast<unsigned long>(end) + 1);
    return result;
}
