all:
	# empty

atomic:
	gcc atomic.c -o atomic.out -lpthread
	./atomic.out

spinlock:
	gcc spinlock.c -o spinlock.out -lpthread
	./spinlock.out

sema:
	gcc sema.c -o sema.out -lpthread
	./sema.out

semlock:
	gcc semlock.c -o semlock.out
	./semlock.out

clean:
	rm -rf *.a *.out *.o *.s
