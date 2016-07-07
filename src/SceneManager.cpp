#include "SceneManager.hh"

SceneManager::SceneManager(Ogre::RenderWindow* window) : _window(window), _currentScene(0)
{

}

SceneManager::~SceneManager()
{

}

void SceneManager::addScene(AScene *scene, std::string const &sceneName)
{
  this->_sceneMgr[sceneName] = scene;
}

void SceneManager::loadScene(std::string const &sceneName)
{
  if (this->_sceneMgr.find(sceneName) != this->_sceneMgr.end())
  {
    this->_window->removeAllViewports();
    this->_sceneMgr[sceneName]->createViewports(*_window);
    this->_currentScene = this->_sceneMgr[sceneName];
  }
  else
    std::cerr << "loadScene : Scene not found" << std::endl;
}

AScene* SceneManager::getCurrentScene() const
{
  return this->_currentScene;
}

AScene *SceneManager::getScene(std::string sceneName)
{
  if (this->_sceneMgr.find(sceneName) != this->_sceneMgr.end())
  {
    return this->_sceneMgr[sceneName];
  }
  else
  {
    std::cerr << "getScene : "<< sceneName <<" not found" << std::endl;
    return NULL;
  }
}
