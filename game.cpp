#include "game.h"

#include <iostream>

#include <string>


void Game::loadMap() {
  std::ifstream file_in("map.txt");
  int i = 0;
  std::string str;
  while (!file_in.eof()) {
    file_in >> str;
    this->map.emplace_back();
    for (int j = 0; j < str.size(); ++j) {
      if (str[j] == Knight(0, 0, 0, 0).getSymbol()) {
        shared_ptr<Knight> k = make_shared<Knight>(i, j, conf.configJson["Knight"]["hp"], conf.configJson["Knight"]["mp"]);
        knight = k.get();
        map[i].push_back(k);
      }
      if (str[j] == Zombie(0, 0).getSymbol()) {
        shared_ptr<Zombie> z = make_shared<Zombie>(i, j);
        map[i].push_back(z);
      }
      if (str[j] == Dragon(0, 0).getSymbol()) {
        shared_ptr<Dragon> d = make_shared<Dragon>(i, j);
        map[i].push_back(d);
      }
      if (str[j] == Princess().getSymbol()) {
        shared_ptr<Princess> p = make_shared<Princess>();
        princess = p.get();
        map[i].push_back(p);
      }
      if (str[j] == Wall().getSymbol()) {
        shared_ptr<Wall> w = make_shared<Wall>();
        map[i].push_back(w);
      }
      if (str[j] == AidKid().getSymbol()) {
        shared_ptr<AidKid> a = make_shared<AidKid>();
        map[i].push_back(a);
      }
      if (str[j] == Empty().getSymbol()) {
        shared_ptr<Empty> e = make_shared<Empty>();
        map[i].push_back(e);
      }
    }
    ++i;
  }
}

Game::Game() {
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
  refresh();
  keypad(stdscr, true);
  //wait action from user (0.5 second)
  halfdelay(5);
}

void Game::play() {
  int command = 0;
  while (command != conf.configJson["Key"]["esc"]) {
    drawGame();
    command = getch();
    //mvprintw(3, static_cast<int>(map[0].size()), "%d", command);
    int up = -1 * (command == KEY_UP);
    int down = (command == KEY_DOWN);
    int left = -1 * (command == KEY_LEFT);
    int right = (command == KEY_RIGHT);
    knight->setSide(up + down, left + right);

    int side = 0;
    side += 1 * (command == conf.configJson["Key"]["w"]);
    side += 2 * (command == conf.configJson["Key"]["d"]);
    side += 3 * (command == conf.configJson["Key"]["s"]);
    side += 4 * (command == conf.configJson["Key"]["a"]);
    knight->setSideMagic(side);

    moveObj();

    if (knight->isDead()) {
      halfdelay(100);
      endGame();
      lose();
      break;
    }
    if (princess->isGetting()) {
      halfdelay(100);
      endGame();
      win();
      break;
    }
  }
  endwin();
}

void Game::moveObj() {
  for (auto &i : map) {
    for (auto &j : i) {
      j->move(map);
    }
  }

  for (auto &i : map) {
    for (auto &j : i) {
      j->setMoved();
    }
  }
}

void Game::drawGame() {
  //clear();
  for (int i = 0; i < map.size(); ++i) {
    for (int j = 0; j < map[i].size(); ++j) {
      char c = map[i][j]->getSymbol();
      mvprintw(i, j, &c);
    }
    mvprintw(i, static_cast<int>(map[0].size()), "  ");
  }

  mvprintw(7, static_cast<int>(map[7].size() + 2), "HP: %d", knight->getHP());
  mvprintw(8, static_cast<int>(map[8].size() + 2), "MP: %d", knight->getMP());
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