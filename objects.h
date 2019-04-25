class MapObject {
 public:
  //virtual void setCoord(int x, int y) = 0;
  virtual int getCoord_X() = 0;
  virtual int getCoord_Y() = 0;
};

class Knight : MapObject {
 private:
  int x_ = 0;
  int y_ = 0;
  int hp_ = 5;
 public:
  Knight() = default;
  void setCoord(int y, int x) {
    this->x_ = x;
    this->y_ = y;
  }
  int getCoord_X() override {
    return this->x_;
  }
  int getCoord_Y() override {
    return this->y_;
  }
  void wasAttacked() {
    this->hp_ -= 1;
  }
  int getHP() {
    return this->hp_;
  }
};

class Zombie : MapObject {
 private:
  int x_ = 0;
  int y_ = 0;
 public:
  //Zombie(int y, int x) {
  //  this->x_ = x;
  //  this->y_ = y;
  //}
 public:
  void setCoord(int y, int x) {
    this->x_ = x;
    this->y_ = y;
  }
  int getCoord_X() override {
    return this->x_;
  }
  int getCoord_Y() override {
    return this->y_;
  }
};

class Princess /*: MapObject*/ {
 private:
  //int x_ = 0;
  //int y_ = 0;
  bool is_getting = false;
 public:
  //Princess(int y, int x) {
  //  this->y_ = y;
  //  this->x_ = x;
  //}
  //int getCoord_X() override {
  //  return this->x_;
  //}
  //int getCoord_Y() override {
  //  return this->y_;
  //}
  bool isGetting() {
    return this->is_getting;
  }
  void setGetting() {
    this->is_getting = true;
  }
};