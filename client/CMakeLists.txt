set(SOURCE_FILES 
    src/main.cpp
    src/networking/windowstcpclient.cpp
    src/app/app.cpp
    src/app/window.cpp
    src/app/waitforconnectionwindow.cpp
    src/app/playingwindow.cpp
    src/app/windowarea.cpp
)

include_directories("include")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})


target_link_libraries(${PROJECT_NAME}
  common
  sfml-graphics
  sfml-window 
  sfml-system
)
