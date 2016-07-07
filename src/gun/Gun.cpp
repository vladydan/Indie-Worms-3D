#include <iostream>
#include "gun/Gun.hh"

Gun::Gun(Ogre::SceneManager *mSceneMgr, std::string const &song,
         std::string const &mesh, float maxScale)
  : _mSceneMgr(mSceneMgr), _mesh(mesh), _isFree(true), _afterEffect(false), _scale(0.2), _maxScale(maxScale)
{
  _isFree = true;
}

Ogre::SceneManager *Gun::getScene()
{
  return (this->_mSceneMgr);
}

Gun::~Gun()
{
}

bool Gun::getIsFree() const
{
  return (this->_isFree);
}

void Gun::setIsFree(bool const a)
{
  this->_isFree = a;
}

Ogre::SceneNode *Gun::getChild()
{
  return (this->_childNode);
}

bool Gun::getAfterEffect() const
{
  return _afterEffect;
}
