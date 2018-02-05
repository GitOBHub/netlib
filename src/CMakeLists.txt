SET(LIB_SRCS 
	loop.cc
	server.cc
	connection.cc 
	inetaddr.cc
	buffer.cc
	)

SET(HEADERS
	loop.h
	server.h
	connection.h
	inetaddr.h
	buffer.h
   )

ADD_LIBRARY(net_static STATIC ${LIB_SRCS})
ADD_LIBRARY(net_shared SHARED ${LIB_SRCS})

SET_TARGET_PROPERTIES(net_static PROPERTIES OUTPUT_NAME "net")
SET_TARGET_PROPERTIES(net_shared PROPERTIES OUTPUT_NAME "net")

#INSTALL(TARGETS net_static DESTINATION lib)
#INSTALL(TARGETS	net_shared DESTINATION lib)
#INSTALL(FILES ${HEADERS} DESTINATION include)

#SET_TARGET_PROPERTIES(net_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
#SET_TARGET_PROPERTIES(net_shared PROPERTIES CLEAN_DIRECT_OUTPUT 1)
