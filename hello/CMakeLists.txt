add_executable(boost_iter "boost_iter.cpp")
add_executable(regex_example "regex_example.cpp")


target_link_directories(regex_example PRIVATE "/Users/milley/DevTools/boost/boost_1_83_0/stage/lib")
target_link_libraries(regex_example libboost_regex.dylib)