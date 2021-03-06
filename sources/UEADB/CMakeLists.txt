file (GLOB CommandExecutors CommandsExecutors/*.hpp)
set (cuIncludes "")
set (sceCode "")
set (pclCode "")
set (pchCode "")
set (isFirst 1)

foreach (CommandExecutor ${CommandExecutors})
    get_filename_component (fileName ${CommandExecutor} NAME_WE)
    if (NOT ${isFirst})
        set (cuIncludes "${cuIncludes}\n")
        set (sceCode "${sceCode}\n    ")
        set (pchCode "${pchCode}\n    else if (")
    else ()
        set (pclCode "std::cout")
        set (pchCode "if (")
        set (isFirst 0)
    endif ()

    set (cuIncludes "${cuIncludes}#include <UEADB/CommandsExecutors/${fileName}.hpp>")
    set (sceCode "${sceCode}commandExecutors.emplace (UEAA::CStringToHash (\"${fileName}\"), ${fileName}::${fileName});")
    set (pclCode "${pclCode} <<\n        \"    ${fileName}\" << std::endl")
    set (pchCode "${pchCode}strcmp (commandName, \"${fileName}\") == 0)\n    {\n        ${fileName}::PrintHelp ();\n    }")
endforeach ()
set (pclCode "${pclCode};")
configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/Utils/CommandsUtils.cpp.cmake" "${CMAKE_CURRENT_SOURCE_DIR}/Utils/CommandsUtils.cpp")

file (COPY "${CMAKE_SOURCE_DIR}/scripts" DESTINATION "${CMAKE_BINARY_DIR}/bin")
file (GLOB_RECURSE Headers *.hpp)
file (GLOB_RECURSE Objects *.cpp)
add_executable (UEADB ${Headers} ${Objects})
target_link_libraries (UEADB UEAA)
set_target_properties (UEADB PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
