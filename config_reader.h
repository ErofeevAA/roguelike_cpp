#include "json.hpp"
#include <fstream>

class ConfigReader {
 public:
  nlohmann::json configJson;
  ConfigReader() {
    std::ifstream stream("config.json");
    stream >> configJson;
  }
};