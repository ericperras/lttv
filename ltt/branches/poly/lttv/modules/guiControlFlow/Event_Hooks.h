/* Event_hooks.c defines the hooks that are given to processTrace as parameter.
 * These hooks call the Drawing API to draw the information on the screen,
 * using information from Context, but mostly state (running, waiting...).
 */


#ifndef _EVENT_HOOKS_H
#define _EVENT_HOOKS_H

#include <gtk/gtk.h>
#include <lttv/mainWindow.h>
#include "Process_List.h"

void send_test_data(ProcessList *Process_List);

GtkWidget *hGuiControlFlow(mainWindow *pmParentWindow);

int Event_Selected_Hook(void *hook_data, void *call_data);

/* Hook called before drawing. Gets the initial context at the beginning of the
 * drawing interval and copy it to the context in Event_Request.
 */
int Draw_Before_Hook(void *hook_data, void *call_data);

/*
 * The draw event hook is called by the reading API to have a
 * particular event drawn on the screen.
 * @param hook_data ControlFlowData structure of the viewer. 
 * @param call_data Event context.
 *
 * This function basically draw lines and icons. Two types of lines are drawn :
 * one small (3 pixels?) representing the state of the process and the second
 * type is thicker (10 pixels?) representing on which CPU a process is running
 * (and this only in running state).
 *
 * Extremums of the lines :
 * x_min : time of the last event context for this process kept in memory.
 * x_max : time of the current event.
 * y : middle of the process in the process list. The process is found in the
 * list, therefore is it's position in pixels.
 *
 * The choice of lines'color is defined by the context of the last event for this
 * process.
 */
int Draw_Event_Hook(void *hook_data, void *call_data);

int Draw_After_Hook(void *hook_data, void *call_data);

#endif // _EVENT_HOOKS_H
