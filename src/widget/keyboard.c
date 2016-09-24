/* $Id$ */
/* Copyright (c) 2015-2016 Pierre Pronchery <khorben@defora.org> */
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



#include <gtk/gtk.h>
#include <System.h>
#include <Desktop.h>
#include "../callbacks.h"
#include "../common.h"
#include "../layout.h"
#include "../key.h"
#include "../keyboard.h"

#include "../callbacks.c"
#include "../common.c"
#include "../layout.c"
#include "../key.c"
#include "../keyboard.c"


/* KeyboardWidget */
/* private */
/* types */
typedef struct _DesktopWidgetPlugin
{
	Keyboard * keyboard;
} KeyboardWidget;


/* prototypes */
static KeyboardWidget * _keyboard_init(char const * name);
static void _keyboard_destroy(KeyboardWidget * keyboard);

static GtkWidget * _keyboard_get_widget(KeyboardWidget * keyboard);

static int _keyboard_set_property(KeyboardWidget * keyboard, va_list ap);


/* public */
/* variables */
DesktopWidgetDefinition widget =
{
	"Keyboard",
	"input-keyboard",
	NULL,
	_keyboard_init,
	_keyboard_destroy,
	_keyboard_get_widget,
	_keyboard_set_property
};


/* private */
/* functions */
/* keyboard_init */
static KeyboardWidget * _keyboard_init(char const * name)
{
	KeyboardWidget * keyboard;
	KeyboardPrefs prefs;

	if((keyboard = object_new(sizeof(*keyboard))) == NULL)
		return NULL;
	prefs.monitor = -1;
	prefs.font = NULL;
	prefs.mode = KEYBOARD_MODE_WIDGET;
	prefs.wait = 0;
	if((keyboard->keyboard = keyboard_new(&prefs)) == NULL)
	{
		_keyboard_destroy(keyboard);
		return NULL;
	}
	return keyboard;
}


/* keyboard_destroy */
static void _keyboard_destroy(KeyboardWidget * keyboard)
{
	if(keyboard->keyboard != NULL)
		keyboard_delete(keyboard->keyboard);
	object_delete(keyboard);
}


/* accessors */
/* keyboard_get_widget */
static GtkWidget * _keyboard_get_widget(KeyboardWidget * keyboard)
{
	return keyboard_get_widget(keyboard->keyboard);
}


/* keyboard_set_property */
static int _keyboard_set_property(KeyboardWidget * keyboard, va_list ap)
{
	String const * property;
	unsigned int u;

	while((property = va_arg(ap, String const *)) != NULL)
		if(strcmp(property, "layout") == 0)
		{
			u = va_arg(ap, unsigned int);
			keyboard_set_layout(keyboard->keyboard, u);
		}
		else if(strcmp(property, "page") == 0)
		{
			u = va_arg(ap, unsigned int);
			keyboard_set_page(keyboard->keyboard, u);
		}
	return 0;
}
