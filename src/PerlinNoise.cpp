#include "PerlinNoise.hh"
#include <fstream>

PerlinNoise::PerlinNoise(float x, float y)
{
unsigned int lel[] = {151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,
            142,8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,
            203,117,35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,
            74,165,71,134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,
            105,92,41,55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,
            187,208,89,18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,3,
            64,52,217,226,250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,
            47,16,58,17,182,189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,221,
            153,101,155,167,43,172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,
            112,104,218,246,97,228,251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,
            235,249,14,239,107,49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,
            127,4,150,254,138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,
            156,180};

  std::copy(&lel[0], &lel[255], &this->numb[0]);
  this->sizeX = x;
  this->sizeY = y;
  this->map = new float*[(int)y];
  for(int i = 0; i < (int)y; i++)
    this->map[i] = new float[(int)x];
  this->newNoise();
}

void		PerlinNoise::Randomize()
{
  std::random_shuffle(&this->numb[0], &this->numb[254]);
}

float		PerlinNoise::get2dPoint(float x, float y, float res)
{
  float tempX,tempY;
  int x0, y0, ii, jj, gi0, gi1, gi2, gi3;
  float tmp, s, t, u, v, Cx, Cy, Li1, Li2;
  float unit = 1.0f / std::sqrt(2);
  float gradient2[][2] = {{unit,unit},{-unit,unit},{unit,-unit},{-unit,-unit},{1,0},{-1,0},{0,1},{0,-1}};

  x /= res;
  y /= res;
  x0 = (int)(x);
  y0 = (int)(y);
  ii = x0 & 255;
  jj = y0 & 255;
  gi0 = this->numb[ii + this->numb[jj]] % 8;
  gi1 = this->numb[ii + 1 + this->numb[jj]] % 8;
  gi2 = this->numb[ii + this->numb[jj + 1]] % 8;
  gi3 = this->numb[ii + 1 + this->numb[jj + 1]] % 8;
  tempX = x-x0;
  tempY = y-y0;
  s = gradient2[gi0][0]*tempX + gradient2[gi0][1]*tempY;
  tempX = x-(x0+1);
  tempY = y-y0;
  t = gradient2[gi1][0]*tempX + gradient2[gi1][1]*tempY;
  tempX = x-x0;
  tempY = y-(y0+1);
  u = gradient2[gi2][0]*tempX + gradient2[gi2][1]*tempY;
  tempX = x-(x0+1);
  tempY = y-(y0+1);
  v = gradient2[gi3][0]*tempX + gradient2[gi3][1]*tempY;
  tmp = x-x0;
  Cx = 3 * tmp * tmp - 2 * tmp * tmp * tmp;
  Li1 = s + Cx*(t-s);
  Li2 = u + Cx*(v-u);
  tmp = y - y0;
  Cy = 3 * tmp * tmp - 2 * tmp * tmp * tmp;
  return Li1 + Cy*(Li2-Li1);
}

void		PerlinNoise::newNoise()
{
  this->Randomize();
  for(int i = 0; i < this->sizeY; i++)
    {
      for (int e = 0; e < this->sizeX; e++)
      {
        this->map[i][e] = this->get2dPoint(e, i, 10);
        this->map[i][e] += 1;
        this->map[i][e] *= 0.5 * 255;
      }
    }
}

float**		PerlinNoise::getMap() const
{
  return this->map;
}
