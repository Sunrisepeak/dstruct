set_languages("c++11")

add_includedirs(
    "."
    --"port",
    --"core"
    --"core/ds",
    --"core/ds/static"
)

--add_includedirs("port/libc")

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

target("dstruct_deque")
    set_kind("binary")
    add_cxxflags("-g")
    add_files("examples/deque.cpp")

target("dstruct_queue")
    set_kind("binary")
    add_cxxflags("-g")
    add_files("examples/queue.cpp")

target("dstruct_stack")
    set_kind("binary")
    add_cxxflags("-g")
    add_files("examples/stack.cpp")

-- algorithms
target("dstruct_for_each")
    set_kind("binary")
    add_cxxflags("-g")
    add_files("examples/algorithms/for_each.cpp")