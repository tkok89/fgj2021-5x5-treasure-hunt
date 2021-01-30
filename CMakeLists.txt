cmake_minimum_required(VERSION 3.12)

project(ggj21)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE GGJ21_SOURCES src/*.cpp src/*.c)

add_compile_definitions(
	SFML_STATIC
)

add_executable(ggj21 ${GGJ21_SOURCES})

if (WIN32)
	target_include_directories(ggj21 PRIVATE "external/include/")

	set(LIBS_DIR "external/lib")
	
	find_library(FREETYPE        freetype          ${LIBS_DIR})
	find_library(OGG             ogg               ${LIBS_DIR})
	find_library(OPENAL          openal32          ${LIBS_DIR})
	find_library(FLAC            flac              ${LIBS_DIR})
	find_library(VORBIS          vorbis            ${LIBS_DIR})
	find_library(VORBISFILE      vorbisfile        ${LIBS_DIR})
	
	
	find_library(SFML_SYSTEM_D   sfml-system-s-d   ${LIBS_DIR})
	find_library(SFML_GRAPHICS_D sfml-graphics-s-d ${LIBS_DIR})
	find_library(SFML_AUDIO_D    sfml-audio-s-d    ${LIBS_DIR})
	find_library(SFML_WINDOW_D   sfml-window-s-d   ${LIBS_DIR})
	find_library(SFML_NETWORK_D   sfml-network-s-d   ${LIBS_DIR})

	find_library(SFML_SYSTEM     sfml-system-s     ${LIBS_DIR})
	find_library(SFML_GRAPHICS   sfml-graphics-s   ${LIBS_DIR})
	find_library(SFML_AUDIO      sfml-audio-s      ${LIBS_DIR})
	find_library(SFML_WINDOW     sfml-window-s     ${LIBS_DIR})
	find_library(SFML_NETWORK   sfml-network-s   ${LIBS_DIR})
else()
	target_include_directories(ggj21 BEFORE PRIVATE "external/include/")
	
	set(LIBS_DIR "external/lib")
	find_library(SFML_SYSTEM   sfml-system   ${LIBS_DIR})
	find_library(SFML_GRAPHICS sfml-graphics ${LIBS_DIR})
	find_library(SFML_AUDIO    sfml-audio    ${LIBS_DIR})
	find_library(SFML_WINDOW   sfml-window   ${LIBS_DIR})
	find_library(SFML_NETWORK   sfml-network   ${LIBS_DIR})
endif()

if (WIN32)
	target_link_libraries(
		ggj21
		
		general    opengl32
		general    ${FREETYPE}
		general    ${OGG}
		general    ${OPENAL}
		general    ${FLAC}
		general    ${VORBIS}
		general    ${VORBISFILE}
		general    winmm
		general    gdi32
		general    ws2_32

		optimized  ${SFML_GRAPHICS}
		debug      ${SFML_GRAPHICS_D}

		optimized  ${SFML_WINDOW}
		debug      ${SFML_WINDOW_D}

		optimized  ${SFML_SYSTEM}
		debug      ${SFML_SYSTEM_D}

		optimized  ${SFML_AUDIO}
		debug      ${SFML_AUDIO_D}

		optimized  ${SFML_NETWORK}
		debug      ${SFML_NETWORK_D}
	)
else()
	target_link_libraries(
		ggj21

		GL
		freetype
		ogg
		openal
		pthread
		${SFML_SYSTEM}
		${SFML_GRAPHICS}
		${SFML_AUDIO}
		${SFML_WINDOW}
		${SFML_NETWORK}
	)
endif()

