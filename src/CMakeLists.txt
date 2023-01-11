set(TARGET find_awesome_islands)

add_executable(${TARGET}
    custom_finders.h
    custom_finders.cpp
    find_awesome_islands.cpp
    )

add_dependencies(find_awesome_islands cubiomes_src)

target_link_libraries(${TARGET} PRIVATE cubiomes)
