#pragma once

#include "Gun.hh"

class GunShortRange : public Gun
{
public:
  GunShortRange(Ogre::SceneManager *mSceneMgr);
  ~GunShortRange();
  void shoot(SoundManager *soundMgr, Ogre::SceneNode *node, float A, float V0);
  bool afterEffect(float time);
  bool move(float, CollisionManager *, std::list<Team*> &);
  bool particularWeapon();
  Ogre::Entity* getExplosion() const;
  float getScale() const;
private:
  Ogre::Entity *_bullet;
  float         _speed;
  float         _initialX;
  float         _initialY;
  float         _angle;
  float         _power;
  Ogre::Entity  *_explosion;
  float         _time;
  Ogre::SceneNode *_explosionNode;
};
