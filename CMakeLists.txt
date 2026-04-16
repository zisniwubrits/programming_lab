cmake_minimum_required(VERSION 3.20)
project(ProgrammingLab)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(wxWidgets_ROOT_DIR "E:/LIB/wxWidgets-3.3.2")
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

# 启用 RC 语言支持
enable_language(RC)

# 搜集源文件
file(GLOB_RECURSE SOURCES "src/*.cpp")
list(APPEND SOURCES "resources/ProgrammingLab.rc")

add_executable(ProgrammingLab ${SOURCES})

# 链接标志（GUI 应用）
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")

target_link_libraries(ProgrammingLab ${wxWidgets_LIBRARIES})
target_include_directories(ProgrammingLab PRIVATE src)
