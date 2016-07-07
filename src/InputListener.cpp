#include "InputListener.hh"
#include "gun/Grenade.hh"
#include "gun/Gun.hh"
#include "WormsApp.hh"
#include "UIManager.hh"
#include "IA.hh"

UserInput input;

UserCommands usrCommands[] =
{
  {
    OIS::KC_A, std::bind(&UserInput::selectGrenade, &input,std::placeholders::_1 , std::placeholders::_2, std::placeholders::_3)
  },
  {
    OIS::KC_Z, std::bind(&UserInput::selectBazooka, &input, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
  },
  {
    OIS::KC_E, std::bind(&UserInput::selectAWP, &input,std::placeholders::_1 , std::placeholders::_2, std::placeholders::_3)
  },
  {
    OIS::KC_R, std::bind(&UserInput::selectNuke, &input,std::placeholders::_1 , std::placeholders::_2, std::placeholders::_3)
  },
  {
    OIS::KC_Q, std::bind(&UserInput::selectBat, &input,std::placeholders::_1 , std::placeholders::_2, std::placeholders::_3)
  },
  {
    OIS::KC_T, std::bind(&UserInput::selectTeleport, &input,std::placeholders::_1 , std::placeholders::_2, std::placeholders::_3)
  },
  {
    OIS::KC_LEFT, std::bind(&UserInput::moveLeft, &input, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
  },
  {
    OIS::KC_RIGHT, std::bind(&UserInput::moveRight, &input, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
  },
  {
    OIS::KC_RETURN, std::bind(&UserInput::jump, &input, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
  },
  {
    OIS::KC_UP, std::bind(&UserInput::moveAngleUp, &input, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
  },
  {
    OIS::KC_DOWN, std::bind(&UserInput::moveAngleDown, &input, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
  },
  {
    OIS::KC_SPACE, std::bind(&UserInput::spacePressed, &input, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
  },
  {
    static_cast<OIS::KeyCode>(-1), nullptr
  }
};


InputListener::InputListener(WormsApp *app, std::list<PhysBody> *list, CollisionManager *collisionMgr, Game *game)
{
  this->_WormsApp = app;
  this->_SceneMgr = app->getSceneManager()->getScene("PlayScene")->getScene();
  this->_CollisionMgr = collisionMgr;
  this->_PhysicList = list;
  this->_Vitesse = 100;
  this->_VitesseRotation = 0.3;
  this->_Mouvement = Ogre::Vector3::ZERO;
  this->_fixedCam = true;
  this->startOIS();
  this->_delayTime = 2;
  this->_current = nullptr;
}

void InputListener::startOIS()
{
  Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
  OIS::ParamList pl;
  size_t windowHnd = 0;
  std::ostringstream windowHndStr;
  this->_WormsApp->getWindow()->getCustomAttribute("WINDOW", &windowHnd);
  windowHndStr << windowHnd;
  pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
  this->_InputManager = OIS::InputManager::createInputSystem(pl);
  this->_Mouse = static_cast<OIS::Mouse*>(this->_InputManager->createInputObject( OIS::OISMouse, true));
  this->_Keyboard = static_cast<OIS::Keyboard*>(this->_InputManager->createInputObject( OIS::OISKeyboard, true));
  _Mouse->setEventCallback(this);
  _Keyboard->setEventCallback(this);
}

void InputListener::updateVariables()
{
  this->_Game = this->_WormsApp->getSceneManager()->getScene("PlayScene")->getGame();
  this->_currentScene = this->_WormsApp->getSceneManager()->getCurrentScene();
}

bool InputListener::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  if (this->_WormsApp->getWindow()->isClosed())
    return false;
  this->_Keyboard->capture();
  this->_Mouse->capture();
  this->updateVariables();
  if (this->_WormsApp->getUIManager()->getLeave() != false)
    return false;
  _delayTime += evt.timeSinceLastFrame;
  if (this->_currentScene->getScene() != this->_SceneMgr)
  {
    if (_delayTime > 5)
    {
      this->_WormsApp->getUIManager()->hideSplashScreen();
    }
    return true;
  }
  if (!this->_Game->getGun(this->_Game->getNextTeam()->getChoseGun()).getAfterEffect() &&
    this->_WormsApp->getSceneManager()->getScene("PlayScene")->getGame()
           ->getGun(this->_Game->getNextTeam()->getChoseGun()).getIsFree())
  {
    if (_delayTime > 2 && this->_Game->addToTime(evt.timeSinceLastFrame) == true)
    {
      this->_Game->setTeleportActivated(false);
      this->_WormsApp->getUIManager()->desactivateCursor();
      _delayTime = 0;
    }
  }
  this->updateHealthBars();
  _current = this->_Game->getCurrentWorm();
  if (!this->_Game->getTeleportActivated())
    this->_WormsApp->getUIManager()->setActiveWeapon(this->_Game->getNextTeam()->getChoseGun());
  if (_current == nullptr)
  {
    this->_Game->nextWorm();
    _current = this->_Game->getCurrentWorm();
  }
  this->updateGravity(evt);
  if (_delayTime < 2)
    return true;
  if (_current->getHP() <= 0)
  {
    this->changeTurn();
  }
  if (this->_current != nullptr)
    this->updateCam(evt, _current);
  if (this->_Game->getWinnerName() != nullptr)
  {
    this->_WormsApp->getUIManager()->displayEndMenu(this->_Game->getWinnerName());
  }
  if (this->_Game->getNextTeam()->getIA() == true)
  {
    if (this->_Game->getIA()->getTarget() == nullptr)
      this->_Game->findCloseWorms();
    if (this->_WormsApp->getSceneManager()->getScene("PlayScene")->getGame()
          ->getGun(this->_Game->getNextTeam()->getChoseGun()).getIsFree())
      this->_Game->getIA()->shoot(*this->_Game, this->_WormsApp->getSoundManager(), this->getCollisionMgr());
  }
  else if (_fixedCam)
    this->checkWormsInput(_current, evt);
  this->shootingMouvements(evt);
  this->_WormsApp->getUIManager()->updateFPS(this->_WormsApp->getWindow()->getAverageFPS());
  this->_WormsApp->getUIManager()->updateTimer(this->_Game->getTimer() - this->_Game->getTimeLeft());
  this->updateWater(evt);
  return true;
}

void InputListener::changeTurn()
{
  this->_Game->nextWorm();
  this->_Game->setTeleportActivated(false);
  _current = this->_Game->getCurrentWorm();
  _delayTime = 0;
}

void InputListener::updateGravity(const Ogre::FrameEvent& evt)
{
  for (std::list<PhysBody>::iterator it = this->_PhysicList->begin(); it != this->_PhysicList->end(); it++)
  {
    Ogre::Node *node = this->_SceneMgr->getRootSceneNode()->getChild(it->getName());
    Worms *worm;
    worm = this->_Game->findWormByName(it->getName());

    if (!this->_CollisionMgr->isCollidedToStatic(it->getName(), CollisionManager::DOWN) && node->getPosition().y > 0 && !this->_Game->getTeleportActivated())
      {
        if (!this->_CollisionMgr->isCollidedToStatic(it->getName(), CollisionManager::DOWN))
          node->translate(0, -1 * it->getWeight() * evt.timeSinceLastFrame, 0);
        worm->fall(1);
      }
    else
      worm->fall(0);
  }
}

void InputListener::shootingMouvements(const Ogre::FrameEvent& evt)
{
  if (this->_Game->getGun(this->_Game->getNextTeam()->getChoseGun()).getAfterEffect() == true)
  {
    if (this->_Game->getGun(this->_Game->getNextTeam()->getChoseGun()).afterEffect(evt.timeSinceLastFrame) == true)
    {
      this->changeTurn();
    }
    else
    {
      this->_Game->doDamageToWorms(this->_Game->
        getGun(this->_Game->getNextTeam()->getChoseGun()).getExplosion(), this->_Game->
        getGun(this->_Game->getNextTeam()->getChoseGun()).getScale() / 20);
    }
  }
  else if (!this->_WormsApp->getSceneManager()->getScene("PlayScene")->getGame()
           ->getGun(this->_Game->getNextTeam()->getChoseGun()).getIsFree())
    this->_Game->getGun(this->_Game->getNextTeam()->getChoseGun()).move(evt.timeSinceLastFrame, this->_CollisionMgr, this->_Game->getTeam());
  if (this->_Mouse->getMouseState().buttonDown(OIS::MB_Left))
   {
    if (this->_Game->getTeleportActivated())
    {
      Ogre::Vector2 pos = this->_WormsApp->getUIManager()->teleportPressed();
      pos.x = pos.x / this->_currentScene->getViewPort()->getActualWidth();
      pos.y = pos.y / this->_currentScene->getViewPort()->getActualHeight();
      Ogre::Ray ray = this->_currentScene->getCamera()->getCameraToViewportRay(pos.x, pos.y);

      Ogre::Vector3 newRay= ray.getOrigin() + (ray.getDirection() * 3);
      newRay.z = 0;
      _current->getNode()->setPosition(newRay);
    }
  }
}

bool InputListener::checkWormsInput(Worms *_current, const Ogre::FrameEvent& evt)
{
  for (int i = 0; usrCommands[i].key != -1; ++i)
  {
    if (this->_Keyboard->isKeyDown(usrCommands[i].key))
    {
      if (usrCommands[i].func != nullptr)
      {
        this->_Game->setTeleportActivated(false);
        this->_WormsApp->getUIManager()->desactivateCursor();
        usrCommands[i].func(this, _current, evt);
      }
    }
  }
  if (_current->isJumping())
    _current->jump(evt.timeSinceLastFrame, this->_CollisionMgr);
  if (this->_Keyboard->isKeyDown(OIS::KC_RIGHT) && !this->_CollisionMgr->
      isCollidedToStatic(_current->getName(), CollisionManager::RIGHT))
    _current->getNode()->translate(0.1, 0, 0);
  return (true);
}

void InputListener::updateWater(const Ogre::FrameEvent& evt)
{
  float WaterFlow = FLOW_SPEED * evt.timeSinceLastFrame;
  static float FlowAmount = 0.0f;
  static bool FlowUp = true;
  Ogre::SceneNode *pWaterNode = static_cast<Ogre::SceneNode*>(this->_currentScene->getCamera()->getSceneManager()->getRootSceneNode()->getChild("WaterNode"));
  if (FlowUp)
    FlowAmount += WaterFlow;
  else
    FlowAmount -= WaterFlow;
  if (FlowAmount >= this->_currentScene->getWorld()->getWaterLevel() + FLOW_HEIGHT)
    FlowUp = false;
  else if (FlowAmount <=this->_currentScene->getWorld()->getWaterLevel())
    FlowUp = true;
  pWaterNode->translate(0, (FlowUp ? WaterFlow : -WaterFlow), 0);
  if (this->_Game->getGameType() == Game::GameType::SUDDEN_DEATH)
    this->_currentScene->getWorld()->riseWater();
  this->_Game->killWormsUnderWater(pWaterNode);
}

void InputListener::updateHealthBars()
{
  for (auto *team : this->_Game->getTeam())
  {
    for (auto *worm : team->getWorms())
    {
      this->_WormsApp->getUIManager()->moveHealhBar(worm->getNode(), worm->getName(), team->getName() + "\n" + worm->getNickName()
                                      + "\nHP : " + std::to_string(static_cast<int>(worm->getHP())), *this->_currentScene->getCamera());
    }
  }
}

void InputListener::updateCam(const Ogre::FrameEvent& evt, Worms *_current)
{
  Ogre::Vector3 deplacement = Ogre::Vector3::ZERO;
  if (this->_fixedCam)
  {
    if (this->_Keyboard->isKeyDown(OIS::KC_W))
    {
      this->_CamSupposedPosition = Ogre::Vector3(600, 250.0, 950);
    }
    else
    {
      this->_CamSupposedPosition = _current->getNode()->getPosition() - Ogre::Vector3(0, 0, -300);
    }
  }
  if (!this->_fixedCam)
  {
    Ogre::Vector3 mov = this->_Mouvement;

    if (this->_currentScene->getCamera()->getPosition().x < 100)
      mov.x = 1;
    if (this->_currentScene->getCamera()->getPosition().x > 1200)
      mov.x = -1;
    if (this->_currentScene->getCamera()->getPosition().y < 50)
      mov.y = 1;
    if (this->_currentScene->getCamera()->getPosition().y > 510)
      mov.y = -1;
    if (this->_currentScene->getCamera()->getPosition().z < 50)
      mov.z = 1;
    if (this->_currentScene->getCamera()->getPosition().z > 510)
      mov.z = -1;
    deplacement = mov * this->_Vitesse * evt.timeSinceLastFrame;
    this->_currentScene->getCamera()->moveRelative(deplacement);
  }
  else
    this->_currentScene->getCamera()->setPosition(this->_currentScene->getCamera()->getPosition() + ((this->_CamSupposedPosition - this->_currentScene->getCamera()->getPosition()) * 0.05f));
  Ogre::Node *node = this->_WormsApp->getSceneManager()->getScene("PlayScene")->getScene()->getRootSceneNode()->getChild("CrossHair");
  node->setPosition(_current->getNode()->getPosition().x + 40 * cos(_current->getAngle()), _current->getNode()->getPosition().y + 40 * sin(_current->getAngle()), 30);
}

CEGUI::MouseButton InputListener::convertButton(OIS::MouseButtonID buttonID)
{
  switch (buttonID)
  {
  case OIS::MB_Left:
    return CEGUI::LeftButton;

  case OIS::MB_Right:
    return CEGUI::RightButton;

  case OIS::MB_Middle:
    return CEGUI::MiddleButton;

  default:
    return CEGUI::LeftButton;
  }
}

bool InputListener::mouseMoved(const OIS::MouseEvent &e)
{
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(e.state.X.rel, e.state.Y.rel);
  return true;
}

bool InputListener::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertButton(id));
  return true;
}

bool InputListener::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertButton(id));
  if (id == OIS::MB_Left && this->_Game->getTeleportActivated())
  {
    this->_Game->setTeleportActivated(false);
    this->_WormsApp->getUIManager()->desactivateCursor();
  }
  return true;
}

bool InputListener::keyPressed(const OIS::KeyEvent &e)
{
  CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
  context.injectKeyDown((CEGUI::Key::Scan)e.key);
  context.injectChar((CEGUI::Key::Scan)e.text);
  switch (e.key)
  {
    case OIS::KC_Z:
      this->_Mouvement.y += 1;
      break;
    case OIS::KC_S:
      this->_Mouvement.y -= 1;
      break;
    case OIS::KC_Q:
      this->_Mouvement.x -= 1;
      break;
    case OIS::KC_D:
      this->_Mouvement.x += 1;
      break;
    case OIS::KC_UP:
      this->_Mouvement.z -= 1;
      break;
    case OIS::KC_DOWN:
      this->_Mouvement.z += 1;
      break;
    case OIS::KC_LSHIFT:
      this->_Vitesse *= 4;
      break;
    case OIS::KC_K:
    {
      this->_fixedCam = !this->_fixedCam;
      if (this->_fixedCam)
      {
        this->_currentScene->getCamera()->setPosition(Ogre::Vector3(600, 250.0, 790));
        this->_currentScene->getCamera()->lookAt(Ogre::Vector3(590.0, 200, 0.0));
      }
    }
    break;
  }
  return true;
}

bool InputListener::keyReleased(const OIS::KeyEvent &e)
{
  CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
  context.injectKeyUp((CEGUI::Key::Scan)e.key);
  if (this->_Game != nullptr)
    switch (e.key)
      {
      case OIS::KC_SPACE:
      {
        if (this->_Game->getNextTeam() != nullptr && this->_Game->getNextTeam()->getNbrSpace() > 0 && this->_WormsApp->getSceneManager()->getScene("PlayScene")->getGame()
            ->getGun(this->_Game->getNextTeam()->getChoseGun()).getIsFree())
        {
          if (this->_Game->getNextTeam()->getNbrSpace() > 200)
          {
            this->_Game->getNextTeam()->setNbrSpace(200);
          }
          this->_Game->getGun(this->_Game->getNextTeam()->getChoseGun()).shoot(this->_WormsApp->getSoundManager(), _current->getNode(), _current->getAngle(), this->_Game->getNextTeam()->getNbrSpace() / 20);
          this->_Game->getNextTeam()->setNbrSpace(0);
        }
      }
        break;
      case OIS::KC_Z:
        this->_Mouvement.y -= 1;
        break;
      case OIS::KC_S:
        this->_Mouvement.y += 1;
        break;
      case OIS::KC_Q:
        this->_Mouvement.x += 1;
        break;
      case OIS::KC_D:
        this->_Mouvement.x -= 1;
        break;
      case OIS::KC_UP:
        this->_Mouvement.z += 1;
       break;
      case OIS::KC_DOWN:
        this->_Mouvement.z -= 1;
        break;
      case OIS::KC_LSHIFT:
        this->_Vitesse /= 4;
        break;
      case ::OIS::KC_ESCAPE:
        this->_WormsApp->getUIManager()->echapPressed();
        break;
      }
  return true;
}

Game          *InputListener::getGame() const
{
  return this->_Game;
}

WormsApp          *InputListener::getWormsApp() const
{
  return this->_WormsApp;
}

CollisionManager      *InputListener::getCollisionMgr() const
{
  return this->_CollisionMgr;
}
