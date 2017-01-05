#include "lib_proxy.h"

//#else // WIN32
class WinLib : public LibProxy {
public:
  WinLib();
  ~WinLib() override;

  void* LibraryFunction(std::string name) override;

  bool Open(std::string lib_path) override;

  bool Close() override;

private:
  HINSTANCE _hinstLib;
};
#endif // WIN32