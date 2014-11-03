TARGETS	= $(OBJDIR)plug $(OBJDIR)snooper $(OBJDIR)xkey
OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
BINDIR	= $(PREFIX)/bin
SBINDIR	= $(PREFIX)/sbin
CC	= cc
CPPFLAGSF=
CPPFLAGS=
CFLAGSF	= -W `pkg-config --cflags gtk+-2.0`
CFLAGS	= -Wall -g -O2
LDFLAGSF= `pkg-config --libs gtk+-2.0`
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

plug_OBJS = $(OBJDIR)plug.o
plug_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
plug_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)plug: $(plug_OBJS)
	$(CC) -o $(OBJDIR)plug $(plug_OBJS) $(plug_LDFLAGS)

snooper_OBJS = $(OBJDIR)snooper.o
snooper_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
snooper_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)snooper: $(snooper_OBJS)
	$(CC) -o $(OBJDIR)snooper $(snooper_OBJS) $(snooper_LDFLAGS)

xkey_OBJS = $(OBJDIR)xkey.o
xkey_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
xkey_LDFLAGS = $(LDFLAGSF) $(LDFLAGS) `pkg-config --libs x11` -lXtst

$(OBJDIR)xkey: $(xkey_OBJS)
	$(CC) -o $(OBJDIR)xkey $(xkey_OBJS) $(xkey_LDFLAGS)

$(OBJDIR)plug.o: plug.c
	$(CC) $(plug_CFLAGS) -o $(OBJDIR)plug.o -c plug.c

$(OBJDIR)snooper.o: snooper.c
	$(CC) $(snooper_CFLAGS) -o $(OBJDIR)snooper.o -c snooper.c

$(OBJDIR)xkey.o: xkey.c
	$(CC) $(xkey_CFLAGS) -o $(OBJDIR)xkey.o -c xkey.c

clean:
	$(RM) -- $(plug_OBJS) $(snooper_OBJS) $(xkey_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)

uninstall:

.PHONY: all clean distclean install uninstall
