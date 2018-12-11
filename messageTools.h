//
// Created by piotr on 07.12.18.
//

#ifndef SK_STRING_H
#define SK_STRING_H

#include <string>


using namespace std;

std::string &trim_right_inplace(
        std::string &s,
        const std::string &delimiters = " \f\n\r\t\v");

std::string &trim_left_inplace(
        std::string &s,
        const std::string &delimiters = " \f\n\r\t\v");

std::string &trim_inplace(
        std::string &s,
        const std::string &delimiters = " \f\n\r\t\v");

inline std::string trim_right_copy(
        const std::string &s,
        const std::string &delimiters = " \f\n\r\t\v");

inline std::string trim_left_copy(
        const std::string &s,
        const std::string &delimiters = " \f\n\r\t\v");

string trim_copy(
        const std::string &s,
        const std::string &delimiters = " \f\n\r\t\v");

std::string get_first_word(std::string &s, const string &delimiter);


#endif //SK_STRING_H
