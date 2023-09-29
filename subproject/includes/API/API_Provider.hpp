#ifndef __CONAN_PACKAGE_EXAMPLE_API_PROVIDER_HPP
#define __CONAN_PACKAGE_EXAMPLE_API_PROVIDER_HPP

#include "oatpp/network/Server.hpp"

#include <memory>
#include <thread>

namespace REST {
struct API_Provider {
  API_Provider();
  ~API_Provider();

  void start();
  void stop();

private:
  std::shared_ptr<oatpp::network::Server> server_;
  std::unique_ptr<std::thread> server_thread_;
};
} // namespace REST

#endif //__CONAN_PACKAGE_EXAMPLE_API_PROVIDER_HPP
