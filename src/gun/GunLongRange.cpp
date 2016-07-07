#include <string>
#include <iostream>
#include <math.h>
#include "gun/GunLongRange.hh"

#define _USE_MATH_DEFINES
#define GRAVITY 7

GunLongRange::GunLongRange(Ogre::SceneManager *mSceneMgr,
                           std::string const &song, std::string const &mesh, float maxScale)
  : Gun(mSceneMgr, song, mesh, maxScale)
{
  _time = 0;
}

Ogre::Entity *GunLongRange::getBullet()
{
  return (this->_bullet);
}

bool GunLongRange::move(float time, CollisionManager *collisionMgr, std::list<Team*> &)
{
  float X;
  float Y;

  _time += time * 3;
  X = _initialX + cos(_angle) * (_power * 10) * _time;
  Y = _initialY + sin(_angle) * (_power * 10) * _time - 19.81 * _time * _time * 0.5;
  this->_childNode->setPosition(X, Y, 0);
  if (collisionMgr->isCollidedToStatic(_childNode->getName()) || _time > 20 || this->particularWeapon())
  {
    _time = 0;
    _isFree = true;
    _afterEffect = true;
    this->_explosionNode->setPosition(_childNode->getPosition());
    this->_childNode->setPosition(-1000, -1000, 0);
    return true;
  }
  return false;
}

bool GunLongRange::afterEffect(float time)
{
  _scale += time / 2;
  this->_explosionNode->setScale(_scale, _scale, _scale);
  if (_scale > _maxScale)
  {
    this->_explosionNode->setPosition(-1000, -1000, 0);
    _afterEffect = false;
    _scale = 0.2;
    return true;
  }
  return false;
}


void GunLongRange::deleteBullet()
{
  delete this->_bullet;
}

GunLongRange::~GunLongRange()
{

}

Ogre::Entity* GunLongRange::getExplosion() const
{
  return this->_explosion;
}

float GunLongRange::getScale() const
{
  return _scale;
}
