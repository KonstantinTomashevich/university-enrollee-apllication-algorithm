add_executable (TestXMLIO TestXMLIO.cpp)
target_link_libraries (TestXMLIO UEAA)
set_target_properties (TestXMLIO PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Tests/")
add_test (NAME test_xml_io COMMAND TestXMLIO)
