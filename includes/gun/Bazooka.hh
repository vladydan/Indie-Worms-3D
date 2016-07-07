#pragma once

#include <string>
#include "GunLongRange.hh"

class Bazooka : public GunLongRange
{
public:
  Bazooka(Ogre::SceneManager *);
  ~Bazooka();
  void trajectory(float, float, Ogre::SceneNode *, float, float);
  void shoot(SoundManager *soundMgr, Ogre::SceneNode *node, float, float);
  Ogre::SceneNode *getChild();
  bool particularWeapon();
};
