find_package(SDL2 REQUIRED)

add_library(World
	CellAutomaton.cpp
	Chunk.cpp
	InputHandler.cpp
	Cell.cpp
	Element.cpp
	World.cpp
	Renderer.cpp
	Camera.cpp)
target_include_directories(World
	INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}"
	PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(World PUBLIC ${SDL2_LIBARIES})
