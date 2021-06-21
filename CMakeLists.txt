cmake_minimum_required(VERSION 3.14)
project(SpeechEnhancement)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_PREFIX_PATH  "/home/nitin525/Documents/libtorch/share/cmake")

find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

aux_source_directory(./source DIR_SRC)
add_executable(SpeechEnhancement ${DIR_SRC})
target_link_libraries(SpeechEnhancement ${TORCH_LIBRARIES})
set_property(TARGET SpeechEnhancement PROPERTY CXX_STANDARD 14)

if (MSVC)
    file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
    add_custom_command(TARGET SpeechEnhancement
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${TORCH_DLLS}
            $<TARGET_FILE_DIR:SpeechEnhancement>)
endif (MSVC)