/*
* Copyright (c) 2003-2013 Rony Shapiro <ronys@users.sourceforge.net>.
* All rights reserved. Use of the code is allowed under the
* Artistic License 2.0 terms, as specified in the LICENSE file
* distributed with this code, or available from
* http://www.opensource.org/licenses/artistic-license-2.0.php
*/

#ifndef __PWSFILE_H
#define __PWSFILE_H

// PWSfile.h
// Abstract the gory details of reading and writing an encrypted database
//-----------------------------------------------------------------------------

#include <stdio.h> // for FILE *
#include <vector>

#include "ItemData.h"
#include "os/UUID.h"
#include "UnknownField.h"
#include "StringX.h"
#include "Proxy.h"
#include "sha256.h"

#include "coredefs.h"

// HASH_ITERATIONS is used by the key stretching algorithm.
// MIN_HASH_ITERATIONS is a lower limit - anything lower than this
// is considered inherently insecure.
#define MIN_HASH_ITERATIONS 2048
// MAX_USABLE_HASH_ITERS is a guesstimate on what's acceptable to a user
// with a reasonably powerful CPU. Real limit's 2^32-1.
#define MAX_USABLE_HASH_ITERS (1 << 20)

#define DEFAULT_SUFFIX      _T("psafe3")

class Fish;
class Asker;

class PWSfile
{
public:
  enum VERSION {V17, V20, V30, VCURRENT = V30,
    NEWFILE = 98,
    UNKNOWN_VERSION = 99}; // supported file versions: V17 is last pre-2.0

  enum RWmode {Read, Write};

  enum {SUCCESS = 0, FAILURE = 1, 
    UNSUPPORTED_VERSION,                     //  2
    WRONG_VERSION,                           //  3
    NOT_PWS3_FILE,                           //  4
    WRONG_PASSWORD,                          //  5 - see PWScore.h
    BAD_DIGEST,                              //  6 - see PWScore.h
    END_OF_FILE,                             //  7
    TRUNCATED_FILE,                          //  8 (missing EOF marker)
    READ_FAIL,                               //  9
    WRITE_FAIL,                              //  10
    CANT_OPEN_FILE = -10                     //  -10 - see PWScore.h
  };

  /**
  * The format defines a handful of fields in the file's header
  * Since the application needs these after the PWSfile object's
  * lifetime, it makes sense to define a nested header structure that
  * the app. can keep a copy of, rather than duplicating
  * data members, getters and setters willy-nilly.
  */
  struct HeaderRecord {
    HeaderRecord();
    HeaderRecord(const HeaderRecord &hdr);
    HeaderRecord &operator =(const HeaderRecord &hdr);
    ~HeaderRecord();
    unsigned short m_nCurrentMajorVersion, m_nCurrentMinorVersion;
    pws_os::CUUID m_file_uuid;         // Unique DB ID
    std::vector<bool> m_displaystatus; // Tree expansion state vector
    StringX m_prefString;              // Prefererences stored in the file
    time_t m_whenlastsaved; // When last saved
    StringX m_lastsavedby; // and by whom
    StringX m_lastsavedon; // and by which machine
    StringX m_whatlastsaved; // and by what application
    StringX m_dbname, m_dbdesc;        // Descriptive name, Description
    UUIDList m_RUEList;
    unsigned char *m_yubi_sk;  // YubiKey HMAC key, added in 0x030a / 3.27Y
    enum {YUBI_SK_LEN = 20};
  };

  static PWSfile *MakePWSfile(const StringX &a_filename, VERSION &version,
                              RWmode mode, int &status, 
                              Asker *pAsker = NULL, Reporter *pReporter = NULL);

  static VERSION ReadVersion(const StringX &filename);
  static int CheckPasskey(const StringX &filename,
                          const StringX &passkey, VERSION &version);

  // Following for 'legacy' use of pwsafe as file encryptor/decryptor
  static bool Encrypt(const stringT &fn, const StringX &passwd, stringT &errmess);
  static bool Decrypt(const stringT &fn, const StringX &passwd, stringT &errmess);

  virtual ~PWSfile();

  virtual int Open(const StringX &passkey) = 0;
  virtual int Close();

  virtual int WriteRecord(const CItemData &item) = 0;
  virtual int ReadRecord(CItemData &item) = 0;

  const HeaderRecord &GetHeader() const {return m_hdr;}
  void SetHeader(const HeaderRecord &h) {m_hdr = h;}

  void SetDefUsername(const StringX &du) {m_defusername = du;} // for V17 conversion (read) only
  void SetCurVersion(VERSION v) {m_curversion = v;}
  void GetUnknownHeaderFields(UnknownFieldList &UHFL);
  void SetUnknownHeaderFields(UnknownFieldList &UHFL);
  int GetNumRecordsWithUnknownFields() const
  {return m_nRecordsWithUnknownFields;}
  
  size_t WriteField(unsigned char type,
                    const StringX &data) {return WriteCBC(type, data);}
  size_t WriteField(unsigned char type,
                    const unsigned char *data,
                    size_t length) {return WriteCBC(type, data, length);}
  size_t ReadField(unsigned char &type,
                   unsigned char* &data,
                   size_t &length) {return ReadCBC(type, data, length);}
  
protected:
  PWSfile(const StringX &filename, RWmode mode);
  void FOpen(); // calls right variant of m_fd = fopen(m_filename);
  virtual size_t WriteCBC(unsigned char type, const StringX &data) = 0;
  virtual size_t WriteCBC(unsigned char type, const unsigned char *data,
                          size_t length);
  virtual size_t ReadCBC(unsigned char &type, unsigned char* &data,
                         size_t &length);
  const StringX m_filename;
  StringX m_passkey;
  FILE *m_fd;
  VERSION m_curversion;
  const RWmode m_rw;
  StringX m_defusername; // for V17 conversion (read) only
  unsigned char *m_IV; // points to correct m_ipthing for *CBC()
  Fish *m_fish;
  unsigned char *m_terminal;
  HeaderRecord m_hdr;
  // Save unknown header fields on read to put back on write unchanged
  UnknownFieldList m_UHFL;
  int m_nRecordsWithUnknownFields;
  size_t m_fileLength;
  Asker *m_pAsker;
  Reporter *m_pReporter;

private:
  PWSfile& operator=(const PWSfile&); // Do not implement
};

// A quick way to determine if two files are equal,
// or if a given file has been modified. For large files,
// this may miss changes made to the middle. This is due
// to a performance trade-off.
class PWSFileSig
{
public:
  PWSFileSig(const stringT &fname);
  PWSFileSig(const PWSFileSig &pfs);
  PWSFileSig &operator=(const PWSFileSig &that);

  bool IsValid() {return m_iErrorCode == PWSfile::SUCCESS;}
  int GetErrorCode() {return m_iErrorCode;}

  bool operator==(const PWSFileSig &that);
  bool operator!=(const PWSFileSig &that) {return !(*this == that);}

private:
  long m_length; // -1 if file doesn't exist or zero length
  unsigned char m_digest[SHA256::HASHLEN];
  int m_iErrorCode;
};
#endif /* __PWSFILE_H */
