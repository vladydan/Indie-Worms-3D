#include "gun/Nuke.hh"

Nuke::Nuke(Ogre::SceneManager *mSceneMgr)
  : GunLongRange(mSceneMgr, "ressources/sounds/Nuke.ogg", "sphere.mesh", 1.5)
{
  this->_bullet = this->_mSceneMgr->createEntity("Nuke", this->_mesh);
  this->_childNode = this->_mSceneMgr->getRootSceneNode()->createChildSceneNode
                       ("Nuke", Ogre::Vector3(0, 0, 1000));
  this->_childNode->attachObject(this->_bullet);
  this->_childNode->setScale(0.09, 0.09, 0.09);
  this->_bullet->setMaterialName("Black");
  _explosion = this->_mSceneMgr->createEntity("ExplosionNuke", "uv_sphere.mesh");
  _explosion->setMaterialName("Yellow");
  this->_explosionNode = this->_mSceneMgr->getRootSceneNode()->createChildSceneNode
                       ("ExplosionNuke", Ogre::Vector3(50, 0, 1000));
  this->_explosionNode->attachObject(this->_explosion);
  this->_explosionNode->setScale(_scale, _scale, 0);
  _scale = 0.2;
}

bool Nuke::particularWeapon()
{
  return false;
}

Ogre::SceneNode *Nuke::getChild()
{
  return (this->_childNode);
}

void Nuke::shoot(SoundManager *soundMgr, Ogre::SceneNode *node, float A, float V0)
{
  Ogre::Vector3 position;
  position = node->getPosition();
  soundMgr->play("Nuke");
  _angle = A;
  _power = V0;
  this->_childNode->setPosition(position.x, position.y, 0);
  _initialX = node->getPosition().x;
  _initialY = node->getPosition().y;
  _isFree = false;
}

Nuke::~Nuke()
{
}
