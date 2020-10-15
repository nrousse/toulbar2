# target operating system
        SET(CMAKE_SYSTEM_NAME Windows)
        SET(CMAKE_SYSTEM_VERSION 1)

#setup of cross compilator location
        SET(CMAKE_C_COMPILER /usr/bin/x86_64-w64-mingw32-g++-win32)
        SET(CMAKE_CXX_COMPILER /usr/bin/x86_64-w64-mingw32-g++-win32)

#setup of cros compilation required library and other header file for the target#plateform
#by default under ubuntu 
# the second varibale correspond to the prefix variable 

        SET(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32 ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/win32)

# search for programs in the build host directories
        SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
        SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
        SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

	MESSAGE(STATUS "WIN32 ON SWITCH TO ${CMAKE_C_COMPILER} environement")
