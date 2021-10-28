include_directories(${INCLUDE_DIR})
target_sources(${PROJECT_NAME} PUBLIC 
    ${SOURCE_DIR}/inputArgHandler.cpp
    ${SOURCE_DIR}/helpCommands.cpp
    ${SOURCE_DIR}/funcFactory.cpp
    )
add_subdirectory(${SOURCE_DIR}/line_types)
add_library(utils ${SOURCE_DIR}/lib/utils.cpp)