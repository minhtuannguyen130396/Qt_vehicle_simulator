add_executable(messageQueue_example
    main.cpp
    src/Device.cpp
    src/Controller.cpp
)

target_include_directories(messageQueue_example
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(messageQueue_example
    PRIVATE
        messageQueue
)

target_compile_features(messageQueue_example PRIVATE cxx_std_17)
