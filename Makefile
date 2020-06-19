objects = micromouse_server.o moving_in_screen.o reading_a_file.o pthread_wrappers.o
objects_server = surroundings_server.o
headers_server = ./shared_files/moving.h ./server_files/reading.h ./server_files/pthread_wrappers.h

micromouse_server : $(objects) $(objects_server) $(headers)
	gcc -o micromouse_server -pthread $(objects) $(objects_server)
	rm $(objects) $(objects_server)

micromouse_client : micromouse_client.o moving_in_screen.o display_client.o
	gcc -o micromouse_client micromouse_client.o moving_in_screen.o display_client.o
	rm micromouse_client.o moving_in_screen.o display_client.o

micromouse_server.o : $(headers_server) surroundings_server.o
	gcc -c ./server_files/micromouse_server.c
micromouse_client.o : ./shared_files/moving.h ./client_files/display_client.h
	gcc -c ./client_files/micromouse_client.c

moving_in_screen.o :./shared_files/moving.h
	gcc -c ./shared_files/moving_in_screen.c
reading_a_file.o : ./server_files/reading.h ./shared_files/moving.h
	gcc -c ./server_files/reading_a_file.c
pthread_wrappers.o : ./server_files/pthread_wrappers.h
	gcc -c -pthread ./server_files/pthread_wrappers.c
surroundings_server.o: ./server_files/surroundings_server.h
	gcc -c ./server_files/surroundings_server.c
display_client.o: ./client_files/display_client.h
	gcc -c ./client_files/display_client.c

clean-objs :
	rm $(objects) micromouse_client.o $(objects_server) display_client.o
.PHONY : clean
clean : 
	rm $(objects) $(objects_server) client.o micromouse_server micromouse_client
