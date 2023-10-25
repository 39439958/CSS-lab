all:
	# empty

square-server:
	gcc square-server.c -o square-server.out
	./square-server.out

square-server-udp:
	gcc square-server-udp.c -o square-server-udp.out
	./square-server-udp.out

square-client:
	gcc square-client.c -o square-client.out
	./square-client.out

square-client-udp:
	gcc square-client-udp.c -o square-client-udp.out
	./square-client-udp.out

web-server:
	mkdir -p $(HOME)/mywebsite 
	cp index.html $(HOME)/mywebsite/index.html
	gcc -I. web-server.c sbuf.c -o web-server.out -lpthread
	./web-server.out

clean:
	rm -rf *.i *.s *.o *.out core.*



