/* $Id$ */
/* Copyright (c) 2010-2020 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS Desktop Keyboard */
/* All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */



#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <libintl.h>
#include <gtk/gtk.h>
#include "keyboard.h"
#include "../config.h"
#define _(string) gettext(string)

/* constants */
#ifndef PROGNAME_KEYBOARD
# define PROGNAME_KEYBOARD	"keyboard"
#endif
#ifndef PREFIX
# define PREFIX			"/usr/local"
#endif
#ifndef DATADIR
# define DATADIR		PREFIX "/share"
#endif
#ifndef LOCALEDIR
# define LOCALEDIR		DATADIR "/locale"
#endif


/* keyboard */
/* private */
/* prototypes */
static int _keyboard(KeyboardPrefs * prefs);

static int _error(char const * message, int ret);
static int _usage(void);


/* functions */
/* keyboard */
static int _keyboard(KeyboardPrefs * prefs)
{
	Keyboard * keyboard;

	if((keyboard = keyboard_new(prefs)) == NULL)
		return -1;
	gtk_main();
	keyboard_delete(keyboard);
	return 0;
}


/* error */
static int _error(char const * message, int ret)
{
	fputs(PROGNAME_KEYBOARD ": ", stderr);
	perror(message);
	return ret;
}


/* usage */
static int _usage(void)
{
	fprintf(stderr, _("Usage: %s [-d|-p|-w|-x][-f font][-l layout]"
"[-m monitor][-n]\n"
"  -d	Start in docked mode\n"
"  -l	Select a different layout\n"
"  -p	Start as a popup window\n"
"  -w	Start in windowed mode\n"
"  -x	Start in embedded mode\n"
"  -f	Set the font used for the keys\n"
"  -m	Place on a particular monitor (in docked or popup mode)\n"
"  -n	Start without showing up directly (if not embedded)\n"),
			PROGNAME_KEYBOARD);
	return 1;
}


/* public */
/* functions */
/* main */
int main(int argc, char * argv[])
{
	int o;
	KeyboardPrefs prefs;
	char * p;

	if(setlocale(LC_ALL, "") == NULL)
		_error("setlocale", 1);
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);
	memset(&prefs, 0, sizeof(prefs));
	gtk_init(&argc, &argv);
	while((o = getopt(argc, argv, "df:l:m:npwx")) != -1)
		switch(o)
		{
			case 'd':
				prefs.mode = KEYBOARD_MODE_DOCKED;
				break;
			case 'f':
				prefs.font = optarg;
				break;
			case 'l':
				prefs.layout = optarg;
				break;
			case 'm':
				prefs.monitor = strtol(optarg, &p, 10);
				if(optarg[0] == '\0' || *p != '\0')
					return _usage();
				break;
			case 'n':
				prefs.wait = 1;
				break;
			case 'p':
				prefs.mode = KEYBOARD_MODE_POPUP;
				break;
			case 'w':
				prefs.mode = KEYBOARD_MODE_WINDOWED;
				break;
			case 'x':
				prefs.mode = KEYBOARD_MODE_EMBEDDED;
				break;
			default:
				return _usage();
		}
	if(optind != argc)
		return _usage();
	return (_keyboard(&prefs) == 0) ? 0 : 2;
}
