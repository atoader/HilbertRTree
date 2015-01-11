find_package (Threads)

add_executable(main_test main_test.cc RectangleTest.cc HilbertValueTest.cc NodeTest.cc)

target_link_libraries (main_test LINK_PUBLIC hrtree)
target_link_libraries (main_test LINK_PUBLIC gtest)
target_link_libraries (main_test LINK_PUBLIC easylogging)
target_link_libraries (main_test LINK_PUBLIC Hilbert)
target_link_libraries (main_test LINK_PRIVATE ${CMAKE_THREAD_LIBS_INIT})
