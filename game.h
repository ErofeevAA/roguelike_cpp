#include <vector>

#include "objects.h"
#include "config_reader.h"

class Game {
 private:
  ConfigReader conf = ConfigReader();
  const int num_wall = conf.configJson["Wall"]["symbol"];
  const int num_knight = conf.configJson["Knight"]["symbol"];
  const int num_zombie = conf.configJson["Zombie"]["symbol"];
  const int num_fire = conf.configJson["Fire"]["symbol"];
  const int num_empty = conf.configJson["Empty"]["symbol"];
  const int num_dragon = conf.configJson["Dragon"]["symbol"];
  const int num_princess = conf.configJson["Princess"]["symbol"];
  const int num_aidkit = conf.configJson["Aidkit"]["symbol"];
  const char sym_wall = static_cast<char>(num_wall);
  const char sym_knight = static_cast<char>(num_knight);
  const char sym_zombie = static_cast<char>(num_zombie);
  const char sym_fire = static_cast<char>(num_fire);
  const char sym_empty = static_cast<char>(num_empty);
  const char sym_dragon = static_cast<char>(num_dragon);
  const char sym_princess = static_cast<char>(num_princess);
  const char sym_aidkit = static_cast<char>(num_aidkit);
  Knight knight = Knight(conf.configJson["Knight"]["hp"], conf.configJson["Knight"]["mp"]);
  Princess princess = Princess();
  std::vector<Zombie> zombies;
  std::vector<Dragon> dragons;
  std::vector<Fire> fires;
  std::vector<std::vector<char>> map;
  void loadMap();
  void moveZombies();
  void moveDragon();
  void moveFire();
  void moveKnight(int py, int px);
  void killZombie(int y, int x);
  void killFire(int y, int x);
  void killDragon(int y, int x);
  void makeDragonFire(Dragon* dragon);
  void makeKnightMagic(int px, int py, int side);
  void drawGame();
  void endGame();
  void win();
  void lose();
  bool tryRandomDragonFire(Dragon* dragon);
  bool collisionForZombie(int y, int x);
  bool collisionForKnight(int y, int x);
  bool collisionForDragon(int y, int x);
  bool collisionForFire(int y, int x);
  bool isMakingFire(int y, int x, int s);
 public:
  Game() = default;
  void init();
  void start();
};




