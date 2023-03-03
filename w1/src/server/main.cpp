#include "Server.hpp"

int main() {
  Server server("localhost", "2023", "2024");
  server.Run();
  return 0;
}