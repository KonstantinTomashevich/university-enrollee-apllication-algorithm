add_executable (TestXMLSerialization TestXMLSerialization.hpp TestXMLSerialization.cpp)
target_link_libraries (TestXMLSerialization UEAA)
set_target_properties (TestXMLSerialization PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Tests/")
add_test (NAME test_xml_serialization COMMAND TestXMLSerialization)
