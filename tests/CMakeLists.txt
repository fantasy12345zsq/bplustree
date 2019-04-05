set(DEMO_NAME ${PROJECT_NAME}_demo)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

set(INC_DIR ${PROJECT_SOURCE_DIR}/lib)
include_directories(${INC_DIR})

set(SRC_LIST bplustree_demo.cc)
add_executable(${DEMO_NAME} ${SRC_LIST})
target_link_libraries(${DEMO_NAME} ${LIB_BPLUSTREE_NAME})