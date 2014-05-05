/*
* Copyright (c) 2003-2013 Rony Shapiro <ronys@users.sourceforge.net>.
* All rights reserved. Use of the code is allowed under the
* Artistic License 2.0 terms, as specified in the LICENSE file
* distributed with this code, or available from
* http://www.opensource.org/licenses/artistic-license-2.0.php
*/

// AdvancedValues.h : header file
//

#pragma once

#include "core/ItemData.h"
#include <bitset>

namespace WZAdvanced {
  enum AdvType {INVALID = -1,
                COMPARE = 0,
                MERGE,
                SYNCH,
                EXPORT_TEXT,
                EXPORT_ENTRYTEXT,
                EXPORT_XML,
                EXPORT_ENTRYXML,
                LAST};
}

struct st_SaveAdvValues {
  st_SaveAdvValues()
  : subgroup_name(L""),
    subgroup_bset(false), subgroup_bcase(false), 
    btreatwhitespaceasempty(true),
    subgroup_object(0), subgroup_function(0)
  {
    bsFields.set();
  }

  st_SaveAdvValues(const st_SaveAdvValues &adv)
    : bsFields(adv.bsFields), subgroup_name(adv.subgroup_name),
    subgroup_bset(adv.subgroup_bset), subgroup_object(adv.subgroup_object),
    subgroup_function(adv.subgroup_function), subgroup_bcase(adv.subgroup_bcase),
    btreatwhitespaceasempty(adv.btreatwhitespaceasempty)
  {
  }

  st_SaveAdvValues &operator =(const st_SaveAdvValues &adv)
  {
    if (this != &adv) {
      bsFields = adv.bsFields;
      subgroup_name = adv.subgroup_name;
      subgroup_bset = adv.subgroup_bset;
      subgroup_object = adv.subgroup_object;
      subgroup_function = adv.subgroup_function;
      subgroup_bcase = adv.subgroup_bcase;
      btreatwhitespaceasempty = adv.btreatwhitespaceasempty;
    }
    return *this;
  }

  void Clear() {
    bsFields.set();
    subgroup_bset = subgroup_bcase = BST_UNCHECKED;
    btreatwhitespaceasempty = BST_CHECKED;
    subgroup_object = subgroup_function = 0;
    subgroup_name = L"";
  }

  CItemData::FieldBits bsFields;
  std::wstring subgroup_name;
  int subgroup_object, subgroup_function;
  bool subgroup_bset, subgroup_bcase, btreatwhitespaceasempty;
};
