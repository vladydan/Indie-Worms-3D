#include <list>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "Team.hh"
#include "Worms.hh"

static const std::vector<std::string> NAMES = {"Ayy lmao", "Roger", "Le chien",
                                               "Gros Leo", "SHA1", "Batman",
                                               "J0G", "M0X", "TOUMIS", "Crvor",
                                               "Kappa", "Keppo", "Pride", "TEK",
                                               "Pepe", "D4T4Z", "Simba", "Thor",
                                               "Captain", "WADDUP BOI", "Hulk",
                                               "Scarlett", "Iron Man", "Vador",
                                               "800Z", "F0URDR", "PR05P3R", "DUROTAN",
                                                "RAGNAR", "MEDHIV"};

Team::Team(std::string const& name, Ogre::ColourValue *color,
           Ogre::SceneManager *scene, unsigned int worms,
           unsigned char hp, bool ia) :
  _name(name), _color(*color), _ia(ia)
{
  if (worms != 0)
  {
    unsigned int i = 0;
    while (++i <= worms)
    {
      std::string nickName = NAMES[rand() % NAMES.size()];
      this->_worms.push_back(new Worms(nickName, name + "_" + nickName +
                                       "_" + std::to_string(i), scene, *color, hp));
    }
    this->_nextWorms = this->_worms.begin();
  }
  this->_nbrSpace = 0;
  this->_choseGun = "Grenade";
}

Team::Team(Team const& cpy) :
  _name(cpy.getName()), _color(cpy.getColor())
{
  Ogre::ColourValue color(1, 0, 0, 0);

  if (cpy.getWorms().size() != 0)
  {
    for (std::list<Worms*>::const_iterator it = cpy.getWorms().begin();
         it != cpy.getWorms().end(); ++it)
    {
      this->_worms.push_back(new Worms((*it)->getNickName(), (*it)->getName(), nullptr, color, (*it)->getHP()));
      if (cpy.getNextWorms() == *it)
      {
        this->_nextWorms = this->_worms.end();
        --(this->_nextWorms);
      }
    }
  }
}

Team::~Team()
{
  for (std::list<Worms*>::iterator it = this->_worms.begin();
       it != this->_worms.end(); ++it)
  {
    delete *it;
    it = this->_worms.erase(it);
  }
}

std::string const&              Team::getName() const
{
  return (this->_name);
}

Ogre::ColourValue const&         Team::getColor() const
{
  return (this->_color);
}

std::list<Worms*> const&        Team::getWorms() const
{
  return (this->_worms);
}


void Team::setChoseGun(std::string const &gun)
{
    this->_choseGun = gun;
}

void Team::setNbrSpace(const float i)
{
    this->_nbrSpace = i;
}

std::string Team::getChoseGun() const
{
  return (this->_choseGun);
}

float Team::getNbrSpace() const
{
  return (this->_nbrSpace);
}


Worms*                          Team::getNextWorms() const
{
  return (*this->_nextWorms);
}

bool Team::setNextWorms(Worms const* nextWorms)
{
  for (std::list<Worms*>::const_iterator it = this->_worms.begin();
       it != this->_worms.end(); ++it)
  {
    if (*it == nextWorms)
    {
      this->_nextWorms = it;
      return (true);
    }
  }
  return (false);
}

Worms*                    Team::nextWorms()
{
  if (this->_worms.size() == 0)
  {
    this->_nextWorms = this->_worms.end();
    return (nullptr);
  }
  else if (++(this->_nextWorms) == this->_worms.end())
  {
    this->_nextWorms = this->_worms.begin();
  }
  return (*this->_nextWorms);
}

unsigned int Team::wormsCount() const
{
  return (this->_worms.size());
}

unsigned int Team::addWorms(std::string const &nickName, std::string const& name,
                            Ogre::ColourValue color, Ogre::SceneManager *scene, unsigned char hp)
{
  this->_worms.push_back(new Worms(nickName, name, scene, color, hp));
  this->_nextWorms = this->_worms.begin();
  return (this->_worms.size());
}

unsigned int Team::removeWorms(Worms const* worms)
{
  for (std::list<Worms*>::iterator it = this->_worms.begin();
       it != this->_worms.end(); ++it)
  {
    if (*it == worms)
    {
      if (this->_worms.size() == 1)
        this->_nextWorms = this->_worms.end();
      else if (it == this->_nextWorms)
        this->nextWorms();
      delete *it;
      this->_worms.erase(it);
      return (this->_worms.size());
    }
  }
  return (this->_worms.size());
}

bool Team::getIA() const
{
  return this->_ia;
}
