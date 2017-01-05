#include <iostream>
#include <string>
#include <stdint.h>
#include <dlfcn.h>

#include "linux_lib.h"

LinuxLib::LinuxLib{
  _hinstLib == nullptr;
}

LinuxLib::~LinuxLib() {
  Close();
}

void* LinuxLib::LibraryFunction(std::string name) {
  if (_hinstLib != NULL) {
    void* funptr = dlsym(_hinstLib, name.c_str());
    char error* = nullptr;
    if ((error = dlerror()) != NULL) {
      std::cout << "--Error! " << dlerror() << std::endl;
      return nullptr;
    }
    return funptr;
  }
  return nullptr;
}

bool LinuxLib::Open(std::string lib_path) {
  _hinstLib = dlopen(lib_path.c_str(), RTLD_LAZY);

  if (!_hinstLib) {
    std::cout << "--Error! " << dlerror() << std::endl;
    return false;
  }
  return true;
}

bool LinuxLib::Close() {
  bool retVal = (0 == dlclose(_hinstLib);
  _hinstLib = nullptr;
  return retVal;
}