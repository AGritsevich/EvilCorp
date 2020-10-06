#pragma once

#include "lib_proxy.h"
 
class LinuxLib : public LibProxy {
  LinuxLib();
  ~LinuxLib() override;

  void* library_function(const std::string& name) override;

  bool open(const std::string& lib_path) override;

  bool close() override;
};
