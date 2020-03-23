CC=gcc
CFLAGS= -Ishared -lpthread `pkg-config --cflags --libs glib-2.0`
ODIR= bin
SERVER= server
CLIENT= client

install:
	mkdir ./bin
	$(CC) -o ./$(ODIR)/$(SERVER) $(SERVER)/*.c $(CFLAGS)
	$(CC) -o ./$(ODIR)/$(CLIENT) $(CLIENT)/*.c $(CFLAGS)

clean:
	rm -rf ./$(ODIR)

runServer:
	./$(ODIR)/$(SERVER)

runClient:
	./$(ODIR)/$(CLIENT)
