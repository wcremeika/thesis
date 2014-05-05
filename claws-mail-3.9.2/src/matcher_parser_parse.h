/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     MATCHER_ALL = 258,
     MATCHER_UNREAD = 259,
     MATCHER_NOT_UNREAD = 260,
     MATCHER_NEW = 261,
     MATCHER_NOT_NEW = 262,
     MATCHER_MARKED = 263,
     MATCHER_NOT_MARKED = 264,
     MATCHER_DELETED = 265,
     MATCHER_NOT_DELETED = 266,
     MATCHER_REPLIED = 267,
     MATCHER_NOT_REPLIED = 268,
     MATCHER_FORWARDED = 269,
     MATCHER_NOT_FORWARDED = 270,
     MATCHER_SUBJECT = 271,
     MATCHER_NOT_SUBJECT = 272,
     MATCHER_FROM = 273,
     MATCHER_NOT_FROM = 274,
     MATCHER_TO = 275,
     MATCHER_NOT_TO = 276,
     MATCHER_CC = 277,
     MATCHER_NOT_CC = 278,
     MATCHER_TO_OR_CC = 279,
     MATCHER_NOT_TO_AND_NOT_CC = 280,
     MATCHER_AGE_GREATER = 281,
     MATCHER_AGE_LOWER = 282,
     MATCHER_NEWSGROUPS = 283,
     MATCHER_AGE_GREATER_HOURS = 284,
     MATCHER_AGE_LOWER_HOURS = 285,
     MATCHER_NOT_NEWSGROUPS = 286,
     MATCHER_INREPLYTO = 287,
     MATCHER_NOT_INREPLYTO = 288,
     MATCHER_REFERENCES = 289,
     MATCHER_NOT_REFERENCES = 290,
     MATCHER_SCORE_GREATER = 291,
     MATCHER_SCORE_LOWER = 292,
     MATCHER_HEADER = 293,
     MATCHER_NOT_HEADER = 294,
     MATCHER_HEADERS_PART = 295,
     MATCHER_NOT_HEADERS_PART = 296,
     MATCHER_MESSAGE = 297,
     MATCHER_NOT_MESSAGE = 298,
     MATCHER_BODY_PART = 299,
     MATCHER_NOT_BODY_PART = 300,
     MATCHER_TEST = 301,
     MATCHER_NOT_TEST = 302,
     MATCHER_MATCHCASE = 303,
     MATCHER_MATCH = 304,
     MATCHER_REGEXPCASE = 305,
     MATCHER_REGEXP = 306,
     MATCHER_SCORE = 307,
     MATCHER_MOVE = 308,
     MATCHER_FOUND_IN_ADDRESSBOOK = 309,
     MATCHER_NOT_FOUND_IN_ADDRESSBOOK = 310,
     MATCHER_IN = 311,
     MATCHER_COPY = 312,
     MATCHER_DELETE = 313,
     MATCHER_MARK = 314,
     MATCHER_UNMARK = 315,
     MATCHER_LOCK = 316,
     MATCHER_UNLOCK = 317,
     MATCHER_EXECUTE = 318,
     MATCHER_MARK_AS_READ = 319,
     MATCHER_MARK_AS_UNREAD = 320,
     MATCHER_FORWARD = 321,
     MATCHER_MARK_AS_SPAM = 322,
     MATCHER_MARK_AS_HAM = 323,
     MATCHER_FORWARD_AS_ATTACHMENT = 324,
     MATCHER_EOL = 325,
     MATCHER_OR = 326,
     MATCHER_AND = 327,
     MATCHER_COLOR = 328,
     MATCHER_SCORE_EQUAL = 329,
     MATCHER_REDIRECT = 330,
     MATCHER_SIZE_GREATER = 331,
     MATCHER_SIZE_SMALLER = 332,
     MATCHER_SIZE_EQUAL = 333,
     MATCHER_LOCKED = 334,
     MATCHER_NOT_LOCKED = 335,
     MATCHER_PARTIAL = 336,
     MATCHER_NOT_PARTIAL = 337,
     MATCHER_COLORLABEL = 338,
     MATCHER_NOT_COLORLABEL = 339,
     MATCHER_IGNORE_THREAD = 340,
     MATCHER_NOT_IGNORE_THREAD = 341,
     MATCHER_WATCH_THREAD = 342,
     MATCHER_NOT_WATCH_THREAD = 343,
     MATCHER_CHANGE_SCORE = 344,
     MATCHER_SET_SCORE = 345,
     MATCHER_ADD_TO_ADDRESSBOOK = 346,
     MATCHER_STOP = 347,
     MATCHER_HIDE = 348,
     MATCHER_IGNORE = 349,
     MATCHER_WATCH = 350,
     MATCHER_SPAM = 351,
     MATCHER_NOT_SPAM = 352,
     MATCHER_HAS_ATTACHMENT = 353,
     MATCHER_HAS_NO_ATTACHMENT = 354,
     MATCHER_SIGNED = 355,
     MATCHER_NOT_SIGNED = 356,
     MATCHER_TAG = 357,
     MATCHER_NOT_TAG = 358,
     MATCHER_SET_TAG = 359,
     MATCHER_UNSET_TAG = 360,
     MATCHER_TAGGED = 361,
     MATCHER_NOT_TAGGED = 362,
     MATCHER_CLEAR_TAGS = 363,
     MATCHER_ENABLED = 364,
     MATCHER_DISABLED = 365,
     MATCHER_RULENAME = 366,
     MATCHER_ACCOUNT = 367,
     MATCHER_STRING = 368,
     MATCHER_SECTION = 369,
     MATCHER_INTEGER = 370
   };
#endif
/* Tokens.  */
#define MATCHER_ALL 258
#define MATCHER_UNREAD 259
#define MATCHER_NOT_UNREAD 260
#define MATCHER_NEW 261
#define MATCHER_NOT_NEW 262
#define MATCHER_MARKED 263
#define MATCHER_NOT_MARKED 264
#define MATCHER_DELETED 265
#define MATCHER_NOT_DELETED 266
#define MATCHER_REPLIED 267
#define MATCHER_NOT_REPLIED 268
#define MATCHER_FORWARDED 269
#define MATCHER_NOT_FORWARDED 270
#define MATCHER_SUBJECT 271
#define MATCHER_NOT_SUBJECT 272
#define MATCHER_FROM 273
#define MATCHER_NOT_FROM 274
#define MATCHER_TO 275
#define MATCHER_NOT_TO 276
#define MATCHER_CC 277
#define MATCHER_NOT_CC 278
#define MATCHER_TO_OR_CC 279
#define MATCHER_NOT_TO_AND_NOT_CC 280
#define MATCHER_AGE_GREATER 281
#define MATCHER_AGE_LOWER 282
#define MATCHER_NEWSGROUPS 283
#define MATCHER_AGE_GREATER_HOURS 284
#define MATCHER_AGE_LOWER_HOURS 285
#define MATCHER_NOT_NEWSGROUPS 286
#define MATCHER_INREPLYTO 287
#define MATCHER_NOT_INREPLYTO 288
#define MATCHER_REFERENCES 289
#define MATCHER_NOT_REFERENCES 290
#define MATCHER_SCORE_GREATER 291
#define MATCHER_SCORE_LOWER 292
#define MATCHER_HEADER 293
#define MATCHER_NOT_HEADER 294
#define MATCHER_HEADERS_PART 295
#define MATCHER_NOT_HEADERS_PART 296
#define MATCHER_MESSAGE 297
#define MATCHER_NOT_MESSAGE 298
#define MATCHER_BODY_PART 299
#define MATCHER_NOT_BODY_PART 300
#define MATCHER_TEST 301
#define MATCHER_NOT_TEST 302
#define MATCHER_MATCHCASE 303
#define MATCHER_MATCH 304
#define MATCHER_REGEXPCASE 305
#define MATCHER_REGEXP 306
#define MATCHER_SCORE 307
#define MATCHER_MOVE 308
#define MATCHER_FOUND_IN_ADDRESSBOOK 309
#define MATCHER_NOT_FOUND_IN_ADDRESSBOOK 310
#define MATCHER_IN 311
#define MATCHER_COPY 312
#define MATCHER_DELETE 313
#define MATCHER_MARK 314
#define MATCHER_UNMARK 315
#define MATCHER_LOCK 316
#define MATCHER_UNLOCK 317
#define MATCHER_EXECUTE 318
#define MATCHER_MARK_AS_READ 319
#define MATCHER_MARK_AS_UNREAD 320
#define MATCHER_FORWARD 321
#define MATCHER_MARK_AS_SPAM 322
#define MATCHER_MARK_AS_HAM 323
#define MATCHER_FORWARD_AS_ATTACHMENT 324
#define MATCHER_EOL 325
#define MATCHER_OR 326
#define MATCHER_AND 327
#define MATCHER_COLOR 328
#define MATCHER_SCORE_EQUAL 329
#define MATCHER_REDIRECT 330
#define MATCHER_SIZE_GREATER 331
#define MATCHER_SIZE_SMALLER 332
#define MATCHER_SIZE_EQUAL 333
#define MATCHER_LOCKED 334
#define MATCHER_NOT_LOCKED 335
#define MATCHER_PARTIAL 336
#define MATCHER_NOT_PARTIAL 337
#define MATCHER_COLORLABEL 338
#define MATCHER_NOT_COLORLABEL 339
#define MATCHER_IGNORE_THREAD 340
#define MATCHER_NOT_IGNORE_THREAD 341
#define MATCHER_WATCH_THREAD 342
#define MATCHER_NOT_WATCH_THREAD 343
#define MATCHER_CHANGE_SCORE 344
#define MATCHER_SET_SCORE 345
#define MATCHER_ADD_TO_ADDRESSBOOK 346
#define MATCHER_STOP 347
#define MATCHER_HIDE 348
#define MATCHER_IGNORE 349
#define MATCHER_WATCH 350
#define MATCHER_SPAM 351
#define MATCHER_NOT_SPAM 352
#define MATCHER_HAS_ATTACHMENT 353
#define MATCHER_HAS_NO_ATTACHMENT 354
#define MATCHER_SIGNED 355
#define MATCHER_NOT_SIGNED 356
#define MATCHER_TAG 357
#define MATCHER_NOT_TAG 358
#define MATCHER_SET_TAG 359
#define MATCHER_UNSET_TAG 360
#define MATCHER_TAGGED 361
#define MATCHER_NOT_TAGGED 362
#define MATCHER_CLEAR_TAGS 363
#define MATCHER_ENABLED 364
#define MATCHER_DISABLED 365
#define MATCHER_RULENAME 366
#define MATCHER_ACCOUNT 367
#define MATCHER_STRING 368
#define MATCHER_SECTION 369
#define MATCHER_INTEGER 370




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2068 of yacc.c  */
#line 310 "matcher_parser_parse.y"

	char *str;
	int value;



/* Line 2068 of yacc.c  */
#line 287 "matcher_parser_parse.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


