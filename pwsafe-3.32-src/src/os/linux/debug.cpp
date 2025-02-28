/*
* Copyright (c) 2003-2013 Rony Shapiro <ronys@users.sourceforge.net>.
* All rights reserved. Use of the code is allowed under the
* Artistic License 2.0 terms, as specified in the LICENSE file
* distributed with this code, or available from
* http://www.opensource.org/licenses/artistic-license-2.0.php
*/

#include "../debug.h"
#include "../fmtspecs_cvt.h"
#include "../core/Util.h"

#if defined(_DEBUG) || defined(DEBUG)

// TRACE replacement - only need this Debug mode
#include <stdio.h>
#include <stdarg.h>
#include <syslog.h>



// Debug output - Same usage as MFC TRACE
void pws_os::Trace(LPCTSTR lpszFormat, ...)
{
  openlog("pwsafe:", LOG_PID|LOG_PERROR, LOG_USER);
  va_list args;
  va_start(args, lpszFormat);

  int num_required, num_written;

#ifdef UNICODE
  stringT format = FormatStr(lpszFormat);
  num_required = GetStringBufSize(format.c_str(), args);
  va_end(args);//after using args we should reset list
  va_start(args, lpszFormat);

  wchar_t *wcbuffer = new wchar_t[num_required];
  num_written = vswprintf(wcbuffer, num_required, format.c_str(), args);
  assert(num_required == num_written+1);
  wcbuffer[num_required-1] = L'\0';

  size_t N = wcstombs(NULL, wcbuffer, 0) + 1;
  char *szbuffer = new char[N];
  wcstombs(szbuffer, wcbuffer, N);
  delete[] wcbuffer;
#else
  num_required = GetStringBufSize(lpszFormat, args);
  va_end(args);//after using args we should reset list
  va_start(args, lpszFormat);

  char *szbuffer = new char[num_required];
  num_written = vsnprintf(szbuffer, num_required, lpszFormat, args);
  assert(num_required == num_written+1);
  szbuffer[num_required-1] = '\0';
#endif
  syslog(LOG_DEBUG, "%s", szbuffer);

  delete[] szbuffer;
  closelog();

  va_end(args);
}

void pws_os::Trace0(LPCTSTR lpszFormat)
{
  openlog("pwsafe:", LOG_PID|LOG_PERROR, LOG_USER);

#ifdef UNICODE
  size_t N = wcstombs(NULL, lpszFormat, 0) + 1;
  char *szbuffer = new char[N];
  wcstombs(szbuffer, lpszFormat, N);

  syslog(LOG_DEBUG, "%s", szbuffer);

  delete[] szbuffer;
#else
  syslog(LOG_DEBUG, "%s", lpszFormat);
#endif

  closelog();
}
#else   /* _DEBUG || DEBUG */
void pws_os::Trace(LPCTSTR , ...)
{
//  Do nothing in non-Debug mode
}
void pws_os::Trace0(LPCTSTR )
{
//  Do nothing in non-Debug mode
}
#endif  /* _DEBUG || DEBUG */

#if defined(_DEBUG) || defined(DEBUG)
#include "../../core/StringX.h"
#include <iostream>
#include "../pws_tchar.h"

// This routine uses Windows functions
void pws_os::IssueError(const stringT &csFunction, bool bMsgBox)
{
  // Stub?
  if (bMsgBox)
    std::cout << csFunction.c_str();
  else
    std::cerr << csFunction.c_str();
}

void pws_os::HexDump(unsigned char *pmemory, const int &length,
                       const stringT &cs_prefix, const int &maxnum)
{
  TCHAR szBuffer[256];
  unsigned char *pmem;
  stringT cs_outbuff, cs_hexbuff, cs_charbuff;
  int i, j, len(length);
  unsigned char c;

  pmem = pmemory;
  while (len > 0) {
    // Show offset for this line.
    cs_charbuff.clear();
    cs_hexbuff.clear();
    Format(cs_outbuff, _T("%s: %08x *"), cs_prefix.c_str(), pmem);

    // Format hex portion of line and save chars for ascii portion
    if (len > maxnum)
      j = maxnum;
    else
      j = len;

    for (i = 0; i < j; i++) {
      c = *pmem++;

      if ((i % 4) == 0 && i != 0)
        cs_outbuff += _T(' ');

      Format(cs_hexbuff, _T("%02x"), c);
      cs_outbuff += cs_hexbuff;

      if (c >= 32 && c < 127)
        cs_charbuff += static_cast<TCHAR>(c);
      else
        cs_charbuff += _T('.');
    }

    j = maxnum - j;

    // Fill out hex portion of short lines.
    for (i = j; i > 0; i--) {
      if ((i % 4) != 0)
        cs_outbuff += _T("  ");
      else
        cs_outbuff += _T("   ");
    }

    // Add ASCII character portion to line.
    cs_outbuff += _T("* |");
    cs_outbuff += cs_charbuff;

    // Fill out end of short lines.
    for (i = j; i > 0; i--)
      cs_outbuff += _T(' ');

    cs_outbuff += _T('|');

    // Next line
    len -= maxnum;

    _stprintf_s(szBuffer, sizeof(szBuffer) / sizeof(TCHAR),
                _T("%s\n"), cs_outbuff.c_str());
    Trace0(szBuffer);
  };
}
#else  /* _DEBUG or DEBUG */
void pws_os::IssueError(const stringT &, bool )
{
}

void pws_os::HexDump(unsigned char *, const int &,
                     const stringT &, const int & )
{
}
#endif /* _DEBUG or DEBUG */
