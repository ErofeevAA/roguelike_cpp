#include "game.h"
#include <ncurses.h>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

const int y_max = 20;
int x_max = 40;
const int esc = 27;
const int letter_w = 119;
const int letter_a = 97;
const int letter_s = 115;
const int letter_d = 100;

void Game::loadMap() {
  std::string str_map[y_max] = {"##########################################",
                                "#.K.#.......#........#.....Z......###....#",
                                "#...#.Z...Z........................#.....#",
                                "#...............#.......Z.......##....Z..#",
                                "###........Z....#...A............##..Z...#",
                                "#..#..##########.........Z...............#",
                                "#....Z........Z....#..D.........Z........#",
                                "#......Z..........Z........#.............#",
                                "##..#........#Z.............#....Z.......#",
                                "#..Z#.....Z.....#.....#......#.....#.....#",
                                "#...#...........#.......Z...............##",
                                "#..........#....#....Z...........Z.......#",
                                "#...#........Z.......############....#...#",
                                "#......Z........Z....#.....D............##",
                                "#.............Z.......Z...........Z.....##",
                                "#....Z.....Z....Z...............Z.##.Z..##",
                                "#....Z.............##.....#...####.#######",
                                "#.A......#........#.......########......P#",
                                "#.....Z.........Z.....D............#.....#",
                                "##########################################"};
  x_max = static_cast<int>(str_map[0].size());
  for (int i = 0; i < y_max; ++i) {
    for (int j = 0; j < x_max ; ++j) {
      this->map[i][j] = str_map[i][j];
      if (this->map[i][j] == 'K') {
        knight.setCoord(i, j);
      } else if (this->map[i][j] == 'Z') {
        Zombie z = Zombie();
        z.setCoord(i, j);
        zombies.push_back(z);
      } else if (this->map[i][j] == 'D') {
        Dragon d = Dragon();
        d.setCoord(i, j);
        dragons.push_back(d);
      }
    }
  }
}

void Game::init() {
  loadMap();
  initscr();
  noecho();
  refresh();
  int y = 3;
  int x = 7;
  mvprintw(y++, x, "***Battle for Et princeps Pisum***");
  mvprintw(y++, x, "Your aim is saving princess from zombies and dragons");
  mvprintw(y++, x, "arrow to up(down, left, right) move");
  mvprintw(y++, x, "W shoot fire up");
  mvprintw(y++, x, "D shoot fire right");
  mvprintw(y++, x, "S shoot fire down");
  mvprintw(y++, x, "A shoot fire left");
  mvprintw(y++, x, "ESC exit");
  mvprintw(y, x, "Press any key to start");
  move(0,0);
  getch();
  clear();
}

void Game::start() {
  refresh();
  curs_set(0);
  keypad(stdscr, true);
  //wait action from user (1 second)
  halfdelay(10);
  int command = 0;
  while (command != esc) {
    drawGame();
    command = getch();
    //mvprintw(21, 2, "%d", command);
    moveFire();
    if (command == KEY_UP || command == KEY_DOWN ||
        command == KEY_LEFT || command == KEY_RIGHT) {
      int up = -1 * (command == KEY_UP);
      int down = (command == KEY_DOWN);
      int left = -1 * (command == KEY_LEFT);
      int right = (command == KEY_RIGHT);
      moveKnight(up, down, left, right);
    } else if (command == letter_w || command == letter_s ||
               command == letter_a || command == letter_d) {
      int fireUp = -1 * (command == letter_w);
      int fireDown = (command == letter_s);
      int fireLeft  = -1 * (command == letter_a);
      int fireRight = (command == letter_d);
      //mvprintw(0, x_max + 1, "%d %d %d %d", fireUp, fireDown, fireLeft, fireRight);
      makeKnightMagic(fireUp, fireDown, fireLeft, fireRight);
    }
    moveZombies();
    moveDragon();
    if (princess.isGetting()) {
      nodelay(stdscr, false);
      endGame();
      win();
      break;
    }
    if (knight.getHP() < 1) {
      nodelay(stdscr, false);
      endGame();
      lose();
      break;
    }
  }

  endwin();
}

void Game::moveZombies() {
  for (auto &z : zombies) {
    int side = rand() % 4;
    if (side == 0) {
      if (!collisionForZombie(z.getCoord_Y() - 1, z.getCoord_X())) {
        this->map[z.getCoord_Y()][z.getCoord_X()] = '.';
        z.setCoord(z.getCoord_Y() - 1, z.getCoord_X());
        this->map[z.getCoord_Y()][z.getCoord_X()] = 'Z';
      }
    } else if (side == 1) {
      if (!collisionForZombie(z.getCoord_Y(), z.getCoord_X() + 1)) {
        this->map[z.getCoord_Y()][z.getCoord_X()] = '.';
        z.setCoord(z.getCoord_Y(), z.getCoord_X() + 1);
        this->map[z.getCoord_Y()][z.getCoord_X()] = 'Z';
      }
    } else if (side == 2) {
      if (!collisionForZombie(z.getCoord_Y() + 1, z.getCoord_X())) {
        this->map[z.getCoord_Y()][z.getCoord_X()] = '.';
        z.setCoord(z.getCoord_Y() + 1, z.getCoord_X());
        this->map[z.getCoord_Y()][z.getCoord_X()] = 'Z';
      }
    } else if (side == 3) {
      if (!collisionForZombie(z.getCoord_Y(), z.getCoord_X() - 1)) {
        this->map[z.getCoord_Y()][z.getCoord_X()] = '.';
        z.setCoord(z.getCoord_Y(), z.getCoord_X() - 1);
        this->map[z.getCoord_Y()][z.getCoord_X()] = 'Z';
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

void Game::moveDragon() {
  for (auto &d : dragons) {
    if (!d.isFire()) {
      int side = rand() % 4;
      switch (side) {
        case 0:
          if (!collisionForDragon(d.getCoord_Y() - 1, d.getCoord_X())) {
            this->map[d.getCoord_Y()][d.getCoord_X()] = '.';
            d.setCoord(d.getCoord_Y() - 1, d.getCoord_X());
            this->map[d.getCoord_Y()][d.getCoord_X()] = 'D';
          }
          break;
        case 1:
          if (!collisionForDragon(d.getCoord_Y() , d.getCoord_X() + 1)) {
            this->map[d.getCoord_Y()][d.getCoord_X()] = '.';
            d.setCoord(d.getCoord_Y(), d.getCoord_X() + 1);
            this->map[d.getCoord_Y()][d.getCoord_X()] = 'D';
          }
          break;
        case 2:
          if (!collisionForDragon(d.getCoord_Y() + 1, d.getCoord_X())) {
            this->map[d.getCoord_Y()][d.getCoord_X()] = '.';
            d.setCoord(d.getCoord_Y() + 1, d.getCoord_X());
            this->map[d.getCoord_Y()][d.getCoord_X()] = 'D';
          }
          break;
        case 3:
          if (!collisionForDragon(d.getCoord_Y(), d.getCoord_X() - 1)) {
            this->map[d.getCoord_Y()][d.getCoord_X()] = '.';
            d.setCoord(d.getCoord_Y(), d.getCoord_X() - 1);
            this->map[d.getCoord_Y()][d.getCoord_X()] = 'D';
          }
          break;
        default:break;
      }
      d.changeFire();
    } else {
      makeDragonFire(&d);
      d.changeFire();
    }
  }
}

void Game::moveFire() {
  int i = 0;
  while (i < fires.size()) {
    if (fires[i].isDestroy()) {
      killFire(fires[i].getCoord_Y(), fires[i].getCoord_X());
      continue;
    }
    switch (fires[i].getSide()) {
      case 0:
        if (!collisionForFire(fires[i].getCoord_Y() - 1, fires[i].getCoord_X())) {
          this->map[fires[i].getCoord_Y()][fires[i].getCoord_X()] = '.';
          fires[i].setCoord(fires[i].getCoord_Y() - 1, fires[i].getCoord_X());
          this->map[fires[i].getCoord_Y()][fires[i].getCoord_X()] = '*';
          ++i;
        } else {
          killFire(fires[i].getCoord_Y(), fires[i].getCoord_X());
        }
        break;
      case 1:
        if (!collisionForFire(fires[i].getCoord_Y(), fires[i].getCoord_X() + 1)) {
          this->map[fires[i].getCoord_Y()][fires[i].getCoord_X()] = '.';
          fires[i].setCoord(fires[i].getCoord_Y(), fires[i].getCoord_X() + 1);
          this->map[fires[i].getCoord_Y()][fires[i].getCoord_X()] = '*';
          ++i;
        } else {
          killFire(fires[i].getCoord_Y(), fires[i].getCoord_X());
        }
        break;
      case 2:
        if (!collisionForFire(fires[i].getCoord_Y() + 1, fires[i].getCoord_X())) {
          this->map[fires[i].getCoord_Y()][fires[i].getCoord_X()] = '.';
          fires[i].setCoord(fires[i].getCoord_Y() + 1, fires[i].getCoord_X());
          this->map[fires[i].getCoord_Y()][fires[i].getCoord_X()] = '*';
          ++i;
        } else {
          killFire(fires[i].getCoord_Y(), fires[i].getCoord_X());
        }
        break;
      case 3:
        if (!collisionForFire(fires[i].getCoord_Y(), fires[i].getCoord_X() - 1)) {
          this->map[fires[i].getCoord_Y()][fires[i].getCoord_X()] = '.';
          fires[i].setCoord(fires[i].getCoord_Y(), fires[i].getCoord_X() - 1);
          this->map[fires[i].getCoord_Y()][fires[i].getCoord_X()] = '*';
          ++i;
        } else {
          killFire(fires[i].getCoord_Y(), fires[i].getCoord_X());
        }
        break;
      default: break;
    }
  }
}

bool Game::collisionForZombie(int y, int x) {
  if (this->map[y][x] == '#' || this->map[y][x] == 'P' ||
      this->map[y][x] == 'Z' || this->map[y][x] == 'A' ||
      this->map[y][x] == '*' || this->map[y][x] == 'D') {
    return true;
  } else if (this->map[y][x] == 'K') {
    knight.wasAttacked(zombies[0].getDamage());
    return true;
  } else {
    return false;
  }
}

bool Game::collisionForKnight(int y, int x) {
  if (this->map[y][x] == '#') {
    return true;
  } else if (this->map[y][x] == 'D') {
    killDragon(y, x);
    return true;
  } else if (this->map[y][x] == '*' ) {
    knight.wasAttacked(fires[0].getDamage());
    killFire(y, x);
    return false;
  } else if (this->map[y][x] == 'P') {
    princess.setGetting();
    return true;
  } else if (this->map[y][x] == 'Z') {
    killZombie(y, x);
    return true;
  } else if (this->map[y][x] == 'A') {
    if (knight.getHP() < knight.getMaxHP()) {
      knight.upHP();
      return false;
    } else {
      return true;
    }
  } else {
    return false;
  }
}

bool Game::collisionForDragon(int y, int x) {
  if (this->map[y][x] == '#' || this->map[y][x] == 'D'  ||
      this->map[y][x] == 'A') {
    return  true;
  } else if (this->map[y][x] == 'Z') {
    killZombie(y, x);
    return true;
  } else if (this->map[y][x] == '*') {
    killFire(y, x);
    return true;
  } else if (this->map[y][x] == 'K') {
    knight.wasAttacked(dragons[0].getDamage());
    return true;
  } else {
    return false;
  }
}

bool Game::collisionForFire(int y, int x) {
  if (this->map[y][x] == '#' || this->map[y][x] == 'D' ||
      this->map[y][x] == 'A' || this->map[y][x] == 'P') {
    return  true;
  } else if (this->map[y][x] == 'Z') {
    killZombie(y, x);
    return true;
  } else if (this->map[y][x] == 'K') {
    knight.wasAttacked(fires[0].getDamage());
    return true;
  } else {
    return false;
  }
}

bool Game::isMakingFire(int y, int x, int s) {
  if (this->map[y][x] == 'Z') {
    killZombie(y, x);
    return true;
  } else if (this->map[y][x] == 'K') {
    knight.wasAttacked(fires[0].getDamage());
    return true;
  } else if (this->map[y][x] == 'P' || this->map[y][x] == '#' ||
      this->map[y][x] == 'A'|| this->map[y][x] == 'D') {
    return false;
  } else {
    Fire fire = Fire();
    fire.setCoord(y, x);
    fire.setSide(s);
    fires.push_back(fire);
    this->map[y][x] = '*';
    return true;
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

void Game::killFire(int y, int x) {
  for (int i = 0; i < fires.size(); ++i) {
    if (fires[i].getCoord_Y() == y && fires[i].getCoord_X() == x) {
      this->map[fires[i].getCoord_Y()][fires[i].getCoord_X()] = '.';
      fires.erase(fires.begin() + i);
      break;
    }
  }
}

void Game::killDragon(int y, int x) {
  this->map[y][x] = '.';
  for (int i = 0; i < dragons.size(); ++i) {
    if (dragons[i].getCoord_Y() == y && dragons[i].getCoord_X() == x) {
      dragons.erase(dragons.begin() + i);
      break;
    }
  }
}

void Game::makeDragonFire(Dragon* dragon) {
  int s = 0;
  if (isMakingFire(dragon->getCoord_Y() - 1, dragon->getCoord_X(), s++)) {
    //dummy
  } else if (isMakingFire(dragon->getCoord_Y(), dragon->getCoord_X() + 1, s++)) {
    //dummy
  } else if (isMakingFire(dragon->getCoord_Y() + 1, dragon->getCoord_X(), s++)) {
    //dummy
  } else if (isMakingFire(dragon->getCoord_Y(), dragon->getCoord_X() - 1, s)) {
    //dummy
  }
}

void Game::makeKnightMagic(int up, int down, int left, int right) {
  if (knight.getMP() > 0) {
    int s;
    if (up != 0) {
      s = 0;
    } else if (right != 0) {
      s = 1;
    } else if (down != 0) {
      s = 2;
    } else {
      s = 3;
    }
    if (isMakingFire(knight.getCoord_Y() + up + down,
                     knight.getCoord_X() + left + right, s)) {
      knight.setMP(knight.getMP() - 1);
    }
  }
}

void Game::drawGame() {
  clear();
  for (int i = 0; i < y_max; ++i) {
    mvprintw(i, 0, this->map[i]);
  }
  mvprintw(7, x_max + 1, "HP: %d", knight.getHP());
  mvprintw(8, x_max + 1, "MP: %d", knight.getMP());
  move(0,0);
}

void Game::endGame() {
  clear();
  mvprintw(10, 8, "Game Over");
  mvprintw(20, 9, "press any key to exit");
}

void Game::win() {
  mvprintw(11, 9, "You win");
  getch();
}

void Game::lose() {
  mvprintw(11, 9, "You lose");
  getch();
}