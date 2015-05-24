/* $Id$ */
/* Copyright (c) 2015 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS Desktop Keyboard */
/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. */



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


/* public */
/* variables */
DesktopWidgetDefinition widget =
{
	"Keyboard",
	"input-keyboard",
	NULL,
	_keyboard_init,
	_keyboard_destroy,
	_keyboard_get_widget
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
