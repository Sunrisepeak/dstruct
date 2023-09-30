add_rules("mode.debug", "mode.release")

set_languages("cxx11")

--add_defines("NDEBUG")

add_includedirs(
    ".",
    "tests"
)

--add_includedirs("port/libc")
--[[
target("dstruct_cpp_test")
    set_kind("binary")
    add_files("tests/cpp_test.cpp")
--]]

target("dstruct_test")
    set_kind("binary")
    add_files("tests/main.cpp")

target("dstruct_array")
    set_kind("binary")
    add_files("examples/array/array.cpp")

target("dstruct_vector")
    set_kind("binary")
    add_files("examples/array/vector.cpp")

target("dstruct_string")
    set_kind("binary")
    add_files("examples/string.cpp")

target("embedded_list")
    set_kind("binary")
    add_files("examples/linked-list/embedded_list.cpp")

target("dstruct_single_linked_list")
    set_kind("binary")
    add_files("examples/linked-list/single_linked_list.cpp")

target("dstruct_double_linked_list")
    set_kind("binary")
    add_files("examples/linked-list/double_linked_list.cpp")

target("dstruct_queue")
    set_kind("binary")
    add_files("examples/queue/queue.cpp")

target("dstruct_deque")
    set_kind("binary")
    add_files("examples/queue/deque.cpp")

target("dstruct_stack")
    set_kind("binary")
    add_files("examples/stack/stack.cpp")

target("dstruct_xvalue_stack")
    set_kind("binary")
    add_files("examples/stack/xvalue_stack.cpp")

target("dstruct_heap")
    set_kind("binary")
    add_files("examples/heap.cpp")

target("dstruct_binary_search_tree")
    set_kind("binary")
    add_files("examples/binary_search_tree.cpp")

target("dstruct_smemory_vector")
    set_kind("binary")
    add_files("examples/smemory_vector.cpp")

-- algorithms
target("dstruct_for_each")
    set_kind("binary")
    add_files("examples/algorithms/for_each.cpp")

target("dstruct_heap_algo")
    set_kind("binary")
    add_files("examples/algorithms/heap_algo.cpp")

-- other
target("dstruct_static_mem_allocator")
    set_kind("binary")
    add_files("examples/static_mem_allocator.cpp")