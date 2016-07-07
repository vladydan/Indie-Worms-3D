#pragma once

#include "data.hh"
#include <OIS.h>
#include "CollisionManager.hh"
#include "PhysBody.hh"
#include "Game.hh"
#include "UserInput.hh"

#define FLOW_SPEED 2
#define FLOW_HEIGHT 5

class WormsApp;
class AScene;
struct UserCommands
{
  OIS::KeyCode key;
  std::function<bool(InputListener *, Worms *, const Ogre::FrameEvent& evt)> func;
};

class	InputListener: public Ogre::FrameListener, OIS::KeyListener, Ogre::WindowEventListener, OIS::MouseListener
{
  public:
    InputListener(WormsApp *app, std::list<PhysBody> *list, CollisionManager *collisionMgr, Game *game);
    bool          frameRenderingQueued(const Ogre::FrameEvent& evt);
    void          startOIS();
    virtual bool  mouseMoved(const OIS::MouseEvent &e);
    virtual bool  mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
    virtual bool  mouseReleased(const OIS::MouseEvent &e , OIS::MouseButtonID id);
    virtual bool  keyPressed(const OIS::KeyEvent &e);
    virtual bool  keyReleased(const OIS::KeyEvent &e);
    CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID);
    bool          checkWormsInput(Worms *worm, const Ogre::FrameEvent& evt);
    void          updateHealthBars();
    void          updateVariables();
    Game          *getGame() const;
    WormsApp      *getWormsApp() const;
    CollisionManager *getCollisionMgr() const;
    void          changeTurn();
  private:
    void          updateWater(const Ogre::FrameEvent& evt);
    void          updateCam(const Ogre::FrameEvent& evt, Worms *worm);
    void          updateGravity(const Ogre::FrameEvent& evt);
    void          shootingMouvements(const Ogre::FrameEvent& evt);
    OIS::InputManager*  _InputManager;
    OIS::Mouse*         _Mouse;
    OIS::Keyboard*      _Keyboard;
    Ogre::SceneManager  *_SceneMgr;
    CollisionManager    *_CollisionMgr;
    std::list<PhysBody> *_PhysicList;
    AScene              *_currentScene;
    Worms               *_current;
    float               _delayTime;
    WormsApp            *_WormsApp;
    bool                _fixedCam;
    Ogre::Vector3       _CamSupposedPosition;
    Game                *_Game;
    Ogre::Vector3       _Mouvement;
    Ogre::Real          _Vitesse;
    Ogre::Real          _VitesseRotation;
};
