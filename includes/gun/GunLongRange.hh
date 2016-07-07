#pragma once

#include "Gun.hh"

class GunLongRange : public Gun
{
public:
  GunLongRange(Ogre::SceneManager *mSceneMgr, std::string const&,
	       std::string const &, float maxScale);
  ~GunLongRange();
  Ogre::Entity *getBullet();
  void deleteBullet();
  bool move(float, CollisionManager *, std::list<Team*> &);
  bool afterEffect(float time);
  Ogre::Entity* getExplosion() const;
  float getScale() const;
protected:
  Ogre::Entity *_bullet;
  float         _speed;
  float         _xVelocity;
  float         _yVelocity;
  float         _initialX;
  float         _initialY;
  float         _angle;
  float         _power;
  Ogre::Entity  *_explosion;
  float         _time;
  Ogre::SceneNode *_explosionNode;
};
