#include "API_Provider.hpp"
#include "Formatter.hpp"

#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/web/server/HttpConnectionHandler.hpp"

namespace REST {
API_Provider::API_Provider() {
  oatpp::base::Environment::init();

  auto uuid_string = toString(generateUUID());
  OATPP_LOGI("API_Provider", "UUID %s", uuid_string.c_str());

  auto router = oatpp::web::server::HttpRouter::createShared();
  auto connection_handler =
      oatpp::web::server::HttpConnectionHandler::createShared(router);
  auto connection_provider =
      oatpp::network::tcp::server::ConnectionProvider::createShared(
          {"localhost", 8000, oatpp::network::Address::IP_4});

  server_ = std::make_shared<oatpp::network::Server>(
      connection_provider, connection_handler);
}

API_Provider::~API_Provider() {
  server_.reset();
  oatpp::base::Environment::destroy();
}

void API_Provider::start() {
  if (!server_thread_) {
    server_thread_ = std::make_unique<std::thread>(
        [](const std::shared_ptr<oatpp::network::Server>& server) {
          server->run();
        },
        server_);
  }
}

void API_Provider::stop() {
  server_->stop();
  server_thread_->join();
  server_thread_.reset();
}
} // namespace REST