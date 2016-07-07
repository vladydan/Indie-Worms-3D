#pragma once

# include <list>
# include "IA.hh"
# include "DLLoader.hpp"
# include "Team.hh"
# include "gun/Gun.hh"
# include "IA.hh"

# define IALIBPATH      "libwormsIA.so" // need .dll for windows too
# define WAR_LOADIA     "Warning: couldn't load IA. Game will be PVP."

class                   wormsIA;

class                   Game
{
 public:
  enum                  GameType
    {
      DEFAULT,
      SUDDEN_DEATH
    };

 private:
  wormsIA               *_IA;
  DLLoader<wormsIA>     _IALoader;
  GameType              _type;
  std::list<Team*>      _teams;
  std::list<Team*>::const_iterator _nextTeam;
  unsigned int const    _timer;
  Worms*                _currentWorm;
  float                 _time;
  std::map<std::string, Gun *>    _gun;
  int                   _colorIndex;
  bool                  _teleportActivated;

 public:
  Game(Ogre::SceneManager *scene, GameType gameType = DEFAULT, unsigned int timer = 30,
       unsigned int teams = 2, unsigned int worms = 5,
       unsigned int hp = 100);
  ~Game();

  std::list<Team*>      &getTeam();
  wormsIA               *getIA() const;
  Team*                 getNextTeam() const;
  unsigned int          getTimer() const;
  GameType              getGameType() const;
  Gun                   &getGun(std::string const &index);
  bool                  addToTime(float time);
  bool                  setNextTeam(Team const* nextTeam);
  Team*                 nextTeam();
  unsigned int          teamCount() const;
  unsigned int          addTeam(std::string & name, Ogre::ColourValue *color,
                                Ogre::SceneManager *_scene, unsigned int worms = 5,
                                  unsigned char hp = 100, bool ia = false);
  unsigned int          removeTeam(Team const* team);
  bool                  nextWorm();
  Worms                 *getCurrentWorm() const;
  unsigned int          getTimeLeft() const;
  void                  setGameType(GameType);
  Worms                 *findWormByName(std::string const &) const;
  Worms                 *findCloseWorms() const;
  std::string           generateTeamName() const;
  std::string           *getWinnerName() const;
  void                  killWormsUnderWater(Ogre::SceneNode *waterNode);
  void                  doDamageToWorms(Ogre::Entity *explosion, float damage);
  bool                  getTeleportActivated() const;
  void                  setTeleportActivated(bool);
};
