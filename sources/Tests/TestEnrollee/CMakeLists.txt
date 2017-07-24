add_executable (TestEnrollee TestEnrollee.cpp)
target_link_libraries (TestEnrollee UEAA)
set_target_properties (TestEnrollee PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Tests/")
add_test (NAME test_enrollee COMMAND TestEnrollee)
