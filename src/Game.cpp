#include <list>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include "Game.hh"
#include "gun/Gun.hh"
#include "gun/Grenade.hh"
#include "gun/Bazooka.hh"
#include "gun/AWP.hh"
#include "gun/GunShortRange.hh"
#include "Team.hh"
#include "DLLoader.hpp"
#include "gun/Nuke.hh"

static const std::vector<std::string> NAMES = {"B0C4L", "TEK", "4ST3K",
                                               "K04L4", "ADM", "MERICA",
                                               "PEPES COMPAGNY", "Avengers",
                                               "DC", "MARVEL", "EMACS TEAM",
                                               "VIM TEAM", "OP3X", "PEDAGO",
                                               "BIG MAC TEAM(MERICA)",
                                               "GREAT EMPIRE OF RUSSIA"};

static std::vector<Ogre::ColourValue> COLORS = {Ogre::ColourValue(0, 1, 0, 0.5), Ogre::ColourValue(1, 0, 0, 0.5),
                                                 Ogre::ColourValue(0, 0, 1, 0.5), Ogre::ColourValue(0, 1, 1, 0.5),
                                                                                     Ogre::ColourValue(1, 0, 1, 0.5), Ogre::ColourValue(1, 1, 0, 0.5)
                                            };

Game::Game(Ogre::SceneManager *scene, GameType gameType, unsigned int timer,
           unsigned int team, unsigned int worms,
           unsigned int hp) :
  _type(gameType), _timer(timer), _time(0), _colorIndex(0), _currentWorm(nullptr), _nextTeam(nullptr), _teleportActivated(false)
{
  Ogre::ColourValue color(1, 0, 0, 0);

  if (team != 0)
    {
      unsigned int i = 0;

      while (++i <= team)
        {
          this->_teams.push_back(new Team(NAMES[rand() % NAMES.size()] +
                                          "_" + std::to_string(i),
                                          &COLORS[i % COLORS.size()], scene, worms, hp, false));
        }
      this->_nextTeam = this->_teams.begin();
      this->nextWorm();
    }
  this->_gun["Grenade"] = new Grenade(scene);
  this->_gun["Bazooka"] =  new Bazooka(scene);
  this->_gun["AWP"] =  new AWP(scene);
  this->_gun["Nuke"] =  new Nuke(scene);
  this->_gun["Bat"] =  new GunShortRange(scene);
  this->_IALoader.loadDL(IALIBPATH, "entryPoint");
  this->_IA = this->_IALoader.getInstance();
}

Game::~Game()
{
  for (std::list<Team*>::iterator it = this->_teams.begin();
       it != this->_teams.end(); ++it)
  {
    delete *it;
    it = this->_teams.erase(it);
  }
}

unsigned int Game::getTimer() const
{
  return (this->_timer);
}

unsigned int Game::getTimeLeft() const
{
  return (this->_time);
}

Game::GameType Game::getGameType() const
{
  return (this->_type);
}

std::list<Team*>&               Game::getTeam()
{
  return (this->_teams);
}

wormsIA                        *Game::getIA() const
{
  return (this->_IA);
}

Team*                          Game::getNextTeam() const
{
  if (this->_teams.size() == 0)
    return nullptr;
  return (*this->_nextTeam);
}

Gun                            &Game::getGun(std::string const &index)
{
  return (*this->_gun[index]);
}

void                  Game::setGameType(GameType typeGame)
{
  this->_type = typeGame;
}

bool                            Game::setNextTeam(Team const* nextTeam)
{
  for (std::list<Team*>::const_iterator it = this->_teams.begin();
       it != this->_teams.end(); ++it)
  {
    if (*it == nextTeam)
    {
      this->_nextTeam = it;
      return (true);
    }
  }
  return (false);
}

Team*                           Game::nextTeam()
{
  if (this->_teams.size() == 0)
  {
    this->_nextTeam = this->_teams.end();
    return (nullptr);
  }
  else if (++(this->_nextTeam) == this->_teams.end())
    this->_nextTeam = this->_teams.begin();
  return (*this->_nextTeam);
}

unsigned int Game::teamCount() const
{
  return (this->_teams.size());
}

std::string Game::generateTeamName() const
{
  return NAMES[rand() % NAMES.size()] + "_" + std::to_string(_colorIndex);
}

unsigned int Game::addTeam(std::string & name, Ogre::ColourValue *color,
        Ogre::SceneManager *scene, unsigned int worms,
          unsigned char hp, bool ia)
{
  bool teamExist = false;

  if (color == NULL)
    color = &COLORS[_colorIndex % COLORS.size()];
  for (auto *team : this->_teams)
  {
    if (team->getName() == name)
      teamExist = true;
  }
  if (teamExist == true || name == "")
    name = NAMES[rand() % NAMES.size()] + "_" + std::to_string(_colorIndex);
  this->_teams.push_back(new Team(name, color, scene, worms, hp, ia));
  this->_nextTeam = this->_teams.begin();
  _colorIndex++;
  return (this->_teams.size());
}

unsigned int Game::removeTeam(Team const* team)
{
  for (std::list<Team*>::iterator it = this->_teams.begin();
       it != this->_teams.end(); ++it)
  {
    if (*it == team)
    {
      if (this->_teams.size() == 1)
        this->_nextTeam = this->_teams.end();
      else if (it == this->_nextTeam)
        this->nextTeam();
      delete *it;
      this->_teams.erase(it);
      return (this->_teams.size());
    }
  }
  return (this->_teams.size());
}

bool Game::nextWorm()
{
  Worms     *tmp;

  this->_currentWorm = this->nextTeam()->nextWorms();
  tmp = this->_currentWorm;
  while (this->_currentWorm->getHP() <= 0)
  {
    if (this->_currentWorm == tmp)
    {
      this->_currentWorm = this->nextTeam()->nextWorms();
      return false;
    }
  }
  _time = 0;
  return true;
}

Worms*                         Game::getCurrentWorm() const
{
  return this->_currentWorm;
}

bool Game::addToTime(float time)
{
  this->_time += time;
  if (this->_time > this->_timer)
  {
    this->_time = 0;
    this->nextWorm();
    return true;
  }
  return false;
}

Worms                         *Game::findCloseWorms() const
{
  Worms                       *close;

  close = nullptr;
  for (auto *team : this->_teams)
    {
      if (*(this->_nextTeam) != team)
        for (auto *worm : team->getWorms())
          {
            if (close == nullptr || (worm->getNode()->getPosition() - this->_currentWorm->getNode()->getPosition() < close->getNode()->getPosition() - this->_currentWorm->getNode()->getPosition()))
              close = worm;
          }
    }
  this->_IA->setTarget(close);
  return (close);
}

Worms                         *Game::findWormByName(std::string const &name) const
{
  for (auto *team : this->_teams)
  {
    for (auto *worm : team->getWorms())
    {
      if (worm->getName() == name)
        return worm;
    }
  }
}

std::string                  *Game::getWinnerName() const
{
  int teamCount = 0;
  std::string teamName;

  for (auto *team : this->_teams)
  {
    for (auto *worm : team->getWorms())
    {
      if (worm->getHP() > 0)
      {
        teamCount++;
        teamName = team->getName();
        break;
      }
    }
  }
  if (teamCount == 1)
    return new std::string(teamName);
  return nullptr;
}

void                    Game::killWormsUnderWater(Ogre::SceneNode *waterNode)
{
  for (auto *team : this->_teams)
  {
    for (auto *worm : team->getWorms())
    {
      if (worm->getHP() > 0)
      {
        if (worm->getNode()->getPosition().y < waterNode->getPosition().y)
          worm->setHP(0);
      }
    }
  }
}

void                  Game::doDamageToWorms(Ogre::Entity *explosion, float damage)
{
  for (auto *team : this->_teams)
  {
    for (auto *worm : team->getWorms())
    {
      if (explosion->getWorldBoundingBox().contains(worm->getEntity()->getWorldBoundingBox()))
      {
        worm->setHP(worm->getHP() - damage);
      }
    }
  }
}

bool                  Game::getTeleportActivated() const
{
  return _teleportActivated;
}

void                  Game::setTeleportActivated(bool teleport)
{
  _teleportActivated = teleport;
}
