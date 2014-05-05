/*
* Copyright (c) 2003-2013 Rony Shapiro <ronys@users.sourceforge.net>.
* All rights reserved. Use of the code is allowed under the
* Artistic License 2.0 terms, as specified in the LICENSE file
* distributed with this code, or available from
* http://www.opensource.org/licenses/artistic-license-2.0.php
*/

/**
 * \file Windows-specific implementation of mem.h
 */

#include "../typedefs.h"
#include "../mem.h"
#include "../lib.h"

#include <Wincrypt.h>
#pragma comment(lib, "crypt32.lib")

bool pws_os::mlock(void *p, size_t size)
{
  return VirtualLock(p, size) != 0;
}

bool pws_os::munlock(void *p, size_t size)
{
  return VirtualUnlock(p, size) != 0;
}

typedef BOOL (WINAPI *LP_CryptProtectMemory)(LPVOID pDataIn, DWORD cbDataIn, DWORD dwFlags);

bool pws_os::mcryptProtect(void *p, size_t size)
{
  ASSERT((size % CRYPTPROTECTMEMORY_BLOCK_SIZE) == 0);
  bool res = true;
  //CryptProtectMemory available only in Vista+, so we need to check and load it manually 
  HINSTANCE hCRYPT32 = HINSTANCE(pws_os::LoadLibrary(_T("crypt32.dll"),
                                                     pws_os::LOAD_LIBRARY_SYS));
  if (hCRYPT32) {
     LP_CryptProtectMemory protectPtr = (LP_CryptProtectMemory)GetProcAddress(hCRYPT32, "CryptProtectMemory");
     if (protectPtr)
       res = (protectPtr(p, size, CRYPTPROTECTMEMORY_SAME_PROCESS) == TRUE);
     FreeLibrary(hCRYPT32);
  }
  return res;
}

bool pws_os::mcryptUnprotect(void *p, size_t size)
{
  ASSERT((size % CRYPTPROTECTMEMORY_BLOCK_SIZE) == 0);
   bool res = true;
  //CryptProtectMemory available only in Vista+, so we need to check and load it manually 
   HINSTANCE hCRYPT32 = HINSTANCE(pws_os::LoadLibrary(_T("crypt32.dll"),
                                                      pws_os::LOAD_LIBRARY_SYS));
  if (hCRYPT32) {
     LP_CryptProtectMemory unprotectPtr =
       LP_CryptProtectMemory(pws_os::GetFunction(hCRYPT32, "CryptUnprotectMemory"));
     if (unprotectPtr)
       res = (unprotectPtr(p, size, CRYPTPROTECTMEMORY_SAME_PROCESS) == TRUE);
     pws_os::FreeLibrary(hCRYPT32);
  }
  return res;
}
