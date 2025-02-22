# CMake generated Testfile for 
# Source directory: /Users/ArnavShah/Desktop/498-search-engines/lib/tests
# Build directory: /Users/ArnavShah/Desktop/498-search-engines/lib/tests/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(bloom_filter_test "/Users/ArnavShah/Desktop/498-search-engines/lib/tests/build/bloom_filter_test")
set_tests_properties(bloom_filter_test PROPERTIES  _BACKTRACE_TRIPLES "/Users/ArnavShah/Desktop/498-search-engines/lib/tests/CMakeLists.txt;20;add_test;/Users/ArnavShah/Desktop/498-search-engines/lib/tests/CMakeLists.txt;0;")
add_test(dary_heap_test "/Users/ArnavShah/Desktop/498-search-engines/lib/tests/build/dary_heap_test")
set_tests_properties(dary_heap_test PROPERTIES  _BACKTRACE_TRIPLES "/Users/ArnavShah/Desktop/498-search-engines/lib/tests/CMakeLists.txt;20;add_test;/Users/ArnavShah/Desktop/498-search-engines/lib/tests/CMakeLists.txt;0;")
add_test(optional_test "/Users/ArnavShah/Desktop/498-search-engines/lib/tests/build/optional_test")
set_tests_properties(optional_test PROPERTIES  _BACKTRACE_TRIPLES "/Users/ArnavShah/Desktop/498-search-engines/lib/tests/CMakeLists.txt;20;add_test;/Users/ArnavShah/Desktop/498-search-engines/lib/tests/CMakeLists.txt;0;")
add_test(string_test "/Users/ArnavShah/Desktop/498-search-engines/lib/tests/build/string_test")
set_tests_properties(string_test PROPERTIES  _BACKTRACE_TRIPLES "/Users/ArnavShah/Desktop/498-search-engines/lib/tests/CMakeLists.txt;20;add_test;/Users/ArnavShah/Desktop/498-search-engines/lib/tests/CMakeLists.txt;0;")
subdirs("_deps/googletest-build")
