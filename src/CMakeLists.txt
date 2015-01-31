find_package (Threads)

list( APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")

add_library(hrtree Common.cc Rectangle.cc HilbertValue.cc NodeEntry.cc LeafEntry.cc
NonLeafEntry.cc Node.cc RTreeHelper.cc RTree.cc)
target_include_directories (hrtree PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(main main.cc)
target_link_libraries (main LINK_PUBLIC hrtree)
target_link_libraries (main LINK_PUBLIC easylogging)
target_link_libraries (main LINK_PUBLIC gtest)
target_link_libraries (main LINK_PUBLIC Hilbert)
target_link_libraries (main LINK_PUBLIC boost_timer)
target_link_libraries (main LINK_PUBLIC boost_system)
target_link_libraries (main LINK_PRIVATE ${CMAKE_THREAD_LIBS_INIT})
