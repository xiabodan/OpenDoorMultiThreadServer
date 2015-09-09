CFLAGS  = -lmysqlclient -lpthread -lwiringPi
Objects = OpenDoorMultiThreadServer.o mydb.o public.o log.o
OpenDoorMultiThreadServer:$(Objects)
	g++ -o OpenDoorMultiThreadServer $(Objects) $(CFLAGS)
clean:
	rm OpenDoorMultiThreadServer 
	rm *.o
