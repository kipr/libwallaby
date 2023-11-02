option(with_create3 "Enable Create 3 API" ON)

if(with_create3)
    add_module(create3)


    include(FetchContent)
    FetchContent_Declare(create3
    GIT_REPOSITORY https://github.com/kipr/create3
    GIT_TAG main
     # Optionally, specify CMake arguments for the external project
    )
    
    FetchContent_Populate(
        create3
    )

    set(server OFF)

    FetchContent_MakeAvailable(create3)

    

    target_include_directories(create3 INTERFACE $<BUILD_INTERFACE:${create3_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)
   
    target_link_libraries(create3 INTERFACE create3_client)

endif()