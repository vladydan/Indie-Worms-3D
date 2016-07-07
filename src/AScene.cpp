#include <json/json.h>
#include <chrono>
#include "Game.hh"
#include "AScene.hh"
#include "UIManager.hh"

AScene::AScene(Ogre::Root& root, Ogre::RenderWindow& window,
  std::string const &sceneName) : _camera(0), _view(0), _game(nullptr)
{
  this->_scene = root.createSceneManager(Ogre::ST_GENERIC, sceneName);
  this->_collisionMgr = new CollisionManager(10, this->_scene);
  this->createCamera();
}

AScene::~AScene()
{
  if (this->_game != nullptr)
    delete this->_game;
}

void AScene::createSceneFromFile(std::string const &file)
{
  (void)file;
}

void AScene::createLighting()
{

}

void AScene::createCustomScene(float **map)
{
  if (map != nullptr)
    this->_world = new World(this->_collisionMgr, this->_scene, 1920, 1080, map);
}

void AScene::resetScene(Ogre::SceneNode* node)
{
  this->_scene->clearScene();
  this->_physicList.clear();
}

void AScene::generateWormsPhysAndHPBar(UIManager *ui)
{
  PhysBody  *wormPh2 = new PhysBody(PhysBody::RECTANGLE, 25 * 0.2, 25 * 0.4, 0, "Grenade");
  this->_collisionMgr->addEntity(wormPh2, "Grenade");
  PhysBody  *wormPh3 = new PhysBody(PhysBody::RECTANGLE, 25 * 0.2, 25 * 0.4, 0, "Bazooka");
  this->_collisionMgr->addEntity(wormPh3, "Bazooka");
  PhysBody  *wormPh4 = new PhysBody(PhysBody::RECTANGLE, 25 * 0.2, 25 * 0.4, 0, "AWP");
  this->_collisionMgr->addEntity(wormPh4, "AWP");
  PhysBody  *wormPh5 = new PhysBody(PhysBody::RECTANGLE, 25 * 0.2, 25 * 0.4, 0, "Bat");
  this->_collisionMgr->addEntity(wormPh5, "Bat");
  PhysBody  *wormPh6 = new PhysBody(PhysBody::RECTANGLE, 25 * 0.2, 25 * 0.4, 0, "Nuke");
  this->_collisionMgr->addEntity(wormPh5, "Nuke");

  for (auto *team : this->_game->getTeam())
  {
    for (auto *worm : team->getWorms())
    {
      PhysBody  *wormPh = new PhysBody(PhysBody::RECTANGLE, 50.8734 * 0.2, 54.0165 * 0.4 , 250, worm->getName());
      this->_collisionMgr->addEntity(wormPh, worm->getName());
      this->_physicList.push_back(*wormPh);
      ui->createHealthBar(wormPh->getName());
      ui->moveHealhBar(worm->getNode(), worm->getName(), worm->getNickName(), *this->_camera);
    }
  }
}

void AScene::createRandomScene()
{
  this->_world = new World(this->_collisionMgr, this->_scene, 1920, 1080, nullptr);
}

void AScene::createCamera()
{
  this->_camera = this->_scene->createCamera("Ma Camera");
  this->_camera->setPosition(Ogre::Vector3(310, 150.0, 700));
  //this->_Camera->setFarClipDistance(20000);
  this->_camera->setNearClipDistance(2);
}

Ogre::Viewport *AScene::getViewPort() const
{
  return this->_view;
}

Ogre::Camera *AScene::getCamera() const
{
  return this->_camera;
}

Game *AScene::getGame() const
{
  return this->_game;
}

Ogre::SceneManager *AScene::getScene() const
{
  return this->_scene;
}

std::list<PhysBody> *AScene::getPhysicList()
{
  return &this->_physicList;
}

CollisionManager    *AScene::getCollisionManager() const
{
  return this->_collisionMgr;
}

World		*AScene::getWorld() {
  return (this->_world);
}

void AScene::serializeScene() const
{
  std::chrono::time_point<std::chrono::system_clock> start, end;

  start = std::chrono::system_clock::now();
  end = std::chrono::system_clock::now();
  std::time_t end_time = std::chrono::system_clock::to_time_t(end);
  std::string timeString = std::ctime(&end_time);
  timeString.pop_back();
  std::ofstream file_id;

  file_id.open(SAVE_DIRECTORY + timeString + ".json");
  Json::Value value_obj;
  for (auto *team : this->_game->getTeam())
  {
    value_obj["teams"][team->getName()]["name"] = team->getName();
    value_obj["teams"][team->getName()]["IA"] = team->getIA();
    Json::Value color(Json::arrayValue);
    color.append(team->getColor().r);
    color.append(team->getColor().g);
    color.append(team->getColor().b);
    value_obj["teams"][team->getName()]["teamColor"] = color;
    for (auto *worm : team->getWorms())
    {
      Json::Value position(Json::arrayValue);
      position.append(worm->getNode()->getPosition().x);
      position.append(worm->getNode()->getPosition().y);
      position.append(worm->getNode()->getPosition().z);
      value_obj["teams"][team->getName()][worm->getName()]["health"] = worm->getHP();
      value_obj["teams"][team->getName()][worm->getName()]["position"] = position;
      value_obj["teams"][team->getName()][worm->getName()]["health"] = worm->getHP();
      value_obj["teams"][team->getName()][worm->getName()]["name"] = worm->getName();
      value_obj["teams"][team->getName()][worm->getName()]["nickname"] = worm->getNickName();
    }
  }
  float **map = this->_world->getRandMap().getMap();
  for (int i = 0; i < this->_world->getRandMap().getSizeY(); ++i)
  {
    Json::Value coord(Json::arrayValue);
    for (int j = 0; j < this->_world->getRandMap().getSizeX(); ++j)
    {
      coord.append(map[i][j]);
    }
    value_obj["map"][i] = coord;
  }
  Json::StyledWriter styledWriter;
  file_id << styledWriter.write(value_obj);
  file_id.close();
}

void AScene::unSerializeScene(std::string const& fileName)
{
  this->createGame(Game::DEFAULT, 30, 0, 0, 10);
  Json::Value root;
  Json::Reader reader;
  std::ifstream file(fileName, std::ifstream::binary);
  bool parsingSuccessful = reader.parse(file, root, false);
  if (!parsingSuccessful)
  {
      std::cerr  << reader.getFormattedErrorMessages() << std::endl;;
  }
  for (auto &team : root["teams"])
  {
    std::string name = team["name"].asString();
    this->_game->addTeam(name, nullptr, this->_scene, 0, 10, team["IA"].asBool());
    this->_game->setNextTeam(this->_game->getTeam().back());
    Ogre::ColourValue color(team["teamColor"][0].asFloat(), team["teamColor"][1].asFloat(), team["teamColor"][2].asFloat(), 0.5);
    for (auto &worm : team)
    {
      if (worm.type() == Json::objectValue)
      {
        Json::Value arr(Json::arrayValue);
        arr = worm["position"];
        std::string nickname = worm["nickname"].asString();
        std::string name = worm["name"].asString();
        this->_game->getNextTeam()->addWorms(nickname, name, color, this->_scene, worm["health"].asInt());
        this->_game->findWormByName(name)->getNode()->setPosition(Ogre::Vector3(arr[0].asFloat(), arr[1].asFloat(), arr[2].asFloat()));
      }
    }
  }
  float **map = new float*[static_cast<int>(300) + 2];
  for (int i = 0; i < 300 + 2; i++)
    map[i] = new float[static_cast<int>(300) + 2];

  int i = 0;
  for (auto &mapJson : root["map"])
  {
    for (int j = 0; j != mapJson.size(); ++j)
    {
      map[i][j] = mapJson[j].asFloat();
    }
    i++;
  }
  this->createCustomScene(map);
  this->_game->nextWorm();
}
