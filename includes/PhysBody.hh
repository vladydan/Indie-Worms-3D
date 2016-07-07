#pragma once

#include "data.hh"

class	PhysBody {
public:
    typedef enum {
      CIRCLE,
      RECTANGLE
    }		Shape;

  PhysBody(PhysBody::Shape, float sizeX, float sizeY, float weight, const std::string EntityName);
  PhysBody(PhysBody::Shape, float radius, float weight, const std::string EntityName);
  ~PhysBody();
  bool	isCollided(PhysBody&, Ogre::SceneManager *);
  PhysBody::Shape	getShapeType() const;
  float			getSizeX() const;
  float			getSizeY() const;
  std::string		getName() const;
  float			getRadius() const;
  float			getWeight() const;
  void			setRadius(float radius);

  private:
  bool			RectOnlyCollision(PhysBody&, Ogre::SceneManager *);
  bool			CircleOnlyCollision(PhysBody &, PhysBody&);
  bool			BothCollision(PhysBody &Rect, PhysBody &Circle);

  std::string		_EntityName;
  float			_Weight;
  float			_sizeX;
  float			_sizeY;
  float			_Radius;
  PhysBody::Shape	_Shape;
 };
