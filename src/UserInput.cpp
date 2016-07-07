#include "UserInput.hh"
#include "InputListener.hh"
#include "Worms.hh"
#include "WormsApp.hh"
#include "UIManager.hh"

UserInput::UserInput()
{

}

UserInput::~UserInput()
{

}

bool UserInput::selectBazooka(InputListener *listener, Worms *currentcurrent, const Ogre::FrameEvent& evt)
{
  if (listener->getGame()->getGun(listener->getGame()->getNextTeam()->getChoseGun()).getIsFree() && !listener->getGame()->getGun(listener->getGame()->getNextTeam()->getChoseGun()).getAfterEffect())
  {
    listener->getWormsApp()->getUIManager()->setActiveWeapon("Bazooka");
    listener->getGame()->getNextTeam()->setChoseGun("Bazooka");
  }
  return true;
}

bool UserInput::selectGrenade(InputListener *listener, Worms *current, const Ogre::FrameEvent& evt)
{
  if (listener->getGame()->getGun(listener->getGame()->getNextTeam()->getChoseGun()).getIsFree() && !listener->getGame()->getGun(listener->getGame()->getNextTeam()->getChoseGun()).getAfterEffect())
  {
    listener->getWormsApp()->getUIManager()->setActiveWeapon("Grenade");
    listener->getGame()->getNextTeam()->setChoseGun("Grenade");
  }
  return true;
}

bool UserInput::selectAWP(InputListener *listener, Worms *, const Ogre::FrameEvent& evt)
{
  if (listener->getGame()->getGun(listener->getGame()->getNextTeam()->getChoseGun()).getIsFree() && !listener->getGame()->getGun(listener->getGame()->getNextTeam()->getChoseGun()).getAfterEffect())
  {
    listener->getWormsApp()->getUIManager()->setActiveWeapon("AWP");
    listener->getGame()->getNextTeam()->setChoseGun("AWP");
  }
  return true;
}

bool UserInput::selectNuke(InputListener *listener, Worms *, const Ogre::FrameEvent& evt)
{
  if (listener->getGame()->getGun(listener->getGame()->getNextTeam()->getChoseGun()).getIsFree() && !listener->getGame()->getGun(listener->getGame()->getNextTeam()->getChoseGun()).getAfterEffect())
  {
    listener->getWormsApp()->getUIManager()->setActiveWeapon("Nuke");
    listener->getGame()->getNextTeam()->setChoseGun("Nuke");
  }
  return true;
}


bool UserInput::selectTeleport(InputListener *listener, Worms *, const Ogre::FrameEvent& evt)
{
  if (listener->getGame()->getGun(listener->getGame()->getNextTeam()->getChoseGun()).getIsFree() && !listener->getGame()->getGun(listener->getGame()->getNextTeam()->getChoseGun()).getAfterEffect())
  {
    listener->getWormsApp()->getUIManager()->setActiveWeapon("Teleport");
    listener->getGame()->setTeleportActivated(true);
    listener->getWormsApp()->getUIManager()->teleportPressed();
  }
  return true;
}

bool UserInput::selectBat(InputListener *listener, Worms *, const Ogre::FrameEvent& evt)
{
  if (listener->getGame()->getGun(listener->getGame()->getNextTeam()->getChoseGun()).getIsFree() && !listener->getGame()->getGun(listener->getGame()->getNextTeam()->getChoseGun()).getAfterEffect())
  {
    listener->getWormsApp()->getUIManager()->setActiveWeapon("Bat");
    listener->getGame()->getNextTeam()->setChoseGun("Bat");
  }
  return true;
}

bool UserInput::moveLeft(InputListener *listener, Worms *current, const Ogre::FrameEvent& evt)
{
  if (listener->getCollisionMgr()->isCollidedToStatic(current->getName(), CollisionManager::LEFT))
    current->getNode()->translate(0, 0.5, 0);
  if (!listener->getCollisionMgr()->isCollidedToStatic(current->getName(), CollisionManager::LEFT))
    current->getNode()->translate(-evt.timeSinceLastFrame * 50, 0, 0);
  if (current->isFacingRight())
  {
    current->getNode()->rotate(Ogre::Vector3::UNIT_Z, Ogre::Degree(180));
    current->setFacing(false);
  }
  return true;
}

bool UserInput::moveRight(InputListener *listener, Worms *current, const Ogre::FrameEvent& evt)
{
  if (listener->getCollisionMgr()->isCollidedToStatic(current->getName(), CollisionManager::RIGHT))
    current->getNode()->translate(0, 0.5, 0);
  if (!listener->getCollisionMgr()->isCollidedToStatic(current->getName(), CollisionManager::RIGHT))
    current->getNode()->translate(evt.timeSinceLastFrame * 50, 0, 0);
  if (!current->isFacingRight())
  {
    current->getNode()->rotate(Ogre::Vector3::UNIT_Z, Ogre::Degree(180));
    current->setFacing(true);
  }
  return true;

}

bool UserInput::jump(InputListener *listener, Worms *current, const Ogre::FrameEvent& evt)
{
  if (!current->isJumping() && current->getFalling() == 0)
    current->activateJump();
  return true;
}

bool UserInput::moveAngleUp(InputListener *listener, Worms *current, const Ogre::FrameEvent& evt)
{
  current->setAngle(current->getAngle() + 0.01);
  return true;

}

bool UserInput::moveAngleDown(InputListener *listener, Worms *current, const Ogre::FrameEvent& evt)
{
  current->setAngle(current->getAngle() - 0.01);
  return true;
}

bool UserInput::spacePressed(InputListener *listener, Worms *current, const Ogre::FrameEvent& evt)
{
  listener->getGame()->getNextTeam()->setNbrSpace(listener->getGame()->getNextTeam()->getNbrSpace() + 1);
  // if (!listener->getGame()->getNextTeam()->getIsSpace() && listener->getGame()->getGun(listener->getGame()->getNextTeam()->getSaveChoseGun()).getIsFree())
  // {
  //   listener->getGame()->getNextTeam()->setIsSpace(true);
  //   listener->getGame()->getNextTeam()->setNbrSpace(0);
  // }
  // else if (listener->getGame()->getNextTeam()->getSaveSpace())
  // {
  //   listener->getGame()->getNextTeam()->setIsSpace(true);
  //   listener->getGame()->getNextTeam()->incNbrSpace();
  // }
    return true;
}
