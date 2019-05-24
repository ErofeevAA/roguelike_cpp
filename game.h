#include <vector>

#include "objects.h"

using std::vector;
using std::shared_ptr;
using std::make_shared;


class Game {
 private:
  ConfigReader conf = ConfigReader();

  Knight* knight;
  Princess* princess;

  vector<vector<shared_ptr<MapObject>>> map;

  void loadMap();

  void moveObj();

  void drawGame();
  void endGame();
  void win();
  void lose();

 public:
  Game();

  void play();
};