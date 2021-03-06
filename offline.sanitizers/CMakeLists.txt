# sanitizers CMakeLists.txt

set(EXECUTABLE_NAME river.sanitizer)
set(TEST_Z3 z3.handler.test)
set(TEST_INTERVAL_TREE interval.tree.test)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -m32 -std=c++11 -D__cdecl=\"\" -D__stdcall=\"\"")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -m32 -std=c++11 -D__cdecl=\"\" -D__stdcall=\"\"")

include_directories(./include ${CMAKE_INSTALL_PREFIX}/include)

link_directories(${CMAKE_INSTALL_PREFIX}/lib)
find_library (z3 libz3.so)

add_subdirectory(address.sanitizer)
#add_subdirectory(memory.sanitizer)

add_executable(${EXECUTABLE_NAME}
	Z3Handler.cpp
	crc32.cpp
	CallStack.cpp
	TraceParser.cpp
	SanitizerMain.cpp
	)

add_executable(${TEST_Z3}
	Z3Handler.cpp
	TestZ3Handler.cpp
	)

add_executable(${TEST_INTERVAL_TREE}
	IntervalTree.cpp
	TestIntervalTree.cpp
	)

target_link_libraries(${TEST_Z3}
	z3)

target_link_libraries(${EXECUTABLE_NAME}
	asan
	z3
	)

install (TARGETS ${EXECUTABLE_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install (TARGETS ${TEST_Z3} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install (TARGETS ${TEST_INTERVAL_TREE} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)



