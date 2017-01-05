
set(HEADERS
    ${CMAKE_SOURCE_DIR}/src/include/
)

add_definitions(-DAPI_LIB ${CMAKE_SOURCE_DIR}/lib/api.dll)

file( GLOB SRC *.cc)

set(API_LIB ${CMAKE_SOURCE_DIR}/src/api/api.dll)
message(STATUS "API_LIB located: " ${API_LIB})

add_executable(${APP_NAME} ${SRC})

target_include_directories(${APP_NAME} PUBLIC ${HEADERS})
#target_link_libraries(${APP_NAME} STATIC API_LIB)
