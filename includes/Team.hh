#pragma once

# include <list>
# include <string>
# include "Worms.hh"
# include "data.hh"

class Team
{
  std::string const _name;
  std::list<Worms*>     _worms;
  std::list<Worms*>::const_iterator _nextWorms;
  // Inventory             _inventory;
  Ogre::ColourValue _color;
  bool _ia;
  std::string _choseGun;
  float _nbrSpace;
public:
  Team(Team const& cpy);
  Team(std::string const& name, Ogre::ColourValue *color,
       Ogre::SceneManager *_scene, unsigned int worms = 5,
       unsigned char hp = 100, bool ia = false);
  ~Team();
  std::string const&    getName() const;
  Ogre::ColourValue const& getColor() const;
  std::list<Worms*> const& getWorms() const;
  Worms*                getNextWorms() const;
  bool                  setNextWorms(Worms const* nextWorms);

  Worms*                nextWorms();
  unsigned int          wormsCount() const;
  unsigned int          addWorms(std::string const &nickName, std::string const& name,
                                 Ogre::ColourValue color, Ogre::SceneManager *scene, unsigned char hp);
  unsigned int          removeWorms(Worms const* worms);
  bool                  getIA() const;


  void setNbrSpace(const float);
  float getNbrSpace() const;
  void setChoseGun(std::string const &);
  std::string getChoseGun() const;

};
