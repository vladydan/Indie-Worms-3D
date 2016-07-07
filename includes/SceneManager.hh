#pragma once

#include "data.hh"
#include "AScene.hh"

class SceneManager
{
  public:
    SceneManager(Ogre::RenderWindow* window);
    ~SceneManager();
    void addScene(AScene *scene, std::string const &sceneName);
    void loadScene(std::string const &sceneName);
    AScene* getCurrentScene() const;
    AScene *getScene(std::string sceneName);
  private:
    std::map<std::string, AScene *> _sceneMgr;
    Ogre::RenderWindow* _window;
    AScene*             _currentScene;
};
