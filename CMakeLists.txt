cmake_minimum_required(VERSION 3.12)

project(AVDL2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# If this fails, you can suggest where to find QT by setting the environment variable: `Qt6_ROOT=<Qt Installation Folder>`
# Or by passing it on the command line: `cmake -B build -DQt6_ROOT=C:\tools\Qt\6.10.1\mingw_64 .`
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets SerialPort)

qt_standard_project_setup()

qt_add_executable("${PROJECT_NAME}"
    dialog.ui
    main.cpp
    dialog.cpp
    serialconnect.cpp
    serialportfacade.cpp
    strobethread.cpp
)

target_link_libraries("${PROJECT_NAME}" PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::SerialPort)

set_target_properties("${PROJECT_NAME}" PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)


set(RESOURCES_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set(RESOURCES_DEST_DIR "$<TARGET_FILE_DIR:${PROJECT_NAME}>")

set(RESOURCES_TO_COPY
    "Background.png"
    "Nuvomotorsportlogo.png"
)

# 4. Add the Custom Command that runs after the executable is built
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${RESOURCES_SOURCE_DIR} ${RESOURCES_DEST_DIR}
    COMMENT "Copying image assets to build directory."
)
