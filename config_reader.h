#include "json.hpp"
#include <fstream>

class ConfigReader {
 public:
  nlohmann::json configJson;
  ConfigReader() {
    std::ifstream stream("/home/artyom/CLionProjects/cats/roguelike/config.json");
    stream >> configJson;
  }
};