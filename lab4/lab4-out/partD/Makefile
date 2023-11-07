all:
	# empty

stack:
	gcc -g concur-stack.c test-stack.c -o stack.out -lpthread
	./stack.out

hash:
	gcc -g concur-hash.c test-hash.c -o hash.out -lpthread
	./hash.out

clean:
	rm -rf *.out
