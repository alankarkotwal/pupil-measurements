cmake_minimum_required(VERSION 2.8)
project( PupilTracker )
find_package( OpenCV REQUIRED )
add_executable( PupilTracker PupilTracker.cpp )
target_link_libraries( PupilTracker ${OpenCV_LIBS} )
