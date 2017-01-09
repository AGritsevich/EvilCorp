#pragma once
#include <string>

class LibProxy {
public:
  LibProxy() : m_hinstLib(nullptr) {};
  virtual ~LibProxy() {};

  virtual bool open(const std::string& lib_path) =0;
  virtual bool close() =0;
  virtual void* library_function(const std::string& name) =0;

protected:
  void* m_hinstLib;
};
