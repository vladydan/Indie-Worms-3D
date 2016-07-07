#pragma once

#include "gun/Gun.hh"

class AWP : public Gun
{
  public:
    AWP(Ogre::SceneManager *);
    ~AWP();
    void trajectory(float, float, Ogre::SceneNode *, float, float);
    void shoot(SoundManager *soundMgr, Ogre::SceneNode *node, float, float);
    Ogre::SceneNode *getChild();
    bool particularWeapon();
    bool move(float, CollisionManager *, std::list<Team*> &);
    bool afterEffect(float time);
    Ogre::Entity* getExplosion() const;
    float getScale() const;
    bool  wormsCollided(std::list<Team*> &teams);
  private:
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
