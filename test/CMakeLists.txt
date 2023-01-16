
add_executable(custom_finders_test
    custom_finders_test.cpp
    )

add_dependencies(custom_finders_test CubiomesIF custom_finders)

include_directories(${PROJECT_SOURCE_DIR}/src/)

target_link_libraries(custom_finders_test 
    PRIVATE 
        cubiomesIF
        custom_finders
        GTest::gtest_main
)
