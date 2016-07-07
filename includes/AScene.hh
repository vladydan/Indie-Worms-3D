#pragma once

#include "Game.hh"
#include "data.hh"
#include "RandMap.hh"
#include "CollisionManager.hh"
#include "InputListener.hh"
#include "World.hh"

class UIManager;

#define SAVE_DIRECTORY "saves/"

class AScene
{
  public:
    AScene(Ogre::Root& root, Ogre::RenderWindow& window,
      std::string const &sceneName);
    virtual ~AScene();
    void createSceneFromFile(std::string const &file);
    virtual void createLighting();
    virtual void createCustomScene(float **map);
    void createRandomScene();
    void generateWormsPhysAndHPBar(UIManager *scene);
    Ogre::Viewport *getViewPort() const;
    Game *getGame() const;
    virtual void createViewports(Ogre::RenderWindow& window) = 0;
    Ogre::Camera *getCamera() const;
    Ogre::SceneManager *getScene() const;
    std::list<PhysBody> *getPhysicList();
    CollisionManager    *getCollisionManager() const;
    virtual void createGame(Game::GameType gameType = Game::DEFAULT,
                    unsigned int timer = 30, unsigned int teams = 2,
                    unsigned int worms = 5, unsigned int hp = 100) = 0;
    World        *getWorld();
    void         resetScene(Ogre::SceneNode* node);
    void         serializeScene() const;
    void         unSerializeScene(std::string const& fileName);
  protected:
    void createCamera();

  CollisionManager          *_collisionMgr;
  Ogre::SceneManager        *_scene;
  Ogre::Viewport            *_view;
  Ogre::Camera              *_camera;
  std::list<PhysBody>       _physicList;
  Game                      *_game;
  World                     *_world;
};
