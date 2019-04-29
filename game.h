#include <vector>
#include <string>
#include "objects.h"

class Game {
 private:
  Knight knight = Knight();
  Princess princess = Princess();
  std::vector<Zombie> zombies;
  std::vector<Dragon> dragons;
  std::vector<Fire> fires;
  char map[20][43];
  void loadMap();
  void moveZombies();
  void moveDragon();
  void moveFire();
  void moveKnight(int up, int down, int left, int right);
  void killZombie(int y, int x);
  void killFire(int y, int x);
  void killDragon(int y, int x);
  void makeDragonFire(Dragon* dragon);
  void makeKnightMagic(int up, int down, int left, int right);
  void drawGame();
  void endGame();
  void win();
  void lose();
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




