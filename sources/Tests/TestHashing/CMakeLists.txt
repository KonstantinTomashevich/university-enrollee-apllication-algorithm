add_executable (TestHashing TestHashing.cpp)
target_link_libraries (TestHashing UEAA)
set_target_properties (TestHashing PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Tests/")
add_test (NAME test_hashing COMMAND TestHashing)
