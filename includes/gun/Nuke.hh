#pragma once

#include <string>
#include "GunLongRange.hh"

class Nuke : public GunLongRange
{
public:
  Nuke(Ogre::SceneManager *);
  ~Nuke();
  void trajectory(float, float, Ogre::SceneNode *, float, float);
  void shoot(SoundManager *soundMgr, Ogre::SceneNode *node, float, float);
  Ogre::SceneNode *getChild();
  bool particularWeapon();
};
