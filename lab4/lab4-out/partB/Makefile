mutex:
	gcc -o mutex.out mutex.c -lpthread

spinlock:
	gcc -o spinlock.out spinlock.c -lpthread

producer-consumer:
	gcc -o producer-consumer.out producer-consumer.c -lpthread
	./producer-consumer.out > result.txt

race_condition:
	gcc -o race_condition.out race_condition.c -lpthread

clean:
	rm *.out
