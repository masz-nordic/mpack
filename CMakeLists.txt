if(CONFIG_MPACK)
zephyr_interface_library_named(MPACK)

zephyr_include_directories(MPACK INTERFACE src/mpack)

zephyr_library()
zephyr_library_sources(
    src/mpack/mpack-common.c
    src/mpack/mpack-expect.c
    src/mpack/mpack-node.c
    src/mpack/mpack-platform.c
    src/mpack/mpack-reader.c
    src/mpack/mpack-writer.c
)

zephyr_library_link_libraries(MPACK)
zephyr_link_libraries(MPACK INTERFACE zephyr_interface)
endif()
