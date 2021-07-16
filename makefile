roonit: roonit.c
	$(CC) -Os -s -o roonit roonit.c
	
install: roonit
	cp ./roonit /usr/local/bin

remove:
	rm /usr/local/bin/roonit
