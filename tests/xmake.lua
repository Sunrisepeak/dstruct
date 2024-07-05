add_rules("mode.debug", "mode.release")

set_languages("cxx14")

add_requires("gtest 1.14")

add_includedirs("..")

target("dstruct_test")
    set_kind("binary")
    add_files("*.cpp")
    add_packages("gtest")