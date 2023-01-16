message(STATUS "Source files ${CMAKE_CXX_SOURCE_FILE_EXTENSIONS}")

add_library(CubiomesIF STATIC
    cubiomesIF.cpp
    )

add_dependencies(CubiomesIF cubiomes_src cubiomes)
target_link_libraries(CubiomesIF PRIVATE cubiomes)

add_library(custom_finders STATIC
    custom_finders.cpp
    )

add_dependencies(custom_finders CubiomesIF)

target_link_libraries(custom_finders
    PRIVATE 
        custom_finders.cpp
)

add_executable(find_awesome_islands
    find_awesome_islands.cpp
    )

add_dependencies(find_awesome_islands
    CubiomesIF 
    custom_finders
)

target_link_libraries(find_awesome_islands
    PRIVATE 
        CubiomesIF 
        custom_finders 
        boost-geometry
)
