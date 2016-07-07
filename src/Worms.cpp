#include <string>
#include "Worms.hh"
#include "Team.hh"
#include "PhysBody.hh"

Worms::Worms(std::string const& nickname, std::string const& name, Ogre::SceneManager *scene,
  Ogre::ColourValue &color, unsigned char hp) :
  _name(name), _hp(hp), _scene(scene), _color(color), _nickName(nickname), _falling(0), _angle(0),
  _maxHP(hp), _firstTouch(false), _jump(false), _timeInAir(0)
{
  this->_ent = scene->createEntity(name, "Worm.mesh");
  if (rand() % 2 == 0)
    this->_facingRight = true;
  else
    this->_facingRight = false;
  this->generatePosition();
  this->setColor(color);
}

Worms::Worms(Worms const& cpy) :
  _name(cpy.getName()), _hp(cpy.getHP()), _facingRight(cpy.isFacingRight())
{
}

Worms::~Worms()
{
}

unsigned int                    Worms::getFalling() const
{
  return (this->_falling);
}

void                            Worms::fall(unsigned int falling)
{
  if (falling == 0)
  {
    if (this->_firstTouch == false)
    {
      this->_firstTouch = true;
      this->_falling = 0;
      return;
    }
    this->_hp -= this->_falling / 200;
    if (this->_hp < 0)
      this->_hp = 0;
    this->_falling = 0;
  }
  else
    this->_falling += falling;
}


std::string const&              Worms::getName() const
{
  return (this->_name);
}

std::string const&              Worms::getNickName() const
{
  return (this->_nickName);
}

float                           Worms::getHP() const
{
  return (this->_hp);
}

void                            Worms::setHP(float hp)
{
  this->_hp = hp;
}

Ogre::Entity*                   Worms::getEntity() const
{
  return _ent;
}

void                            Worms::setColor(Ogre::ColourValue &color)
{
  Ogre::MaterialPtr new_mat;
  Ogre::MaterialPtr mat = _ent->getSubEntity(2)->getMaterial();
  new_mat = mat->clone(_name);
  //new_mat->getTechnique(0)->getPass("Skin")->setSpecular(color);
  new_mat->getTechnique(0)->getPass("Skin")->setDiffuse(color);
  this->_ent->getSubEntity(2)->setMaterialName(_name);
}

void                            Worms::generatePosition()
{
  this->_node = this->_scene->getRootSceneNode()->createChildSceneNode(this->_name ,
    Ogre::Vector3(rand() % 800 + 100, 500, 0));
  this->_node->attachObject(this->_ent);
 // this->_node->showBoundingBox(true);
  this->_node->scale(_node->getScale() * 4);
  _node->rotate(Ogre::Vector3::UNIT_X, Ogre::Degree(90));
  if (this->_facingRight)
    _node->rotate(Ogre::Vector3::UNIT_Z, Ogre::Degree(-90));
  else
    _node->rotate(Ogre::Vector3::UNIT_Z, Ogre::Degree(90));
}

unsigned char                   Worms::takeDmg(unsigned char dmg)
{
  if (this->_hp - dmg <= 0)
    this->_hp = 0;
  else
    this->_hp -= dmg;
  return (this->_hp);
}

Ogre::SceneNode       *Worms::getNode() const
{
  return this->_node;
}

bool		Worms::isFacingRight() const
{
  return this->_facingRight;
}

void		Worms::setFacing(bool face)
{
  this->_facingRight = face;
}

const Ogre::ColourValue     &Worms::getColor() const
{
  return this->_color;
}

void Worms::setAngle(float angle)
{
  this->_angle = angle;
}

float Worms::getAngle() const
{
  return this->_angle;
}

void                  Worms::activateJump()
{
  _jump = true;
}

bool                  Worms::isJumping() const
{
  return _jump;
}

void                  Worms::jump(float time, CollisionManager *collisionMgr)
{
  if (!collisionMgr->isCollidedToStatic(_name, CollisionManager::UP))
    this->_node->translate(0, 2.5, 0);
  _timeInAir += time;
  _falling = 0;
  if (_timeInAir > 0.2)
  {
    _timeInAir = 0;
    _jump = false;
  }
}
