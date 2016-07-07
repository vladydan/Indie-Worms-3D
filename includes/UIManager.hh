#pragma once

#include "WormsApp.hh"

struct GameData
{
  std::string teamName;
  std::string wormsCount;
  bool        ia;
  int         worms;
};

class UIManager
{
  public:
    explicit UIManager(WormsApp &app);
    ~UIManager();
    bool createWindows();
    bool subscribeEvent();
    bool setActiveWeapon(std::string const &weaponName);
    bool exitPressed(const CEGUI::EventArgs&);
    bool playPressed(const CEGUI::EventArgs&);
    bool mapPressed(const CEGUI::EventArgs&);
    CEGUI::Window *createTeamGUI();
    bool addTeamPressed(const CEGUI::EventArgs&);
    bool playGamePressed(const CEGUI::EventArgs&);
    void updateFPS(int fps);
    void updateTimer(int timer);
    Ogre::Vector2 worldToScreen(const Ogre::Vector3& worldPoint,
        Ogre::Camera &cam) const;
    void moveHealhBar(Ogre::SceneNode *node,
        std::string const &name,  std::string const &data, Ogre::Camera &camera);
    static std::list<std::string> *readDirectory(std::string const &directoryName);
    void createHealthBar(std::string const &wormName);
    bool getLeave() const;
    bool echapPressed();
    bool backToMenuPressed(const CEGUI::EventArgs&);
    void displayEndMenu(std::string const *);
    bool savePressed(const CEGUI::EventArgs&);
    bool loadGameButtonPressed(const CEGUI::EventArgs&);
    bool loadPressed(const CEGUI::EventArgs&);
    bool resumePressed(const CEGUI::EventArgs&);
    Ogre::Vector2 teleportPressed();
    void desactivateCursor();
    void hideSplashScreen();
  private:
    WormsApp *_app;
    std::map<std::string, CEGUI::Window *> _windowList;
    CEGUI::OgreRenderer* _UIRenderer;
    std::list<GameData> _gameData;
    bool _leave;
};
