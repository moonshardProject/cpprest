#include "http.h"
#include <iostream>

std::string HTTP::connect(const std::string url){
  std::string content;
  struct curl_slist *list = NULL;
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 2L);

    // to disable console output:
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

    // to save content to string
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);

    // puts the timeout limit to 1 second so that program wont hangs on a request
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

    list = curl_slist_append(list, "Accept:text/html");
    list = curl_slist_append(list, "Shoesize: 10");
    list = curl_slist_append(list, "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:62.0) Gecko/20100101 Firefox/62.0");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

    res = curl_easy_perform(curl);
        if(res != CURLE_OK){
          // std::cout << curl_easy_strerror(res) << '\n';
    }
  }

  curl_slist_free_all(list); /* free the list again */
  return content;
}

HTTP::HTTP(){
  curl = curl_easy_init();
}

HTTP::~HTTP(){
  curl_easy_cleanup(curl);
}

size_t HTTP::write_data(void *content, size_t size, size_t nmemb, void *userp) {
  ((std::string*)userp)->append((char*)content, size * nmemb);
  return size * nmemb;
}

std::string HTTP::getCurlVersion(){
  return curl_version();
}
