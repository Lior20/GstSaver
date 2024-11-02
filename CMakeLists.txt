cmake_minimum_required(VERSION 3.10)
project(GstVideoCapture)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GST REQUIRED gstreamer-1.0)

include_directories(${GST_INCLUDE_DIRS})
link_directories(${GST_LIBRARY_DIRS})

add_executable(GstVideoCapture 
    main.cpp
    GstSaver.cpp
)

# Add the test executable
add_executable(test_GstSaver 
    test_GstSaver.cpp
    GstSaver.cpp
)

target_link_libraries(GstVideoCapture ${GST_LIBRARIES})
target_link_libraries(test_GstSaver ${GST_LIBRARIES})

