file(GLOB SOURCE_CODE
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

add_executable(Memory
	${SOURCE_CODE})

target_compile_features(Memory PRIVATE cxx_std_17)
target_include_directories(Memory PRIVATE "${CMAKE_SOURCE_DIR}/olcPixelGameEngine")
target_include_directories(Memory PRIVATE "${CMAKE_SOURCE_DIR}/include")
#target_link_libraries(Memory PRIVATE "${CMAKE_SOURCE_DIR}/lib/freetype.lib")