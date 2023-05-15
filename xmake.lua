set_languages("c++11")

add_includedirs(
    "port",
    "core",
    "core/ds",
    "core/ds/static"
)

add_includedirs("port/libc")

target("dstruct_array")
    set_kind("binary")
    add_files("examples/array.cpp")

target("dstruct_vector")
    set_kind("binary")
    add_files("examples/vector.cpp")