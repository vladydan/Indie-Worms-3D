#include "UIManager.hh"
#include <sys/types.h>
#include <dirent.h>

UIManager::UIManager(WormsApp &app)  : _app(&app), _leave(false)
{
  this->_UIRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
  CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
  CEGUI::Font::setDefaultResourceGroup("Fonts");
  CEGUI::Scheme::setDefaultResourceGroup("Schemes");
  CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
  CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
  try
  {
    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    CEGUI::SchemeManager::getSingleton().createFromFile("AlfiskoSkin.scheme");
    CEGUI::SchemeManager::getSingleton().createFromFile("fuckcegui.scheme"); //sorry
    CEGUI::SchemeManager::getSingleton().createFromFile("weapons.scheme");
    CEGUI::SchemeManager::getSingleton().createFromFile("OgreTray.scheme");
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("AlfiskoSkin/MouseArrow");
    CEGUI::FontManager::getSingleton().createFromFile("DejaVuSans-10.font");
    CEGUI::FontManager::getSingleton().createFromFile("Jura-18.font");
    CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont("DejaVuSans-10");
  }
  catch (CEGUI::Exception &e)
  {
    std::cerr << "Something went wrong : " << e.what() << std::endl;
    std::cerr << "Try to reinstall libs, or to do 'export LD_LIBRARY_PATH=./libs/:$LD_LIBRARY_PATH'" << std::endl;
  }
  this->createWindows();
  this->subscribeEvent();
  this->_app->getSceneManager()->loadScene("MenuScene");
  this->_app->getSceneManager()->getScene("PlayScene")->createGame(Game::DEFAULT, 30, 0, 0, 10);
  this->_windowList["createGame"]->disable();
  this->_windowList["createGame"]->hide();
  this->_windowList["hud"]->disable();
  this->_windowList["hud"]->hide();
  this->_windowList["hud_end"]->disable();
  this->_windowList["hud_end"]->hide();
  this->_windowList["loadGame"]->disable();
  this->_windowList["loadGame"]->hide();
  this->_app->getSoundManager()->play("menu");
  this->_windowList["splashscreen"]->show();
  this->_windowList["splashscreen"]->enable();
}

UIManager::~UIManager()
{
}

bool UIManager::createWindows()
{
  this->_windowList["root"] = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "rootWindow");
  CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(this->_windowList["root"]);
  this->_windowList["menu"] = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("menu.layout");
  CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(this->_windowList["menu"]);
  this->_windowList["createGame"] = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("createGame.layout");
  CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(this->_windowList["createGame"]);
  this->_windowList["hud"] = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("hud.layout");
  CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(this->_windowList["hud"]);
  this->_windowList["weapons"] = this->_windowList["hud"]->getChild("Weapons");
  this->_windowList["hud_menu"] = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("hud_menu.layout");
  CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(this->_windowList["hud_menu"]);
  this->_windowList["hud_end"] = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("hud_end.layout");
  CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(this->_windowList["hud_end"]);
  this->_windowList["loadGame"] = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("loadGame.layout");
  CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(this->_windowList["loadGame"]);
  this->_windowList["splashscreen"] = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("splashscreen.layout");
  CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(this->_windowList["splashscreen"]);

  CEGUI::Window* t1 = this->_windowList["createGame"]->getChild("Team");
  CEGUI::Window* t2 = CEGUI::WindowManager::getSingleton().createWindow("OgreTray/Button", "tab2");
  CEGUI::TabControl* tc = static_cast<CEGUI::TabControl *>(this->_windowList["createGame"]->getChild("TabControl"));
  CEGUI::UDim bar_bottom(0, 10);
  t2->setText("Map");
  t1->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp, CEGUI::Event::Subscriber(&UIManager::mapPressed, this));
  t2->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp, CEGUI::Event::Subscriber(&UIManager::mapPressed, this));
  tc->addTab(t1);
  tc->addTab(t2);
}

bool UIManager::setActiveWeapon(std::string const &weaponName)
{
  for (int i = 0; i < this->_windowList["weapons"]->getChildCount(); ++i)
  {
    CEGUI::Window *child = this->_windowList["weapons"]->getChildAtIdx(i);
    if (child->getName() == weaponName)
    {
      child->enable();
      child->show();
    }
    else
    {
      child->disable();
      child->hide();
    }
  }
  return true;
}

bool UIManager::subscribeEvent()
{
  this->_windowList["menu"]->getChild("exitButton")->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp, CEGUI::Event::Subscriber(&UIManager::exitPressed, this));
  this->_windowList["menu"]->getChild("newGameButton")->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp, CEGUI::Event::Subscriber(&UIManager::playPressed, this));
  static_cast<CEGUI::TabControl *>(this->_windowList["createGame"]->
  getChild("TabControl"))->getTabContents("Team")->getChild("addTeamButton")
  ->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp,
    CEGUI::Event::Subscriber(&UIManager::addTeamPressed, this));
  static_cast<CEGUI::TabControl *>(this->_windowList["createGame"]->
    getChild("TabControl"))->getTabContents("Team")->getChild("playGame")
    ->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp,
      CEGUI::Event::Subscriber(&UIManager::playGamePressed, this));
  this->_windowList["menu"]->getChild("loadGameButton")->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp, CEGUI::Event::Subscriber(&UIManager::loadGameButtonPressed, this));
  this->_windowList["hud_menu"]->getChild("resume")->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp, CEGUI::Event::Subscriber(&UIManager::resumePressed, this));
  this->_windowList["hud_menu"]->getChild("exit")->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp, CEGUI::Event::Subscriber(&UIManager::exitPressed, this));
  this->_windowList["hud_menu"]->getChild("backToMenu")->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp, CEGUI::Event::Subscriber(&UIManager::backToMenuPressed, this));
  this->_windowList["hud_menu"]->getChild("save")->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp, CEGUI::Event::Subscriber(&UIManager::savePressed, this));
  this->_windowList["hud_end"]->getChild("exit")->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp, CEGUI::Event::Subscriber(&UIManager::exitPressed, this));
  this->_windowList["hud_end"]->getChild("backToMenu")->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp, CEGUI::Event::Subscriber(&UIManager::backToMenuPressed, this));
  this->_windowList["loadGame"]->getChild("loadGame")->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp, CEGUI::Event::Subscriber(&UIManager::loadPressed, this));
}

bool UIManager::backToMenuPressed(const CEGUI::EventArgs&)
{
  CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();
  this->_windowList["menu"]->enable();
  this->_windowList["menu"]->show();
  this->_windowList["hud"]->disable();
  this->_windowList["hud"]->hide();
  this->_windowList["hud_end"]->disable();
  this->_windowList["hud_end"]->hide();
  this->_app->getSceneManager()->getScene("PlayScene")->resetScene(this->_app->getSceneManager()->getScene("PlayScene")->getScene()->getRootSceneNode());
  this->_windowList["hud"]->destroyChild(this->_windowList["hud"]->getChild("healthBars"));
  CEGUI::Window *healthbar = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "healthBars");
  this->_windowList["hud"]->addChild(healthbar);
  this->_app->getSceneManager()->getScene("PlayScene")->getCollisionManager()->clear();
  this->_app->getSceneManager()->getScene("PlayScene")->createRandomScene();
  this->_app->getSceneManager()->getScene("PlayScene")->createLighting();
  this->_app->getSceneManager()->getScene("PlayScene")->createGame(Game::DEFAULT, 30, 0, 0, 10);
  this->_app->getSceneManager()->loadScene("MenuScene");
}

bool UIManager::mapPressed(const CEGUI::EventArgs&)
{
}

void UIManager::createHealthBar(std::string const &wormName)
{
  CEGUI::Window *healthbar = CEGUI::WindowManager::getSingleton().createWindow("OgreTray/Label", wormName);
  this->_windowList["hud"]->getChild("healthBars")->addChild(healthbar);
  healthbar->setText(wormName);
}

void UIManager::moveHealhBar(Ogre::SceneNode *node, std::string const &name, std::string const &data, Ogre::Camera &camera)
{
  Ogre::Vector2 pointPos;

  pointPos = this->worldToScreen(node->getPosition(), camera);

  this->_windowList["hud"]->getChild("healthBars")->getChild(name)->setPosition(CEGUI::UVector2(CEGUI::UDim(pointPos.x, 0), CEGUI::UDim(pointPos.y, 0)));
  this->_windowList["hud"]->getChild("healthBars")->getChild(name)->setText(data);
  this->_windowList["hud"]->getChild("healthBars")->getChild(name)->setSize(CEGUI::USize(CEGUI::UDim(0, 150), CEGUI::UDim( 0, 200)));
}

bool UIManager::exitPressed(const CEGUI::EventArgs&)
{
  this->_leave = true;
}

bool UIManager::getLeave() const
{
  return this->_leave;
}

void UIManager::displayEndMenu(std::string const *teamName)
{
  this->_windowList["hud_end"]->setText(*teamName + " is the winner ! ");
  this->_windowList["hud_end"]->enable();
  this->_windowList["hud_end"]->show();
  this->_windowList["hud"]->disable();
  this->_windowList["hud"]->hide();
  CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();
}

bool UIManager::addTeamPressed(const CEGUI::EventArgs&)
{
  GameData data;
  CEGUI::Window *teamTab;
  std::string dataGUI;

  teamTab = static_cast<CEGUI::TabControl *>(this->_windowList["createGame"]->
    getChild("TabControl"))->getTabContents("Team");
  data.teamName = teamTab->getChild("teamName")->getText().c_str();
  data.wormsCount = teamTab->getChild("wormNumber")->getText().c_str();
  data.ia = static_cast<CEGUI::ToggleButton *>(teamTab->getChild("IA"))->isSelected();
  try
  {
    data.worms = std::stoi(data.wormsCount);
  }
  catch (std::exception &e)
  {
    data.worms = 4;
    data.wormsCount = "4";
  }
  if (data.worms <= 0 || data.worms > 1000)
    return true;
  if (data.teamName == "")
  {
    data.teamName = this->_app->getSceneManager()->getScene("PlayScene")->getGame()->generateTeamName();
  }
  this->_gameData.push_back(data);
  teamTab->getChild("teamName")->setText("");
  teamTab->getChild("wormNumber")->setText("");
  static_cast<CEGUI::ToggleButton *>(teamTab->getChild("IA"))->setSelected(false);
  if (data.ia)
    dataGUI = data.teamName + " with " + data.wormsCount + " worms, IA";
  else
    dataGUI = data.teamName + " with " + data.wormsCount + " worms";
  CEGUI::ListboxItem* itemListbox = new CEGUI::ListboxTextItem(dataGUI, 1);
  itemListbox->setSelectionBrushImage("TaharezLook/MultiListSelectionBrush");
  static_cast<CEGUI::Listbox *>(teamTab->getChild("teamList"))->addItem(itemListbox);
  static_cast<CEGUI::Listbox *>(teamTab->getChild("teamList"))->handleUpdatedItemData();
  static_cast<CEGUI::Listbox *>(teamTab->getChild("teamList"))->ensureItemIsVisible(itemListbox);
}

bool UIManager::playPressed(const CEGUI::EventArgs&)
{
  this->_windowList["menu"]->disable();
  this->_windowList["menu"]->hide();
  this->_windowList["createGame"]->enable();
  this->_windowList["createGame"]->show();
}

bool UIManager::playGamePressed(const CEGUI::EventArgs&)
{
  CEGUI::Window *teamTab;

  teamTab = static_cast<CEGUI::TabControl *>(this->_windowList["createGame"]->
    getChild("TabControl"))->getTabContents("Team");
  if (static_cast<CEGUI::ToggleButton *>(teamTab->getChild("SuddenDeath"))->isSelected() == true)
  {
    this->_app->getSceneManager()->getScene("PlayScene")->getGame()->setGameType(Game::GameType::SUDDEN_DEATH);
  }
  if (this->_gameData.size() == 0)
    return true;
  for (auto &data : this->_gameData)
    this->_app->getSceneManager()->getScene("PlayScene")->getGame()->addTeam(data.teamName, nullptr, this->_app->getSceneManager()->getScene("PlayScene")->getScene(), data.worms, 10, data.ia);
  this->_app->getSceneManager()->getScene("PlayScene")->generateWormsPhysAndHPBar(this);
  this->_app->getSceneManager()->loadScene("PlayScene");
  this->_windowList["createGame"]->disable();
  this->_windowList["createGame"]->hide();
  this->_windowList["hud"]->enable();
  this->_windowList["hud"]->show();
  CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
  this->_gameData.clear();
  static_cast<CEGUI::Listbox *>(teamTab->getChild("teamList"))->resetList();
  this->_app->getSoundManager()->play("beach");
  this->_app->getSoundManager()->stop("menu");
}

void          UIManager::updateFPS(int fps)
{
  this->_windowList["hud"]->getChild("FPSCounter")->setText("FPS : " + std::to_string(fps));
}

void          UIManager::updateTimer(int timer)
{
  this->_windowList["hud"]->getChild("Timer")->getChild("Label")->setText(std::to_string(timer));
}

Ogre::Vector2 UIManager::worldToScreen(const Ogre::Vector3& worldPoint, Ogre::Camera &cam) const
{
    Ogre::Vector4 tmp(worldPoint.x, worldPoint.y, worldPoint.z, 1);

    Ogre::Vector4 kappa = cam.getViewMatrix() * tmp;
    Ogre::Vector4 screenPoint = cam.getProjectionMatrix() * kappa;

    return Ogre::Vector2((screenPoint.x * 0.5f - 10) / screenPoint.w + 0.5f,
                   (screenPoint.y * -0.5f - 76)   / screenPoint.w + 0.5f);
}

bool UIManager::echapPressed()
{
  CEGUI::Window *teamTab;

  teamTab = static_cast<CEGUI::TabControl *>(this->_windowList["createGame"]->
    getChild("TabControl"))->getTabContents("Team");
  if (this->_windowList["menu"]->isVisible())
  {
    this->_leave = true;
  }
  else if (this->_windowList["loadGame"]->isVisible())
  {
    this->_windowList["menu"]->enable();
    this->_windowList["menu"]->show();
    this->_windowList["loadGame"]->disable();
    this->_windowList["loadGame"]->hide();
    static_cast<CEGUI::Listbox *>(this->_windowList["loadGame"]->getChild("saveList"))->resetList();
    this->_app->getSoundManager()->play("menu");
    this->_app->getSoundManager()->stop("beach");
  }
  else if (this->_windowList["createGame"]->isVisible())
  {
    this->_windowList["menu"]->enable();
    this->_windowList["menu"]->show();
    this->_windowList["createGame"]->disable();
    this->_windowList["createGame"]->hide();
    this->_gameData.clear();
    static_cast<CEGUI::Listbox *>(teamTab->getChild("teamList"))->resetList();
  }
  else
  {
    this->_windowList["hud_menu"]->enable();
    this->_windowList["hud_menu"]->show();
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();
  }
  return true;
}

bool UIManager::savePressed(const CEGUI::EventArgs&)
{
  this->_app->getSceneManager()->getScene("PlayScene")->serializeScene();
  CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
}

bool UIManager::resumePressed(const CEGUI::EventArgs&)
{
  CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
}


bool UIManager::loadGameButtonPressed(const CEGUI::EventArgs&)
{
  this->_windowList["loadGame"]->enable();
  this->_windowList["loadGame"]->show();
  this->_windowList["menu"]->disable();
  this->_windowList["menu"]->hide();
  std::list<std::string> *fileList = this->readDirectory(SAVE_DIRECTORY);
  if (fileList != nullptr)
  {
    for (auto &file : *fileList)
    {
      CEGUI::ListboxItem* itemListbox = new CEGUI::ListboxTextItem(file, 1);
      itemListbox->setSelectionBrushImage("TaharezLook/MultiListSelectionBrush");
      static_cast<CEGUI::Listbox *>(this->_windowList["loadGame"]->getChild("saveList"))->addItem(itemListbox);
      static_cast<CEGUI::Listbox *>(this->_windowList["loadGame"]->getChild("saveList"))->handleUpdatedItemData();
      static_cast<CEGUI::Listbox *>(this->_windowList["loadGame"]->getChild("saveList"))->ensureItemIsVisible(itemListbox);
    }
    this->_app->getSoundManager()->play("beach");
    this->_app->getSoundManager()->stop("menu");
    delete fileList;
  }
}

std::list<std::string> *UIManager::readDirectory(std::string const &directoryName)
{
  auto dir = opendir(directoryName.c_str());
  if (dir == NULL)
  {
    std::cerr << "could not open directory: " << directoryName.c_str() << std::endl;
    return nullptr;
  }
  std::list<std::string> *fileList = new std::list<std::string>;
  auto entity = readdir(dir);
  while (entity != NULL)
  {
    if (entity->d_type == DT_REG && entity->d_name[0] != '.')
      fileList->push_back(entity->d_name);
    entity = readdir(dir);
  }
  closedir(dir);
  return fileList;
}

bool UIManager::loadPressed(const CEGUI::EventArgs&)
{
  this->_app->getSceneManager()->getScene("PlayScene")->resetScene(this->_app->getSceneManager()->getScene("PlayScene")->getScene()->getRootSceneNode());
  CEGUI::ListboxItem* itemListbox = static_cast<CEGUI::Listbox *>(this->_windowList["loadGame"]->getChild("saveList"))->getFirstSelectedItem();
  if (itemListbox != nullptr)
  {
      this->_app->getSceneManager()->getScene("PlayScene")->getCollisionManager()->clear();
    std::string fileName = std::string(SAVE_DIRECTORY) + itemListbox->getText().c_str();
    this->_app->getSceneManager()->getScene("PlayScene")->unSerializeScene(fileName);
    this->_app->getSceneManager()->getScene("PlayScene")->createLighting();
    this->_app->getSceneManager()->getScene("PlayScene")->generateWormsPhysAndHPBar(this);
    this->_app->getSceneManager()->loadScene("PlayScene");
    this->_windowList["loadGame"]->disable();
    this->_windowList["loadGame"]->hide();
    this->_windowList["hud"]->enable();
    this->_windowList["hud"]->show();
    static_cast<CEGUI::Listbox *>(this->_windowList["loadGame"]->getChild("saveList"))->resetList();
  }
}

Ogre::Vector2 UIManager::teleportPressed()
{
  Ogre::Vector2 posOgre;

  CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();
  CEGUI::Vector2f pos = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition();
  posOgre.x = pos.d_x;
  posOgre.y = pos.d_y;
  return posOgre;
}

void UIManager::desactivateCursor()
{
  CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
}

void UIManager::hideSplashScreen()
{
  this->_windowList["splashscreen"]->disable();
  this->_windowList["splashscreen"]->hide();
}
