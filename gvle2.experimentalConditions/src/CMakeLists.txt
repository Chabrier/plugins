set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})
find_package(PkgConfig REQUIRED)
pkg_check_modules(XMLDEPS libxml-2.0)
include_directories(${XMLDEPS_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${VLE_INCLUDE_DIRS})

if (VLE_VERSION)
  include_directories(${VLE_BINARY_DIR}/src ${VLE_SOURCE_DIR}/src
    ${Boost_INCLUDE_DIRS} ${VLEDEPS_INCLUDE_DIRS})
  set(V_LIB vlelib)
  if (WIN32)
    link_directories(${VLEDEPS_LIBRARY_DIRS} ${Boost_LIBRARY_DIRS})
  endif (WIN32)
else (VLE_VERSION)
  message(STATUS "No VLE_VERSION detected, run standalone build")
  pkg_check_modules(VLEDEPS REQUIRED vle-1.1)
  include_directories(${VLEDEPS_INCLUDE_DIRS})
  link_directories(${VLEDEPS_LIBRARY_DIRS})
  set(V_LIB "vle-1.1")
endif (VLE_VERSION)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

SET(dummy_SOURCES plugin.cpp toolbar.cpp tab.cpp)

QT4_WRAP_UI(ui_toolbar_h  toolbar.ui)
QT4_WRAP_UI(ui_tab_h      tab.ui)

ADD_LIBRARY(ExperimentalConditions SHARED
  plugin.cpp toolbar.cpp tab.cpp
  ${ui_toolbar_h}
  ${ui_tab_h} )

TARGET_LINK_LIBRARIES(ExperimentalConditions
  ${QT_LIBRARIES} ${INTL_LIB})

INSTALL(TARGETS ExperimentalConditions RUNTIME DESTINATION
  plugins/gvle2/condition LIBRARY DESTINATION plugins/gvle2/condition)
