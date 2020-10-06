#pragma once

#include "lib_proxy.h"

class WinLib : public LibProxy {
public:
  WinLib();
  ~WinLib() override;

  void* library_function(const std::string& name) override;

  bool open(const std::string& lib_path) override;

  bool close() override;
};