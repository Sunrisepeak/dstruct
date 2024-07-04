add_rules("mode.debug", "mode.release")

set_languages("cxx11")

--add_defines("NDEBUG")

add_includedirs("..")

--add_includedirs("port/libc")
--[[
target("dstruct_cpp_test")
    set_kind("binary")
    add_files("tests/cpp_test.cpp")
--]]

target("dstruct_array")
    set_kind("binary")
    add_files("array/array.cpp")

target("dstruct_vector")
    set_kind("binary")
    add_files("array/vector.cpp")

target("dstruct_string")
    set_kind("binary")
    add_files("string.cpp")

target("embedded_list")
    set_kind("binary")
    add_files("linked-list/embedded_list.cpp")

target("dstruct_single_linked_list")
    set_kind("binary")
    add_files("linked-list/single_linked_list.cpp")

target("dstruct_double_linked_list")
    set_kind("binary")
    add_files("linked-list/double_linked_list.cpp")

target("dstruct_queue")
    set_kind("binary")
    add_files("queue/queue.cpp")

target("dstruct_deque")
    set_kind("binary")
    add_files("queue/deque.cpp")

target("dstruct_stack")
    set_kind("binary")
    add_files("stack/stack.cpp")

target("dstruct_xvalue_stack")
    set_kind("binary")
    add_files("stack/xvalue_stack.cpp")

target("dstruct_heap")
    set_kind("binary")
    add_files("heap.cpp")

target("dstruct_binary_search_tree")
    set_kind("binary")
    add_files("tree/binary_search_tree.cpp")

target("dstruct_avl_tree")
    set_kind("binary")
    add_files("tree/avl_tree.cpp")

target("dstruct_ufset")
    set_kind("binary")
    add_files("set/ufset.cpp")

target("dstruct_map")
    set_kind("binary")
    add_files("map.cpp")

target("dstruct_smemory_vector")
    set_kind("binary")
    add_files("smemory_vector.cpp")

-- algorithms
target("dstruct_for_each")
    set_kind("binary")
    add_files("algorithms/for_each.cpp")

target("dstruct_heap_algo")
    set_kind("binary")
    add_files("algorithms/heap_algo.cpp")

-- other
target("dstruct_static_mem_allocator")
    set_kind("binary")
    add_files("static_mem_allocator.cpp")