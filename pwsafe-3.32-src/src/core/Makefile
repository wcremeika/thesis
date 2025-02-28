# Makefile for Linux build of pwsafe
# core library

#CONFIG=debug
#CONFIG=release
CONFIG=unicodedebug
#CONFIG=unicoderelease

NAME=core

DEPDIR= ./deps

# Following not used in Linux build
NOTSRC          = PWSclipboard.cpp

LIBSRC          = BlowFish.cpp CheckVersion.cpp \
                  ItemData.cpp ItemField.cpp  PWPolicy.cpp \
                  Match.cpp PWCharPool.cpp CoreImpExp.cpp \
                  PWHistory.cpp PWSAuxParse.cpp \
                  PWScore.cpp PWSdirs.cpp PWSfile.cpp \
                  PWSfileV1V2.cpp PWSfileV3.cpp \
                  PWSFilters.cpp PWSLog.cpp PWSprefs.cpp \
                  Command.cpp PWSrand.cpp Report.cpp \
                  sha1.cpp sha256.cpp core_st.cpp\
                  StringX.cpp SysInfo.cpp \
                  TwoFish.cpp UnknownField.cpp  \
                  UTF8Conv.cpp Util.cpp CoreOtherDB.cpp \
                  VerifyFormat.cpp XMLprefs.cpp \
                  ExpiredList.cpp \
                  pugixml/pugixml.cpp \
                  XML/XMLFileHandlers.cpp XML/XMLFileValidation.cpp \
                  XML/Xerces/XFileSAX2Handlers.cpp XML/Xerces/XFileValidator.cpp \
                  XML/Xerces/XFileXMLProcessor.cpp XML/Xerces/XFilterSAX2Handlers.cpp \
                  XML/Xerces/XFilterXMLProcessor.cpp XML/Xerces/XSecMemMgr.cpp PWSLog.cpp

SRC             = $(LIBSRC)

OBJPATH         = ../../obj/$(CONFIG)/$(NAME)
LIBPATH         = ../../lib/$(CONFIG)

#destination related macros
LIBOBJ    = $(addprefix $(OBJPATH)/,$(subst .cpp,.o,$(LIBSRC)))
LIB       = $(LIBPATH)/libcore.a
OBJ       = $(LIBOBJ)

CPPFLAGS = $(CXXFLAGS) -Wall -I.. -DLINUX

ifeq ($(CONFIG),debug)
CPPFLAGS += -O0 -g -ggdb -D_DEBUG -DDEBUG

else ifeq ($(CONFIG),release)
CPPFLAGS += -O -DNDEBUG
else ifeq ($(CONFIG),unicodedebug)
CPPFLAGS += -O0 -g -ggdb -DUNICODE -DUSE_XML_LIBRARY=XERCES -DWCHAR_INCOMPATIBLE_XMLCH \
			-D_DEBUG -DDEBUG
CFLAGS += -g
else ifeq ($(CONFIG),unicoderelease)
CPPFLAGS += -O -DUNICODE -DUSE_XML_LIBRARY=XERCES -DWCHAR_INCOMPATIBLE_XMLCH -DNDEBUG
endif

# rules
.PHONY: all debug release unicodedebug unicoderelease clean setup

$(OBJPATH)/%.o : %.c
	$(CC) $(CFLAGS)   -c $< -o $@

$(OBJPATH)/%.o : %.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@

all : setup $(LIB)

debug release unicodedebug unicoderelease:
	$(MAKE) all CONFIG=$@

$(LIB): $(LIBOBJ)
	ar rvus $@ $(filter %.o,$^)

core_st.cpp core_st.h : core.rc2
	../../Misc/rc2cpp.pl $^

StringX.cpp: core_st.h

clean:
	@rm -f *~ $(OBJ) $(TEST) $(LIB)
	@rm -rf $(DEPDIR)
	@rm core_st.cpp core_st.h

setup:
	@mkdir -p $(OBJPATH) $(OBJPATH)/pugixml $(OBJPATH)/XML/Xerces $(LIBPATH) $(DEPDIR)

$(DEPDIR)/%.d: %.cpp
	@set -e; mkdir -p $(DEPDIR) $(DEPDIR)/pugixml; \
	mkdir -p $(DEPDIR)/XML/Xerces; \
	$(RM) $@; \
	$(CXX) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,$(OBJECTPATH)/\1.o $@ : ,g' < $@.$$$$ > $@; \
	$(RM) $@.$$$$

-include $(SRC:%.cpp=$(DEPDIR)/%.d)
