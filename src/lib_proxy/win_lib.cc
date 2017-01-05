#include <iostream>
#include <string>
#include <stdint.h>
#include <windows.h>

#include "win_lib.h"

WinLib::WinLib() :
  _hinstLib(nullptr) {};

WinLib::~WinLib() { 
  Close(); 
};

void* WinLib::LibraryFunction(std::string name) {
  if (_hinstLib != NULL) {
    void* ProcAdd = GetProcAddress(_hinstLib, name.c_str());

    if (NULL != ProcAdd) {
      //(ProcAdd) (L"Message sent to the DLL function\n"); 
      return ProcAdd;
    }
  }
  return nullptr;
}

bool WinLib::Open(std::string lib_path) {
  _hinstLib = LoadLibrary(lib_path.c_str());
  
  if (_hinstLib == NULL) {
    DWORD   dwLastError = ::GetLastError();
    if (dwLastError != 0)
      std::cout << "Error open library! Error code: " << dwLastError << std::endl;

    return false;
  }
  return true;
}

bool WinLib::Close() {
  bool retVal = (TRUE == FreeLibrary(_hinstLib));
  _hinstLib = nullptr;
  return retVal;
}