cmake_minimum_required(VERSION 2.6)

#to get sources if added
#find -name "*.cc"

PROJECT(eda-reversing)
SET(EDA_SRCS
./Server.cc
./main.cc
./Core.cc
./tests/File_test.cc
./tests/Memory_test.cc
./tests/ParsedInstruction_test.cc
./tests/InstructionARM_test.cc
./Controller/ARM/InstructionARM.cc
./Controller/ARM/CoreARM.cc
./Controller/StatelessData.cc
./Controller/StatelessChangelist.cc
./Controller/Changelist.cc
./Model/File.cc
./Model/Memory.cc
./Model/FunctionCache.cc
./Model/RegisterFile.cc
./Model/ParsedInstruction.cc
./View/FrontEndServer.cc
./View/FrontEndConsole.cc
./View/FrontEnd.cc
./Bank.cc
./Support/Mailbox.cc
)
INCLUDE_DIRECTORIES(./include/)
ADD_LIBRARY(eda_lib ${EDA_SRCS})
ADD_EXECUTABLE(eda main.cc)
TARGET_LINK_LIBRARIES(eda eda_lib pthread)

