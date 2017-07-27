add_executable (TestUniversity TestUniversity.hpp TestUniversity.cpp)
target_link_libraries (TestUniversity UEAA)
set_target_properties (TestUniversity PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Tests/")
add_test (NAME test_university COMMAND TestUniversity)
