set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Widgets Xml Gui)
if (NOT Qt5_FOUND)
  message(FATAL_ERROR "Qt5 is required")
endif (NOT Qt5_FOUND)
if (NOT Qt5Widgets_FOUND)
  message(FATAL_ERROR "Qt5Widgets is required")
endif (NOT Qt5Widgets_FOUND)
if (NOT Qt5Xml_FOUND)
  message(FATAL_ERROR "Qt5Xml is required")
endif (NOT Qt5Xml_FOUND)
if (NOT Qt5Gui_FOUND)
  message(FATAL_ERROR "Qt5Gui is required")
endif (NOT Qt5Gui_FOUND)

include_directories(${Qt5Widgets_INCLUDES} ${Qt5Xml_INCLUDES}
  ${Qt5Gui_INCLUDES})

INCLUDE_DIRECTORIES(${VLE_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${GVLE_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${VLE_BINARY_DIR}/src
  ${VLE_SOURCE_DIR}/src)

LINK_DIRECTORIES(${Boost_LIBRARY_DIRS} ${VLE_LIBRARY_DIRS}
  ${GVLE_LIBRARY_DIRS})

SET(difference-equation_SOURCES modelerde.cpp editwidget.cpp
    editmodel.cpp vlesm.cpp)

QT5_WRAP_UI(ui_editclass_h editclass.ui)
QT5_WRAP_UI(ui_editmodel_h editmodel.ui)

ADD_DEFINITIONS(-DQT_PLUGIN)

add_library(modeler-difference-equation SHARED ${difference-equation_SOURCES}
  ${ui_editclass_h} ${ui_editmodel_h} )

add_definitions(${Qt5Widgets_DEFINITIONS} ${Qt5Xml_DEFINITIONS}
  ${Qt5Gui_DEFINITIONS})

target_link_libraries(modeler-difference-equation ${INTL_LIB} ${GVLE_LIBRARIES}
  ${VLE_LIBRARIES} ${Qt5Widgets_LIBRARIES} ${Qt5Gui_LIBRARIES} ${Qt5Xml_LIBRARIES})

INSTALL(TARGETS modeler-difference-equation RUNTIME DESTINATION
  plugins/gvle/modeling LIBRARY DESTINATION plugins/gvle/modeling)