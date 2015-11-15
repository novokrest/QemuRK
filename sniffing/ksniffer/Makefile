CC = g++
CFLAGS = 
CPPFLAGS =
LDFLAGS = 
LIBS = 

SRCS = ksniffer.c
OBJS = $(subst .c,.o,$(SRCS))

libksniffer.a: $(OBJS)
	ar -rcs $@ $^

%.o: %.c 
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

clean:
	rm *.o *~ libksniffer.a
