
set(HEADERS
    ${CMAKE_SOURCE_DIR}/src/include/
)
# GLOB is Evil
#file( GLOB_RECURSE SRC ${CMAKE_SOURCE_DIR}/src/*)

set(SRC 
	${CMAKE_SOURCE_DIR}/src/evil_corp.cc
	${CMAKE_SOURCE_DIR}/src/black_market/black_market.cc
	${CMAKE_SOURCE_DIR}/src/miner/mining_farm.cc
	${CMAKE_SOURCE_DIR}/src/miner/wallet.cc
	${CMAKE_SOURCE_DIR}/src/trade/trade.cc
)

if(WIN32)
message(STATUS "It's windows!!!")
list(APPEND SRC ${CMAKE_SOURCE_DIR}/src/lib_proxy/win_lib.cc)
elseif(UNIX)
list(APPEND SRC ${CMAKE_SOURCE_DIR}/src/lib_proxy/linux_lib.cc)
endif(WIN32)

add_executable(${APP_NAME} ${SRC})

set(BIN_DIR ${CMAKE_SOURCE_DIR}/build/bin/)
set_target_properties( ${APP_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR} )

target_include_directories(${APP_NAME} PUBLIC ${HEADERS})

if(NOT EXISTS ${BIN_DIR}/${CMAKE_BUILD_TYPE}/api.dll )
    message(STATUS "Copy api.dll to bin dir")
	file(MAKE_DIRECTORY ${BIN_DIR}/${CMAKE_BUILD_TYPE}/)
    configure_file(${CMAKE_SOURCE_DIR}/lib/api.dll ${BIN_DIR}/${CMAKE_BUILD_TYPE}/ COPYONLY)
endif(NOT EXISTS ${BIN_DIR}/${CMAKE_BUILD_TYPE}/api.dll )