#pragma once
#include <iostream>
#include <scelta.hpp>
using std::cerr;
using std::endl;
using std::get;
using std::pair;
using std::string;
using std::variant;

#include <boost/algorithm/string.hpp>

inline auto split(const string& s, char delim) {
    std::vector<std::string> results;
    boost::split(results, s, [&delim](char c) { return c == delim; });
    return results;
};

inline auto startswith(const string& s, const string& prefix) {
    return (prefix.size() <= s.size() && std::equal(prefix.begin(), prefix.end(), s.begin()));
};
