#Get current directory
CurrentDir = $(shell pwd)

#include makevar file
include ${CurrentDir}/makevar

#Project name (this variable can be modified)
ProjectModuleName = GEBOperatorProcess

#source code file directory
SrcCodeDir = ${CurrentDir}/../${ProjectModuleName}
MySeriesDir = ${CurrentDir}/../ExternalLibrary/include/myseries
EigenHeaderDir = ${CurrentDir}/../ExternalLibrary/include/Eigen

#Config append include path and library path variables with ${SrcCodeDir}
INCLUDE_DIR += -I${MySeriesDir} -I${EigenHeaderDir} -I/usr/include/gdal
#LIBRARY_DIR += -L${SrcCodeDir}/xxx/lib ${SrcCodeDir}/yyyyy/lib
LIBRARIES += -lgdal -lpthread

#find all .cpp/.c files and print
#this operation can ignore some search paths with '-path /some/ignore/path -prune'
#SourceFilesAll = $(shell find $(SrcCodeDir) -path ${SrcCodeDir}/testdir -prune -o -name *.cpp -print -o -name *.c -print)
SourceFilesAll = $(shell find ${SrcCodeDir} -name *.cpp -print -o -name *.c -print)
SourceFilesAll += $(shell find ${MySeriesDir} -name *.cpp -print)

#specified some ignored files to be compiled.
#FilterFiles := test_template.c test1.cpp
#FilterFiles := $(addprefix %/,${FilterFiles})
FilterFiles :=

#list final source files
SourceFiles := $(filter-out ${FilterFiles},${SourceFilesAll})
SourceFiles_CPP := $(filter %.cpp,${SourceFiles})
SourceFiles_C := $(filter %.c,${SourceFiles})

#list all dependent rule files
DepRuleFiles := ${SourceFiles_CPP:.cpp=.d}
DepRuleFiles += ${SourceFiles_C:.c=.d}

#list all object files
ObjectFiles := ${SourceFiles_CPP:%.cpp=%.o}
ObjectFiles += ${SourceFiles_C:%.c=%.o}

#define target execute file name
TargetExeFile := GEBAlgProcess

#variable IsLibFile passed by make command line parameters.
#indicate whether target file is library file(.so/.a) or not.
${TargetExeFile} : ${ObjectFiles}
	@echo "Linker all object files into target execute file..."
	${CXX} ${CSTANDARD} ${CXXFLAGS} ${IsLibFile} $^ ${LIBRARY_DIR} ${LIBRARIES} -o $@

#compile all cpp files into object files
#using static pattern rules
${ObjectFiles} : %.o : %.cpp
	@echo "Compile cpp file into object file..."
	${CXX} ${CSTANDARD} ${CXXFLAGS} ${INCLUDE_DIR} -c $< -o $@

#delete object file and exectue file before remaking.
.PHONY : clean cleanobj cleanexe
clean : cleanobj cleanexe
	@echo "clean object and exe files finished."

cleanobj :
	@echo "start to clean object files."
	find ${SrcCodeDir} -name "*.o" | xargs rm -rf
	find ${MySeriesDir} -name "*.o" | xargs rm -rf

cleanexe :
	@echo "start to clean execute files."
	find ${CurrentDir} -name ${TargetExeFile} | xargs -r rm -rf
	