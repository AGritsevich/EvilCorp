#pragma once
#include <string>

class LibProxy {
public:
  LibProxy() {};
  virtual ~LibProxy() {};

  virtual bool Open(std::string lib_path) =0;
  virtual bool Close() =0;
  virtual void* LibraryFunction(std::string name) =0;
};
