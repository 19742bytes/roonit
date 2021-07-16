roonit: roonit.c
	$(CC) -O3 -o roonit roonit.c
	
install: roonit
	cp ./roonit /usr/local/bin

remove:
	rm /usr/local/bin/roonit
