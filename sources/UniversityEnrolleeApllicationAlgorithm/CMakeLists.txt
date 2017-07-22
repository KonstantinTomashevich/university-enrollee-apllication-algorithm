file (GLOB_RECURSE Headers *.hpp)
file (GLOB_RECURSE Objects *.cpp)
add_library (UniversityEnrolleeApllicationAlgorithm ${Headers} ${Objects})
