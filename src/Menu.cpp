#include "Menu.hh"

Menu::Menu(Ogre::Root& root,  Ogre::RenderWindow &window, std::string const &sceneName) : AScene(root, window, sceneName)
{
}

void Menu::createLighting()
{
  Ogre::Light *light = this->_scene->createLight("MainLight");
  light->setType(Ogre::Light::LT_POINT);
  light->setDiffuseColour(1.0, 1.0, 1.0);
  light->setSpecularColour(1.0, 1.0, 1.0);
  light->setPosition(-400, 200, 400);
}


void Menu::createViewports(Ogre::RenderWindow& window)
{
  this->_view = window.addViewport(this->_camera);
  this->_camera->setAspectRatio(Ogre::Real(this->_view->getActualWidth() / Ogre::Real(this->_view->getActualHeight())));
  this->_view->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
}


void Menu::createCustomScene()
{
  //Tu met ton code ici Amlan Kappa
}

void Menu::createGame(Game::GameType gameType,
                unsigned int timer, unsigned int teams,
                unsigned int worms, unsigned int hp)
{
  this->_game = nullptr;
}
