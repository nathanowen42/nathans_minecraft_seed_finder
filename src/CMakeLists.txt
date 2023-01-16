add_executable(find_awesome_islands
    cubiomesIF.cpp
    custom_finders.h
    custom_finders.cpp
    find_awesome_islands.cpp
    )

add_dependencies(find_awesome_islands
    cubiomes_src
)

target_link_libraries(find_awesome_islands
    PRIVATE 
        cubiomes
)
