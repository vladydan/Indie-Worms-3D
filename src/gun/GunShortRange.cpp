#include <iostream>
#include "gun/GunShortRange.hh"
#include "Team.hh"

GunShortRange::GunShortRange(Ogre::SceneManager *mSceneMgr) : Gun(mSceneMgr, "ressources/sounds/AWP.ogg", "bat.mesh", 0.2)
{
  this->_bullet = this->_mSceneMgr->createEntity("Bat", this->_mesh);
  this->_childNode = this->_mSceneMgr->getRootSceneNode()->createChildSceneNode
                       ("Bat", Ogre::Vector3(0, 0, 1000));
  this->_childNode->attachObject(this->_bullet);
  this->_childNode->setScale(0.1, 0.1, 0.1);
  _childNode->rotate(Ogre::Vector3::UNIT_X, Ogre::Degree(70));
  _bullet->setMaterialName("bat");
  _time = 0;
}

GunShortRange::~GunShortRange()
{

}

void GunShortRange::shoot(SoundManager *soundMgr, Ogre::SceneNode *node, float A, float V0)
{
  Ogre::Vector3 position;
  position = node->getPosition();
  soundMgr->play("Bat");
  _angle = A;
  this->_childNode->setPosition(position.x + (20 * cos(A)), position.y + (20 * sin(A)), 0);
  _initialX = node->getPosition().x;
  _initialY = node->getPosition().y;
  _isFree = false;
}

bool GunShortRange::afterEffect(float time)
{
  _afterEffect = false;
  return true;
}

bool GunShortRange::move(float time, CollisionManager *collMgr, std::list<Team*> &teams)
{
  float X;
  float Y;

  _time += time;
  X = 1 * cos(_angle);
  Y = 1 * sin(_angle);
  this->_childNode->translate(X, Y, 0);
  for (auto *team : teams)
  {
    for (auto *worm : team->getWorms())
    {
      if (_bullet->getWorldBoundingBox().intersects(worm->getEntity()->getWorldBoundingBox()))
      {
        int i = 0;
        worm->setHP(worm->getHP() - 3);
        while (!collMgr->isCollidedToStatic(_childNode->getName(), CollisionManager::LEFT) &&
          !collMgr->isCollidedToStatic(_childNode->getName(), CollisionManager::RIGHT) &&
          i < 100)
        {
          worm->getNode()->translate(X, Y, 0);
          i++;
        }
        _time = 0.3;
      }
    }
  }
  if (_time > 0.2)
  {
    this->_childNode->setPosition(-1000, 0, 0);
    _time = 0;
    _isFree = true;
    _afterEffect = true;
    return true;
  }
}

bool GunShortRange::particularWeapon()
{
  return true;
}

Ogre::Entity* GunShortRange::getExplosion() const
{
  return nullptr;
}

float GunShortRange::getScale() const
{
  return _scale;
}
