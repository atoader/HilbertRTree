add_library(hrtree Common.cc Rectangle.cc HilbertValue.cc NodeEntry.cc LeafEntry.cc NonLeafEntry.cc Node.cc)
target_include_directories (hrtree PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(main main.cc)
target_link_libraries (main LINK_PUBLIC hrtree)
target_link_libraries (main LINK_PUBLIC easylogging)
