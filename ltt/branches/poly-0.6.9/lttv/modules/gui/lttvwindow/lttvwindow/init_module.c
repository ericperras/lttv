/* This file is part of the Linux Trace Toolkit viewer
 * Copyright (C) 2003-2004 Mathieu Desnoyers and XangXiu Yang
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License Version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, 
 * MA 02111-1307, USA.
 */

/*
 * Initial main.c file generated by Glade. Edit as required.
 * Glade will not overwrite this file.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <glib.h>

#include <lttv/lttv.h>
#include <lttv/attribute.h>
#include <lttv/hook.h>
#include <lttv/option.h>
#include <lttv/module.h>
#include <lttv/tracecontext.h>
#include <lttv/state.h>
#include <lttv/stats.h>
#include <lttvwindow/menu.h>
#include <lttvwindow/toolbar.h>
#include <lttvwindow/lttvwindowtraces.h>

#include "interface.h"
#include "support.h"
#include <lttvwindow/mainwindow.h>
#include <lttvwindow/mainwindow-private.h>
#include "callbacks.h"
#include <ltt/trace.h>


LttvTraceInfo LTTV_TRACES,
       LTTV_COMPUTATION,
       LTTV_VIEWER_CONSTRUCTORS,
       LTTV_REQUESTS_QUEUE,
       LTTV_REQUESTS_CURRENT,
       LTTV_NOTIFY_QUEUE,
       LTTV_NOTIFY_CURRENT,
       LTTV_COMPUTATION_TRACESET,
       LTTV_COMPUTATION_TRACESET_CONTEXT,
       LTTV_COMPUTATION_SYNC_POSITION,
       LTTV_BEFORE_CHUNK_TRACESET,
       LTTV_BEFORE_CHUNK_TRACE,
       LTTV_BEFORE_CHUNK_TRACEFILE,
       LTTV_AFTER_CHUNK_TRACESET,
       LTTV_AFTER_CHUNK_TRACE,
       LTTV_AFTER_CHUNK_TRACEFILE,
       LTTV_BEFORE_REQUEST,
       LTTV_AFTER_REQUEST,
       LTTV_EVENT_HOOK,
       LTTV_EVENT_HOOK_BY_ID,
       LTTV_HOOK_ADDER,
       LTTV_HOOK_REMOVER,
       LTTV_IN_PROGRESS,
       LTTV_READY,
       LTTV_LOCK;


/** Array containing instanced objects. */
GSList * g_main_window_list = NULL ;

LttvHooks
  *main_hooks;

/* Initial trace from command line */
//LttvTrace *g_init_trace = NULL;

static char *a_trace;
static char g_init_trace[PATH_MAX] = "";


void lttv_trace_option(void *hook_data)
{ 
  LttTrace *trace;

  get_absolute_pathname(a_trace, g_init_trace);
}

/*****************************************************************************
 *                 Functions for module loading/unloading                    *
 *****************************************************************************/
/**
 * plugin's init function
 *
 * This function initializes the GUI.
 */

static gboolean window_creation_hook(void *hook_data, void *call_data)
{
  g_debug("GUI window_creation_hook()");
#ifdef ENABLE_NLS
  bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);
#endif

  gtk_set_locale ();
  gtk_init (&lttv_argc, &lttv_argv);

  add_pixmap_directory (PACKAGE_DATA_DIR "/" PACKAGE "/pixmaps");
  add_pixmap_directory ("pixmaps");
  add_pixmap_directory ("../modules/gui/main/pixmaps");

  /* First window, use command line trace */
  if(strcmp(g_init_trace, "") != 0){
    create_main_window_with_trace(g_init_trace);
  } else {
    construct_main_window(NULL);
  }

  gtk_main ();

  return FALSE;
}

static void init() {

  LttvAttributeValue value;
 
  // Global attributes only used for interaction with main() here.
  LttvIAttribute *attributes = LTTV_IATTRIBUTE(lttv_global_attributes());
  
  LTTV_TRACES = g_quark_from_string("traces");
  LTTV_COMPUTATION = g_quark_from_string("computation");
  LTTV_VIEWER_CONSTRUCTORS = g_quark_from_string("viewer_constructors");
  LTTV_REQUESTS_QUEUE = g_quark_from_string("requests_queue");
  LTTV_REQUESTS_CURRENT = g_quark_from_string("requests_current");
  LTTV_NOTIFY_QUEUE = g_quark_from_string("notify_queue");
  LTTV_NOTIFY_CURRENT = g_quark_from_string("notify_current");
  LTTV_COMPUTATION_TRACESET = g_quark_from_string("computation_traceset");
  LTTV_COMPUTATION_TRACESET_CONTEXT =
                        g_quark_from_string("computation_traceset_context");
  LTTV_COMPUTATION_SYNC_POSITION =
                        g_quark_from_string("computation_sync_position");
  LTTV_BEFORE_CHUNK_TRACESET = g_quark_from_string("before_chunk_traceset");
  LTTV_BEFORE_CHUNK_TRACE = g_quark_from_string("before_chunk_trace");
  LTTV_BEFORE_CHUNK_TRACEFILE = g_quark_from_string("before_chunk_tracefile");
  LTTV_AFTER_CHUNK_TRACESET = g_quark_from_string("after_chunk_traceset");
  LTTV_AFTER_CHUNK_TRACE = g_quark_from_string("after_chunk_trace");
  LTTV_AFTER_CHUNK_TRACEFILE = g_quark_from_string("after_chunk_tracefile");
  LTTV_BEFORE_REQUEST = g_quark_from_string("before_request");
  LTTV_AFTER_REQUEST = g_quark_from_string("after_request");
  LTTV_EVENT_HOOK = g_quark_from_string("event_hook");
  LTTV_EVENT_HOOK_BY_ID = g_quark_from_string("event_hook_by_id");
  LTTV_HOOK_ADDER = g_quark_from_string("hook_adder");
  LTTV_HOOK_REMOVER = g_quark_from_string("hook_remover");
  LTTV_IN_PROGRESS = g_quark_from_string("in_progress");
  LTTV_READY = g_quark_from_string("ready");
  LTTV_LOCK = g_quark_from_string("lock");

  g_debug("GUI init()");

  lttv_option_add("trace", 't', 
      "add a trace to the trace set to analyse", 
      "pathname of the directory containing the trace", 
      LTTV_OPT_STRING, &a_trace, lttv_trace_option, NULL);

  g_assert(lttv_iattribute_find_by_path(attributes, "hooks/main/before",
      LTTV_POINTER, &value));
  g_assert((main_hooks = *(value.v_pointer)) != NULL);

  lttv_hooks_add(main_hooks, window_creation_hook, NULL, LTTV_PRIO_DEFAULT);

  {
    /* Register state calculator */
    LttvHooks *hook_adder = lttv_hooks_new();
    lttv_hooks_add(hook_adder, lttv_state_save_hook_add_event_hooks, NULL,
                   LTTV_PRIO_DEFAULT);
    lttv_hooks_add(hook_adder, lttv_state_hook_add_event_hooks, NULL,
                   LTTV_PRIO_DEFAULT);
    LttvHooks *hook_remover = lttv_hooks_new();
    lttv_hooks_add(hook_remover, lttv_state_save_hook_remove_event_hooks,
                                    NULL, LTTV_PRIO_DEFAULT);
    lttv_hooks_add(hook_remover, lttv_state_hook_remove_event_hooks,
                                    NULL, LTTV_PRIO_DEFAULT);
    /* Add state computation background hook adder to attributes */
    lttvwindowtraces_register_computation_hooks(g_quark_from_string("state"),
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        hook_adder, hook_remover);
  }

  {
    /* Register statistics calculator */
    LttvHooks *hook_adder = lttv_hooks_new();
    lttv_hooks_add(hook_adder, lttv_stats_hook_add_event_hooks, NULL,
                   LTTV_PRIO_DEFAULT);
    lttv_hooks_add(hook_adder, lttv_state_hook_add_event_hooks, NULL,
                   LTTV_PRIO_DEFAULT);
    LttvHooks *hook_remover = lttv_hooks_new();
    lttv_hooks_add(hook_remover, lttv_stats_hook_remove_event_hooks,
                                    NULL, LTTV_PRIO_DEFAULT);
    lttv_hooks_add(hook_remover, lttv_state_hook_remove_event_hooks,
                                    NULL, LTTV_PRIO_DEFAULT);
    LttvHooks *after_request = lttv_hooks_new();
    lttv_hooks_add(after_request, lttv_stats_sum_traceset_hook, NULL,
        LTTV_PRIO_DEFAULT);
    /* Add state computation background hook adder to attributes */
    lttvwindowtraces_register_computation_hooks(g_quark_from_string("stats"),
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
        after_request, NULL, NULL,
        hook_adder, hook_remover);
  }
}

void
main_window_destructor(MainWindow * mw)
{
  g_assert(GTK_IS_WIDGET(mw->mwindow));
  gtk_widget_destroy(mw->mwindow);
}

static void destroy_walk(gpointer data, gpointer user_data)
{
  main_window_destructor((MainWindow*)data);
}

/**
 * plugin's destroy function
 *
 * This function releases the memory reserved by the module and unregisters
 * everything that has been registered in the gtkTraceSet API.
 */
static void destroy() {

  LttvAttributeValue value;  
  LttvTrace *trace;
  GSList *iter = NULL;
  
  lttv_option_remove("trace");

  lttv_hooks_remove_data(main_hooks, window_creation_hook, NULL);

  g_debug("GUI destroy()");

  g_slist_foreach(g_main_window_list, destroy_walk, NULL);
  
  g_slist_free(g_main_window_list);
  
}


LTTV_MODULE("lttvwindow", "Viewer main window", \
    "Viewer with multiple windows, tabs and panes for graphical modules", \
	    init, destroy, "stats", "option")
