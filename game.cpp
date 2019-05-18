#include "game.h"

#include <ncurses.h>

#include <iostream>


void Game::loadMap() {
  std::ifstream file_in("map.txt");
  int i = 0;
  std:: string str;
  while (!file_in.eof()) {
    file_in >> str;
    this->map.emplace_back();
    for (int j = 0; j < str.size(); ++j) {
      this->map[i].push_back(str[j]);
      if (this->map[i][j] == sym_knight) {
        knight.setCoord(i, j);
      } else if (this->map[i][j] == sym_zombie) {
        Zombie z = Zombie();
        z.setCoord(i, j);
        zombies.push_back(z);
      } else if (this->map[i][j] == sym_dragon) {
        Dragon d = Dragon();
        d.setCoord(i, j);
        dragons.push_back(d);
      }
    }
    ++i;
  }
}

void Game::init() {
  loadMap();
  initscr();
  noecho();
  curs_set(0);
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
  keypad(stdscr, true);
  //wait action from user (0.5 second)
  halfdelay(5);
  int command = 0;
  while (command != conf.configJson["Key"]["esc"]) {
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
      moveKnight(up + down, left + right);
    } else if (command == conf.configJson["Key"]["w"] ||
               command == conf.configJson["Key"]["s"] ||
               command == conf.configJson["Key"]["a"] ||
               command == conf.configJson["Key"]["d"]) {
      int fireUp = -1 * (command == conf.configJson["Key"]["w"]);
      int fireDown = (command == conf.configJson["Key"]["s"]);
      int fireLeft  = -1 * (command == conf.configJson["Key"]["a"]);
      int fireRight = (command == conf.configJson["Key"]["d"]);
      //mvprintw(0, x_max + 1, "%d %d %d %d", fireUp, fireDown, fireLeft, fireRight);
      int side = 0;
       side += 1 * (command == conf.configJson["Key"]["d"]);
       side += 2 * (command == conf.configJson["Key"]["s"]);
       side += 3 * (command == conf.configJson["Key"]["a"]);
      makeKnightMagic(fireUp + fireDown, fireLeft + fireRight, side);
    }
    moveZombies();
    moveDragon();
    if (princess.isGetting()) {
      halfdelay(100);
      endGame();
      win();
      break;
    }
    if (knight.getHP() < 1) {
      halfdelay(100);
      endGame();
      lose();
      break;
    }
  }

  endwin();
}

void Game::moveZombies() {
  int m[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
  for (auto &z : zombies) {
    int side = rand() % 4;
    if (!collisionForZombie(z.getCoord_Y() + m[side][0], z.getCoord_X() + m[side][1])) {
      this->map[z.getCoord_Y()][z.getCoord_X()] = sym_empty;
      z.setCoord(z.getCoord_Y() + m[side][0], z.getCoord_X() + m[side][1]);
      this->map[z.getCoord_Y()][z.getCoord_X()] = sym_zombie;
    }
  }
}

void Game::moveKnight(int py, int px) {
  if (!collisionForKnight(knight.getCoord_Y() + py,
      knight.getCoord_X() + px)) {
    this->map[knight.getCoord_Y()][knight.getCoord_X()] = sym_empty;
    knight.setCoord(knight.getCoord_Y() + py,
                    knight.getCoord_X() + px);
    this->map[knight.getCoord_Y()][knight.getCoord_X()] = sym_knight;
  }
}

void Game::moveDragon() {
  int m[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
  for (auto &d : dragons) {
    if (!d.isFire()) {
      int side = rand() % 4;
      if (!collisionForDragon(d.getCoord_Y() + m[side][0],
          d.getCoord_X() + m[side][1])) {
        this->map[d.getCoord_Y()][d.getCoord_X()] = sym_empty;
        d.setCoord(d.getCoord_Y() + m[side][0], d.getCoord_X() + m[side][1]);
        this->map[d.getCoord_Y()][d.getCoord_X()] = sym_dragon;
      }
      d.changeFire();
    } else {
      makeDragonFire(&d);
      d.changeFire();
    }
  }
}

void Game::moveFire() {
  int m[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
  int i = 0;
  while (i < fires.size()) {
    if (fires[i].isDestroy()) {
      killFire(fires[i].getCoord_Y(), fires[i].getCoord_X());
      continue;
    }
    if (!collisionForFire(fires[i].getCoord_Y() + m[fires[i].getSide()][0],
        fires[i].getCoord_X() + m[fires[i].getSide()][1])) {
      this->map[fires[i].getCoord_Y()][fires[i].getCoord_X()] = sym_empty;
      fires[i].setCoord(fires[i].getCoord_Y() + m[fires[i].getSide()][0],
          fires[i].getCoord_X() + m[fires[i].getSide()][1]);
      this->map[fires[i].getCoord_Y()][fires[i].getCoord_X()] = sym_fire;
      ++i;
    } else {
      killFire(fires[i].getCoord_Y(), fires[i].getCoord_X());
    }
  }
}

bool Game::collisionForZombie(int y, int x) {
  if (this->map[y][x] == sym_wall || this->map[y][x] == sym_princess ||
      this->map[y][x] == sym_zombie || this->map[y][x] == sym_aidkit ||
      this->map[y][x] == sym_fire || this->map[y][x] == sym_dragon ) {
    return true;
  } else if (this->map[y][x] == sym_knight) {
    knight.wasAttacked(zombies[0].getDamage());
    return true;
  } else {
    return false;
  }
}

bool Game::collisionForKnight(int y, int x) {
  if (this->map[y][x] == sym_wall) {
    return true;
  } else if (this->map[y][x] == sym_dragon) {
    killDragon(y, x);
    return true;
  } else if (this->map[y][x] == sym_fire ) {
    knight.wasAttacked(fires[0].getDamage());
    killFire(y, x);
    return false;
  } else if (this->map[y][x] == sym_princess) {
    princess.setGetting();
    return true;
  } else if (this->map[y][x] == sym_zombie) {
    killZombie(y, x);
    return true;
  } else if (this->map[y][x] == sym_aidkit) {
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
  if (this->map[y][x] == sym_wall || this->map[y][x] == sym_dragon  ||
      this->map[y][x] == sym_aidkit || this->map[y][x] == sym_princess) {
    return  true;
  } else if (this->map[y][x] == sym_zombie) {
    killZombie(y, x);
    return true;
  } else if (this->map[y][x] == sym_fire) {
    killFire(y, x);
    return true;
  } else if (this->map[y][x] == sym_knight) {
    knight.wasAttacked(dragons[0].getDamage());
    return true;
  } else {
    return false;
  }
}

bool Game::collisionForFire(int y, int x) {
  if (this->map[y][x] == sym_wall || this->map[y][x] == sym_dragon  ||
      this->map[y][x] == sym_aidkit || this->map[y][x] == sym_princess) {
    return  true;
  } else if (this->map[y][x] == sym_zombie) {
    killZombie(y, x);
    return true;
  } else if (this->map[y][x] == sym_knight) {
    knight.wasAttacked(fires[0].getDamage());
    return true;
  } else {
    return false;
  }
}

bool Game::isMakingFire(int y, int x, int s) {
  if (this->map[y][x] == sym_zombie) {
    killZombie(y, x);
    return true;
  } else if (this->map[y][x] == sym_fire) {
    knight.wasAttacked(fires[0].getDamage());
    return true;
  } else if (this->map[y][x] == sym_princess || this->map[y][x] == sym_wall ||
      this->map[y][x] == sym_aidkit || this->map[y][x] == sym_dragon) {
    return false;
  } else {
    int h = conf.configJson["Fire"]["hp"];
    Fire fire = Fire(h);
    fire.setCoord(y, x);
    fire.setSide(s);
    fires.push_back(fire);
    this->map[y][x] = sym_fire;
    return true;
  }
}

void Game::killZombie(int y, int x) {
  this->map[y][x] = sym_empty;
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
      this->map[fires[i].getCoord_Y()][fires[i].getCoord_X()] = sym_empty;
      fires.erase(fires.begin() + i);
      break;
    }
  }
}

void Game::killDragon(int y, int x) {
  this->map[y][x] = sym_empty;
  for (int i = 0; i < dragons.size(); ++i) {
    if (dragons[i].getCoord_Y() == y && dragons[i].getCoord_X() == x) {
      dragons.erase(dragons.begin() + i);
      break;
    }
  }
}

void Game::makeDragonFire(Dragon* dragon) {
  if (!tryRandomDragonFire(dragon)) {
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
}

bool Game::tryRandomDragonFire(Dragon *dragon) {
  int s = rand() % 4;
  return (s == 0 && isMakingFire(dragon->getCoord_Y() - 1, dragon->getCoord_X(), s)) ||
    (s == 1 && isMakingFire(dragon->getCoord_Y(), dragon->getCoord_X() + 1, s)) ||
    (s == 2 && isMakingFire(dragon->getCoord_Y() + 1, dragon->getCoord_X(), s)) ||
    (s == 3 && isMakingFire(dragon->getCoord_Y(), dragon->getCoord_X() - 1, s));
}

void Game::makeKnightMagic(int py, int px, int side) {
  if (knight.getMP() > 0 && isMakingFire(knight.getCoord_Y() + py,
      knight.getCoord_X() + px, side)) {
    knight.setMP(knight.getMP() - 1);
  }
}

void Game::drawGame() {
  //clear();
  for (int i = 0; i < map.size(); ++i) {
    for (int j = 0; j < map[i].size(); ++j) {
      char c = this->map[i][j];
      mvprintw(i, j, &c);
    }
    mvprintw(i, static_cast<int>(map[0].size()), "  ");
  }

  mvprintw(7, static_cast<int>(map[0].size() + 2), "HP: %d", knight.getHP());
  mvprintw(8, static_cast<int>(map[0].size() + 2), "MP: %d", knight.getMP());
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