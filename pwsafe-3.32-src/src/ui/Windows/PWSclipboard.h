/*
* Copyright (c) 2003-2013 Rony Shapiro <ronys@users.sourceforge.net>.
* All rights reserved. Use of the code is allowed under the
* Artistic License 2.0 terms, as specified in the LICENSE file
* distributed with this code, or available from
* http://www.opensource.org/licenses/artistic-license-2.0.php
*/

#ifndef __PWSCLIPBOARD_H
#define __PWSCLIPBOARD_H

/** \file
* A small utility class to handle the clipboard
* securely. Specifically, we keep a hash
* of the data that we put on the clipboard, so that
* ClearData() only clears the clipboard if it has what we put on it, and
* if isSensitive was true when we added it.
*/

#include "core/sha256.h"

#if defined(UNICODE)
#define CLIPBOARD_TEXT_FORMAT CF_UNICODETEXT
#else
#define CLIPBOARD_TEXT_FORMAT CF_TEXT
#endif

#include "core/StringX.h"

class PWSclipboard
{
public:
  PWSclipboard();
  ~PWSclipboard();

  bool SetData(const StringX &data,
    bool isSensitive = true,
    CLIPFORMAT cfFormat = CLIPBOARD_TEXT_FORMAT);
  // returns true if succeeded
  bool ClearData(); // return true if cleared or if data wasn't ours

private:
  PWSclipboard(const PWSclipboard &); // don't even THINK of implementing this!
  PWSclipboard &operator=(const PWSclipboard &); // ditto!
  
  bool m_set;
  unsigned char m_digest[SHA256::HASHLEN];
};

#endif /* __PWSCLIPBOARD_H */
