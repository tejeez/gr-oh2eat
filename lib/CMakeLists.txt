# Copyright 2011,2012,2016 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})
list(APPEND oh2eat_sources
    int8_to_complex_impl.cc
    stream_to_vector_overlap_impl.cc
    sum_vectors_ff_impl.cc
    vec_mov_avg_ff_impl.cc
    correlator_bank_demod_impl.cc )

set(oh2eat_sources "${oh2eat_sources}" PARENT_SCOPE)
if(NOT oh2eat_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT oh2eat_sources)

add_library(gnuradio-oh2eat SHARED ${oh2eat_sources})
target_link_libraries(gnuradio-oh2eat ${Boost_LIBRARIES} ${GNURADIO_ALL_LIBRARIES})
set_target_properties(gnuradio-oh2eat PROPERTIES DEFINE_SYMBOL "gnuradio_oh2eat_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-oh2eat PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
include(GrMiscUtils)
GR_LIBRARY_FOO(gnuradio-oh2eat RUNTIME_COMPONENT "oh2eat_runtime" DEVEL_COMPONENT "oh2eat_devel")

########################################################################
# Build and register unit test
########################################################################
include(GrTest)

include_directories(${CPPUNIT_INCLUDE_DIRS})

list(APPEND test_oh2eat_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_oh2eat.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_oh2eat.cc
)

add_executable(test-oh2eat ${test_oh2eat_sources})

target_link_libraries(
  test-oh2eat
  ${GNURADIO_RUNTIME_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CPPUNIT_LIBRARIES}
  gnuradio-oh2eat
)

GR_ADD_TEST(test_oh2eat test-oh2eat)

########################################################################
# Print summary
########################################################################
message(STATUS "Using install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Building for version: ${VERSION} / ${LIBVER}")

