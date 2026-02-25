add_library(messageQueue
    src/MessageBus.cpp
)
target_sources(messageQueue
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS include
    FILES
      include/messageQueue/MessageBus.h
      include/messageQueue/ThreadSafeQueue.h
      include/messageQueue/Messages.h
)
target_include_directories(messageQueue
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(messageQueue PUBLIC cxx_std_17)

# Example app
#add_subdirectory(example)
