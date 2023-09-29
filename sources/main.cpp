#include "fmt/core.h"

#include "selinux/selinux.h"

#ifdef ENABLE_NANODBC
#include "nanodbc/nanodbc.h"
#endif

#ifdef ENABLE_CURL
#include "curl/curl.h"
#endif

#ifdef ENABLE_SUBPROJECT
#include "EXAMPLE_REST/API_Provider.hpp"
#endif

#include <chrono>
#include <memory>
#include <thread>

#ifdef ENABLE_SUBPROJECT
struct Server : REST::API_Provider {
  void start() {
    fmt::print("I am alive!\n");
    API_Provider::start();
  }

  void stop() {
    fmt::print("Please don't kill me...\n");
    API_Provider::stop();
  }
};
#endif

int main() {
  using namespace std::chrono_literals;

  fmt::print("Hello, world!\n");

#ifdef ENABLE_NANODBC
  auto odbc_drivers = nanodbc::list_drivers();
  for (const auto& driver : odbc_drivers) {
    fmt::print("{}\n", driver.name);
  }
#endif

#ifdef ENABLE_CURL
  auto* curl = curl_version_info(CURLVERSION_NOW);
  fmt::print("Curl version {}\n", curl->version);
#endif

#ifdef ENABLE_SUBPROJECT
  auto rest_server = std::make_shared<Server>();
  rest_server->start();
  std::this_thread::sleep_for(1s);
  rest_server->stop();
#endif

  fmt::print(
      "Selinux is {}\n", (is_selinux_enabled() != 0 ? "enabled" : "disabled"));

  fmt::print("Goodbye, cruel world!\n");
  return 0;
}