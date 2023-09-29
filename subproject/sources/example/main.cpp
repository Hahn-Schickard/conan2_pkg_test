#include "API_Provider.hpp"

#include <thread>

int main() {
  using namespace std::chrono_literals;

  auto provider = REST::API_Provider();
  provider.start();

  std::this_thread::sleep_for(1s);
  provider.stop();

  return 0;
}