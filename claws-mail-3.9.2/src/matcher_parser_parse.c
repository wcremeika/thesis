/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 1 "matcher_parser_parse.y"

/*
 * Sylpheed -- a GTK+ based, lightweight, and fast e-mail client
 * Copyright (c) 2001-2007 by Hiroyuki Yamamoto & The Claws Mail Team
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include "defs.h"

#include <glib.h>
#include <glib/gi18n.h>

#include "utils.h"
#include "filtering.h"
#include "matcher.h"
#include "matcher_parser.h"
#include "matcher_parser_lex.h"
#include "colorlabel.h"
#include "folder_item_prefs.h"

static gint error = 0;
static gint bool_op = 0;
static gint match_type = 0;
static gchar *header = NULL;

static MatcherProp *prop;

static GSList *matchers_list = NULL;

static gboolean enabled = TRUE;
static gchar *name = NULL;
static gint account_id = 0;
static MatcherList *cond;
static GSList *action_list = NULL;
static FilteringAction *action = NULL;
static gboolean matcher_is_fast = TRUE;
static gboolean disable_warnings = FALSE;

static FilteringProp *filtering;

static GSList **prefs_filtering = NULL;
static int enable_compatibility = 0;

enum {
        MATCHER_PARSE_FILE,
        MATCHER_PARSE_NO_EOL,
	MATCHER_PARSE_ENABLED,
	MATCHER_PARSE_NAME,
	MATCHER_PARSE_ACCOUNT,
        MATCHER_PARSE_CONDITION,
        MATCHER_PARSE_FILTERING_ACTION,
};

static int matcher_parse_op = MATCHER_PARSE_FILE;


/* ******************************************************************** */
/* redeclarations to avoid warnings */
void matcher_parserrestart(FILE *input_file);
void matcher_parser_init(void);
void matcher_parser_switch_to_buffer(void * new_buffer);
void matcher_parser_delete_buffer(void * b);
void matcher_parserpop_buffer_state(void);
int matcher_parserlex(void);

void matcher_parser_disable_warnings(const gboolean disable)
{
	disable_warnings = disable;
}

void matcher_parser_start_parsing(FILE *f)
{
	matcher_parserlineno = 1;
	matcher_parserrestart(f);
	account_id = 0;
	matcher_parserparse();
}

 
void * matcher_parser_scan_string(const char * str);
 
FilteringProp *matcher_parser_get_filtering(gchar *str)
{
	void *bufstate;
	void *tmp_str = NULL;
	
	/* little hack to allow passing rules with no names */
	if (!strncmp(str, "rulename ", 9))
		tmp_str = g_strdup(str);
	else 
		tmp_str = g_strconcat("rulename \"\" ", str, NULL);

	/* bad coding to enable the sub-grammar matching
	   in yacc */
	matcher_parserlineno = 1;
	matcher_parse_op = MATCHER_PARSE_NO_EOL;
	matcher_parserrestart(NULL);
	matcher_parserpop_buffer_state();
        matcher_parser_init();
	bufstate = matcher_parser_scan_string((const char *) tmp_str);
        matcher_parser_switch_to_buffer(bufstate);
	if (matcher_parserparse() != 0)
		filtering = NULL;
	matcher_parse_op = MATCHER_PARSE_FILE;
	matcher_parser_delete_buffer(bufstate);
	g_free(tmp_str);
	return filtering;
}

static gboolean check_quote_symetry(gchar *str)
{
	const gchar *walk;
	int ret = 0;
	
	if (str == NULL)
		return TRUE; /* heh, that's symetric */
	if (*str == '\0')
		return TRUE;
	for (walk = str; *walk; walk++) {
		if (*walk == '\"') {
			if (walk == str 	/* first char */
			|| *(walk - 1) != '\\') /* not escaped */
				ret ++;
		}
	}
	return !(ret % 2);
}

MatcherList *matcher_parser_get_name(gchar *str)
{
	void *bufstate;

	if (!check_quote_symetry(str)) {
		cond = NULL;
		return cond;
	}
	
	/* bad coding to enable the sub-grammar matching
	   in yacc */
	matcher_parserlineno = 1;
	matcher_parse_op = MATCHER_PARSE_NAME;
	matcher_parserrestart(NULL);
	matcher_parserpop_buffer_state();
        matcher_parser_init();
	bufstate = matcher_parser_scan_string(str);
	matcher_parserparse();
	matcher_parse_op = MATCHER_PARSE_FILE;
	matcher_parser_delete_buffer(bufstate);
	return cond;
}

MatcherList *matcher_parser_get_enabled(gchar *str)
{
	void *bufstate;

	if (!check_quote_symetry(str)) {
		cond = NULL;
		return cond;
	}
	
	/* bad coding to enable the sub-grammar matching
	   in yacc */
	matcher_parserlineno = 1;
	matcher_parse_op = MATCHER_PARSE_ENABLED;
	matcher_parserrestart(NULL);
	matcher_parserpop_buffer_state();
	matcher_parser_init();
	bufstate = matcher_parser_scan_string(str);
	matcher_parserparse();
	matcher_parse_op = MATCHER_PARSE_FILE;
	matcher_parser_delete_buffer(bufstate);
	return cond;
}

MatcherList *matcher_parser_get_account(gchar *str)
{
	void *bufstate;

	if (!check_quote_symetry(str)) {
		cond = NULL;
		return cond;
	}
	
	/* bad coding to enable the sub-grammar matching
	   in yacc */
	matcher_parserlineno = 1;
	matcher_parse_op = MATCHER_PARSE_ACCOUNT;
	matcher_parserrestart(NULL);
	matcher_parserpop_buffer_state();
	matcher_parser_init();
	bufstate = matcher_parser_scan_string(str);
	matcher_parserparse();
	matcher_parse_op = MATCHER_PARSE_FILE;
	matcher_parser_delete_buffer(bufstate);
	return cond;
}

MatcherList *matcher_parser_get_cond(gchar *str, gboolean *is_fast)
{
	void *bufstate;

	if (!check_quote_symetry(str)) {
		cond = NULL;
		return cond;
	}
	
	matcher_is_fast = TRUE;
	/* bad coding to enable the sub-grammar matching
	   in yacc */
	matcher_parserlineno = 1;
	matcher_parse_op = MATCHER_PARSE_CONDITION;
	matcher_parserrestart(NULL);
	matcher_parserpop_buffer_state();
        matcher_parser_init();
	bufstate = matcher_parser_scan_string(str);
	matcher_parserparse();
	matcher_parse_op = MATCHER_PARSE_FILE;
	matcher_parser_delete_buffer(bufstate);
	if (is_fast)
		*is_fast = matcher_is_fast;
	return cond;
}

GSList *matcher_parser_get_action_list(gchar *str)
{
	void *bufstate;

	if (!check_quote_symetry(str)) {
		action_list = NULL;
		return action_list;
	}
	
	/* bad coding to enable the sub-grammar matching
	   in yacc */
	matcher_parserlineno = 1;
	matcher_parse_op = MATCHER_PARSE_FILTERING_ACTION;
	matcher_parserrestart(NULL);
	matcher_parserpop_buffer_state();
        matcher_parser_init();
	bufstate = matcher_parser_scan_string(str);
	matcher_parserparse();
	matcher_parse_op = MATCHER_PARSE_FILE;
	matcher_parser_delete_buffer(bufstate);
	return action_list;
}

MatcherProp *matcher_parser_get_prop(gchar *str)
{
	MatcherList *list;
	MatcherProp *prop;

	matcher_parserlineno = 1;
	list = matcher_parser_get_cond(str, NULL);
	if (list == NULL)
		return NULL;

	if (list->matchers == NULL)
		return NULL;

	if (list->matchers->next != NULL)
		return NULL;

	prop = list->matchers->data;

	g_slist_free(list->matchers);
	g_free(list);

	return prop;
}

void matcher_parsererror(char *str)
{
	GSList *l;

	if (matchers_list) {
		for (l = matchers_list; l != NULL; l = g_slist_next(l)) {
			matcherprop_free((MatcherProp *)
					 l->data);
			l->data = NULL;
		}
		g_slist_free(matchers_list);
		matchers_list = NULL;
	}
	cond = NULL;
	if (!disable_warnings)
		g_warning("filtering parsing: %i: %s\n",
		  	matcher_parserlineno, str);
	error = 1;
}

int matcher_parserwrap(void)
{
	return 1;
}


/* Line 268 of yacc.c  */
#line 381 "matcher_parser_parse.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


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

/* Line 293 of yacc.c  */
#line 310 "matcher_parser_parse.y"

	char *str;
	int value;



/* Line 293 of yacc.c  */
#line 654 "matcher_parser_parse.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 666 "matcher_parser_parse.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   718

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  116
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  25
/* YYNRULES -- Number of rules.  */
#define YYNRULES  142
/* YYNRULES -- Number of states.  */
#define YYNSTATES  245

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   370

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     7,    10,    12,    14,    15,    18,
      21,    24,    31,    37,    43,    48,    53,    57,    59,    61,
      63,    65,    67,    69,    71,    73,    76,    79,    81,    84,
      86,    88,    90,    92,    94,    96,    98,   102,   104,   106,
     108,   110,   112,   114,   116,   118,   120,   122,   124,   126,
     128,   130,   132,   134,   136,   138,   140,   142,   144,   146,
     148,   150,   152,   154,   157,   160,   162,   164,   166,   168,
     172,   176,   180,   184,   188,   192,   196,   200,   204,   208,
     212,   216,   218,   220,   223,   226,   229,   232,   236,   240,
     244,   248,   252,   256,   259,   262,   265,   268,   271,   274,
     275,   281,   282,   288,   292,   296,   297,   303,   304,   310,
     314,   318,   322,   326,   329,   332,   335,   338,   341,   344,
     346,   349,   351,   353,   355,   357,   359,   361,   363,   365,
     367,   371,   375,   379,   382,   385,   388,   391,   393,   395,
     397,   398,   403
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     117,     0,    -1,    -1,   118,   119,    -1,   120,   119,    -1,
     120,    -1,   122,    -1,    -1,   121,   123,    -1,     1,    70,
      -1,   114,    70,    -1,   124,   125,   126,   131,   127,    70,
      -1,   124,   125,   126,   131,   127,    -1,   124,   125,   131,
     127,    70,    -1,   124,   125,   131,   127,    -1,   125,   131,
     127,    70,    -1,   125,   131,   127,    -1,   124,    -1,   126,
      -1,   125,    -1,   131,    -1,   128,    -1,    70,    -1,   109,
      -1,   110,    -1,   111,   113,    -1,   112,   115,    -1,   128,
      -1,   129,   128,    -1,   129,    -1,   139,    -1,    48,    -1,
      49,    -1,    50,    -1,    51,    -1,   132,    -1,   132,   133,
     134,    -1,   134,    -1,    72,    -1,    71,    -1,     3,    -1,
       4,    -1,     5,    -1,     6,    -1,     7,    -1,     8,    -1,
       9,    -1,    10,    -1,    11,    -1,    12,    -1,    13,    -1,
      14,    -1,    15,    -1,    79,    -1,    80,    -1,    96,    -1,
      97,    -1,    98,    -1,    99,    -1,   100,    -1,   101,    -1,
      81,    -1,    82,    -1,    83,   115,    -1,    84,   115,    -1,
      85,    -1,    86,    -1,    87,    -1,    88,    -1,    16,   130,
     113,    -1,    17,   130,   113,    -1,    18,   130,   113,    -1,
      19,   130,   113,    -1,    20,   130,   113,    -1,    21,   130,
     113,    -1,    22,   130,   113,    -1,    23,   130,   113,    -1,
      24,   130,   113,    -1,    25,   130,   113,    -1,   102,   130,
     113,    -1,   103,   130,   113,    -1,   106,    -1,   107,    -1,
      26,   115,    -1,    27,   115,    -1,    29,   115,    -1,    30,
     115,    -1,    28,   130,   113,    -1,    31,   130,   113,    -1,
      32,   130,   113,    -1,    33,   130,   113,    -1,    34,   130,
     113,    -1,    35,   130,   113,    -1,    36,   115,    -1,    37,
     115,    -1,    74,   115,    -1,    76,   115,    -1,    77,   115,
      -1,    78,   115,    -1,    -1,    38,   113,   135,   130,   113,
      -1,    -1,    39,   113,   136,   130,   113,    -1,    40,   130,
     113,    -1,    41,   130,   113,    -1,    -1,    54,   113,   137,
      56,   113,    -1,    -1,    55,   113,   138,    56,   113,    -1,
      42,   130,   113,    -1,    43,   130,   113,    -1,    44,   130,
     113,    -1,    45,   130,   113,    -1,    46,   113,    -1,    47,
     113,    -1,    63,   113,    -1,    53,   113,    -1,   104,   113,
      -1,   105,   113,    -1,   108,    -1,    57,   113,    -1,    58,
      -1,    59,    -1,    60,    -1,    61,    -1,    62,    -1,    64,
      -1,    65,    -1,    67,    -1,    68,    -1,    66,   115,   113,
      -1,    69,   115,   113,    -1,    75,   115,   113,    -1,    73,
     115,    -1,    89,   115,    -1,    52,   115,    -1,    90,   115,
      -1,    93,    -1,    94,    -1,    95,    -1,    -1,    91,   113,
     140,   113,    -1,    92,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   366,   366,   366,   374,   376,   380,   382,   382,   384,
     390,   423,   424,   425,   426,   427,   428,   437,   446,   455,
     464,   473,   482,   486,   490,   497,   504,   511,   543,   544,
     548,   556,   560,   564,   568,   575,   583,   587,   595,   599,
     606,   613,   620,   627,   634,   641,   648,   655,   662,   669,
     676,   683,   690,   697,   704,   711,   718,   725,   732,   739,
     746,   753,   760,   767,   778,   789,   796,   803,   810,   817,
     826,   835,   844,   853,   862,   871,   880,   889,   898,   907,
     916,   925,   932,   939,   948,   957,   966,   975,   984,   993,
    1002,  1011,  1020,  1029,  1038,  1047,  1056,  1064,  1072,  1081,
    1080,  1094,  1093,  1106,  1115,  1125,  1124,  1138,  1137,  1150,
    1159,  1168,  1177,  1186,  1195,  1207,  1216,  1225,  1234,  1243,
    1250,  1259,  1266,  1273,  1280,  1287,  1294,  1301,  1308,  1315,
    1322,  1334,  1346,  1358,  1367,  1376,  1384,  1392,  1396,  1400,
    1405,  1404,  1417
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "MATCHER_ALL", "MATCHER_UNREAD",
  "MATCHER_NOT_UNREAD", "MATCHER_NEW", "MATCHER_NOT_NEW", "MATCHER_MARKED",
  "MATCHER_NOT_MARKED", "MATCHER_DELETED", "MATCHER_NOT_DELETED",
  "MATCHER_REPLIED", "MATCHER_NOT_REPLIED", "MATCHER_FORWARDED",
  "MATCHER_NOT_FORWARDED", "MATCHER_SUBJECT", "MATCHER_NOT_SUBJECT",
  "MATCHER_FROM", "MATCHER_NOT_FROM", "MATCHER_TO", "MATCHER_NOT_TO",
  "MATCHER_CC", "MATCHER_NOT_CC", "MATCHER_TO_OR_CC",
  "MATCHER_NOT_TO_AND_NOT_CC", "MATCHER_AGE_GREATER", "MATCHER_AGE_LOWER",
  "MATCHER_NEWSGROUPS", "MATCHER_AGE_GREATER_HOURS",
  "MATCHER_AGE_LOWER_HOURS", "MATCHER_NOT_NEWSGROUPS", "MATCHER_INREPLYTO",
  "MATCHER_NOT_INREPLYTO", "MATCHER_REFERENCES", "MATCHER_NOT_REFERENCES",
  "MATCHER_SCORE_GREATER", "MATCHER_SCORE_LOWER", "MATCHER_HEADER",
  "MATCHER_NOT_HEADER", "MATCHER_HEADERS_PART", "MATCHER_NOT_HEADERS_PART",
  "MATCHER_MESSAGE", "MATCHER_NOT_MESSAGE", "MATCHER_BODY_PART",
  "MATCHER_NOT_BODY_PART", "MATCHER_TEST", "MATCHER_NOT_TEST",
  "MATCHER_MATCHCASE", "MATCHER_MATCH", "MATCHER_REGEXPCASE",
  "MATCHER_REGEXP", "MATCHER_SCORE", "MATCHER_MOVE",
  "MATCHER_FOUND_IN_ADDRESSBOOK", "MATCHER_NOT_FOUND_IN_ADDRESSBOOK",
  "MATCHER_IN", "MATCHER_COPY", "MATCHER_DELETE", "MATCHER_MARK",
  "MATCHER_UNMARK", "MATCHER_LOCK", "MATCHER_UNLOCK", "MATCHER_EXECUTE",
  "MATCHER_MARK_AS_READ", "MATCHER_MARK_AS_UNREAD", "MATCHER_FORWARD",
  "MATCHER_MARK_AS_SPAM", "MATCHER_MARK_AS_HAM",
  "MATCHER_FORWARD_AS_ATTACHMENT", "MATCHER_EOL", "MATCHER_OR",
  "MATCHER_AND", "MATCHER_COLOR", "MATCHER_SCORE_EQUAL",
  "MATCHER_REDIRECT", "MATCHER_SIZE_GREATER", "MATCHER_SIZE_SMALLER",
  "MATCHER_SIZE_EQUAL", "MATCHER_LOCKED", "MATCHER_NOT_LOCKED",
  "MATCHER_PARTIAL", "MATCHER_NOT_PARTIAL", "MATCHER_COLORLABEL",
  "MATCHER_NOT_COLORLABEL", "MATCHER_IGNORE_THREAD",
  "MATCHER_NOT_IGNORE_THREAD", "MATCHER_WATCH_THREAD",
  "MATCHER_NOT_WATCH_THREAD", "MATCHER_CHANGE_SCORE", "MATCHER_SET_SCORE",
  "MATCHER_ADD_TO_ADDRESSBOOK", "MATCHER_STOP", "MATCHER_HIDE",
  "MATCHER_IGNORE", "MATCHER_WATCH", "MATCHER_SPAM", "MATCHER_NOT_SPAM",
  "MATCHER_HAS_ATTACHMENT", "MATCHER_HAS_NO_ATTACHMENT", "MATCHER_SIGNED",
  "MATCHER_NOT_SIGNED", "MATCHER_TAG", "MATCHER_NOT_TAG",
  "MATCHER_SET_TAG", "MATCHER_UNSET_TAG", "MATCHER_TAGGED",
  "MATCHER_NOT_TAGGED", "MATCHER_CLEAR_TAGS", "MATCHER_ENABLED",
  "MATCHER_DISABLED", "MATCHER_RULENAME", "MATCHER_ACCOUNT",
  "MATCHER_STRING", "MATCHER_SECTION", "MATCHER_INTEGER", "$accept",
  "file", "$@1", "file_line_list", "file_line", "$@2",
  "section_notification", "instruction", "enabled", "name", "account",
  "filtering", "filtering_action_list", "filtering_action_b", "match_type",
  "condition", "condition_list", "bool_op", "one_condition", "$@3", "$@4",
  "$@5", "$@6", "filtering_action", "$@7", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   116,   118,   117,   119,   119,   120,   121,   120,   120,
     122,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   124,   124,   125,   126,   127,   128,   128,
     129,   130,   130,   130,   130,   131,   132,   132,   133,   133,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   135,
     134,   136,   134,   134,   134,   137,   134,   138,   134,   134,
     134,   134,   134,   134,   134,   139,   139,   139,   139,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   139,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   139,   139,
     140,   139,   139
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     1,     1,     0,     2,     2,
       2,     6,     5,     5,     4,     4,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     1,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     1,     2,     2,     2,     2,     3,     3,     3,
       3,     3,     3,     2,     2,     2,     2,     2,     2,     0,
       5,     0,     5,     3,     3,     0,     5,     0,     5,     3,
       3,     3,     3,     2,     2,     2,     2,     2,     2,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     2,     2,     2,     2,     1,     1,     1,
       0,     4,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     0,     1,     0,     0,     3,     0,     0,     6,
       9,    10,     4,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   121,   122,   123,   124,   125,     0,   126,
     127,     0,   128,   129,     0,    22,     0,     0,     0,     0,
       0,     0,    53,    54,    61,    62,     0,     0,    65,    66,
      67,    68,     0,     0,     0,   142,   137,   138,   139,    55,
      56,    57,    58,    59,    60,     0,     0,     0,     0,    81,
      82,   119,    23,    24,     0,     0,     8,    17,    19,    18,
      21,    29,    20,    35,    37,    30,    31,    32,    33,    34,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,     0,    85,    86,     0,     0,     0,     0,     0,
      93,    94,    99,   101,     0,     0,     0,     0,     0,     0,
     113,   114,   135,   116,   105,   107,   120,   115,     0,     0,
     133,    95,     0,    96,    97,    98,    63,    64,   134,   136,
     140,     0,     0,   117,   118,    25,    26,     0,     0,    28,
      39,    38,     0,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    87,    88,    89,    90,    91,    92,     0,
       0,   103,   104,   109,   110,   111,   112,     0,     0,   130,
     131,   132,     0,    79,    80,     0,     0,    16,    27,    36,
       0,     0,     0,     0,   141,     0,    14,    15,   100,   102,
     106,   108,    12,    13,    11
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,     6,     7,     8,     9,   116,   117,   118,
     119,   227,   228,   121,   130,   122,   123,   192,   124,   209,
     210,   217,   218,   125,   222
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -217
static const yytype_int16 yypact[] =
{
    -217,     9,   183,  -217,   -50,   -49,  -217,    29,   295,  -217,
    -217,  -217,  -217,  -217,  -217,  -217,  -217,  -217,  -217,  -217,
    -217,  -217,  -217,  -217,  -217,  -217,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    97,   -93,   -38,    97,   -35,
     -30,    97,    97,    97,    97,    97,   -15,   -14,    36,    37,
      97,    97,    97,    97,    97,    97,    38,    39,    27,    40,
      41,    42,    43,  -217,  -217,  -217,  -217,  -217,    44,  -217,
    -217,    45,  -217,  -217,    46,  -217,    47,    48,    49,    50,
      51,    52,  -217,  -217,  -217,  -217,    53,    54,  -217,  -217,
    -217,  -217,    55,    56,    59,  -217,  -217,  -217,  -217,  -217,
    -217,  -217,  -217,  -217,  -217,    97,    97,    60,    61,  -217,
    -217,  -217,  -217,  -217,    62,    63,  -217,    65,   558,  -217,
    -217,   610,  -217,   -59,  -217,  -217,  -217,  -217,  -217,  -217,
      64,    66,    67,    68,    72,   118,   119,   120,   121,   126,
    -217,  -217,   141,  -217,  -217,   142,   230,   231,   232,   233,
    -217,  -217,  -217,  -217,   238,   253,   254,   341,   342,   343,
    -217,  -217,  -217,  -217,  -217,  -217,  -217,  -217,   344,   345,
    -217,  -217,   348,  -217,  -217,  -217,  -217,  -217,  -217,  -217,
    -217,   349,   350,  -217,  -217,  -217,  -217,   405,   610,  -217,
    -217,  -217,   558,  -217,  -217,  -217,  -217,  -217,  -217,  -217,
    -217,  -217,  -217,  -217,  -217,  -217,  -217,  -217,  -217,    97,
      97,  -217,  -217,  -217,  -217,  -217,  -217,   102,   103,  -217,
    -217,  -217,   351,  -217,  -217,   558,   610,   226,  -217,  -217,
     352,   353,   354,   355,  -217,   610,   399,  -217,  -217,  -217,
    -217,  -217,   400,  -217,  -217
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -217,  -217,  -217,   464,  -217,  -217,  -217,  -217,  -217,   356,
     285,  -216,    23,  -217,   -27,   335,  -217,  -217,   282,  -217,
    -217,  -217,  -217,  -217,  -217
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -8
static const yytype_int16 yytable[] =
{
     131,   132,   133,   134,   135,   136,   137,   138,   139,     3,
     236,   142,   190,   191,   145,   146,   147,   148,   149,   242,
      10,    11,   140,   154,   155,   156,   157,   158,   159,    -5,
       4,   120,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,    -7,    -7,    -7,    -7,   141,   181,   182,
     143,    -7,    -7,    -7,    -7,   144,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
     150,   151,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,    -7,   162,     5,   189,   126,   127,   128,   129,   152,
     153,   160,   161,   163,   164,   165,   166,   167,   232,   233,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   183,   184,   185,   114,   193,   186,   194,
     195,   196,   230,   231,     4,   197,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,   198,   199,   200,   201,    -7,    -7,    -7,    -7,   202,
      -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,    -7,   203,   204,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,    -7,    -7,    -7,   237,     5,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,   205,   206,   207,   208,    58,    59,    60,
      61,   211,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,   212,   213,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,   188,   214,   215,   216,   219,   220,    60,
      61,   221,   223,   224,   234,   238,   239,   240,   241,   243,
     244,    12,   225,   187,   229,     0,     0,     0,     0,    77,
       0,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,     0,     0,     0,     0,     0,     0,
       0,    99,   100,   101,   102,   103,   104,   105,   106,     0,
       0,   109,   110,     0,     0,     0,     0,   115,     0,     0,
       0,     0,   226,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     235,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,     0,     0,     0,     0,
       0,     0,    60,    61,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    77,     0,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,     0,     0,     0,
       0,     0,     0,     0,    99,   100,   101,   102,   103,   104,
     105,   106,    58,    59,   109,   110,     0,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
       0,     0,     0,    76,     0,    78,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,    97,    98,     0,     0,     0,     0,
       0,     0,     0,     0,   107,   108,     0,     0,   111
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-217))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      27,    28,    29,    30,    31,    32,    33,    34,    35,     0,
     226,    38,    71,    72,    41,    42,    43,    44,    45,   235,
      70,    70,   115,    50,    51,    52,    53,    54,    55,     0,
       1,     8,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,   115,   105,   106,
     115,    52,    53,    54,    55,   115,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
     115,   115,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   115,   114,   121,    48,    49,    50,    51,   113,
     113,   113,   113,   113,   113,   113,   113,   113,    56,    56,
     115,   115,   115,   115,   115,   115,   115,   115,   115,   115,
     115,   115,   113,   113,   113,   113,   111,   113,   115,   113,
     113,   113,   209,   210,     1,   113,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,   113,   113,   113,   113,    52,    53,    54,    55,   113,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,   113,   113,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,    70,   114,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,   113,   113,   113,   113,    52,    53,    54,
      55,   113,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,   113,   113,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,   118,   113,   113,   113,   113,   113,    54,
      55,   113,   113,   113,   113,   113,   113,   113,   113,    70,
      70,     7,   187,   117,   192,    -1,    -1,    -1,    -1,    74,
      -1,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,   102,   103,    -1,
      -1,   106,   107,    -1,    -1,    -1,    -1,   112,    -1,    -1,
      -1,    -1,   187,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     225,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    74,    -1,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
     102,   103,    52,    53,   106,   107,    -1,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      -1,    -1,    -1,    73,    -1,    75,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      90,    91,    92,    93,    94,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,   105,    -1,    -1,   108
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   117,   118,     0,     1,   114,   119,   120,   121,   122,
      70,    70,   119,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    52,    53,
      54,    55,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   123,   124,   125,   126,
     128,   129,   131,   132,   134,   139,    48,    49,    50,    51,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     115,   115,   130,   115,   115,   130,   130,   130,   130,   130,
     115,   115,   113,   113,   130,   130,   130,   130,   130,   130,
     113,   113,   115,   113,   113,   113,   113,   113,   115,   115,
     115,   115,   115,   115,   115,   115,   115,   115,   115,   115,
     113,   130,   130,   113,   113,   113,   115,   125,   131,   128,
      71,    72,   133,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   113,   135,
     136,   113,   113,   113,   113,   113,   113,   137,   138,   113,
     113,   113,   140,   113,   113,   126,   131,   127,   128,   134,
     130,   130,    56,    56,   113,   131,   127,    70,   113,   113,
     113,   113,   127,    70,    70
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1806 of yacc.c  */
#line 366 "matcher_parser_parse.y"
    {
	if (matcher_parse_op == MATCHER_PARSE_FILE) {
		prefs_filtering = &pre_global_processing;
	}
}
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 382 "matcher_parser_parse.y"
    { action_list = NULL; }
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 385 "matcher_parser_parse.y"
    {
	yyerrok;
}
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 391 "matcher_parser_parse.y"
    {
	gchar *folder = (yyvsp[(1) - (2)].str);
	FolderItem *item = NULL;

	if (matcher_parse_op == MATCHER_PARSE_FILE) {
                enable_compatibility = 0;
		if (!strcmp(folder, "global")) {
                        /* backward compatibility */
                        enable_compatibility = 1;
                }
		else if (!strcmp(folder, "preglobal")) {
			prefs_filtering = &pre_global_processing;
                }
		else if (!strcmp(folder, "postglobal")) {
			prefs_filtering = &post_global_processing;
                }
		else if (!strcmp(folder, "filtering")) {
                        prefs_filtering = &filtering_rules;
		}
                else {
			item = folder_find_item_from_identifier(folder);
			if (item != NULL) {
				prefs_filtering = &item->prefs->processing;
			} else {
				prefs_filtering = NULL;
			}
		}
	}
}
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 429 "matcher_parser_parse.y"
    {
	if (matcher_parse_op == MATCHER_PARSE_NO_EOL)
		YYACCEPT;
	else {
		matcher_parsererror("parse error [no eol]");
		YYERROR;
	}
}
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 438 "matcher_parser_parse.y"
    {
	if (matcher_parse_op == MATCHER_PARSE_ENABLED)
		YYACCEPT;
	else {
		matcher_parsererror("parse error [enabled]");
		YYERROR;
	}
}
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 447 "matcher_parser_parse.y"
    {
	if (matcher_parse_op == MATCHER_PARSE_ACCOUNT)
		YYACCEPT;
	else {
		matcher_parsererror("parse error [account]");
		YYERROR;
	}
}
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 456 "matcher_parser_parse.y"
    {
	if (matcher_parse_op == MATCHER_PARSE_NAME)
		YYACCEPT;
	else {
		matcher_parsererror("parse error [name]");
		YYERROR;
	}
}
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 465 "matcher_parser_parse.y"
    {
	if (matcher_parse_op == MATCHER_PARSE_CONDITION)
		YYACCEPT;
	else {
		matcher_parsererror("parse error [condition]");
		YYERROR;
	}
}
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 474 "matcher_parser_parse.y"
    {
	if (matcher_parse_op == MATCHER_PARSE_FILTERING_ACTION)
		YYACCEPT;
	else {
		matcher_parsererror("parse error [filtering action]");
		YYERROR;
	}
}
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 487 "matcher_parser_parse.y"
    {
	enabled = TRUE;
}
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 491 "matcher_parser_parse.y"
    {
	enabled = FALSE;
}
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 498 "matcher_parser_parse.y"
    {
	name = g_strdup((yyvsp[(2) - (2)].str));
}
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 505 "matcher_parser_parse.y"
    {
	account_id = strtol((yyvsp[(2) - (2)].str), NULL, 10);
}
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 512 "matcher_parser_parse.y"
    {
	filtering = filteringprop_new(enabled, name, account_id, cond, action_list);
	enabled = TRUE;
	account_id = 0;
	g_free(name);
	name = NULL;
        if (enable_compatibility) {
                prefs_filtering = &filtering_rules;
                if (action_list != NULL) {
                        FilteringAction * first_action;
                        
                        first_action = action_list->data;
                        
                        if (first_action->type == MATCHACTION_CHANGE_SCORE)
                                prefs_filtering = &pre_global_processing;
                }
        }
        
	cond = NULL;
	action_list = NULL;
        
	if ((matcher_parse_op == MATCHER_PARSE_FILE) &&
            (prefs_filtering != NULL)) {
		*prefs_filtering = g_slist_append(*prefs_filtering,
						  filtering);
		filtering = NULL;
	}
}
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 549 "matcher_parser_parse.y"
    {
        action_list = g_slist_append(action_list, action);
        action = NULL;
}
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 557 "matcher_parser_parse.y"
    {
	match_type = MATCHTYPE_MATCHCASE;
}
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 561 "matcher_parser_parse.y"
    {
	match_type = MATCHTYPE_MATCH;
}
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 565 "matcher_parser_parse.y"
    {
	match_type = MATCHTYPE_REGEXPCASE;
}
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 569 "matcher_parser_parse.y"
    {
	match_type = MATCHTYPE_REGEXP;
}
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 576 "matcher_parser_parse.y"
    {
	cond = matcherlist_new(matchers_list, (bool_op == MATCHERBOOL_AND));
	matchers_list = NULL;
}
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 584 "matcher_parser_parse.y"
    {
	matchers_list = g_slist_append(matchers_list, prop);
}
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 588 "matcher_parser_parse.y"
    {
	matchers_list = NULL;
	matchers_list = g_slist_append(matchers_list, prop);
}
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 596 "matcher_parser_parse.y"
    {
	bool_op = MATCHERBOOL_AND;
}
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 600 "matcher_parser_parse.y"
    {
	bool_op = MATCHERBOOL_OR;
}
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 607 "matcher_parser_parse.y"
    {
	gint criteria = 0;

	criteria = MATCHCRITERIA_ALL;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 614 "matcher_parser_parse.y"
    {
	gint criteria = 0;

	criteria = MATCHCRITERIA_UNREAD;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 621 "matcher_parser_parse.y"
    {
	gint criteria = 0;

	criteria = MATCHCRITERIA_NOT_UNREAD;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 628 "matcher_parser_parse.y"
    {
	gint criteria = 0;

	criteria = MATCHCRITERIA_NEW;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 635 "matcher_parser_parse.y"
    {
	gint criteria = 0;

	criteria = MATCHCRITERIA_NOT_NEW;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 642 "matcher_parser_parse.y"
    {
	gint criteria = 0;

	criteria = MATCHCRITERIA_MARKED;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 649 "matcher_parser_parse.y"
    {
	gint criteria = 0;

	criteria = MATCHCRITERIA_NOT_MARKED;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 656 "matcher_parser_parse.y"
    {
	gint criteria = 0;

	criteria = MATCHCRITERIA_DELETED;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 663 "matcher_parser_parse.y"
    {
	gint criteria = 0;

	criteria = MATCHCRITERIA_NOT_DELETED;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 670 "matcher_parser_parse.y"
    {
	gint criteria = 0;

	criteria = MATCHCRITERIA_REPLIED;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 677 "matcher_parser_parse.y"
    {
	gint criteria = 0;

	criteria = MATCHCRITERIA_NOT_REPLIED;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 684 "matcher_parser_parse.y"
    {
	gint criteria = 0;

	criteria = MATCHCRITERIA_FORWARDED;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 691 "matcher_parser_parse.y"
    {
	gint criteria = 0;

	criteria = MATCHCRITERIA_NOT_FORWARDED;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 698 "matcher_parser_parse.y"
    {
	gint criteria = 0;

	criteria = MATCHCRITERIA_LOCKED;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 705 "matcher_parser_parse.y"
    {
	gint criteria = 0;

	criteria = MATCHCRITERIA_NOT_LOCKED;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 712 "matcher_parser_parse.y"
    {
	gint criteria = 0;

	criteria = MATCHCRITERIA_SPAM;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 719 "matcher_parser_parse.y"
    {
	gint criteria = 0;

	criteria = MATCHCRITERIA_NOT_SPAM;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 726 "matcher_parser_parse.y"
    {
	gint criteria = 0;

	criteria = MATCHCRITERIA_HAS_ATTACHMENT;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 733 "matcher_parser_parse.y"
    {
	gint criteria = 0;

	criteria = MATCHCRITERIA_HAS_NO_ATTACHMENT;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 740 "matcher_parser_parse.y"
    {
	gint criteria = 0;

	criteria = MATCHCRITERIA_SIGNED;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 747 "matcher_parser_parse.y"
    {
	gint criteria = 0;

	criteria = MATCHCRITERIA_NOT_SIGNED;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 754 "matcher_parser_parse.y"
    {
	gint criteria = 0;

	criteria = MATCHCRITERIA_PARTIAL;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 761 "matcher_parser_parse.y"
    {
	gint criteria = 0;

	criteria = MATCHCRITERIA_NOT_PARTIAL;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 768 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gint value = 0;

	criteria = MATCHCRITERIA_COLORLABEL;
	value = strtol((yyvsp[(2) - (2)].str), NULL, 10);
	if (value < 0) value = 0;
	else if (value > COLORLABELS) value = COLORLABELS;
	prop = matcherprop_new(criteria, NULL, 0, NULL, value);
}
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 779 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gint value = 0;

	criteria = MATCHCRITERIA_NOT_COLORLABEL;
	value = strtol((yyvsp[(2) - (2)].str), NULL, 0);
	if (value < 0) value = 0;
	else if (value > COLORLABELS) value = COLORLABELS;
	prop = matcherprop_new(criteria, NULL, 0, NULL, value);
}
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 790 "matcher_parser_parse.y"
    {
	gint criteria = 0;

	criteria = MATCHCRITERIA_IGNORE_THREAD;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 797 "matcher_parser_parse.y"
    {
	gint criteria = 0;

	criteria = MATCHCRITERIA_NOT_IGNORE_THREAD;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 804 "matcher_parser_parse.y"
    {
	gint criteria = 0;

	criteria = MATCHCRITERIA_WATCH_THREAD;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 811 "matcher_parser_parse.y"
    {
	gint criteria = 0;

	criteria = MATCHCRITERIA_NOT_WATCH_THREAD;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 818 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_SUBJECT;
	expr = (yyvsp[(3) - (3)].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 827 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_NOT_SUBJECT;
	expr = (yyvsp[(3) - (3)].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 836 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_FROM;
	expr = (yyvsp[(3) - (3)].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 845 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_NOT_FROM;
	expr = (yyvsp[(3) - (3)].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 854 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_TO;
	expr = (yyvsp[(3) - (3)].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 863 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_NOT_TO;
	expr = (yyvsp[(3) - (3)].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 872 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_CC;
	expr = (yyvsp[(3) - (3)].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 881 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_NOT_CC;
	expr = (yyvsp[(3) - (3)].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 890 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_TO_OR_CC;
	expr = (yyvsp[(3) - (3)].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 899 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_NOT_TO_AND_NOT_CC;
	expr = (yyvsp[(3) - (3)].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 908 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_TAG;
	expr = (yyvsp[(3) - (3)].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 917 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_NOT_TAG;
	expr = (yyvsp[(3) - (3)].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 926 "matcher_parser_parse.y"
    {
	gint criteria = 0;

	criteria = MATCHCRITERIA_TAGGED;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 933 "matcher_parser_parse.y"
    {
	gint criteria = 0;

	criteria = MATCHCRITERIA_NOT_TAGGED;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 940 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gint value = 0;

	criteria = MATCHCRITERIA_AGE_GREATER;
	value = strtol((yyvsp[(2) - (2)].str), NULL, 0);
	prop = matcherprop_new(criteria, NULL, 0, NULL, value);
}
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 949 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gint value = 0;

	criteria = MATCHCRITERIA_AGE_LOWER;
	value = strtol((yyvsp[(2) - (2)].str), NULL, 0);
	prop = matcherprop_new(criteria, NULL, 0, NULL, value);
}
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 958 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gint value = 0;

	criteria = MATCHCRITERIA_AGE_GREATER_HOURS;
	value = strtol((yyvsp[(2) - (2)].str), NULL, 0);
	prop = matcherprop_new(criteria, NULL, 0, NULL, value);
}
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 967 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gint value = 0;

	criteria = MATCHCRITERIA_AGE_LOWER_HOURS;
	value = strtol((yyvsp[(2) - (2)].str), NULL, 0);
	prop = matcherprop_new(criteria, NULL, 0, NULL, value);
}
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 976 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_NEWSGROUPS;
	expr = (yyvsp[(3) - (3)].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 985 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_NOT_NEWSGROUPS;
	expr = (yyvsp[(3) - (3)].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 994 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_INREPLYTO;
	expr = (yyvsp[(3) - (3)].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 1003 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_NOT_INREPLYTO;
	expr = (yyvsp[(3) - (3)].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 1012 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_REFERENCES;
	expr = (yyvsp[(3) - (3)].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 1021 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_NOT_REFERENCES;
	expr = (yyvsp[(3) - (3)].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 1030 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gint value = 0;

	criteria = MATCHCRITERIA_SCORE_GREATER;
	value = strtol((yyvsp[(2) - (2)].str), NULL, 0);
	prop = matcherprop_new(criteria, NULL, 0, NULL, value);
}
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 1039 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gint value = 0;

	criteria = MATCHCRITERIA_SCORE_LOWER;
	value = strtol((yyvsp[(2) - (2)].str), NULL, 0);
	prop = matcherprop_new(criteria, NULL, 0, NULL, value);
}
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 1048 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gint value = 0;

	criteria = MATCHCRITERIA_SCORE_EQUAL;
	value = strtol((yyvsp[(2) - (2)].str), NULL, 0);
	prop = matcherprop_new(criteria, NULL, 0, NULL, value);
}
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 1057 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gint value    = 0;
	criteria = MATCHCRITERIA_SIZE_GREATER;
	value = strtol((yyvsp[(2) - (2)].str), NULL, 0);
	prop = matcherprop_new(criteria, NULL, 0, NULL, value);
}
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 1065 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gint value    = 0;
	criteria = MATCHCRITERIA_SIZE_SMALLER;
	value = strtol((yyvsp[(2) - (2)].str), NULL, 0);
	prop = matcherprop_new(criteria, NULL, 0, NULL, value);
}
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 1073 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gint value    = 0;
	criteria = MATCHCRITERIA_SIZE_EQUAL;
	value = strtol((yyvsp[(2) - (2)].str), NULL, 0);
	prop = matcherprop_new(criteria, NULL, 0, NULL, value);
}
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 1081 "matcher_parser_parse.y"
    {
	header = g_strdup((yyvsp[(2) - (2)].str));
}
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 1084 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gchar *expr = NULL;
	matcher_is_fast = FALSE;
	criteria = MATCHCRITERIA_HEADER;
	expr = (yyvsp[(2) - (5)].str);
	prop = matcherprop_new(criteria, header, match_type, expr, 0);
	g_free(header);
}
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 1094 "matcher_parser_parse.y"
    {
	header = g_strdup((yyvsp[(2) - (2)].str));
}
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 1097 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gchar *expr = NULL;
	matcher_is_fast = FALSE;
	criteria = MATCHCRITERIA_NOT_HEADER;
	expr = (yyvsp[(2) - (5)].str);
	prop = matcherprop_new(criteria, header, match_type, expr, 0);
	g_free(header);
}
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 1107 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gchar *expr = NULL;
	matcher_is_fast = FALSE;
	criteria = MATCHCRITERIA_HEADERS_PART;
	expr = (yyvsp[(3) - (3)].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 1116 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gchar *expr = NULL;
	matcher_is_fast = FALSE;
	criteria = MATCHCRITERIA_NOT_HEADERS_PART;
	expr = (yyvsp[(3) - (3)].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 1125 "matcher_parser_parse.y"
    {
	header = g_strdup((yyvsp[(2) - (2)].str));
}
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 1128 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_FOUND_IN_ADDRESSBOOK;
	expr = (yyvsp[(2) - (5)].str);
	prop = matcherprop_new(criteria, header, match_type, expr, 0);
	g_free(header);
}
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 1138 "matcher_parser_parse.y"
    {
	header = g_strdup((yyvsp[(2) - (2)].str));
}
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 1141 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_NOT_FOUND_IN_ADDRESSBOOK;
	expr = (yyvsp[(2) - (5)].str);
	prop = matcherprop_new(criteria, header, match_type, expr, 0);
	g_free(header);
}
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 1151 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gchar *expr = NULL;
	matcher_is_fast = FALSE;
	criteria = MATCHCRITERIA_MESSAGE;
	expr = (yyvsp[(3) - (3)].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 1160 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gchar *expr = NULL;
	matcher_is_fast = FALSE;
	criteria = MATCHCRITERIA_NOT_MESSAGE;
	expr = (yyvsp[(3) - (3)].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 1169 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gchar *expr = NULL;
	matcher_is_fast = FALSE;
	criteria = MATCHCRITERIA_BODY_PART;
	expr = (yyvsp[(3) - (3)].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 1178 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gchar *expr = NULL;
	matcher_is_fast = FALSE;
	criteria = MATCHCRITERIA_NOT_BODY_PART;
	expr = (yyvsp[(3) - (3)].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 1187 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gchar *expr = NULL;
	matcher_is_fast = FALSE;
	criteria = MATCHCRITERIA_TEST;
	expr = (yyvsp[(2) - (2)].str);
	prop = matcherprop_new(criteria, NULL, MATCHTYPE_MATCH, expr, 0);
}
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 1196 "matcher_parser_parse.y"
    {
	gint criteria = 0;
	gchar *expr = NULL;
	matcher_is_fast = FALSE;
	criteria = MATCHCRITERIA_NOT_TEST;
	expr = (yyvsp[(2) - (2)].str);
	prop = matcherprop_new(criteria, NULL, MATCHTYPE_MATCH, expr, 0);
}
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 1208 "matcher_parser_parse.y"
    {
	gchar *cmd = NULL;
	gint action_type = 0;

	action_type = MATCHACTION_EXECUTE;
	cmd = (yyvsp[(2) - (2)].str);
	action = filteringaction_new(action_type, 0, cmd, 0, 0, NULL);
}
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 1217 "matcher_parser_parse.y"
    {
	gchar *destination = NULL;
	gint action_type = 0;

	action_type = MATCHACTION_MOVE;
	destination = (yyvsp[(2) - (2)].str);
	action = filteringaction_new(action_type, 0, destination, 0, 0, NULL);
}
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 1226 "matcher_parser_parse.y"
    {
	gchar *destination = NULL;
	gint action_type = 0;

	action_type = MATCHACTION_SET_TAG;
	destination = (yyvsp[(2) - (2)].str);
	action = filteringaction_new(action_type, 0, destination, 0, 0, NULL);
}
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 1235 "matcher_parser_parse.y"
    {
	gchar *destination = NULL;
	gint action_type = 0;

	action_type = MATCHACTION_UNSET_TAG;
	destination = (yyvsp[(2) - (2)].str);
	action = filteringaction_new(action_type, 0, destination, 0, 0, NULL);
}
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 1244 "matcher_parser_parse.y"
    {
	gint action_type = 0;

	action_type = MATCHACTION_CLEAR_TAGS;
	action = filteringaction_new(action_type, 0, NULL, 0, 0, NULL);
}
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 1251 "matcher_parser_parse.y"
    {
	gchar *destination = NULL;
	gint action_type = 0;

	action_type = MATCHACTION_COPY;
	destination = (yyvsp[(2) - (2)].str);
	action = filteringaction_new(action_type, 0, destination, 0, 0, NULL);
}
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 1260 "matcher_parser_parse.y"
    {
	gint action_type = 0;

	action_type = MATCHACTION_DELETE;
	action = filteringaction_new(action_type, 0, NULL, 0, 0, NULL);
}
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 1267 "matcher_parser_parse.y"
    {
	gint action_type = 0;

	action_type = MATCHACTION_MARK;
	action = filteringaction_new(action_type, 0, NULL, 0, 0, NULL);
}
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 1274 "matcher_parser_parse.y"
    {
	gint action_type = 0;

	action_type = MATCHACTION_UNMARK;
	action = filteringaction_new(action_type, 0, NULL, 0, 0, NULL);
}
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 1281 "matcher_parser_parse.y"
    {
	gint action_type = 0;

	action_type = MATCHACTION_LOCK;
	action = filteringaction_new(action_type, 0, NULL, 0, 0, NULL);
}
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 1288 "matcher_parser_parse.y"
    {
	gint action_type = 0;

	action_type = MATCHACTION_UNLOCK;
	action = filteringaction_new(action_type, 0, NULL, 0, 0, NULL);
}
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 1295 "matcher_parser_parse.y"
    {
	gint action_type = 0;

	action_type = MATCHACTION_MARK_AS_READ;
	action = filteringaction_new(action_type, 0, NULL, 0, 0, NULL);
}
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 1302 "matcher_parser_parse.y"
    {
	gint action_type = 0;

	action_type = MATCHACTION_MARK_AS_UNREAD;
	action = filteringaction_new(action_type, 0, NULL, 0, 0, NULL);
}
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 1309 "matcher_parser_parse.y"
    {
	gint action_type = 0;

	action_type = MATCHACTION_MARK_AS_SPAM;
	action = filteringaction_new(action_type, 0, NULL, 0, 0, NULL);
}
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 1316 "matcher_parser_parse.y"
    {
	gint action_type = 0;

	action_type = MATCHACTION_MARK_AS_HAM;
	action = filteringaction_new(action_type, 0, NULL, 0, 0, NULL);
}
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 1323 "matcher_parser_parse.y"
    {
	gchar *destination = NULL;
	gint action_type = 0;
	gint account_id = 0;

	action_type = MATCHACTION_FORWARD;
	account_id = strtol((yyvsp[(2) - (3)].str), NULL, 10);
	destination = (yyvsp[(3) - (3)].str);
	action = filteringaction_new(action_type,
            account_id, destination, 0, 0, NULL);
}
    break;

  case 131:

/* Line 1806 of yacc.c  */
#line 1335 "matcher_parser_parse.y"
    {
	gchar *destination = NULL;
	gint action_type = 0;
	gint account_id = 0;

	action_type = MATCHACTION_FORWARD_AS_ATTACHMENT;
	account_id = strtol((yyvsp[(2) - (3)].str), NULL, 10);
	destination = (yyvsp[(3) - (3)].str);
	action = filteringaction_new(action_type,
            account_id, destination, 0, 0, NULL);
}
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 1347 "matcher_parser_parse.y"
    {
	gchar *destination = NULL;
	gint action_type = 0;
	gint account_id = 0;

	action_type = MATCHACTION_REDIRECT;
	account_id = strtol((yyvsp[(2) - (3)].str), NULL, 10);
	destination = (yyvsp[(3) - (3)].str);
	action = filteringaction_new(action_type,
            account_id, destination, 0, 0, NULL);
}
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 1359 "matcher_parser_parse.y"
    {
	gint action_type = 0;
	gint color = 0;

	action_type = MATCHACTION_COLOR;
	color = strtol((yyvsp[(2) - (2)].str), NULL, 10);
	action = filteringaction_new(action_type, 0, NULL, color, 0, NULL);
}
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 1368 "matcher_parser_parse.y"
    {
        gint score = 0;
        
        score = strtol((yyvsp[(2) - (2)].str), NULL, 10);
	action = filteringaction_new(MATCHACTION_CHANGE_SCORE, 0,
				     NULL, 0, score, NULL);
}
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 1377 "matcher_parser_parse.y"
    {
        gint score = 0;
        
        score = strtol((yyvsp[(2) - (2)].str), NULL, 10);
	action = filteringaction_new(MATCHACTION_CHANGE_SCORE, 0,
				     NULL, 0, score, NULL);
}
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 1385 "matcher_parser_parse.y"
    {
        gint score = 0;
        
        score = strtol((yyvsp[(2) - (2)].str), NULL, 10);
	action = filteringaction_new(MATCHACTION_SET_SCORE, 0,
				     NULL, 0, score, NULL);
}
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 1393 "matcher_parser_parse.y"
    {
	action = filteringaction_new(MATCHACTION_HIDE, 0, NULL, 0, 0, NULL);
}
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 1397 "matcher_parser_parse.y"
    {
	action = filteringaction_new(MATCHACTION_IGNORE, 0, NULL, 0, 0, NULL);
}
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 1401 "matcher_parser_parse.y"
    {
	action = filteringaction_new(MATCHACTION_WATCH, 0, NULL, 0, 0, NULL);
}
    break;

  case 140:

/* Line 1806 of yacc.c  */
#line 1405 "matcher_parser_parse.y"
    {
	header = g_strdup((yyvsp[(2) - (2)].str));
}
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 1408 "matcher_parser_parse.y"
    {
	gchar *addressbook = NULL;
	gint action_type = 0;

	action_type = MATCHACTION_ADD_TO_ADDRESSBOOK;
	addressbook = (yyvsp[(2) - (4)].str);
	action = filteringaction_new(action_type, 0, addressbook, 0, 0, header);
	g_free(header);
}
    break;

  case 142:

/* Line 1806 of yacc.c  */
#line 1418 "matcher_parser_parse.y"
    {
	action = filteringaction_new(MATCHACTION_STOP, 0, NULL, 0, 0, NULL);
}
    break;



/* Line 1806 of yacc.c  */
#line 3893 "matcher_parser_parse.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



