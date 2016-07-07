#pragma once

# include "Game.hh"
# include "Worms.hh"
# include "SoundManager.hh"
# include "CollisionManager.hh"

class           Game;

class           wormsIA
{
  Worms         *_target;
  int           _angle;
  int           _power;
  int           _saveX;
  int           _saveY;
  int           _savePower;
  int           _saveAngle;
  int           _i;

 public:
  wormsIA();
  ~wormsIA();

  void          shoot(Game &game, SoundManager *sound, CollisionManager *collisionMgr);
  Worms         *getTarget() const;
  void          setTarget(Worms *target);
};
