#include "gun/AWP.hh"
#include "Team.hh"

AWP::AWP(Ogre::SceneManager *mSceneMgr)
  : Gun(mSceneMgr, "ressources/sounds/AWP.ogg", "sphere.mesh", 0.2)
{
  this->_bullet = this->_mSceneMgr->createEntity("AWP", this->_mesh);
  this->_childNode = this->_mSceneMgr->getRootSceneNode()->createChildSceneNode
                       ("AWP", Ogre::Vector3(0, 0, 1000));
  this->_childNode->attachObject(this->_bullet);
  this->_childNode->setScale(0.02, 0.02, 0.02);
  this->_bullet->setMaterialName("Black");
  _explosion = this->_mSceneMgr->createEntity("ExplosionAWP", "uv_sphere.mesh");
  _explosion->setMaterialName("Yellow");
  this->_explosionNode = this->_mSceneMgr->getRootSceneNode()->createChildSceneNode
                       ("ExplosionAWP", Ogre::Vector3(50, 0, 1000));
  this->_explosionNode->attachObject(this->_explosion);
  this->_explosionNode->setScale(_scale, _scale, 0);
  _time = 0;
}

bool AWP::particularWeapon()
{
  return false;
}

Ogre::SceneNode *AWP::getChild()
{
  return (this->_childNode);
}

void AWP::shoot(SoundManager *soundMgr, Ogre::SceneNode *node, float A, float V0)
{
  Ogre::Vector3 position;
  position = node->getPosition();
  soundMgr->play("AWP");
  _angle = A;
  _power = V0;
  this->_childNode->setPosition(position.x + (20 * cos(A)), position.y + (20 * sin(A)), 0);
  _initialX = node->getPosition().x;
  _initialY = node->getPosition().y;
    _isFree = false;
}

AWP::~AWP()
{
}

bool AWP::move(float time, CollisionManager *collisionMgr, std::list<Team*> &teams)
{
  float X;
  float Y;

  _time += time * 3;
  X = 1 * cos(_angle);
  Y = 1 * sin(_angle);

  this->_childNode->translate(X, Y, 0);
  if (wormsCollided(teams) == true || _time > 20)
  {
    _time = 0;
    _isFree = true;
    _afterEffect = true;
    this->_explosionNode->setPosition(_childNode->getPosition());
    this->_childNode->setPosition(0, 0, 0);
    return true;
  }
  return false;
}


bool AWP::afterEffect(float time)
{
  _scale += 0.001;
  this->_explosionNode->setScale(_scale, _scale, _scale);
  if (_scale > _maxScale)
  {
    _afterEffect = false;
    _scale = 0.2;
    this->_explosionNode->setPosition(0, 0, 0);
    return true;
  }
  return false;
}



Ogre::Entity* AWP::getExplosion() const
{
  return this->_explosion;
}

float AWP::getScale() const
{
  return _scale;
}

bool  AWP::wormsCollided(std::list<Team*> &teams)
{
  for (auto *team : teams)
  {
    for (auto *worm : team->getWorms())
    {
      if (_bullet->getWorldBoundingBox().intersects(worm->getEntity()->getWorldBoundingBox()))
      {
        worm->setHP(worm->getHP() - 3);
        return true;
      }
    }
  }
  return false;
}
