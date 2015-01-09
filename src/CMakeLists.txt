add_library(hrtree Constants.hh Common.cc HilbertValue.cc Node.cc NodeEntry.cc Rectangle.cc RTree.cc RTreeHelper.cc)
target_include_directories (hrtree PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(main main.cc)
target_link_libraries (main LINK_PUBLIC hrtree)
target_link_libraries (main LINK_PUBLIC easylogging)
