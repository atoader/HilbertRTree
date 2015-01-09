add_library (easylogging STATIC easylogging++.hh)
target_include_directories (easylogging PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(easylogging PROPERTIES LINKER_LANGUAGE CXX)