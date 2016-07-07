#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cmath>

class PerlinNoise {
public:
  PerlinNoise(float x, float y);
  void		Randomize();
  void		newNoise();
  float**	getMap() const;

private:
  float		**map;
  unsigned int	numb[256];
  float		sizeX;
  float		sizeY;

  float		get2dPoint(float x, float y, float res);
};
