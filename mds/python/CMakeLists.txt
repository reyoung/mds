pybind11_add_module(pymds main.cpp)
target_link_libraries(pymds PRIVATE mds_base mds_strlist)
target_compile_definitions(pymds PRIVATE VERSION_INFO=${PYMDS_VERSION_INFO})
