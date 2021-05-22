add_library(mds_strlist dumper.cpp reader.cpp)
target_link_libraries(mds_strlist PUBLIC mds_base)

add_executable(mds_strlist_test strlist_test.cpp)
target_link_libraries(mds_strlist_test mds_strlist Catch2WithMain)