add_test(tello_camera_test core tello time camera)
target_link_libraries(tello_camera_test PRIVATE wpa_ctrl opencv_core zbar)