TARGETS	= $(OBJDIR)keyboard $(OBJDIR)keyboardctl
OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
BINDIR	= $(PREFIX)/bin
SBINDIR	= $(PREFIX)/sbin
CC	= cc
CPPFLAGSF=
CPPFLAGS=
CFLAGSF	= -W `pkg-config --cflags libDesktop`
CFLAGS	= -Wall -g -pedantic
LDFLAGSF= `pkg-config --libs libDesktop` -lintl
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

keyboard_OBJS = $(OBJDIR)callbacks.o $(OBJDIR)common.o $(OBJDIR)key.o $(OBJDIR)keyboard.o $(OBJDIR)layout.o $(OBJDIR)main.o
keyboard_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
keyboard_LDFLAGS = $(LDFLAGSF) $(LDFLAGS) `pkg-config --libs x11` -lXtst

$(OBJDIR)keyboard: $(keyboard_OBJS)
	$(CC) -o $(OBJDIR)keyboard $(keyboard_OBJS) $(keyboard_LDFLAGS)

keyboardctl_OBJS = $(OBJDIR)keyboardctl.o
keyboardctl_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS) `pkg-config --cflags libDesktop`
keyboardctl_LDFLAGS = $(LDFLAGSF) $(LDFLAGS) `pkg-config --libs libDesktop`

$(OBJDIR)keyboardctl: $(keyboardctl_OBJS)
	$(CC) -o $(OBJDIR)keyboardctl $(keyboardctl_OBJS) $(keyboardctl_LDFLAGS)

$(OBJDIR)callbacks.o: callbacks.c callbacks.h
	$(CC) $(keyboard_CFLAGS) -o $(OBJDIR)callbacks.o -c callbacks.c

$(OBJDIR)common.o: common.c
	$(CC) $(keyboard_CFLAGS) -o $(OBJDIR)common.o -c common.c

$(OBJDIR)key.o: key.c key.h
	$(CC) $(keyboard_CFLAGS) -o $(OBJDIR)key.o -c key.c

$(OBJDIR)keyboard.o: keyboard.c callbacks.h keyboard.h ../config.h
	$(CC) $(keyboard_CFLAGS) -o $(OBJDIR)keyboard.o -c keyboard.c

$(OBJDIR)layout.o: layout.c layout.h
	$(CC) $(keyboard_CFLAGS) -o $(OBJDIR)layout.o -c layout.c

$(OBJDIR)main.o: main.c keyboard.h
	$(CC) $(keyboard_CFLAGS) -o $(OBJDIR)main.o -c main.c

$(OBJDIR)keyboardctl.o: keyboardctl.c
	$(CC) $(keyboardctl_CFLAGS) -o $(OBJDIR)keyboardctl.o -c keyboardctl.c

clean:
	$(RM) -- $(keyboard_OBJS) $(keyboardctl_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	$(MKDIR) $(DESTDIR)$(BINDIR)
	$(INSTALL) -m 0755 $(OBJDIR)keyboard $(DESTDIR)$(BINDIR)/keyboard
	$(MKDIR) $(DESTDIR)$(BINDIR)
	$(INSTALL) -m 0755 $(OBJDIR)keyboardctl $(DESTDIR)$(BINDIR)/keyboardctl

uninstall:
	$(RM) -- $(DESTDIR)$(BINDIR)/keyboard
	$(RM) -- $(DESTDIR)$(BINDIR)/keyboardctl

.PHONY: all clean distclean install uninstall
