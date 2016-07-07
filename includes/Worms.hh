
#pragma once

# include <string>
#include "CollisionManager.hh"
# include "PhysBody.hh"

class                   Worms
{
  std::string const     _name;
  std::string const     _nickName;
  float                  _hp;
  unsigned char         _maxHP;
  Ogre::Entity          *_ent;
  Ogre::SceneManager    *_scene;
  Ogre::SceneNode       *_node;
  Ogre::ColourValue     _color;
  int                   _falling;
  bool                  _firstTouch;
  bool                  _facingRight;
  float                 _angle;
  bool                  _jump;
  float                 _timeInAir;
 public:
  Worms(Worms const& cpy);
  Worms(std::string const& nickname, std::string const& name, Ogre::SceneManager *scene,
    Ogre::ColourValue &color, unsigned char hp = 100);
  ~Worms();
  std::string const&    getNickName() const;
  std::string const&    getName() const;
  float                 getHP() const;
  unsigned int          getFalling() const;
  const Ogre::ColourValue &getColor() const;
  void                  fall(unsigned int);
  Ogre::SceneNode       *getNode() const;
  bool                  isFacingRight() const;
  void                  setFacing(bool);
  void                  setHP(float hp);
  unsigned char         takeDmg(unsigned char dmg);
  void                  generatePosition();
  void                  setColor(Ogre::ColourValue &color);
  float                 getAngle() const;
  void                  setAngle(float angle);
  Ogre::Entity*         getEntity() const;
  void                  activateJump();
  bool                  isJumping() const;
  void                  jump(float time, CollisionManager *);
};
