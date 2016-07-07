#pragma once

#include "AScene.hh"

class UIManager;

class Scene : public AScene
{
  public:
    Scene(Ogre::Root& root, Ogre::RenderWindow& window,
      std::string const &sceneName);
    virtual void createLighting();
    virtual void createCustomScene();
    void createGame(Game::GameType gameType = Game::DEFAULT,
                    unsigned int timer = 30, unsigned int teams = 2,
                    unsigned int worms = 5, unsigned int hp = 100);
    void createViewports(Ogre::RenderWindow& window);
  private:
    std::vector <Team> _teams;
};
