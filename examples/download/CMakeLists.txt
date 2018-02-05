SET(SRC_LIST downloadSrv.cc main.cc)
ADD_EXECUTABLE(downloadSrv ${SRC_LIST})
TARGET_LINK_LIBRARIES(downloadSrv net)
