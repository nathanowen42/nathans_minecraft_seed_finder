
add_executable(custom_finders_test
    custom_finders_test.cpp
    ../src/custom_finders.h
    ../src/custom_finders.cpp
    )

add_dependencies(custom_finders_test cubiomes_src)

include_directories(${PROJECT_SOURCE_DIR}/src/)

target_link_libraries(custom_finders_test 
    PRIVATE 
        cubiomes
        GTest::gtest_main
)
