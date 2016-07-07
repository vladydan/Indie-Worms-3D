#include "data.hh"
#include "WormsApp.hh"

#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
  #define WIN32_LEAN_AND_MEAN
  #include "window.h"

  INT WINAPI WinMAIN(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLINE, INT)
#else
int main(int ac, char **av)
#endif
{
  WormsApp	app;

  try{
      app.start();
    } catch(Ogre::Exception& e) {
      #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	  MessageBoxA(NULL, e.getFullDescription().c_str(), "An exception has occurred !", MB_OK | MB_ICONERROR | MB_TASKMODAL);
      #else
	fprintf(stderr, "An exception has occurred : %s\n", e.getFullDescription().c_str());
      #endif
    }
  return (0);
}
