/* -*- Mode: C -*- */
/*======================================================================
  FILE: icalvalue.h
  CREATOR: eric 20 March 1999


  $Id$
  $Locker$

  

 (C) COPYRIGHT 2000, Eric Busboom, http://www.softwarestudio.org

 This program is free software; you can redistribute it and/or modify
 it under the terms of either: 

    The LGPL as published by the Free Software Foundation, version
    2.1, available at: http://www.fsf.org/copyleft/lesser.html

  Or:

    The Mozilla Public License Version 1.0. You may obtain a copy of
    the License at http://www.mozilla.org/MPL/

  The original code is icalvalue.h

  ======================================================================*/

#ifndef ICALDERIVEDVALUE_H
#define ICALDERIVEDVALUE_H

#include "icaltypes.h"
#include "icalrecur.h"
#include "icaltime.h"
#include "icalduration.h"
#include "icalperiod.h"
     
typedef void icalvalue;



void icalvalue_set_x(icalvalue* value, const char* v);
icalvalue* icalvalue_new_x(const char* v);
const char* icalvalue_get_x(icalvalue* value);

icalvalue* icalvalue_new_recur (struct icalrecurrencetype v);
void icalvalue_set_recur(icalvalue* value, struct icalrecurrencetype v);
struct icalrecurrencetype icalvalue_get_recur(icalvalue* value);

icalvalue* icalvalue_new_trigger (struct icaltriggertype v);
void icalvalue_set_trigger(icalvalue* value, struct icaltriggertype v);
struct icaltriggertype icalvalue_get_trigger(icalvalue* value);

icalvalue* icalvalue_new_datetimeperiod (struct icaldatetimeperiodtype v);
void icalvalue_set_datetimeperiod(icalvalue* value, struct icaldatetimeperiodtype v);
struct icaldatetimeperiodtype icalvalue_get_datetimeperiod(icalvalue* value);

/* Everything below this line is machine generated. Do not edit. */
typedef enum icalvalue_kind {
   ICAL_ANY_VALUE=5000,
    ICAL_QUERY_VALUE=5001,
    ICAL_DATE_VALUE=5002,
    ICAL_ATTACH_VALUE=5003,
    ICAL_GEO_VALUE=5004,
    ICAL_DATETIMEDATE_VALUE=5005,
    ICAL_STATUS_VALUE=5006,
    ICAL_TRANSP_VALUE=5007,
    ICAL_STRING_VALUE=5008,
    ICAL_TEXT_VALUE=5009,
    ICAL_BINARY_VALUE=5010,
    ICAL_FLOAT_VALUE=5011,
    ICAL_PERIOD_VALUE=5012,
    ICAL_DATETIMEPERIOD_VALUE=5013,
    ICAL_INTEGER_VALUE=5014,
    ICAL_CLASS_VALUE=5015,
    ICAL_URI_VALUE=5016,
    ICAL_TIME_VALUE=5017,
    ICAL_DURATION_VALUE=5018,
    ICAL_BOOLEAN_VALUE=5019,
    ICAL_X_VALUE=5020,
    ICAL_CALADDRESS_VALUE=5021,
    ICAL_TRIGGER_VALUE=5022,
    ICAL_RECUR_VALUE=5023,
    ICAL_ACTION_VALUE=5024,
    ICAL_DATETIME_VALUE=5025,
    ICAL_UTCOFFSET_VALUE=5026,
    ICAL_METHOD_VALUE=5027,
   ICAL_NO_VALUE=5028
} icalvalue_kind ;

#define ICALPROPERTY_FIRST_ENUM 10000

typedef enum icalproperty_action {
    ICAL_ACTION_X = 10000,
    ICAL_ACTION_AUDIO = 10001,
    ICAL_ACTION_DISPLAY = 10002,
    ICAL_ACTION_EMAIL = 10003,
    ICAL_ACTION_PROCEDURE = 10004,
    ICAL_ACTION_NONE = 10005
} icalproperty_action;

typedef enum icalproperty_class {
    ICAL_CLASS_X = 10006,
    ICAL_CLASS_PUBLIC = 10007,
    ICAL_CLASS_PRIVATE = 10008,
    ICAL_CLASS_CONFIDENTIAL = 10009,
    ICAL_CLASS_NONE = 10010
} icalproperty_class;

typedef enum icalproperty_method {
    ICAL_METHOD_X = 10011,
    ICAL_METHOD_PUBLISH = 10012,
    ICAL_METHOD_REQUEST = 10013,
    ICAL_METHOD_REPLY = 10014,
    ICAL_METHOD_ADD = 10015,
    ICAL_METHOD_CANCEL = 10016,
    ICAL_METHOD_REFRESH = 10017,
    ICAL_METHOD_COUNTER = 10018,
    ICAL_METHOD_DECLINECOUNTER = 10019,
    ICAL_METHOD_CREATE = 10020,
    ICAL_METHOD_READ = 10021,
    ICAL_METHOD_RESPONSE = 10022,
    ICAL_METHOD_MOVE = 10023,
    ICAL_METHOD_MODIFY = 10024,
    ICAL_METHOD_GENERATEUID = 10025,
    ICAL_METHOD_DELETE = 10026,
    ICAL_METHOD_NONE = 10027
} icalproperty_method;

typedef enum icalproperty_status {
    ICAL_STATUS_X = 10028,
    ICAL_STATUS_TENTATIVE = 10029,
    ICAL_STATUS_CONFIRMED = 10030,
    ICAL_STATUS_COMPLETED = 10031,
    ICAL_STATUS_NEEDSACTION = 10032,
    ICAL_STATUS_CANCELLED = 10033,
    ICAL_STATUS_INPROCESS = 10034,
    ICAL_STATUS_DRAFT = 10035,
    ICAL_STATUS_FINAL = 10036,
    ICAL_STATUS_NONE = 10037
} icalproperty_status;

typedef enum icalproperty_transp {
    ICAL_TRANSP_X = 10038,
    ICAL_TRANSP_OPAQUE = 10039,
    ICAL_TRANSP_TRANSPARENT = 10040,
    ICAL_TRANSP_NONE = 10041
} icalproperty_transp;

#define ICALPROPERTY_LAST_ENUM 10042


 /* QUERY */ 
icalvalue* icalvalue_new_query(const char* v); 
const char* icalvalue_get_query(icalvalue* value); 
void icalvalue_set_query(icalvalue* value, const char* v);


 /* DATE */ 
icalvalue* icalvalue_new_date(struct icaltimetype v); 
struct icaltimetype icalvalue_get_date(icalvalue* value); 
void icalvalue_set_date(icalvalue* value, struct icaltimetype v);


 /* ATTACH */ 
icalvalue* icalvalue_new_attach(struct icalattachtype v); 
struct icalattachtype icalvalue_get_attach(icalvalue* value); 
void icalvalue_set_attach(icalvalue* value, struct icalattachtype v);


 /* GEO */ 
icalvalue* icalvalue_new_geo(struct icalgeotype v); 
struct icalgeotype icalvalue_get_geo(icalvalue* value); 
void icalvalue_set_geo(icalvalue* value, struct icalgeotype v);


 /* DATE-TIME-DATE */ 
icalvalue* icalvalue_new_datetimedate(struct icaltimetype v); 
struct icaltimetype icalvalue_get_datetimedate(icalvalue* value); 
void icalvalue_set_datetimedate(icalvalue* value, struct icaltimetype v);


 /* STATUS */ 
icalvalue* icalvalue_new_status(enum icalproperty_status v); 
enum icalproperty_status icalvalue_get_status(icalvalue* value); 
void icalvalue_set_status(icalvalue* value, enum icalproperty_status v);


 /* TRANSP */ 
icalvalue* icalvalue_new_transp(enum icalproperty_transp v); 
enum icalproperty_transp icalvalue_get_transp(icalvalue* value); 
void icalvalue_set_transp(icalvalue* value, enum icalproperty_transp v);


 /* STRING */ 
icalvalue* icalvalue_new_string(const char* v); 
const char* icalvalue_get_string(icalvalue* value); 
void icalvalue_set_string(icalvalue* value, const char* v);


 /* TEXT */ 
icalvalue* icalvalue_new_text(const char* v); 
const char* icalvalue_get_text(icalvalue* value); 
void icalvalue_set_text(icalvalue* value, const char* v);


 /* BINARY */ 
icalvalue* icalvalue_new_binary(const char* v); 
const char* icalvalue_get_binary(icalvalue* value); 
void icalvalue_set_binary(icalvalue* value, const char* v);


 /* FLOAT */ 
icalvalue* icalvalue_new_float(float v); 
float icalvalue_get_float(icalvalue* value); 
void icalvalue_set_float(icalvalue* value, float v);


 /* PERIOD */ 
icalvalue* icalvalue_new_period(struct icalperiodtype v); 
struct icalperiodtype icalvalue_get_period(icalvalue* value); 
void icalvalue_set_period(icalvalue* value, struct icalperiodtype v);


 /* INTEGER */ 
icalvalue* icalvalue_new_integer(int v); 
int icalvalue_get_integer(icalvalue* value); 
void icalvalue_set_integer(icalvalue* value, int v);


 /* CLASS */ 
icalvalue* icalvalue_new_class(enum icalproperty_class v); 
enum icalproperty_class icalvalue_get_class(icalvalue* value); 
void icalvalue_set_class(icalvalue* value, enum icalproperty_class v);


 /* URI */ 
icalvalue* icalvalue_new_uri(const char* v); 
const char* icalvalue_get_uri(icalvalue* value); 
void icalvalue_set_uri(icalvalue* value, const char* v);


 /* TIME */ 
icalvalue* icalvalue_new_time(struct icaltimetype v); 
struct icaltimetype icalvalue_get_time(icalvalue* value); 
void icalvalue_set_time(icalvalue* value, struct icaltimetype v);


 /* DURATION */ 
icalvalue* icalvalue_new_duration(struct icaldurationtype v); 
struct icaldurationtype icalvalue_get_duration(icalvalue* value); 
void icalvalue_set_duration(icalvalue* value, struct icaldurationtype v);


 /* BOOLEAN */ 
icalvalue* icalvalue_new_boolean(int v); 
int icalvalue_get_boolean(icalvalue* value); 
void icalvalue_set_boolean(icalvalue* value, int v);


 /* CAL-ADDRESS */ 
icalvalue* icalvalue_new_caladdress(const char* v); 
const char* icalvalue_get_caladdress(icalvalue* value); 
void icalvalue_set_caladdress(icalvalue* value, const char* v);


 /* ACTION */ 
icalvalue* icalvalue_new_action(enum icalproperty_action v); 
enum icalproperty_action icalvalue_get_action(icalvalue* value); 
void icalvalue_set_action(icalvalue* value, enum icalproperty_action v);


 /* DATE-TIME */ 
icalvalue* icalvalue_new_datetime(struct icaltimetype v); 
struct icaltimetype icalvalue_get_datetime(icalvalue* value); 
void icalvalue_set_datetime(icalvalue* value, struct icaltimetype v);


 /* UTC-OFFSET */ 
icalvalue* icalvalue_new_utcoffset(int v); 
int icalvalue_get_utcoffset(icalvalue* value); 
void icalvalue_set_utcoffset(icalvalue* value, int v);


 /* METHOD */ 
icalvalue* icalvalue_new_method(enum icalproperty_method v); 
enum icalproperty_method icalvalue_get_method(icalvalue* value); 
void icalvalue_set_method(icalvalue* value, enum icalproperty_method v);

#endif /*ICALVALUE_H*/
