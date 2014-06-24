SOLNAME=llademo
PROJNAME=LinkLikeApiForCpp
EXECUTIONFILE=llademo

PCHFILE=pch.h
HEADERS=
SOURCES=\
	pch.cpp \
	TestFramework.cpp \
	Getter.cpp \
	ConditionalJudgement.cpp \
	BasicCalc.cpp \
	Filtering.cpp \
	BasicOperation.cpp \
	Conversion.cpp \
	LinqLikeApiForCpp.cpp
INCLUDES=
ifeq ($(ARCH), x86-64)
LIBRARYS=
else
LIBRARYS=
endif

ifeq ($(shell getconf LONG_BIT), 64)
export ARCH=x86-64
else
export ARCH=x86
endif

CXX=clang++
CXXFLAGS=-std=c++1y -stdlib=libc++ -Werror -Wno-unknown-pragmas -O0 -g
release:	CXXFLAGS+=-O3

OPT=opt
OPTFLAGS=

LINK=llvm-link
LINKFLAGS=

LLC=llc
LLCFLAGS=-filetype=obj -relocation-model=pic -O0
release:	LLCFLAGS+=-O3
ifeq ($(ARCH), x86-64)
LLCFLAGS+=-march=x86-64 -mcpu=penryn
else
LLCFLAGS+=-march=x86 -mcpu=penryn
endif

OBJECTDIRBASE=obj/linux-
BINARYDIRBASE=bin/linux-
OBJECTDIR=$(OBJECTDIRBASE)neutral-debug
OBJECTARCHDIR=$(OBJECTDIRBASE)$(ARCH)-debug
BINARYARCHDIR=$(BINARYDIRBASE)$(ARCH)-debug

OBJECTDIR:=../$(OBJECTDIR)/$(PROJNAME)
OBJECTARCHDIR:=../$(OBJECTARCHDIR)/$(PROJNAME)
BINARYARCHDIR:=../$(BINARYARCHDIR)

PCdHFILE=$(OBJECTDIR)/$(PCHFILE).pch
BITCODES=$(patsubst %.cpp, $(OBJECTDIR)/%.bc, $(SOURCES))
OdBITCODES=$(patsubst %.cpp, $(OBJECTDIR)/%.opt.bc, $(SOURCES))
MdBITCODEFILE=$(OBJECTDIR)/$(basename $(EXECUTIONFILE)).merged.bc
OdMdBITCODEFILE=$(OBJECTDIR)/$(basename $(EXECUTIONFILE)).opt.merged.bc
OBJECTFILE=$(OBJECTARCHDIR)/$(basename $(EXECUTIONFILE)).o
BINNARYFILE=$(BINARYARCHDIR)/$(EXECUTIONFILE)


.PHONY: check

all: $(BINNARYFILE)

check:
	@mkdir -p $(OBJECTDIR)
	@mkdir -p $(OBJECTARCHDIR)
	@mkdir -p $(BINARYARCHDIR)

$(PCHFILE): check

$(PCdHFILE): $(PCHFILE)
	@echo "プリ コンパイルしています: $< -> $@"
	@$(CXX) -c -x c++-header $(CXXFLAGS) $(INCLUDES) $< -o $@

$(SOURCES): $(PCdHFILE)

$(BITCODES): $(OBJECTDIR)/%.bc: %.cpp
	@echo "コンパイルしています: $< -> $@"
	@$(CXX) -c -include-pch $(PCdHFILE) -emit-llvm $(CXXFLAGS) $(INCLUDES) $< -o $@

$(OdBITCODES): %.opt.bc: %.bc
	@echo "最適化しています: $^ -> $@"
	@$(OPT) $(OPTFLAGS) $^ -o $@

$(MdBITCODEFILE): $(OdBITCODES)
	@echo "リンクしています: $^ -> $@"
	@$(LINK) $(LINKFLAGS) $^ -o $@

$(OdMdBITCODEFILE): $(MdBITCODEFILE)
	@echo "最適化しています: $^ -> $@"
	@$(OPT) $(OPTFLAGS) $^ -o $@

$(OBJECTFILE): $(OdMdBITCODEFILE)
	@echo "オブジェクト コードに変換しています: $< -> $@"
	@$(LLC) $(LLCFLAGS) $< -o $@

$(BINNARYFILE): $(OBJECTFILE)
	@echo "リンクしています: $< -> $@"
	@$(CXX) $(CXXFLAGS) $(LIBRARYS) $(CXXLIBS) $< -o $@
