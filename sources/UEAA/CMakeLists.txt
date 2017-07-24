file (GLOB_RECURSE Headers *.hpp)
file (GLOB_RECURSE Objects *.cpp)
add_library (UEAA ${Headers} ${Objects})
