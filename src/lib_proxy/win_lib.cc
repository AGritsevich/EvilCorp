#include <iostream>
#include <string>
#include <stdint.h>
#include <windows.h>

#include "lib_proxy/win_lib.h"

WinLib::WinLib(){};

WinLib::~WinLib() { 
  close(); 
};

void* WinLib::library_function(const std::string& name) {
  if (m_hinstLib != NULL) {
    void* ProcAdd = GetProcAddress(static_cast<HMODULE>(m_hinstLib), name.c_str());

    if (NULL != ProcAdd) {
      //(ProcAdd) (L"Message sent to the DLL function\n"); 
      return ProcAdd;
    }
  }
  return nullptr;
}

bool WinLib::open(const std::string& lib_path) {
  m_hinstLib = LoadLibrary(lib_path.c_str());
  
  if (m_hinstLib == nullptr) {
    DWORD   dwLastError = ::GetLastError();
    if (dwLastError != 0)
      std::cout << "Error open library! Error code: " << dwLastError << std::endl;

    return false;
  }
  return true;
}

bool WinLib::close() {
  bool retVal = (TRUE == FreeLibrary(static_cast<HMODULE>(m_hinstLib)));
  m_hinstLib = nullptr;
  return retVal;
}