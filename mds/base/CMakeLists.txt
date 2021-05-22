add_library(mds_base file_descriptor.cpp except.cpp mmap.cpp)
target_link_libraries(mds_base PUBLIC exceptxx::exceptxx)
target_include_directories(mds_base PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../)

add_executable(mds_base_test file_descriptor_test.cpp)
target_link_libraries(mds_base_test PUBLIC mds_base Catch2WithMain)
add_test(NAME mds_base_test COMMAND mds_base_test)