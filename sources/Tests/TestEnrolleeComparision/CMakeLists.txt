add_executable (TestEnrolleeComparision TestEnrolleeComparision.cpp)
target_link_libraries (TestEnrolleeComparision UEAA)
set_target_properties (TestEnrolleeComparision PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Tests/")
add_test (NAME test_enrollee_comparision COMMAND TestEnrolleeComparision)
