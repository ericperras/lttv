/* This file is part of the Linux Trace Toolkit viewer
 * Copyright (C) 2003-2004 Michel Dagenais
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


#include <lttv/hook.h>
#include <lttv/module.h>
#include <lttv/lttv.h>
#include <lttv/iattribute.h>
#include <lttv/attribute.h>
#include <lttv/option.h>
#include <lttv/traceset.h>
#include <ltt/trace.h>
#include <stdio.h>


void lttv_option_init(int argc, char **argv);
void lttv_option_destroy();

void lttv_module_init(int argc, char **argv);
void lttv_module_destroy();

void lttv_state_init(int argc, char **argv);
void lttv_state_destroy();

void lttv_stats_init(int argc, char **argv);
void lttv_stats_destroy();

/* The main program maintains a few central data structures and relies
   on modules for the rest. These data structures may be accessed by modules
   through an exported API */

static LttvIAttribute *attributes;

static LttvHooks
  *before_options,
  *after_options,
  *before_main,
  *after_main;

static char 
  *a_module,
  *a_module_path;

static gboolean
  a_verbose,
  a_debug;

static int a_argc;

static char **a_argv;

static void lttv_module_option(void *hook_data);

static void lttv_module_path_option(void *hook_data);

static void lttv_verbose(void *hook_data);

static void lttv_debug(void *hook_data);

static void lttv_help(void *hook_data);

/* Since everything is done in modules, the main program only takes care
   of the infrastructure. */

int main(int argc, char **argv) {

  LttvAttributeValue value;

#ifdef MEMDEBUG
  g_mem_set_vtable(glib_mem_profiler_table);
  g_message("Memory summary before main");
  g_mem_profile();
#endif

  g_type_init();
  //g_type_init_with_debug_flags (G_TYPE_DEBUG_OBJECTS | G_TYPE_DEBUG_SIGNALS);

  attributes = LTTV_IATTRIBUTE(g_object_new(LTTV_ATTRIBUTE_TYPE, NULL));

  before_options = lttv_hooks_new();
  after_options = lttv_hooks_new();
  before_main = lttv_hooks_new();
  after_main = lttv_hooks_new();

  g_assert(lttv_iattribute_find_by_path(attributes, "hooks/options/before",
      LTTV_POINTER, &value));
  *(value.v_pointer) = before_options;
  g_assert(lttv_iattribute_find_by_path(attributes, "hooks/options/after",
      LTTV_POINTER, &value));
  *(value.v_pointer) = after_options;
  g_assert(lttv_iattribute_find_by_path(attributes, "hooks/main/before",
      LTTV_POINTER, &value));
  *(value.v_pointer) = before_main;
  g_assert(lttv_iattribute_find_by_path(attributes, "hooks/main/after",
      LTTV_POINTER, &value));
  *(value.v_pointer) = after_main;


  /* Initialize the command line options processing */

  a_argc = argc;
  a_argv = argv;
  lttv_option_init(argc,argv);
  lttv_module_init(argc,argv);
  lttv_state_init(argc,argv);
  lttv_stats_init(argc,argv);

  /* Initialize the module loading */

  lttv_module_path_add("/usr/lib/lttv/plugins");

  /* Add some built-in options */

  lttv_option_add("module",'m', "load a module", "name of module to load", 
      LTTV_OPT_STRING, &a_module, lttv_module_option, NULL);
 
  lttv_option_add("modules-path", 'L', 
      "add a directory to the module search path", 
      "directory to add to the path", LTTV_OPT_STRING, &a_module_path, 
      lttv_module_path_option, NULL);
	
  lttv_option_add("help",'h', "basic help", "none", 
      LTTV_OPT_NONE, NULL, lttv_help, NULL);

  a_verbose = FALSE; 
  lttv_option_add("verbose",'v', "print information messages", "none", 
      LTTV_OPT_NONE, NULL, lttv_verbose, NULL);
 
  a_debug = FALSE;
  lttv_option_add("debug",'d', "print debugging messages", "none", 
      LTTV_OPT_NONE, NULL, lttv_debug, NULL);
 
 
  lttv_hooks_call(before_options, NULL);
  lttv_option_parse(argc, argv);
  lttv_hooks_call(after_options, NULL);

  lttv_hooks_call(before_main, NULL);
  lttv_hooks_call(after_main, NULL);

  lttv_stats_destroy();
  lttv_state_destroy();
  lttv_module_destroy();
  lttv_option_destroy();

  lttv_hooks_destroy(before_options);
  lttv_hooks_destroy(after_options);
  lttv_hooks_destroy(before_main);
  lttv_hooks_destroy(after_main);
  g_object_unref(attributes);

#ifdef MEMDEBUG
    g_message("Memory summary after main");
    g_mem_profile();
#endif
}


LttvAttribute *lttv_global_attributes()
{
  return (LttvAttribute*)attributes;
}


void lttv_module_option(void *hook_data)
{ 
  lttv_module_load(a_module,a_argc,a_argv);
}


void lttv_module_path_option(void *hook_data)
{
  lttv_module_path_add(a_module_path);
}

void lttv_verbose(void *hook_data)
{
  g_log_set_handler(NULL, G_LOG_LEVEL_INFO, g_log_default_handler, NULL);
  g_info("Logging set to include INFO level messages");
}

void lttv_debug(void *hook_data)
{
  g_log_set_handler(NULL, G_LOG_LEVEL_DEBUG, g_log_default_handler, NULL);
  g_info("Logging set to include DEBUG level messages");
}

void lttv_help(void *hook_data)
{
	printf("Linux Trace Toolkit Visualizer\n");
	printf("\n");
	lttv_option_show_help();
	printf("\n");
}

/* 

- Make it easier to change modules from builtin to externally loaded.

    have: MODULE_INFO(name, init, destroy, { require} ) in each module.
    Maintain the list of builtin modules and search these first (or 
    optionally last). Add the lib prefix if needed to avoid having to
    specify libbatchAnalysis instead of batchAnalysis.

- Define formally traceset/trace in the GUI for the user and decide how
   trace/traceset sharing goes in the application.

- Use appropriately the new functions in time.h

- remove the separate tracefiles (control/per cpu) arrays/loops in context.

- split processTrace into context.c and processTrace.c

- check spelling conventions.

- get all the copyright notices.

- remove all the warnings.

- get all the .h files properly doxygen commented to produce useful documents.

- have an intro/architecture document.

- write a tutorial */
