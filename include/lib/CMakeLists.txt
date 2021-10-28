add_library(castUtils INTERFACE ${INCLUDE_DIR}/lib/castUtils.hpp)
target_include_directories(castUtils INTERFACE . PUBLIC {INCLUDE_DIR}/lib)