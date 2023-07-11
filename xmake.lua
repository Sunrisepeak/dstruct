set_languages("c99", "cxx11")

add_includedirs(
    ".",
    "tests"
    --"port",
    --"core"
    --"core/ds",
    --"core/ds/static"
)

--add_includedirs("port/libc")
--[[
target("dstruct_cpp_test")
    set_kind("binary")
    add_files("tests/cpp_test.cpp")
--]]

target("dstruct_test")
    set_kind("binary")
    add_cxxflags("-g")
    add_files("tests/main.cpp")

target("dstruct_array")
    set_kind("binary")
    add_cxxflags("-g")
    add_files("examples/array.cpp")

target("dstruct_vector")
    set_kind("binary")
    add_cxxflags("-g")
    add_files("examples/vector.cpp")

target("embedded_list")
    set_kind("binary")
    add_cxxflags("-g")
    add_files("examples/embedded_list.cpp")

target("dstruct_double_linked_list")
    set_kind("binary")
    add_cxxflags("-g")
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

target("dstruct_xvalue_stack")
    set_kind("binary")
    add_cxxflags("-g")
    add_files("examples/stack/xvalue_stack.cpp")

target("dstruct_heap")
    set_kind("binary")
    add_cxxflags("-g")
    add_files("examples/heap.cpp")

target("dstruct_binary_search_tree")
    set_kind("binary")
    add_cxxflags("-g")
    add_files("examples/binary_search_tree.cpp")

-- algorithms
target("dstruct_for_each")
    set_kind("binary")
    add_cxxflags("-g")
    add_files("examples/algorithms/for_each.cpp")

target("dstruct_heap_algo")
    set_kind("binary")
    add_cxxflags("-g")
    add_files("examples/algorithms/heap_algo.cpp")