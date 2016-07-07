#pragma once

# include <string>
# include <dlfcn.h>
# include <iostream>

template <typename T>
class	DLLoader
{
  void	*_handler;
  T	*(*_fcnptr)();

 public:
  DLLoader()
  {
  }

  int loadDL(std::string const& libname, std::string const& entry_point)
  {
    char  *error;

    if ((this->_handler = dlopen(libname.c_str(), RTLD_NOW)) == NULL ||
        (this->_fcnptr = reinterpret_cast<T* (*)()>(dlsym(this->_handler, entry_point.c_str()))) == NULL)
      {
        error = dlerror();
        if (error != NULL)
          std::cerr << "ERROR : " << error << std::endl;
        else
          std::cerr << "Can't load dynamic lib " << libname << " with entry point "
                    << entry_point << ". Something went wrong but dlerror() is NULL." << std::endl;
        return (1);
      }
    return (0);
  }

  ~DLLoader()
  {
    char	*error;

    if (dlclose(this->_handler) != 0)
      {
        error = dlerror();
        if (error != NULL)
          std::cout << error << std::endl;
        else
          std::cout << "Can't close dynamic lib. Something went wrong but dlerror() is NULL." << std::endl;
      }
  }

  T* getInstance() const
  {
    return (_fcnptr());
  }
};
