#include <iostream>
#include <string>
#include <stdint.h>
#include <dlfcn.h>

#include "linux_lib.h"

LinuxLib::LinuxLib{}

LinuxLib::~LinuxLib() {
  close();
}

void* LinuxLib::library_function(const std::string& name) {
  if (m_hinstLib != nullptr) {
    void* funptr = dlsym(m_hinstLib, name.c_str());
    char error* = nullptr;
    if ((error = dlerror()) != NULL) {
      std::cout << "--Error! " << dlerror() << std::endl;
      return nullptr;
    }
    return funptr;
  }
  return nullptr;
}

bool LinuxLib::open(const std::string& lib_path) {
  m_hinstLib = dlopen(lib_path.c_str(), RTLD_LAZY);

  if (!m_hinstLib) {
    std::cout << "--Error! " << dlerror() << std::endl;
    return false;
  }
  return true;
}

bool LinuxLib::close() {
  bool retVal = (0 == dlclose(m_hinstLib);
  m_hinstLib = nullptr;
  return retVal;
}