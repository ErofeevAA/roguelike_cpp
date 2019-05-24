#include "config_reader.h"

#include <ncurses.h>

using std::vector;

class Knight;
class Zombie;
class Dragon;
class Fire;
//class Princess;
//class Wall;
//class AidKid;
//class Empty;


class MapObject {
 protected:
  int y = 0;
  int x = 0;
  char symbol = '.';
  bool dead = false;
  bool ismoved  = false;

  void setCoord(int y, int x);

  void setSymbol(std::string s);

 public:
  ConfigReader c = ConfigReader();
  MapObject() = default;



  //int getX();
  //int getY();

  virtual void move(vector<vector<std::shared_ptr<MapObject>>> &m) {}

  virtual void collide(Knight &k) {}
  virtual void collide(Zombie &z) {}
  virtual void collide(Dragon &d) {}
  virtual void collide(Fire &f) {}
  //virtual void collide(Princess &p) {}
  //virtual void collide(Wall &p) {}
  //virtual void collide(AidKid &a) {}
  //virtual void collide(Empty &e) {}

  //virtual void collide(MapObject &m) {}

  char getSymbol() const;

  bool isDead();

  void deadObj();

  void setMoved() {this->ismoved = true;}

};


class Knight : public MapObject {
 private:
  int px = 0;
  int py = 0;

  int f_y = 0;
  int f_x = 0;
  int side_f = 0;

  int hp = 0;
  int mp = 0;

  int maxHP = 0;

  void useMP();

  bool canUseMP();

 public:
  Knight(int y, int x, int hp, int mp);

  //void setCoord(int y, int x);

  void move(vector<vector<std::shared_ptr<MapObject>>> &m) override;

  void makeFire(vector<vector<std::shared_ptr<MapObject>>> &m);

  void collide(Zombie &z) override;
  void collide(Dragon &d) override;
  void collide(Fire &f) override;

  //void collide(MapObject &m) override;

  void wasAttacked(int dam);

  bool canUpHP();

  void upHP();

  int getHP();
  int getMP();

  void setSide(int py, int px);
  void setSideMagic(int s);
};

class Zombie : public MapObject {
 public:
  Zombie(int y, int x);

  //void setCoord(int y, int x) override;

  void move(vector<vector<std::shared_ptr<MapObject>>> &m) override;

  void collide(Knight &k) override;
  void collide(Dragon &d) override;
  void collide(Fire &f) override;

  //void collide(MapObject &m) override;

  int getDamage();
};

class Dragon : public MapObject {
 private:
  bool set_fire;
 public:
  Dragon(int y, int x);

  //void setCoord(int y, int x) override;

  void move(vector<vector<std::shared_ptr<MapObject>>> &m) override;

  void makeFire(vector<vector<std::shared_ptr<MapObject>>> &m);

  void collide(Knight &k) override;
  void collide(Fire &f) override;

  //void collide(MapObject &m) override;

  int getDamage();
};

class Fire : public MapObject {
 private:
  int side;
 public:
  Fire(int y, int x, int s);

  //void setCoord(int y, int x) override;

  void move(vector<vector<std::shared_ptr<MapObject>>> &m) override;

  void collide(Knight &k) override;
  void collide(Dragon &d) override;
  void collide(Fire &f) override;

  //void collide(MapObject &m) override;

  int getDamage();
};

class Princess : public MapObject{
  bool is_get = false;
 public:
  Princess();

  //void setCoord(int y, int x) override {}

  bool isGetting();

  void setGetting();

  void collide(Knight &k) override;
  void collide(Fire &f) override;

  //void collide(MapObject &m) override;
};

class Wall : public MapObject {
 public:
  Wall();

  //void setCoord(int y, int x) override {}

  void collide(Fire &f) override;

  //void collide(MapObject &m) override;
};

class AidKid : public MapObject {
 public:
  AidKid();

  //void setCoord(int y, int x) override {}

  void collide(Knight &k) override;
  void collide(Fire &f) override;

  //void collide(MapObject &m) override;
};

class Empty : public MapObject {
 public:
  Empty();

  //void setCoord(int y, int x) override {};

  void collide(Knight &k) override;
  void collide(Zombie &z) override;
  void collide(Dragon &d) override;
  void collide(Fire &f) override;

  //void collide(MapObject &m) override;
};