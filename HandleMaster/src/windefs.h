#pragma once

#define WIN32_LEAN_AND_MEAN
#define NO_STRICT
#define NOMINMAX
#include <ntstatus.h>
#define WIN32_NO_STATUS
#include <Windows.h>
#include <winternl.h>

#ifndef _WIN64
#error "x64 only please"
#endif

typedef struct _EPROCESS *PEPROCESS;
typedef struct _HANDLE_TABLE_ENTRY_INFO *PHANDLE_TABLE_ENTRY_INFO;
typedef PVOID EX_PUSH_LOCK;

#define KPROCESS_DIRBASE   0x028
#define EPROCESS_PID       0x180
#define EPROCESS_LINKS     0x188
#define EPROCESS_OBJ_TABLE 0x200

typedef struct _HANDLE_TABLE_ENTRY
{
  union
  {
    PVOID Object;
    ULONG ObAttributes;
    PHANDLE_TABLE_ENTRY_INFO InfoTable;
    ULONGLONG Value;
  };
  union
  {
    ULONGLONG GrantedAccess;
    USHORT GrantedAccessIndex;
    ULONG NextFreeTableEntry;
  };
} HANDLE_TABLE_ENTRY, *PHANDLE_TABLE_ENTRY;

typedef struct _HANDLE_TABLE
{
  ULONGLONG TableCode;
  PEPROCESS QuotaProcess;
  PVOID UniqueProcessId;
  EX_PUSH_LOCK HandleLock;
  LIST_ENTRY HandleTableList;
  EX_PUSH_LOCK HandleContentionEvent;
  PVOID DebugInfo;
  LONG ExtraInfoPages;
  union
  {
    ULONG Flags;
    struct
    {
      UCHAR StrictFIFO : 1;
    };
  };
  ULONG FirstFreeHandle;
  PHANDLE_TABLE_ENTRY LastFreeHandleEntry;
  ULONG HandleCount;
  ULONG NextHandleNeedingPool;
  ULONG HandleCountHighWatermark;
} HANDLE_TABLE, *PHANDLE_TABLE;

//#define SYSTEM_DIRBASE     0x1a7000
//#define KPROCESS_DIRBASE   0x028
//#define EPROCESS_PID       0x2e0
//#define EPROCESS_LINKS     0x2e8
//#define EPROCESS_OBJ_TABLE 0x408

//typedef struct _HANDLE_TABLE_ENTRY
//{
//  union
//  {
//    PVOID Object;
//    ULONG ObAttributes;
//    PHANDLE_TABLE_ENTRY_INFO InfoTable;
//    ULONGLONG Value;
//  };
//  union
//  {
//    ULONG_PTR HighValue;
//    struct _HANDLE_TABLE_ENTRY* NextFreeHandleEntry;
//    ULONGLONG LeafHandleValue;
//    struct
//    {
//      ULONG GrantedAccess : 25;
//      ULONG NoRightsUpgrade : 1;
//      ULONG Spare : 6;
//    };
//  };
//  ULONG TypeInfo;
//} HANDLE_TABLE_ENTRY, *PHANDLE_TABLE_ENTRY;
//
//typedef struct _HANDLE_TABLE
//{
//  ULONG NextHandleNeedingPool;
//  LONG ExtraInfoPages;
//  LONG_PTR TableCode;
//  PEPROCESS QuotaProcess;
//  LIST_ENTRY HandleTableList;
//  ULONG UniqueProcessId;
//  ULONG Flags;
//  EX_PUSH_LOCK HandleContentionEvent;
//  EX_PUSH_LOCK HandleTableLock;
//} HANDLE_TABLE, *PHANDLE_TABLE;
