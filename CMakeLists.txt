
#1.cmake verson，指定cmake版本
cmake_minimum_required(VERSION 3.2)

#2.project name，指定项目的名称，一般和项目的文件夹名称对应
PROJECT(whitebox_impl)

#3.head file path，头文件目录
INCLUDE_DIRECTORIES(include)

#4.source directory，源文件目录
AUX_SOURCE_DIRECTORY(library LIB_SRCS)
AUX_SOURCE_DIRECTORY(library/m4ri LIB_SRCS)
AUX_SOURCE_DIRECTORY(library/math LIB_SRCS)

# simon
AUX_SOURCE_DIRECTORY(library/simon LIB_SRCS)
AUX_SOURCE_DIRECTORY(library/simon_wb LIB_SRCS)

# SWAN
AUX_SOURCE_DIRECTORY(library/swan LIB_SRCS)
AUX_SOURCE_DIRECTORY(library/swan_wb LIB_SRCS)


# # AUX_SOURCE_DIRECTORY(library/sms4 LIB_SRCS)
# SET(LIB_SRCS ${LIB_SRCS} library/sms4/sms4_common.c library/sms4/sms4_setkey.c library/sms4/sms4_enc.c)
# 
# 
# SET(LIB_SRCS ${LIB_SRCS} library/affine/NTLUtils.cpp )
# SET(LIB_SRCS ${LIB_SRCS} library/affine/MatrixUtils.cpp )
# 
# 
# #klein
# AUX_SOURCE_DIRECTORY(library/klein LIB_SRCS)
# 
# #LBlock
# AUX_SOURCE_DIRECTORY(library/LBlock LIB_SRCS)
# 
# #present
# SET(LIB_SRCS
#     ${LIB_SRCS}
#     library/present/present80.cpp
#     library/present/present80wb.cpp    
# )
# # AUX_SOURCE_DIRECTORY(library/present LIB_SRCS)
# 
# 
# AUX_SOURCE_DIRECTORY(library/internal LIB_SRCS)
# AUX_SOURCE_DIRECTORY(library/sms4wb LIB_SRCS)
# 
# test
# SET(TEST_EXEC ${TEST_EXEC} test/utils/testutil.c)
# SET(TEST_EXEC ${TEST_EXEC} test/sm4_control_experiment.cpp test/NTLTest.cpp test/klein_vector.cpp test/LBlock_test.cpp test/present_test.cpp)
# AUX_SOURCE_DIRECTORY(test TEST_EXEC)
# 
AUX_SOURCE_DIRECTORY(. TEST_EXEC)


# set name for build
SET(BUILD_NAME ${PROJECT_NAME})
# SET(BUILD_TEST_NAME ${PROJECT_NAME}_TEST)
SET(TEST_EXEC_NAME ${PROJECT_NAME}_MAIN)



SET(CXXFLAGS $ENV{CXXFLAGS})

set(CMAKE_CXX_FLAGS "-stdlib=libc++")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -O0 -g3")

#add_compile_options(-std=c11)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")


# SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -Os -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS10.3.sdk -F/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS10.3.sdk/System/Library/Frameworks -arch arm64 -arch armv7 -miphoneos-version-min=8.0 -flto")


set(macOS_M4RI ON)

configure_file(config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/config.h @ONLY)

#6.add executable file，添加要编译的可执行文件
if (True) 
    message(${TEST_EXEC_NAME})
    message(${LIB_SRCS})
    message(${TEST_EXEC})
endif()
ADD_EXECUTABLE(${TEST_EXEC_NAME} ${LIB_SRCS} ${TEST_EXEC}  )
# TARGET_LINK_LIBRARIES(${TEST_EXEC_NAME} ${NTL_LIB} ${GMP_DIRS} ${Boost_LIBRARIES})

