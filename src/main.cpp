#include <iostream>
#include <vector>
#include <string>
#include "http.h"
#include "sqlite3.h"
#include "server.h"
#include <chrono>
#include <fstream>
#include "../include/rapidjson/document.h"
#include "../include/rapidjson/writer.h"
#include "../include/rapidjson/stringbuffer.h"

#include "utilities.h"
#include "ping.h"

using namespace std;
using namespace rapidjson;

const std::string VERSION = "0.1";

void greetings();
void run_server();
void sleep(int x);
std::vector<std::string> getAvailableDevices();
void storeDevicesToDB(const std::vector<std::string> &devices);
void json(const std::vector<std::string> &devices);
void writeValidationJson();
bool isRelay(const std::string c);
void writeIndexhtml();

std::string ipw = "192.168.1.1/24";


int main() {
  greetings();
  try {

    std::vector<std::string> devices = getAvailableDevices();
    storeDevicesToDB(devices);

    writeValidationJson();
    json(devices);
    writeIndexhtml();
    run_server();

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return 0;
}

void greetings(){
  std::string logo = R"logo(
               ,^,
             ,'   `,
           .'       `,
         .`:  1   0  :`.
           :         :
           :  1   1  :
           ...........

       <Moonshard Project>
  Host backend version: )logo";
  std::cout << logo << VERSION << "\n";
  std::cout << "https://moonshardProject.github.io/ \n\n";
}

void run_server(){
  std::cout << '\n';
  std::cout << "Starting server...\n";
  std::cout << "http://127.0.0.1:8080\n";
  std::cout << "Press Ctrl + C to quit the server.\n";
  auto const address = boost::asio::ip::make_address("0.0.0.0");
  auto const port = static_cast<unsigned short>(std::atoi("8080"));
  std::string const root = ".";

  boost::asio::io_context ioc{1};
  tcp::acceptor acceptor{ioc, {address, port}};
  for(;;) {
    tcp::socket socket{ioc};
    acceptor.accept(socket);
    std::thread{std::bind(
      &do_session,
      std::move(socket),
      root)}.detach();
    }
}

std::vector<std::string> getAvailableDevices() {
  std::vector<std::string> availableHosts = Ping::getAvailableHosts(ipw);
  std::vector<std::string> devices;
  HTTP h;
  for(const auto &ip : availableHosts){

    std::cout << "Looking up " << ip << '\n';
    string c = h.connect(ip);
    if(isRelay(c)) {
      cout << "Device found: " << ip << '\n';
      devices.push_back(ip);
    }
    sleep(10);
  }
  return devices;
}

void storeDevicesToDB(const std::vector<std::string> &devices){
    SQLite3 s("devices.db");
    std::string sql = "DROP TABLE IF EXISTS Devices;"
    "CREATE TABLE Devices(Id INT, ip TEXT);";

    for(int i = 0; i < devices.size(); i++){
      sql += "INSERT INTO Devices VALUES(" + to_string(i) + ", '" + devices[i] + "');";
    }
    s.execute(sql);
}

void json(const std::vector<std::string> &devices){

  rapidjson::Document d;

  // define the document as an object rather than an array
  d.SetObject();

  // create a rapidjson array type
  Value array(rapidjson::kArrayType);

  // must pass an allocator when the object may need to allocate memory
  Document::AllocatorType& allocator = d.GetAllocator();

  for(const auto &d : devices){
    Value strVal;
    strVal.SetString(d.c_str(), d.length(), allocator);
    array.PushBack(strVal, allocator);
  }

  d.AddMember("numberOfDevices", devices.size(), allocator);
  d.AddMember("addresses", array, allocator);

  StringBuffer buffer;
  Writer<StringBuffer> writer(buffer);
  d.Accept(writer);

  ofstream of{"devices.json"};
  of << buffer.GetString() << '\n';
}

void writeValidationJson() {
  ofstream of{"validation.json"};
  of << "{\"statusText\":\"OK\", \"name\":\"Moonshard\"}";
}

bool isRelay(const std::string c) {
  std::ifstream t{"../pages/relayhtml1.txt"};
  std::ifstream t2{"../pages/relayhtml2.txt"};
  std::stringstream buffer;
  std::stringstream buffer2;

  buffer2 << t2.rdbuf();
  buffer << t.rdbuf();

  if(c == buffer.str() || c == buffer2.str()){
    return true;
  } else {
    return false;
  }
}


void writeIndexhtml(){
  std::string indexHtml = R"index(
<!DOCTYPE html>
<html>
	<head> <title> Moonshard project host web app!</title> </head>
	<body>
		<p> Get <a href="/validation.json">validation.json </a>  for validating moonshard host system.</p>
		<p> Get <a href="/devices.json">devices.json</a> to get available devices list.</p>
	</body>
</html>
)index";
ofstream of{"index.html"};
of << indexHtml;

}
