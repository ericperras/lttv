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


/* module.c : Implementation of the module loading/unloading mechanism.
 * 
 */

#include <lttv/module.h>

struct _LttvModule
{
  GModule *module;
  guint ref_count;
  guint load_count;
  GPtrArray *dependents;
};


/* Table of loaded modules and paths where to search for modules */

static GHashTable *modules = NULL;

static GPtrArray *modulesPaths = NULL;

static void lttv_module_unload_all();


void lttv_module_init(int argc, char **argv) 
{
  g_log(G_LOG_DOMAIN, G_LOG_LEVEL_INFO, "Init module.c");
  modules = g_hash_table_new(g_str_hash, g_str_equal);
  modulesPaths = g_ptr_array_new();
}


void lttv_module_destroy() 
{  
  int i;

  g_log(G_LOG_DOMAIN, G_LOG_LEVEL_INFO, "Destroy module.c");

  /* Unload all modules */
  lttv_module_unload_all();

  /* Free the modules paths pointer array as well as the elements */
  for(i = 0; i < modulesPaths->len ; i++) {
    g_free(modulesPaths->pdata[i]);
  }
  g_ptr_array_free(modulesPaths,TRUE) ;
  modulesPaths = NULL;
  
  /* destroy the hash table */
  g_hash_table_destroy(modules) ;
  modules = NULL;
}


/* Add a new pathname to the modules loading search path */

void lttv_module_path_add(const char *name) 
{
  g_log(G_LOG_DOMAIN, G_LOG_LEVEL_INFO, "Add module path %s", name);
  g_ptr_array_add(modulesPaths,(char*)g_strdup(name));
}


static LttvModule *
module_load(const char *name, int argc, char **argv) 
{
  GModule *gm;

  LttvModule *m;

  int i;

  char *pathname;

  const char *module_name;
  
  LttvModuleInit init_function;

  g_log(G_LOG_DOMAIN, G_LOG_LEVEL_INFO, "Load module %s", name);

  /* Try to find the module along all the user specified paths */

  for(i = 0 ; i < modulesPaths->len ; i++) {
    pathname = g_module_build_path(modulesPaths->pdata[i],name);
    g_log(G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG, "Try path %s", pathname);
    gm = g_module_open(pathname,0);
    g_free(pathname);    
    
    if(gm != NULL) break;
    g_log(G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,"Trial failed, %s",g_module_error());
  }

  /* Try the default system path */

  if(gm == NULL) {
    pathname = g_module_build_path(NULL,name);
    g_log(G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG, "Try default path");
    gm = g_module_open(pathname,0);
    g_free(pathname);
  }

  /* Module cannot be found */
  if(gm == NULL) {
    g_log(G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,"Trial failed, %s",g_module_error());
    g_warning("Failed to load module %s", name); 
    return NULL;
  }

  /* Check if the module was already opened using the hopefully canonical name
     returned by g_module_name. */

  module_name = g_module_name(gm);

  m = g_hash_table_lookup(modules, module_name);

  if(m == NULL) {
    g_log(G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG, 
        "Module %s (%s) loaded, call its init function", name, module_name);

    /* Module loaded for the first time. Insert it in the table and call the
       init function if any. */

    m = g_new(LttvModule, 1);
    m->module = gm;
    m->ref_count = 0;
    m->load_count = 0;
    m->dependents = g_ptr_array_new();
    g_hash_table_insert(modules, (gpointer)module_name, m);

    if(!g_module_symbol(gm, "init", (gpointer)&init_function)) {
      g_warning("module %s (%s) has no init function", name, pathname);
    }
    else init_function(m, argc, argv);
  }
  else {

    /* Module was already opened, check that it really is the same and
       undo the extra g_module_open */

    g_log(G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG, 
        "Module %s (%s) was already loaded, no need to call init function",
	name, module_name);
    if(m->module != gm) g_error("Two gmodules with the same pathname");
    g_module_close(gm);
  }
 
  m->ref_count++;
  return m;
}


LttvModule *
lttv_module_load(const char *name, int argc, char **argv) 
{
  g_log(G_LOG_DOMAIN, G_LOG_LEVEL_INFO, "Load module %s explicitly", name);
  LttvModule *m = module_load(name, argc, argv);
  if(m != NULL) m->load_count++;
  return m;
}


LttvModule *
lttv_module_require(LttvModule *m, const char *name, int argc, char **argv)
{
  LttvModule *module;

  g_log(G_LOG_DOMAIN, G_LOG_LEVEL_INFO, 
      "Load module %s, as %s is a dependent requiring it", name, 
      g_module_name(m->module));
  module = module_load(name, argc, argv);
  if(module != NULL) g_ptr_array_add(module->dependents, m);
  return module;
}


static void module_unload(LttvModule *m) 
{
  LttvModuleDestroy destroy_function;

  char *pathname;

  guint i, len;

  /* Decrement the reference count */

  g_log(G_LOG_DOMAIN, G_LOG_LEVEL_INFO, "Unload module %s", 
      g_module_name(m->module));
  m->ref_count--;
  if(m->ref_count > 0) {
    g_log(G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG, 
        "Module usage count decremented to %d", m->ref_count);
    return;
  }
  /* We really have to unload the module, first unload its dependents */

  len = m->dependents->len;
  g_log(G_LOG_DOMAIN, G_LOG_LEVEL_INFO, "Unload dependent modules");

  for(i = 0 ; i < len ; i++) {
    module_unload(m->dependents->pdata[i]);
  }

  if(len != m->dependents->len) g_error("dependents list modified");

  /* Unload the module itself */

  g_log(G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG, 
      "Call the destroy function and unload the module");
  if(!g_module_symbol(m->module, "destroy", (gpointer)&destroy_function)) {
    g_warning("module (%s) has no destroy function", pathname);
  }
  else destroy_function();

  g_hash_table_remove(modules, g_module_name(m->module));
  g_ptr_array_free(m->dependents, TRUE);
  g_module_close(m->module);
  g_free(m);
}


void lttv_module_unload(LttvModule *m) 
{
  g_log(G_LOG_DOMAIN, G_LOG_LEVEL_INFO, "Explicitly unload module %s", 
      g_module_name(m->module));
  if(m->load_count <= 0) { 
    g_error("more unload than load (%s)", g_module_name(m->module));
    return;
  }
  m->load_count--;
  module_unload(m);
}


static void
list_modules(gpointer key, gpointer value, gpointer user_data)
{
  g_ptr_array_add((GPtrArray *)user_data, value);
}


LttvModule **
lttv_module_list(guint *nb)
{
  GPtrArray *list = g_ptr_array_new();

  LttvModule **array;

  g_hash_table_foreach(modules, list_modules, list);
  *nb = list->len;
  array = (LttvModule **)list->pdata;
  g_ptr_array_free(list, FALSE);
  return array;
}


LttvModule **
lttv_module_info(LttvModule *m, const char **name, 
    guint *ref_count, guint *load_count, guint *nb_dependents)
{
  guint i, len = m->dependents->len;

  LttvModule **array = g_new(LttvModule *, len);

  *name = g_module_name(m->module);
  *ref_count = m->ref_count;
  *load_count = m->load_count;
  *nb_dependents = len;
  for(i = 0 ; i < len ; i++) array[i] = m->dependents->pdata[i];
  return array;
}

char * 
lttv_module_name(LttvModule *m)
{
  return g_module_name(m->module);
}

static void
list_independent(gpointer key, gpointer value, gpointer user_data)
{
  LttvModule *m = (LttvModule *)value;

  if(m->load_count > 0) g_ptr_array_add((GPtrArray *)user_data, m);
}


void
lttv_module_unload_all()
{
  guint i;

  LttvModule *m;

  GPtrArray *independent_modules = g_ptr_array_new();

  g_hash_table_foreach(modules, list_independent, independent_modules);

  for(i = 0 ; i < independent_modules->len ; i++) {
    m = (LttvModule *)independent_modules->pdata[i];
    while(m->load_count > 0) lttv_module_unload(m);
  }

  g_ptr_array_free(independent_modules, TRUE);
  if(g_hash_table_size(modules) != 0) g_warning("cannot unload all modules"); 
}
