#ifndef PING_H
#define PING_H

#include <vector>
#include <string>

class Ping {
public:
  static std::vector<std::string> getAvailableHosts( std::string ip );
};

#endif
