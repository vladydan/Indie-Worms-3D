#ifndef DATA_HH_
#define DATA_HH_

#include <Ogre.h>

#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <CEGUI/CEGUI.h>
#include <OIS.h>
namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm;
        stm << n;
        return stm.str();
    }
}

#endif
