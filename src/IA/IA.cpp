#include <iostream>
#include "IA.hh"
#include "Game.hh"
#include "Worms.hh"
#include "SoundManager.hh"

wormsIA::wormsIA() :
  _target(nullptr), _angle(0), _power(0), _saveX(0), _saveY(0), _savePower(0), _saveAngle(0), _i(2)
{
}

wormsIA::~wormsIA()
{
}

void            wormsIA::shoot(Game &game, SoundManager *sound, CollisionManager *collisionMgr)
{
  int           x, y;
  int           time;

  time = 0;
  game.getNextTeam()->setChoseGun("Bazooka");
  while (this->_angle < 360)
    {
      while (this->_power < 100)
        {
          while (this->_i < 200)
            {
              x = game.getCurrentWorm()->getNode()->getPosition().x +
                cos(this->_angle) * (this->_power * 10) * this->_i;
              y = game.getCurrentWorm()->getNode()->getPosition().y +
                sin(this->_angle) * (this->_power * 10) * this->_i - 19.81 * this->_i * this->_i * 0.5;
              game.getGun(game.getNextTeam()->getChoseGun()).getChild()->setPosition(x, y, 0);
              if (collisionMgr->isCollidedToStatic(game.getGun(game.getNextTeam()->getChoseGun()).getChild()->getName()) &&
                  (this->_saveX == -1 || this->_saveY == -1 ||
                   x - this->_target->getNode()->getPosition().x < this->_saveX - this->_target->getNode()->getPosition().x ||
                   y - this->_target->getNode()->getPosition().y < this->_saveY - this->_target->getNode()->getPosition().y))
                {
                  this->_saveX = x;
                  this->_saveY = y;
                  this->_saveAngle = this->_angle;
                  this->_savePower = this->_power;
                }
              this->_i += 1;
            }
          time += 1;
          if (time >= 20)
            return ;
          this->_i = 10;
          this->_power += 2;
        }
      this->_power = 0;
      this->_angle += 2;
    }
  game.getGun(game.getNextTeam()->getChoseGun()).shoot(sound, game.getCurrentWorm()->getNode(), this->_saveAngle, this->_savePower);
  this->_angle = 0;
  this->_saveX = 0;
  this->_saveY = 0;
  this->_saveAngle = 0;
  this->_savePower = 0;
}

void            wormsIA::setTarget(Worms *target)
{
  this->_target = target;
}

Worms           *wormsIA::getTarget() const
{
  return (this->_target);
}

extern "C"
wormsIA         *entryPoint()
{
  return (new wormsIA());
}
