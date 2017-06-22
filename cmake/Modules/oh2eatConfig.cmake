INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_OH2EAT oh2eat)

FIND_PATH(
    OH2EAT_INCLUDE_DIRS
    NAMES oh2eat/api.h
    HINTS $ENV{OH2EAT_DIR}/include
        ${PC_OH2EAT_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    OH2EAT_LIBRARIES
    NAMES gnuradio-oh2eat
    HINTS $ENV{OH2EAT_DIR}/lib
        ${PC_OH2EAT_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(OH2EAT DEFAULT_MSG OH2EAT_LIBRARIES OH2EAT_INCLUDE_DIRS)
MARK_AS_ADVANCED(OH2EAT_LIBRARIES OH2EAT_INCLUDE_DIRS)

