cmake_minimum_required(VERSION 3.8)
project(untitled)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c
        fixed_si16.c fixed_si16.h
        features_si.h features_si.c
        features_ui.h features_ui.c
        proj_cte.h
        fixed_ui16.h fixed_ui16.c
        matrix_fl.h matrix_fl.c
        mat_si16.h mat_si16.c
        project.h
        lda.h lda.c mat_ui16.h mat_ui16.c)
add_executable(untitled ${SOURCE_FILES})

target_link_libraries(untitled m)