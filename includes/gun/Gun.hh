#pragma once

#include "CollisionManager.hh"
#include "SoundManager.hh"

class Game;
class Team;

class Gun
{
public:
  Gun(Ogre::SceneManager *, std::string const &, std::string const &, float maxScale);
  ~Gun();
  virtual void shoot(SoundManager *soundMgr, Ogre::SceneNode *node, float A, float V0) = 0;
  virtual bool afterEffect(float time) = 0;
  virtual bool move(float, CollisionManager *, std::list<Team*> &) = 0;
  virtual bool particularWeapon() = 0;
  virtual Ogre::Entity* getExplosion() const = 0;
  virtual float getScale() const = 0;
  Ogre::SceneNode *getChild();
  bool getAfterEffect() const;

  bool getIsFree() const;
  void setIsFree(const bool);
  Ogre::SceneManager *getScene();
protected:
  bool _isFree;
  Ogre::SceneManager *_mSceneMgr;
  std::string _mesh;
  Ogre::SceneNode *_childNode;
  bool              _afterEffect;
  float             _scale;
  float             _maxScale;
};
