SET(SRC_LIST main.cc hub.cc codec.cc topic.cc)
ADD_EXECUTABLE(hub ${SRC_LIST})
TARGET_LINK_LIBRARIES(hub net)
