        
all:server client

server:server.o
        g++ -g -o server server.o
client:client.o
        g++ -g -o client client.o

server.o:server.cpp
        g++ -g -c selectServer.cpp

client.o:epollclient.cpp
        g++ -g -c selectClient.cpp

clean:all   
        rm all
