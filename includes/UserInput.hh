#pragma once

#include "data.hh"

class InputListener;
class Worms;

class UserInput
{
public:
  UserInput();
  ~UserInput();
  bool selectBazooka(InputListener *, Worms *, const Ogre::FrameEvent& evt);
  bool selectGrenade(InputListener *, Worms *, const Ogre::FrameEvent& evt);
  bool selectAWP(InputListener *, Worms *, const Ogre::FrameEvent& evt);
  bool selectTeleport(InputListener *, Worms *, const Ogre::FrameEvent& evt);
  bool selectNuke(InputListener *, Worms *, const Ogre::FrameEvent& evt);
  bool selectBat(InputListener *, Worms *, const Ogre::FrameEvent& evt);
  bool moveLeft(InputListener *listener, Worms *, const Ogre::FrameEvent& evt);
  bool moveRight(InputListener *listener, Worms *, const Ogre::FrameEvent& evt);
  bool moveAngleUp(InputListener *listener, Worms *, const Ogre::FrameEvent& evt);
  bool moveAngleDown(InputListener *listener, Worms *, const Ogre::FrameEvent& evt);
  bool spacePressed(InputListener *listener, Worms *current, const Ogre::FrameEvent& evt);
  bool jump(InputListener *listener, Worms *current, const Ogre::FrameEvent& evt);
};
