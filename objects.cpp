#include "objects.h"


void MapObject::setCoord(int y, int x) {
  this->y = y;
  this->x = x;
}

void MapObject::setSymbol(std::string s) {
  this->symbol = s.c_str()[0];
}

char MapObject::getSymbol() const{
  return this->symbol;
}

bool MapObject::isDead() {
  return this->dead;
}

void MapObject::deadObj() {
  this->dead = true;
}

Knight::Knight(int y, int x, int hp, int mp) {
  setCoord(y, x);
  this->hp = hp;
  this->maxHP = hp;
  this->mp = mp;
  setSymbol(c.configJson["Knight"]["symbol"]);
}

void Knight::move(vector<vector<std::shared_ptr<MapObject>>> &m) {
  if (ismoved) {
    if (f_y == f_x) {
      m[y + py][x + px]->collide(*this);
      if (m[y + py][x + px]->isDead()) {
        m[y + py][x + px] = m[y][x];
        m[y][x] = std::make_shared<Empty>();
        setCoord(y + py, x + px);
      }
    }
    else {
      makeFire(m);
    }
    ismoved = false;
  }
}

void Knight::makeFire(vector<vector<std::shared_ptr<MapObject>>> &m) {
  int d[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
  if (canUseMP() && f_y != f_x) {
    std::shared_ptr<Fire> f = std::make_shared<Fire>(y + f_y, x + f_x, side_f);
    m[y + d[side_f][0]][x + d[side_f][1]]->collide(*f);
    if (m[y + d[side_f][0]][x + d[side_f][1]]->isDead()) {
      if (!f->isDead()) {
        m[y + d[side_f][0]][x + d[side_f][1]] = f;
      } else {
        m[y + d[side_f][0]][x + d[side_f][1]] = std::make_shared<Empty>();
      }
      useMP();
    }
  }
}

void Knight::collide(Zombie &z) {
  wasAttacked(z.getDamage());
}
void Knight::collide(Dragon &d) {
  wasAttacked(d.getDamage());
}
void Knight::collide(Fire &f) {
  f.deadObj();
  wasAttacked(f.getDamage());
}

void Knight::wasAttacked(int dam) {
  this->hp -= dam;
  if (this->hp < 1) {
    dead = true;
  }
}

void Knight::useMP() {
  this->mp -= 1;
}

bool Knight::canUpHP() {
  return this->hp < this->maxHP;
}
bool Knight::canUseMP() {
  return mp > 0;
}

void Knight::upHP() {
  this->hp += 1;
}

int Knight::getHP() {
  return this->hp;
}
int Knight::getMP() {
  return this->mp;
}

void Knight::setSide(int py, int px) {
  this->py = py;
  this->px = px;
}
void Knight::setSideMagic(int s) {
  int d[5][2] = {{0, 0},{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
  this->f_y = d[s][0];
  this->f_x = d[s][1];
  this->side_f = s - 1;
}

Zombie::Zombie(int y, int x) {
  setCoord(y, x);
  setSymbol(c.configJson["Zombie"]["symbol"]);
}

void Zombie::move(vector<vector<std::shared_ptr<MapObject>>> &m) {
  if (ismoved) {
    int d[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    int s = rand() % 4;
    m[y + d[s][0]][x + d[s][1]]->collide(*this);
    if (m[y + d[s][0]][x + d[s][1]]->isDead()) {
      m[y + d[s][0]][x + d[s][1]] = m[y][x];
      m[y][x] = std::make_shared<Empty>();
      setCoord(y + d[s][0], x + d[s][1]);
    }
    ismoved = false;
  }
}

void Zombie::collide(Knight &k) {
  deadObj();
}
void Zombie::collide(Dragon &d) {
  deadObj();
}
void Zombie::collide(Fire &f) {
  deadObj();
  f.deadObj();
}

int Zombie::getDamage() {
  return 1;
}


Dragon::Dragon(int y, int x) {
  setCoord(y, x);
  setSymbol(c.configJson["Dragon"]["symbol"]);
  this->set_fire = static_cast<bool>(rand() % 2);
}

void Dragon::move(vector<vector<std::shared_ptr<MapObject>>> &m) {
  if (ismoved) {
    int d[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    int s = rand() % 4;
    if (!set_fire) {
      m[y + d[s][0]][x + d[s][1]]->collide(*this);
      if (m[y + d[s][0]][x + d[s][1]]->isDead()) {
        m[y + d[s][0]][x + d[s][1]] = m[y][x];
        m[y][x] = std::make_shared<Empty>();
        setCoord(y + d[s][0], x + d[s][1]);
      }
      set_fire = !set_fire;
    }
    else {
      makeFire(m);
    }
    ismoved = false;
  }
}

void Dragon::makeFire(vector<vector<std::shared_ptr<MapObject>>> &m) {
  int s = rand() % 4;
  int d[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
  std::shared_ptr<Fire> f = std::make_shared<Fire>(y + d[s][0], x + d[s][1], s);
  m[y + d[s][0]][x + d[s][1]]->collide(*f);
  if (m[y + d[s][0]][x + d[s][1]]->isDead()) {
    if (!f->isDead()) {
      m[y + d[s][0]][x + d[s][1]] = f;
    }
    else {
      m[y + d[s][0]][x + d[s][1]] = std::make_shared<Empty>();
    }
  }
  set_fire = !set_fire;
}

void Dragon::collide(Knight &k) {
  deadObj();
}
void Dragon::collide(Fire &f) {
  f.deadObj();
}

int Dragon::getDamage() {
  return 3;
}


Fire::Fire(int y, int x, int s) {
  setCoord(y, x);
  this->side = s;
  setSymbol(c.configJson["Fire"]["symbol"]);
}

void Fire::move(vector<vector<std::shared_ptr<MapObject>>> &m) {
  if (ismoved) {
    int d[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    m[y + d[side][0]][x + d[side][1]]->collide(*this);
    if (this->isDead()) {
      if (m[y + d[side][0]][x + d[side][1]]->isDead()) {
        m[y + d[side][0]][x + d[side][1]] = std::make_shared<Empty>();
      }
      m[y][x] = std::make_shared<Empty>();
    } else if (m[y + d[side][0]][x + d[side][1]]->isDead()) {
      m[y + d[side][0]][x + d[side][1]] = m[y][x];
      m[y][x] = std::make_shared<Empty>();
      setCoord(y + d[side][0], x + d[side][1]);
    }
    ismoved = false;
  }
}

void Fire::collide(Knight &k) {
  k.wasAttacked(this->getDamage());
  this->deadObj();
}
void Fire::collide(Dragon &d) {
  deadObj();
}
void Fire::collide(Fire &f) {
  deadObj();
  f.deadObj();
}

int Fire::getDamage() {
  return 1;
}


Princess::Princess() {
  setSymbol(c.configJson["Princess"]["symbol"]);
}

bool Princess::isGetting() {
  return this->is_get;
}

void Princess::setGetting() {
  this->is_get = true;
}

void Princess::collide(Knight &k) {
  setGetting();
}
void Princess::collide(Fire &f) {
  f.deadObj();
}


Wall::Wall() {
  setSymbol(c.configJson["Wall"]["symbol"]);
}

void Wall::collide(Fire &f) {
  f.deadObj();
}


AidKid::AidKid() {
  setSymbol(c.configJson["AidKit"]["symbol"]);
}

void AidKid::collide(Knight &k) {
  if (k.canUpHP()) {
    k.upHP();
    deadObj();
  }
}
void AidKid::collide(Fire &f) {
  f.deadObj();
}


Empty::Empty() {
  setSymbol(c.configJson["Empty"]["symbol"]);
}

void Empty::collide(Knight &k) {
  deadObj();
}
void Empty::collide(Zombie &z) {
  deadObj();
}
void Empty::collide(Dragon &d) {
  deadObj();
}
void Empty::collide(Fire &f) {
  deadObj();
}