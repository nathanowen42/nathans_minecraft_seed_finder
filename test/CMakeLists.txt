
add_executable(custom_finders_test
    ../src/cubiomesIF.cpp
    ../src/custom_finders.h
    ../src/custom_finders.cpp
    custom_finders_test.cpp
    )

add_dependencies(custom_finders_test cubiomes_src)

include_directories(${PROJECT_SOURCE_DIR}/src/)

target_link_libraries(custom_finders_test 
    PRIVATE 
        GTest::gtest_main
        cubiomes
)
