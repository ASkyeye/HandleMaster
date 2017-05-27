#include "windefs.h"
#include <stdio.h>
#include <stdlib.h>
#include <VersionHelpers.h>
#include "process.hpp"

BOOL IsWindowsVersion(WORD wMajorVersion, WORD wMinorVersion, WORD wServicePackMajor)
{
  OSVERSIONINFOEXW osvi = { sizeof(osvi), 0, 0, 0, 0,{ 0 }, 0, 0 };
  DWORDLONG dwlConditionMask = 
    VerSetConditionMask(
      VerSetConditionMask(VerSetConditionMask(0, VER_MAJORVERSION, VER_EQUAL), VER_MINORVERSION, VER_EQUAL),
      VER_SERVICEPACKMAJOR, VER_EQUAL);

  osvi.dwMajorVersion = wMajorVersion;
  osvi.dwMinorVersion = wMinorVersion;
  osvi.wServicePackMajor = wServicePackMajor;

  return VerifyVersionInfoW(&osvi, VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR, dwlConditionMask) != FALSE;
}

void EnsureWindowsVersion()
{
  DWORD dwMajor;
  DWORD dwMinor;
  WORD wSPMajor;

  OSVERSIONINFOEXW osvi = { sizeof(OSVERSIONINFOEXW), 0, 0, 0, 0,{ 0 }, 0, 0 };
  GetVersionExW((LPOSVERSIONINFOW)&osvi);

#if defined(WIN7_SP1)
  dwMajor = HIBYTE(_WIN32_WINNT_WIN7);
  dwMinor = LOBYTE(_WIN32_WINNT_WIN7);
  wSPMajor = 1;
#elif defined(WIN8_BLUE)
  dwMajor = HIBYTE(_WIN32_WINNT_WINBLUE);
  dwMinor = LOBYTE(_WIN32_WINNT_WINBLUE);
  wSPMajor = 0;
#elif defined(WIN10)
  dwMajor = HIBYTE(_WIN32_WINNT_WIN10);
  dwMinor = LOBYTE(_WIN32_WINNT_WIN10);
  wSPMajor = 0;
#elif defined(WIN10_AU)
  dwMajor = HIBYTE(0x0A00);
  dwMinor = LOBYTE(0x0A00);
  wSPMajor = 0;
#elif defined(WIN10_CU)
  dwMajor = HIBYTE(0x0A00);
  dwMinor = LOBYTE(0x0A00);
  wSPMajor = 0;
#endif
  if(osvi.dwMajorVersion != dwMajor ||
     osvi.dwMinorVersion != dwMinor ||
     osvi.wServicePackMajor != wSPMajor) {
    char buffer[512];
    sprintf_s(buffer,
              "Unsupported OS version.Expected: %d %d %d. Got: %d %d %d",
              dwMajor, dwMinor, wSPMajor, 
              osvi.dwMajorVersion, osvi.dwMinorVersion, osvi.wServicePackMajor);
    throw std::runtime_error(std::string(buffer));
  }
}

int main()
{
  try {
    EnsureWindowsVersion();

    //auto pid = process::find(L"notepad++.exe");
    //
    //if(!pid)
    //  throw std::runtime_error("Process not running");
    //
    //// 
    //// Open a handle WITHOUT read access, as proof of concept
    //// 
    //auto handle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    //
    //if(!handle)
    //  throw std::runtime_error("Failed to open process");
    //
    //// 
    //// Attach to the process that contains the handle we 
    //// want to elevate (this is the current process on this case)
    //// 
    //if(process::attach(GetCurrentProcessId())) {
    //  // 
    //  // Use CPU-Z to elevate the handle access to PROCESS_ALL_ACCESS
    //  // 
    //  if(!process::grant_handle_access(handle, PROCESS_ALL_ACCESS))
    //    throw std::runtime_error("Failed to set handle access");
    //
    //  process::detach();
    //}
    //
    //// 
    //// Use the now elevated handle to perform a query and some reads.
    //// You can use this handle for pretty much anything you want from now on. :)
    //// 
    //ULONG return_len;
    //PEB   process_peb;
    //PROCESS_BASIC_INFORMATION process_info;
    //RTL_USER_PROCESS_PARAMETERS process_parameters;
    //WCHAR buffer[512];
    //
    //if(NtQueryInformationProcess(handle, ProcessBasicInformation, &process_info, sizeof(process_info), &return_len) < 0)
    //  throw std::runtime_error("NtQueryInformationProcess failed");
    //
    //if(!ReadProcessMemory(handle, process_info.PebBaseAddress, &process_peb, sizeof(process_peb), nullptr) ||
    //   !ReadProcessMemory(handle, process_peb.ProcessParameters, &process_parameters, sizeof(process_parameters), nullptr) ||
    //   !ReadProcessMemory(handle, process_parameters.CommandLine.Buffer, buffer, process_parameters.CommandLine.Length, nullptr))
    //  throw std::runtime_error("ReadProcessMemory failed");
    //
    //printf("CommandLine: %ws\n", buffer);
    //
    //CloseHandle(handle);
  } catch(const std::exception& ex) {
    fprintf(stderr, "%s\n", ex.what());
    fprintf(stderr, "GetLastError: %X\n", GetLastError());
  }

  getc(stdin);
  return 0;
}