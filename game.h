#include <vector>
#include <string>
#include "objects.h"

class Game {
 private:
  Knight knight = Knight();
  Princess princess = Princess();
  std::vector<Zombie> zombies;
  char map[20][41];
  void loadMap();
  void moveZombies();
  void moveKnight(int up, int down, int left, int right);
  int randSide();
  void killZombie(int y, int x);
  void drawGame();
 public:
  Game() = default;
  void init();
  void start();
  bool collisionForZombie(int y, int x);
  bool collisionForKnight(int y, int x);
  //void end(); thinking...
};




