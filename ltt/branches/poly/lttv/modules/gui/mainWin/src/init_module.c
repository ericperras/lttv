/*
 * Initial main.c file generated by Glade. Edit as required.
 * Glade will not overwrite this file.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include <lttv/lttv.h>
#include <lttv/attribute.h>
#include <lttv/hook.h>
#include <lttv/option.h>
#include <lttv/module.h>
#include <lttv/processTrace.h>
#include <lttv/state.h>
#include <lttv/stats.h>
#include <lttv/menu.h>
#include <lttv/toolbar.h>

#include "interface.h"
#include "support.h"
#include <lttv/mainWindow.h>
#include "callbacks.h"

/* global variable */
LttvTracesetStats * gTracesetContext = NULL;
static LttvTraceset * traceset;
WindowCreationData  gWinCreationData;

/** Array containing instanced objects. */
GSList * Main_Window_List = NULL ;

static LttvHooks 
  *before_traceset,
  *after_traceset,
  *before_trace,
  *after_trace,
  *before_tracefile,
  *after_tracefile,
  *before_event,
  *after_event,
  *main_hooks;

static char *a_trace;

void lttv_trace_option(void *hook_data)
{ 
  LttTrace *trace;

  trace = ltt_trace_open(a_trace);
  if(trace == NULL) g_critical("cannot open trace %s", a_trace);
  lttv_traceset_add(traceset, trace);
}

/*****************************************************************************
 *                 Functions for module loading/unloading                    *
 *****************************************************************************/
/**
 * plugin's init function
 *
 * This function initializes the GUI.
 */

static gboolean Window_Creation_Hook(void *hook_data, void *call_data)
{
  WindowCreationData* Window_Creation_Data = (WindowCreationData*)hook_data;

  g_critical("GUI Window_Creation_Hook()");
#ifdef ENABLE_NLS
  bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);
#endif

  gtk_set_locale ();
  gtk_init (&(Window_Creation_Data->argc), &(Window_Creation_Data->argv));

  add_pixmap_directory (PACKAGE_DATA_DIR "/" PACKAGE "/pixmaps");
  add_pixmap_directory ("pixmaps");
  add_pixmap_directory ("modules/gui/mainWin/pixmaps");


  if(!gTracesetContext){
    gTracesetContext = g_object_new(LTTV_TRACESET_STATS_TYPE, NULL);
    lttv_context_init(LTTV_TRACESET_CONTEXT(gTracesetContext), traceset);
  }

  constructMainWin(NULL, Window_Creation_Data);

  gtk_main ();

  return FALSE;
}




G_MODULE_EXPORT void init(LttvModule *self, int argc, char *argv[]) {

  LttvAttributeValue value;
  
  LttvIAttribute *attributes = LTTV_IATTRIBUTE(lttv_global_attributes());
  
  g_critical("GUI init()");
  
  lttv_option_add("trace", 't', 
      "add a trace to the trace set to analyse", 
      "pathname of the directory containing the trace", 
      LTTV_OPT_STRING, &a_trace, lttv_trace_option, NULL);

  traceset = lttv_traceset_new();

  before_traceset = lttv_hooks_new();
  after_traceset = lttv_hooks_new();
  before_trace = lttv_hooks_new();
  after_trace = lttv_hooks_new();
  before_tracefile = lttv_hooks_new();
  after_tracefile = lttv_hooks_new();
  before_event = lttv_hooks_new();
  after_event = lttv_hooks_new();

  g_assert(lttv_iattribute_find_by_path(attributes, "hooks/traceset/before",
      LTTV_POINTER, &value));
  *(value.v_pointer) = before_traceset;
  g_assert(lttv_iattribute_find_by_path(attributes, "hooks/traceset/after",
      LTTV_POINTER, &value));
  *(value.v_pointer) = after_traceset;
  g_assert(lttv_iattribute_find_by_path(attributes, "hooks/trace/before",
      LTTV_POINTER, &value));
  *(value.v_pointer) = before_trace;
  g_assert(lttv_iattribute_find_by_path(attributes, "hooks/trace/after",
      LTTV_POINTER, &value));
  *(value.v_pointer) = after_trace;
  g_assert(lttv_iattribute_find_by_path(attributes, "hooks/tracefile/before",
      LTTV_POINTER, &value));
  *(value.v_pointer) = before_tracefile;
  g_assert(lttv_iattribute_find_by_path(attributes, "hooks/tracefile/after",
      LTTV_POINTER, &value));
  *(value.v_pointer) = after_tracefile;
  g_assert(lttv_iattribute_find_by_path(attributes, "hooks/event/before",
      LTTV_POINTER, &value));
  *(value.v_pointer) = before_event;
  g_assert(lttv_iattribute_find_by_path(attributes, "hooks/event/after",
      LTTV_POINTER, &value));
  *(value.v_pointer) = after_event;

  g_assert(lttv_iattribute_find_by_path(attributes, "hooks/main/before",
      LTTV_POINTER, &value));
  g_assert((main_hooks = *(value.v_pointer)) != NULL);

  gWinCreationData.argc = argc;
  gWinCreationData.argv = argv;
  
  lttv_hooks_add(main_hooks, Window_Creation_Hook, &gWinCreationData);

}

void
free_system_view(systemView * SystemView)
{
  if(!SystemView)return;
  //free_EventDB(SystemView->EventDB);
  //free_SystemInfo(SystemView->SystemInfo);
  //free_Options(SystemView->Options);
  if(SystemView->Next)
    free_system_view(SystemView->Next);
  g_free(SystemView);
}

void free_tab(tab * Tab)
{
  if(!Tab) return;
  if(Tab->custom->vbox)
    gtk_widget_destroy(Tab->custom->vbox);
  if(Tab->Attributes)
    g_object_unref(Tab->Attributes);

  if(Tab->Next) free_tab(Tab->Next);
  g_free(Tab);
}

void
mainWindow_free(mainWindow * mw)
{ 
  if(mw){
    Main_Window_List = g_slist_remove(Main_Window_List, mw);
    
    //should free memory allocated dynamically first
    free_system_view(mw->SystemView);
    free_tab(mw->Tab);
    g_object_unref(mw->Attributes);

    g_free(mw);    
  }
}

void
mainWindow_Destructor(mainWindow * mw)
{
  if(GTK_IS_WIDGET(mw->MWindow)){
    gtk_widget_destroy(mw->MWindow);
    //    gtk_widget_destroy(mw->HelpContents);
    //    gtk_widget_destroy(mw->AboutBox);    
    mw = NULL;
  }
  
  mainWindow_free(mw);
}


void destroy_walk(gpointer data, gpointer user_data)
{
  mainWindow_Destructor((mainWindow*)data);
}



/**
 * plugin's destroy function
 *
 * This function releases the memory reserved by the module and unregisters
 * everything that has been registered in the gtkTraceSet API.
 */
G_MODULE_EXPORT void destroy() {

  LttvAttributeValue value;  

  guint i, nb;

  lttv_option_remove("trace");

  lttv_hooks_destroy(before_traceset);
  lttv_hooks_destroy(after_traceset);
  lttv_hooks_destroy(before_trace);
  lttv_hooks_destroy(after_trace);
  lttv_hooks_destroy(before_tracefile);
  lttv_hooks_destroy(after_tracefile);
  lttv_hooks_destroy(before_event);
  lttv_hooks_destroy(after_event);
  lttv_hooks_remove_data(main_hooks, Window_Creation_Hook, &gWinCreationData);

  nb = lttv_traceset_number(traceset);
  for(i = 0 ; i < nb ; i++) {
    ltt_trace_close(lttv_traceset_get(traceset, i));
  }

  lttv_traceset_destroy(traceset); 

  g_critical("GUI destroy()");

  g_slist_foreach(Main_Window_List, destroy_walk, NULL );
  g_slist_free(Main_Window_List);
  

  g_object_unref(gTracesetContext);
}




