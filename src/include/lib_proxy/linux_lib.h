#include "lib_proxy.h"

//#ifdef __linux__ 
class LinuxLib : public LibProxy {
  LinuxLib();
  ~LinuxLib() override;

  void* LibraryFunction(std::string name) override;

  bool Open(std::string lib_path) override;

  bool Close() override;

private:
  void* _hinstLib;
};
