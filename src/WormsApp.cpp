#include "WormsApp.hh"
#include "UIManager.hh"

WormsApp::WormsApp()
{
  Ogre::ConfigFile configFile;

  try
  {
    this->_Root = new Ogre::Root("plugins.cfg", "ogre.cfg", "Worms.log");
    configFile.load("resources.cfg");
    Ogre::ConfigFile::SectionIterator seci = configFile.getSectionIterator();
    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
      {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        for (Ogre::ConfigFile::SettingsMultiMap::iterator it = settings->begin(); it != settings->end(); ++it)
          {
            typeName = it->first;
            archName = it->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
          }
      }
  }
  catch (Ogre::Exception &e)
  {
    std::cout << e.what() << std::endl;
    exit(0);
  }
}

WormsApp::~WormsApp()
{
  delete this->_Root;
}

bool		WormsApp::start()
{
  if(!(this->_Root->restoreConfig() || this->_Root->showConfigDialog()))
    return false;
  this->_Window = this->_Root->initialise(true, "Indie Worms");
  Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
  SceneManager SceneMgr(this->_Window);
  this->_SceneManager = &SceneMgr;
  Menu menuScene(*this->_Root, *this->_Window, "MenuScene");
  menuScene.createCustomScene();
  menuScene.createLighting();
  this->_SceneManager->addScene(&menuScene, "MenuScene");
  Scene mainScene(*this->_Root, *this->_Window, "PlayScene");
  mainScene.createRandomScene();
  mainScene.createLighting();
  this->_SceneManager->addScene(&mainScene, "PlayScene");
  UIManager UImanager(*this);
  _UIManager = &UImanager;
  this->createFrameListener(*this->_Root);
  this->_Root->startRendering();
  return true;
}

SceneManager *WormsApp::getSceneManager() const
{
  return this->_SceneManager;
}

Ogre::RenderWindow* WormsApp::getWindow() const
{
  return this->_Window;
}

Ogre::Root          *WormsApp::getRoot() const
{
  return this->_Root;
}

UIManager           *WormsApp::getUIManager() const
{
  return this->_UIManager;
}

void	WormsApp::createFrameListener(Ogre::Root& root)
{
  Ogre::FrameListener *frameListener = new InputListener(this, this->_SceneManager->getScene("PlayScene")->getPhysicList(), this->_SceneManager->getScene("PlayScene")->getCollisionManager(),  this->_SceneManager->getScene("PlayScene")->getGame());
  root.addFrameListener(frameListener);
}

SoundManager        *WormsApp::getSoundManager()
{
  return &this->_soundManager;
}
