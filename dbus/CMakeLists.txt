find_package(Qt6 REQUIRED COMPONENTS DBus)

set(NETWORK_MANAGER_INTERFACES
  org.freedesktop.NetworkManager.Device.Wireless
  org.freedesktop.NetworkManager.Device
  org.freedesktop.NetworkManager.Settings.Connection
  org.freedesktop.NetworkManager.Settings
  org.freedesktop.NetworkManager.AccessPoint
  org.freedesktop.NetworkManager.IP4Config
  org.freedesktop.NetworkManager.Connection.Active
  org.freedesktop.NetworkManager
)

set(NETWORK_MANAGER_SOURCES)
foreach (interface ${NETWORK_MANAGER_INTERFACES})
  string (REPLACE "." "_" basename ${interface})
  set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/${interface}.xml PROPERTIES NO_NAMESPACE ON)
  qt_add_dbus_interface(INTERFACE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/${interface}.xml ${basename})
  message(STATUS "Adding DBus interface ${interface} ${NETWORK_MANAGER_SOURCES} \n")
  list(APPEND NETWORK_MANAGER_SOURCES ${INTERFACE_SOURCES})
endforeach()

add_library(network_manager_dbus STATIC ${NETWORK_MANAGER_SOURCES})
target_link_libraries(network_manager_dbus PUBLIC Qt::DBus)
target_include_directories(network_manager_dbus PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

message(STATUS "network_manager_dbus" network_manager_dbus)
