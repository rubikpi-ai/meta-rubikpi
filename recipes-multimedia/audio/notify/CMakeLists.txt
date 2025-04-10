cmake_minimum_required(VERSION 3.8.2)
project(Angora_FW_SDK
  VERSION 1.0
  LANGUAGES C CXX
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(${SYSROOT_INCDIR})
link_directories(${SYSROOT_LIBDIR})

#set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")
#set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -DUSE_SKIA=0 -DUSE_CAIRO=1 -pthread -g -llog")

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(GST_PIPELINE_EXECUTABLE notify)
add_executable(${GST_PIPELINE_EXECUTABLE}
  notify.cpp
)

target_include_directories(${GST_PIPELINE_EXECUTABLE} PRIVATE
  ${GST_INCLUDE_DIRS}
)

target_link_libraries(${GST_PIPELINE_EXECUTABLE} PRIVATE
  ${GST_LIBRARIES}
#  log
#  cutils
)

MESSAGE( STATUS "SYSROOT_INCDIR var key = ${SYSROOT_INCDIR}.")
MESSAGE( STATUS "GST_VERSION_REQUIRED var key = ${GST_VERSION_REQUIRED}.")

install(
  TARGETS ${GST_PIPELINE_EXECUTABLE}
  RUNTIME DESTINATION /usr/sbin
  PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
              GROUP_EXECUTE GROUP_READ
              GROUP_EXECUTE GROUP_READ
)
