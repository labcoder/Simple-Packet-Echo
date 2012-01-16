#	Makefile for talker, listener, and mynetcat
listener/talker: talker.c listener.c
	gcc talker.c -o talker
	gcc listener.c -o listener
