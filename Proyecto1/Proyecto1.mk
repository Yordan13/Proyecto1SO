##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=Proyecto1
ConfigurationName      :=Debug
WorkspacePath          :=/home/yordan/Proyecto1SO/Proyecto1
ProjectPath            :=/home/yordan/Proyecto1SO/Proyecto1
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Yordan
Date                   :=23/09/16
CodeLitePath           :=/home/yordan/.codelite
LinkerName             :=gcc
SharedObjectLinkerName :=gcc -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="Proyecto1.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -pthread
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcus
CXX      := gcc
CC       := gcc
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/server.c$(ObjectSuffix) $(IntermediateDirectory)/heap.c$(ObjectSuffix) $(IntermediateDirectory)/circularList.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/server.c$(ObjectSuffix): server.c $(IntermediateDirectory)/server.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/yordan/Proyecto1SO/Proyecto1/server.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/server.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/server.c$(DependSuffix): server.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/server.c$(ObjectSuffix) -MF$(IntermediateDirectory)/server.c$(DependSuffix) -MM server.c

$(IntermediateDirectory)/server.c$(PreprocessSuffix): server.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/server.c$(PreprocessSuffix)server.c

$(IntermediateDirectory)/heap.c$(ObjectSuffix): heap.c $(IntermediateDirectory)/heap.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/yordan/Proyecto1SO/Proyecto1/heap.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/heap.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/heap.c$(DependSuffix): heap.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/heap.c$(ObjectSuffix) -MF$(IntermediateDirectory)/heap.c$(DependSuffix) -MM heap.c

$(IntermediateDirectory)/heap.c$(PreprocessSuffix): heap.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/heap.c$(PreprocessSuffix)heap.c

$(IntermediateDirectory)/circularList.c$(ObjectSuffix): circularList.c $(IntermediateDirectory)/circularList.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/yordan/Proyecto1SO/Proyecto1/circularList.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/circularList.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/circularList.c$(DependSuffix): circularList.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/circularList.c$(ObjectSuffix) -MF$(IntermediateDirectory)/circularList.c$(DependSuffix) -MM circularList.c

$(IntermediateDirectory)/circularList.c$(PreprocessSuffix): circularList.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/circularList.c$(PreprocessSuffix)circularList.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


