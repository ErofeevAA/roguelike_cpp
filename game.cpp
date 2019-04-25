#include "game.h"
#include <ncurses.h>
#include <vector>
#include <string>

void Game::loadMap() {
  const int y_max = 20;
  std::string str_map[y_max] = {"########################################",
                                "#K..#.......#........#............#....#",
                                "#...#............................#.....#",
                                "#...............#.......Z.......##.....#",
                                "###........Z....#................##....#",
                                "#..#..##########.......................#",
                                "#....Z.............#............Z......#",
                                "#..........................#...........#",
                                "##..#........#..............#..........#",
                                "#..Z#...........#.....#......#.....#...#",
                                "#...#...........#.....................##",
                                "#..........#....#......................#",
                                "#...#................############....#.#",
                                "#....................#................##",
                                "#.....................Z...........Z...##",
                                "#....Z.....Z....Z.....................##",
                                "#..................##.....#...##########",
                                "#........#........#.......######......P#",
                                "#...............Z......................#",
                                "########################################"};
  for (int i = 0; i < y_max; ++i) {
    //this->map.push_back(std::vector<char>());
    for (int j = 0; j < 40 ; ++j) {
      //this->map[i].push_back(str_map[i][j]);
      map[i][j] = str_map[i][j];
      if (this->map[i][j] == 'K') {
        knight.setCoord(i, j);
      } else if (this->map[i][j] == 'Z') {
        Zombie z = Zombie();
        z.setCoord(i, j);
        zombies.push_back(z);
      }
    }
  }
}

void Game::init() {
  loadMap();

}

void Game::start() {
  initscr();
  noecho();
  refresh();
  curs_set(0);
  keypad(stdscr,true);
  int command;
  while (true) {
    drawGame();
    command = getch();
    int up = -1 * (command == KEY_UP);
    int down = (command == KEY_DOWN);
    int left = -1 * (command == KEY_LEFT);
    int right = (command == KEY_RIGHT);
    moveZombies();
    moveKnight(up, down, left, right);

    if (princess.isGetting()) {
      break;
    }
    if (knight.getHP() == 0) {
      break;
    }
  }
  endwin();
}
void Game::moveZombies() {
  for (auto &zombie : zombies) {
    int side = randSide();
    if (side == 0) {
      if (!collisionForZombie(zombie.getCoord_Y() - 1, zombie.getCoord_X())) {
        this->map[zombie.getCoord_Y()][zombie.getCoord_X()] = '.';
        zombie.setCoord(zombie.getCoord_Y() - 1, zombie.getCoord_X());
        this->map[zombie.getCoord_Y()][zombie.getCoord_X()] = 'Z';
      }
    } else if (side == 1) {
      if (!collisionForZombie(zombie.getCoord_Y(), zombie.getCoord_X() + 1)) {
        this->map[zombie.getCoord_Y()][zombie.getCoord_X()] = '.';
        zombie.setCoord(zombie.getCoord_Y(), zombie.getCoord_X() + 1);
        this->map[zombie.getCoord_Y()][zombie.getCoord_X()] = 'Z';
      }
    } else if (side == 2) {
      if (!collisionForZombie(zombie.getCoord_Y() + 1, zombie.getCoord_X())) {
        this->map[zombie.getCoord_Y()][zombie.getCoord_X()] = '.';
        zombie.setCoord(zombie.getCoord_Y() + 1, zombie.getCoord_X());
        this->map[zombie.getCoord_Y()][zombie.getCoord_X()] = 'Z';
      }
    } else if (side == 3) {
      if (!collisionForZombie(zombie.getCoord_Y(), zombie.getCoord_X() - 1)) {
        this->map[zombie.getCoord_Y()][zombie.getCoord_X()] = '.';
        zombie.setCoord(zombie.getCoord_Y(), zombie.getCoord_X() - 1);
        this->map[zombie.getCoord_Y()][zombie.getCoord_X()] = 'Z';
      }
    }
  }
}
void Game::moveKnight(int up, int down, int left, int right) {
  if (!collisionForKnight(knight.getCoord_Y() + up + down,
      knight.getCoord_X() + left + right)) {
    this->map[knight.getCoord_Y()][knight.getCoord_X()] = '.';
    knight.setCoord(knight.getCoord_Y() + up + down,
                    knight.getCoord_X() + left + right);
    this->map[knight.getCoord_Y()][knight.getCoord_X()] = 'K';
  }
}
int Game::randSide() {
  int r = rand() % 4;
  return r;
}
bool Game::collisionForZombie(int y, int x) {
  if (this->map[y][x] == '#' || this->map[y][x] == 'P' || this->map[y][x] == 'Z') {
    return true;
  } else if (this->map[y][x] == 'K') {
    knight.wasAttacked();
    return true;
  } else {
    return false;
  }
}
bool Game::collisionForKnight(int y, int x) {
  if (this->map[y][x] == '#') {
    return true;
  } else if (this->map[y][x] == 'P') {
    princess.setGetting();
    return true;
  } else if (this->map[y][x] == 'Z') {
    killZombie(y, x);
    return true;
  } else {
    return false;
  }
}
void Game::killZombie(int y, int x) {
  this->map[y][x] = '.';
  for (int i = 0; i < zombies.size(); ++i) {
    if (zombies[i].getCoord_Y() == y && zombies[i].getCoord_X() == x) {
      zombies.erase(zombies.begin() + i);
      break;
    }
  }
}
void Game::drawGame() {
  refresh();
  //char op[this->map.size()][this->map[0].size()];
  for (int i = 0; i < 20; ++i) {
    //for (int j = 0; j < this->map[i].size(); ++j) {
      //op[i][j] = this->map[i][j];
    //}
    mvprintw(i, 0, map[i]);
  }
  mvprintw(20 + 1, 15, "Current number of HP:");
  mvprintw(20 + 2, 27,"%d", knight.getHP());
  move(0,0);


}