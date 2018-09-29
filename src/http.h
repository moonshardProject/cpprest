#ifndef HTTP_H
#define HTTP_H

#include <curl/curl.h>
#include <string>

class HTTP {
public:
  HTTP();
  std::string connect(const std::string url);
  ~HTTP();
  static std::string getCurlVersion();
private:
  CURL *curl;
  CURLcode res;
  static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp);
};


#endif // HTTP_H
