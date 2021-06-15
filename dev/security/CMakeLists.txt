cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(crypto_test)

set(SRC_DIR src)

list(APPEND SRSC main.c)
list(APPEND SRSC ${SRC_DIR}/sha.c)
list(APPEND SRSC ${SRC_DIR}/aes.c)
list(APPEND SRSC ${SRC_DIR}/rand.c)
list(APPEND SRSC ${SRC_DIR}/hmac.c)
list(APPEND SRSC ${SRC_DIR}/utils.c)
list(APPEND SRSC ${SRC_DIR}/kbkdf.c)
list(APPEND SRSC ${SRC_DIR}/pbkdf.c)

add_executable(crypto_test ${SRSC})

target_link_libraries(crypto_test ssl crypto)
target_include_directories(crypto_test PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)