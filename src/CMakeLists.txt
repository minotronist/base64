cmake_minimum_required(VERSION 3.20)


add_library(base64 STATIC
            lib.cpp
)

target_include_directories(base64 PUBLIC ../include)
target_compile_features(base64 PUBLIC cxx_std_23)

target_link_libraries(base64
                    PRIVATE readfile
)
