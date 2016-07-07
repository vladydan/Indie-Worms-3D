#include "Scene.hh"
#include "UIManager.hh"

Scene::Scene(Ogre::Root& root,  Ogre::RenderWindow &window, std::string const &sceneName) : AScene(root, window, sceneName)
{
}

void Scene::createLighting()
{
  Ogre::Light *light = this->_scene->createLight("MainLight");
  // light->setPosition(20, 80, 50);
  // Ogre::Vector3 lightdir(0.55f, -0.3f, 0.75f);
  // light = this->_scene->createLight("terrainLight");
  light->setType(Ogre::Light::LT_POINT);
  light->setDiffuseColour(1.0, 1.0, 1.0);
  light->setSpecularColour(1.0, 1.0, 1.0);
  light->setPosition(-400, 200, 400);
}

void Scene::createCustomScene()
{

}

void Scene::createViewports(Ogre::RenderWindow& window)
{
  this->_view = window.addViewport(this->_camera);
  this->_camera->setAspectRatio(Ogre::Real(this->_view->getActualWidth() / Ogre::Real(this->_view->getActualHeight())));
  this->_view->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));
  this->_camera->setPosition(Ogre::Vector3(600, 250.0, 790));
  this->_camera->lookAt(Ogre::Vector3(590.0, 200, 0.0));
}

void Scene::createGame(Game::GameType gameType, unsigned int timer,
                        unsigned int teams, unsigned int worms,
                        unsigned int hp)
{
  this->_game = new Game(this->_scene, gameType, timer, teams, worms, hp);
}
