#pragma once

#include "data.hh"
#include "RandMap.hh"
#include "InputListener.hh"
#include "PhysBody.hh"
#include "Scene.hh"
#include "Menu.hh"
#include "SceneManager.hh"
#include "SoundManager.hh"

class UIManager;

class	WormsApp
{
  public:
    WormsApp();
    ~WormsApp();

    bool	start();
    SceneManager        *getSceneManager() const;
    Ogre::RenderWindow* getWindow() const;
    Ogre::Root          *getRoot() const;
    UIManager           *getUIManager() const;
    void	            createFrameListener(Ogre::Root& root);
    // No const here because we would need to put const SoundManager in the return
    // and SoundManager needs to be modified.
    SoundManager        *getSoundManager();
  private:
    Ogre::Root			       *_Root;
    Ogre::RenderWindow		*_Window;
    SceneManager          *_SceneManager;
    UIManager             *_UIManager;
    SoundManager          _soundManager;
};
