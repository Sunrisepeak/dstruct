set_languages("c++11")

add_includedirs(
    "port",
    "core"
    --"core/ds",
    --"core/ds/static"
)

add_includedirs("port/libc")

target("dstruct_test")
    set_kind("binary")
    add_files("examples/cpp_test.cpp")

target("dstruct_array")
    set_kind("binary")
    add_files("examples/array.cpp")

target("dstruct_vector")
    set_kind("binary")
    add_files("examples/vector.cpp")

target("embedded_list")
    set_kind("binary")
    add_files("examples/embedded_list.cpp")

target("dstruct_double_linked_list")
    set_kind("binary")
    add_files("examples/double_linked_list.cpp")