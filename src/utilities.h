#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <thread>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

inline std::string executeCommand(const char *cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
      throw std::runtime_error("popen() failed!");
    }
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr) {
            result += buffer.data();
        }
    }
    return result;
}

inline std::vector<std::string> textSplit(std::string s, std::string b){
  std::vector<std::string> parts;
  boost::split(parts, s, boost::is_any_of(b), boost::token_compress_on);
  return parts;
}

inline void sleep(int x){
  std::this_thread::sleep_for(std::chrono::milliseconds(x));
}

#endif
