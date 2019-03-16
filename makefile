all:activity

activity: oush.c
	gcc -o- oush oush.c

clean:
	rm oush
