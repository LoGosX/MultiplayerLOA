set(SOURCE_FILES 
    src/main.cpp
    src/networking/windowstcpclient.cpp
)

include_directories("include")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME}
  common
)