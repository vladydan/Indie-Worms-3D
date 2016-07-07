#include <thread>
#include "gun/Grenade.hh"
#include "PhysBody.hh"
#include "CollisionManager.hh"

Grenade::Grenade(Ogre::SceneManager *mSceneMgr)
  : GunLongRange(mSceneMgr, "ressources/sounds/Grenade.ogg", "Grenade_NO_PIN.mesh", 0.5)
{
  this->_bullet = this->_mSceneMgr->createEntity("Grenade", this->_mesh);
  this->_childNode = this->_mSceneMgr->getRootSceneNode()->createChildSceneNode
                       ("Grenade", Ogre::Vector3(50, 0, 1000));
  this->_childNode->attachObject(this->_bullet);
  this->_bullet->setMaterialName("Black");
  _explosion = this->_mSceneMgr->createEntity("ExplosionGranade", "uv_sphere.mesh");
  _explosion->setMaterialName("Yellow");
  _explosionNode = this->_mSceneMgr->getRootSceneNode()->createChildSceneNode
                       ("ExplosionGranade", Ogre::Vector3(150, 0, 1000));
  _explosionNode->attachObject(this->_explosion);
  _explosionNode->setScale(_scale, _scale, _scale);
  /*PhysBody  *wormPh = new PhysBody(PhysBody::RECTANGLE, 25 * 0.2, 25 * 0.4, 1, "Grenade");
  this->_collisionMgr->addEntity(wormPh, "Grenade");
  this->_physicList.push_back(*wormPh);*/
}

void Grenade::shoot(SoundManager *soundMgr, Ogre::SceneNode *node, float A, float V0)
{
  Ogre::Vector3 position;
  position = node->getPosition();
  soundMgr->play("Grenade");
  _angle = A;
  _power = V0;
  this->_childNode->setPosition(position.x + (20 * cos(A)), position.y + (20 * sin(A)), 0);
  _initialX = node->getPosition().x + (20 * cos(A));
  _initialY = node->getPosition().y + (20 * sin(A));
  _isFree = false;
}

bool Grenade::particularWeapon()
{
  if (this->_time > 8)
  {
    return true;
  }
  return false;
}

Grenade::~Grenade()
{
}
