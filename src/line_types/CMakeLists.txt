
include_directories(${INCLUDE_DIR})
target_sources(${PROJECT_NAME} PUBLIC 
    ${SOURCE_DIR}/line_types/logLine.cpp
    ${SOURCE_DIR}/line_types/baseLine.cpp
    ${SOURCE_DIR}/line_types/internalEntry.cpp
    )