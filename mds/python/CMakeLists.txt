pybind11_add_module(mmapds main.cpp)
target_link_libraries(mmapds PRIVATE mds_base mds_strlist)
target_compile_definitions(mmapds PRIVATE VERSION_INFO=${PYMDS_VERSION_INFO})
