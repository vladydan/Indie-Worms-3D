#pragma once

#include "GunLongRange.hh"
#include "CollisionManager.hh"

class Grenade : public GunLongRange
{
public:
  Grenade(Ogre::SceneManager *);
  ~Grenade();
  void trajectory(float, float, Ogre::SceneNode *, float, float);
  void shoot(SoundManager *soundMgr, Ogre::SceneNode *node, float, float);
  bool particularWeapon();
};
