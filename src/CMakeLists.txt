set(TARGET CelestialBodies)
add_executable(${TARGET} 
    ${CMAKE_SOURCE_DIR}/src/main.cpp
)
target_compile_features(${TARGET} PRIVATE cxx_std_23)
target_link_libraries(${TARGET} PRIVATE 
    SFML::Graphics 
    spdlog::spdlog
    EnTT
)
target_include_directories(${TARGET} SYSTEM PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/systems
    ${CMAKE_SOURCE_DIR}/src/components
    ${CMAKE_SOURCE_DIR}/src/logging
    /usr/x86_64-w64-mingw32/include/GL/
)

file(COPY ${CMAKE_SOURCE_DIR}/src/res DESTINATION ${CMAKE_BINARY_DIR}/bin )