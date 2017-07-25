add_executable (TestSpecialty TestSpecialty.cpp)
target_link_libraries (TestSpecialty UEAA)
set_target_properties (TestSpecialty PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Tests/")
add_test (NAME test_specialty COMMAND TestSpecialty)
