ServerMain:ProcessClientMessage.h ProcessClientMessage.cpp redisOperate.cpp redisOperate.h ServerMain.cpp ServerService.cpp ServerService.h
	g++ ProcessClientMessage.cpp redisOperate.cpp ServerMain.cpp ServerService.cpp -o ServerMain `pkg-config libevent --cflags --libs` `pkg-config hiredis --cflags --libs` -pthread
