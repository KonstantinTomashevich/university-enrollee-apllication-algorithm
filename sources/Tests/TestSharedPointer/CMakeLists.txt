add_executable (TestSharedPointer TestSharedPointer.cpp)
target_link_libraries (TestSharedPointer UniversityEnrolleeApllicationAlgorithm)
set_target_properties (TestSharedPointer PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Tests/")
add_test (NAME test_shared_pointer COMMAND TestSharedPointer)
