#pragma once

#include "AScene.hh"

class UIManager;

class Menu : public AScene
{
  public:
    Menu(Ogre::Root& root, Ogre::RenderWindow& window,
      std::string const &sceneName);
    virtual void createLighting();
    virtual void createCustomScene();
    void createViewports(Ogre::RenderWindow& window);
    void createGame(Game::GameType gameType = Game::DEFAULT,
                unsigned int timer = 30, unsigned int teams = 2,
                unsigned int worms = 5, unsigned int hp = 100);
};
