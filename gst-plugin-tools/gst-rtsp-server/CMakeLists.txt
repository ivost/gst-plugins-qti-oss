# GStreamer RTSP service application.
set(GST_RTSP_EXECUTABLE gst-rtsp-server)

add_executable(${GST_RTSP_EXECUTABLE}
  main.c
)

target_include_directories(${GST_RTSP_EXECUTABLE} PRIVATE
  ${GST_INCLUDE_DIRS}
)

target_link_libraries(${GST_RTSP_EXECUTABLE} PRIVATE
  ${GST_LIBRARIES}
  ${GST_RTSP_LIBRARIES}
)

install(
  TARGETS ${GST_RTSP_EXECUTABLE}
  RUNTIME DESTINATION ${GST_PLUGINS_QTI_OSS_INSTALL_BINDIR}
  PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
              GROUP_EXECUTE GROUP_READ
              GROUP_EXECUTE GROUP_READ
)
