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
  int max_hp = 10;
  int max_mp = 10;
  int hp_ = max_hp;
  int mp_ = max_mp;
 public:
  Knight(int hp, int mp) {
    this->hp_ = hp;
    this->mp_ = mp;
    this->max_hp = hp;
    this->max_mp = mp;
  }
  void setCoord(int y, int x) {
    this->x_ = x;
    this->y_ = y;
  }
  int getCoord_X() override{
    return this->x_;
  }
  int getCoord_Y() override{
    return this->y_;
  }
  int getMP() {
    return this->mp_;
  }
  int getHP() {
    return this->hp_;
  }
  int getMaxHP() {
    return this->max_hp;
  }
  void setMP(int mp) {
    this->mp_ = mp;
  }
  void wasAttacked(int d) {
    this->hp_ -= d;
  }
  void upHP () {
    if (this->hp_ < this->max_hp) {
      this->hp_ += 1;
    }
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
  int getDamage() {
    return 1;
  }
};

class Dragon : MapObject{
 private:
  int x_ = 0;
  int y_ = 0;
  bool fire_ = false;
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
  bool isFire() {
    return this->fire_;
  }
  void changeFire() {
    this->fire_ = !fire_;
  }
  int getDamage() {
    return 2;
  }
};

class Fire : MapObject{
 private:
  int x_ = 0;
  int y_ = 0;
  int hp_ = 3;
  int side_ = 0;
 public:
  explicit Fire(int hp) {
    this->hp_ = hp;
  }
  void setCoord(int y, int x) {
    this->x_ = x;
    this->y_ = y;
    this->hp_ -= 1;
  }
  void setSide(int side) {
    this->side_ = side;
  }
  int getCoord_X() override {
    return this->x_;
  }
  int getCoord_Y() override {
    return this->y_;
  }
  int getDamage() {
    return 1;
  }
  bool isDestroy() {
    return this->hp_ == 0;
  }
  int getSide(){
    return this->side_;
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