#include "gun/Bazooka.hh"

Bazooka::Bazooka(Ogre::SceneManager *mSceneMgr)
  : GunLongRange(mSceneMgr, "ressources/sounds/Bazooka.ogg", "sphere.mesh", 0.6)
{
  this->_bullet = this->_mSceneMgr->createEntity("Bazooka", this->_mesh);
  this->_childNode = this->_mSceneMgr->getRootSceneNode()->createChildSceneNode
                       ("Bazooka", Ogre::Vector3(0, 0, 1000));
  this->_childNode->attachObject(this->_bullet);
  this->_childNode->setScale(0.09, 0.09, 0.09);
  this->_bullet->setMaterialName("Black");
  _explosion = this->_mSceneMgr->createEntity("ExplosionBazooka", "uv_sphere.mesh");
  _explosion->setMaterialName("Yellow");
  this->_explosionNode = this->_mSceneMgr->getRootSceneNode()->createChildSceneNode
                       ("ExplosionBazooka", Ogre::Vector3(50, 0, 1000));
  this->_explosionNode->attachObject(this->_explosion);
  this->_explosionNode->setScale(_scale, _scale, 0);
}

bool Bazooka::particularWeapon()
{
  return false;
}

Ogre::SceneNode *Bazooka::getChild()
{
  return (this->_childNode);
}

void Bazooka::shoot(SoundManager *soundMgr, Ogre::SceneNode *node, float A, float V0)
{
  Ogre::Vector3 position;
  position = node->getPosition();
  soundMgr->play("Bazooka");
  _angle = A;
  _power = V0;
  this->_childNode->setPosition(position.x + (20 * cos(A)), position.y + (20 * sin(A)), 0);
  _initialX = node->getPosition().x + (20 * cos(A));
  _initialY = node->getPosition().y + (20 * sin(A));
    _isFree = false;
}

Bazooka::~Bazooka()
{
}
