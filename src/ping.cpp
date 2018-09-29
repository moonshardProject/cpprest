#include "ping.h"
#include "utilities.h"
#include <iostream>

std::vector<std::string> Ping::getAvailableHosts( std::string ip ){
  std::cout << "Finding available hosts by pinging...\n";
  std::string cmd = "fping -a -q -g ";
  cmd += ip;
  std::string pingResults = executeCommand(cmd.c_str());
  return textSplit(pingResults, "\n");
}
