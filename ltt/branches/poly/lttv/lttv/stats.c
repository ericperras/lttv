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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <lttv/module.h>
#include <lttv/stats.h>
#include <lttv/lttv.h>
#include <lttv/attribute.h>
#include <ltt/facility.h>
#include <ltt/trace.h>
#include <ltt/event.h>
#include <ltt/type.h>

#define BUF_SIZE 256
#define MAX_64_HEX_STRING_LEN 19

GQuark
  LTTV_STATS_PROCESS_UNKNOWN,
  LTTV_STATS_PROCESSES,
  LTTV_STATS_CPU,
  LTTV_STATS_MODE_TYPES,
  LTTV_STATS_MODES,
  LTTV_STATS_SUBMODES,
	LTTV_STATS_FUNCTIONS,
  LTTV_STATS_EVENT_TYPES,
  LTTV_STATS_CPU_TIME,
  LTTV_STATS_CUMULATIVE_CPU_TIME,
  LTTV_STATS_ELAPSED_TIME,
  LTTV_STATS_EVENTS,
  LTTV_STATS_EVENTS_COUNT,
  LTTV_STATS_USE_COUNT,
  LTTV_STATS,
  LTTV_STATS_TRACEFILES,
  LTTV_STATS_SUMMED,
  LTTV_STATS_BEFORE_HOOKS,
  LTTV_STATS_AFTER_HOOKS;

static void
find_event_tree(LttvTracefileStats *tfcs, GQuark pid_time, guint cpu,
		guint64 function,
    GQuark mode, GQuark sub_mode, LttvAttribute **events_tree, 
    LttvAttribute **event_types_tree);


static void lttv_stats_init(LttvTracesetStats *self)
{
  guint i, j, nb_trace, nb_tracefile;

  LttvTraceContext *tc;

  LttvTraceStats *tcs;

  LttvTracefileContext *tfc;

  LttvTracefileContext **tfs;
  LttvTracefileStats *tfcs;
  
  LttTime timestamp = {0,0};

  LttvAttributeValue v;

  LttvAttribute
    *stats_tree,
    *tracefiles_stats;

  LttvTraceset *ts = self->parent.parent.ts;

  self->stats = lttv_attribute_find_subdir(
                      lttv_traceset_attribute(self->parent.parent.ts),
                      LTTV_STATS);
  lttv_attribute_find(lttv_traceset_attribute(self->parent.parent.ts),
                      LTTV_STATS_USE_COUNT, 
                      LTTV_UINT, &v);

  (*(v.v_uint))++;
  if(*(v.v_uint) == 1) { 
    g_assert(lttv_attribute_get_number(self->stats) == 0);
  }

  nb_trace = lttv_traceset_number(ts);

  for(i = 0 ; i < nb_trace ; i++) {
    tc = self->parent.parent.traces[i];
    tcs = LTTV_TRACE_STATS(tc);

    tcs->stats = lttv_attribute_find_subdir(tcs->parent.parent.t_a,LTTV_STATS);
    tracefiles_stats = lttv_attribute_find_subdir(tcs->parent.parent.t_a, 
          LTTV_STATS_TRACEFILES);
    lttv_attribute_find(tcs->parent.parent.t_a, LTTV_STATS_USE_COUNT, 
        LTTV_UINT, &v);

    (*(v.v_uint))++;
    if(*(v.v_uint) == 1) { 
      g_assert(lttv_attribute_get_number(tcs->stats) == 0);
    }

    nb_tracefile = tc->tracefiles->len;

    for(j = 0 ; j < nb_tracefile ; j++) {
      tfs = &g_array_index(tc->tracefiles,
                      LttvTracefileContext*, j);
      tfcs = LTTV_TRACEFILE_STATS(*tfs);
      tfcs->stats = lttv_attribute_find_subdir(tracefiles_stats, 
          ltt_tracefile_long_name(tfcs->parent.parent.tf));
  		guint cpu = tfcs->parent.cpu;
      find_event_tree(tfcs, LTTV_STATS_PROCESS_UNKNOWN,
					cpu,
					0x0ULL,
          LTTV_STATE_MODE_UNKNOWN, 
          LTTV_STATE_SUBMODE_UNKNOWN, &tfcs->current_events_tree,
          &tfcs->current_event_types_tree);
    }
  }

}

static void lttv_stats_fini(LttvTracesetStats *self)
{
  guint i, j, nb_trace, nb_tracefile;

  LttvTraceset *ts;

  LttvTraceContext *tc;

  LttvTraceStats *tcs;

  LttvTracefileContext *tfc;

  LttvTracefileStats *tfcs;
  
  LttTime timestamp = {0,0};

  LttvAttributeValue v;

  LttvAttribute *tracefiles_stats;

  lttv_attribute_find(self->parent.parent.ts_a, LTTV_STATS_USE_COUNT, 
        LTTV_UINT, &v);
  (*(v.v_uint))--;

  if(*(v.v_uint) == 0) {
    lttv_attribute_remove_by_name(self->parent.parent.ts_a, LTTV_STATS);
  }
  self->stats = NULL;

  ts = self->parent.parent.ts;
  nb_trace = lttv_traceset_number(ts);

  for(i = 0 ; i < nb_trace ; i++) {
    tcs = (LttvTraceStats *)(tc = (LTTV_TRACESET_CONTEXT(self)->traces[i]));

    lttv_attribute_find(tcs->parent.parent.t_a, LTTV_STATS_USE_COUNT, 
        LTTV_UINT, &v);
    (*(v.v_uint))--;

    if(*(v.v_uint) == 0) { 
      lttv_attribute_remove_by_name(tcs->parent.parent.t_a,LTTV_STATS);
      tracefiles_stats = lttv_attribute_find_subdir(tcs->parent.parent.t_a, 
          LTTV_STATS_TRACEFILES);
      lttv_attribute_remove_by_name(tcs->parent.parent.t_a,
          LTTV_STATS_TRACEFILES);
    }
    tcs->stats = NULL;

    nb_tracefile = tc->tracefiles->len;

    for(j = 0 ; j < nb_tracefile ; j++) {
      tfc = g_array_index(tc->tracefiles,
                                  LttvTracefileContext*, j);
      tfcs = (LttvTracefileStats *)tfc;
      tfcs->stats = NULL;
      tfcs->current_events_tree = NULL;
      tfcs->current_event_types_tree = NULL;
    }
  }
}


void lttv_stats_reset(LttvTracesetStats *self)
{
  lttv_stats_fini(self);
  lttv_stats_init(self);
}



static void
init(LttvTracesetStats *self, LttvTraceset *ts)
{
  LTTV_TRACESET_CONTEXT_CLASS(g_type_class_peek(LTTV_TRACESET_STATE_TYPE))->
      init((LttvTracesetContext *)self, ts);
  
  lttv_stats_init(self);
}


static void
fini(LttvTracesetStats *self)
{
  lttv_stats_fini(self);

  LTTV_TRACESET_CONTEXT_CLASS(g_type_class_peek(LTTV_TRACESET_STATE_TYPE))->
      fini((LttvTracesetContext *)self);
}


static LttvTracesetContext *
new_traceset_context(LttvTracesetContext *self)
{
  return LTTV_TRACESET_CONTEXT(g_object_new(LTTV_TRACESET_STATS_TYPE, NULL));
}


static LttvTraceContext * 
new_trace_context(LttvTracesetContext *self)
{
  return LTTV_TRACE_CONTEXT(g_object_new(LTTV_TRACE_STATS_TYPE, NULL));
}


static LttvTracefileContext *
new_tracefile_context(LttvTracesetContext *self)
{
  return LTTV_TRACEFILE_CONTEXT(g_object_new(LTTV_TRACEFILE_STATS_TYPE, NULL));
}


static void
traceset_stats_instance_init (GTypeInstance *instance, gpointer g_class)
{
}


static void
traceset_stats_finalize (LttvTracesetStats *self)
{
  G_OBJECT_CLASS(g_type_class_peek(LTTV_TRACESET_STATE_TYPE))->
      finalize(G_OBJECT(self));
}


static void
traceset_stats_class_init (LttvTracesetContextClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS(klass);

  gobject_class->finalize = (void (*)(GObject *self)) traceset_stats_finalize;
  klass->init = (void (*)(LttvTracesetContext *self, LttvTraceset *ts))init;
  klass->fini = (void (*)(LttvTracesetContext *self))fini;
  klass->new_traceset_context = new_traceset_context;
  klass->new_trace_context = new_trace_context;
  klass->new_tracefile_context = new_tracefile_context;
}


GType 
lttv_traceset_stats_get_type(void)
{
  static GType type = 0;
  if (type == 0) {
    static const GTypeInfo info = {
      sizeof (LttvTracesetStatsClass),
      NULL,   /* base_init */
      NULL,   /* base_finalize */
      (GClassInitFunc) traceset_stats_class_init,   /* class_init */
      NULL,   /* class_finalize */
      NULL,   /* class_data */
      sizeof (LttvTracesetStats),
      0,      /* n_preallocs */
      (GInstanceInitFunc) traceset_stats_instance_init,    /* instance_init */
      NULL    /* Value handling */
    };

    type = g_type_register_static (LTTV_TRACESET_STATE_TYPE,
                                   "LttvTracesetStatsType", 
                                   &info, 0);
  }
  return type;
}


static void
trace_stats_instance_init (GTypeInstance *instance, gpointer g_class)
{
}


static void
trace_stats_finalize (LttvTraceStats *self)
{
  G_OBJECT_CLASS(g_type_class_peek(LTTV_TRACE_STATE_TYPE))->
      finalize(G_OBJECT(self));
}


static void
trace_stats_class_init (LttvTraceContextClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS(klass);

  gobject_class->finalize = (void (*)(GObject *self)) trace_stats_finalize;
}


GType 
lttv_trace_stats_get_type(void)
{
  static GType type = 0;
  if (type == 0) {
    static const GTypeInfo info = {
      sizeof (LttvTraceStatsClass),
      NULL,   /* base_init */
      NULL,   /* base_finalize */
      (GClassInitFunc) trace_stats_class_init,   /* class_init */
      NULL,   /* class_finalize */
      NULL,   /* class_data */
      sizeof (LttvTraceStats),
      0,      /* n_preallocs */
      (GInstanceInitFunc) trace_stats_instance_init,    /* instance_init */
      NULL    /* Value handling */
    };

    type = g_type_register_static (LTTV_TRACE_STATE_TYPE, 
        "LttvTraceStatsType", &info, 0);
  }
  return type;
}


static void
tracefile_stats_instance_init (GTypeInstance *instance, gpointer g_class)
{
}


static void
tracefile_stats_finalize (LttvTracefileStats *self)
{
  G_OBJECT_CLASS(g_type_class_peek(LTTV_TRACEFILE_STATE_TYPE))->
      finalize(G_OBJECT(self));
}


static void
tracefile_stats_class_init (LttvTracefileStatsClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS(klass);

  gobject_class->finalize = (void (*)(GObject *self)) tracefile_stats_finalize;
}


GType 
lttv_tracefile_stats_get_type(void)
{
  static GType type = 0;
  if (type == 0) {
    static const GTypeInfo info = {
      sizeof (LttvTracefileStatsClass),
      NULL,   /* base_init */
      NULL,   /* base_finalize */
      (GClassInitFunc) tracefile_stats_class_init,   /* class_init */
      NULL,   /* class_finalize */
      NULL,   /* class_data */
      sizeof (LttvTracefileStats),
      0,      /* n_preallocs */
      (GInstanceInitFunc) tracefile_stats_instance_init,    /* instance_init */
      NULL    /* Value handling */
    };

    type = g_type_register_static (LTTV_TRACEFILE_STATE_TYPE, 
        "LttvTracefileStatsType", &info, 0);
  }
  return type;
}


static void
find_event_tree(LttvTracefileStats *tfcs,
                GQuark pid_time,
                guint cpu,
								guint64 function,
                GQuark mode,
                GQuark sub_mode,
                LttvAttribute **events_tree, 
                LttvAttribute **event_types_tree)
{
  LttvAttribute *a, *prev_a;
	gchar fstring[MAX_64_HEX_STRING_LEN];
	gint ret;

	ret = snprintf(fstring, MAX_64_HEX_STRING_LEN-1,
				"0x%llX", function) > 0;
	g_assert(ret > 0);
	fstring[MAX_64_HEX_STRING_LEN-1] = '\0';

  LttvTraceStats *tcs = (LttvTraceStats*)tfcs->parent.parent.t_context;
  a = lttv_attribute_find_subdir(tcs->stats, LTTV_STATS_PROCESSES);
  a = lttv_attribute_find_subdir(a, pid_time);
  a = lttv_attribute_find_subdir(a, LTTV_STATS_CPU);
  a = lttv_attribute_find_subdir_unnamed(a, cpu);
  a = lttv_attribute_find_subdir(a, LTTV_STATS_FUNCTIONS);
  a = lttv_attribute_find_subdir(a, g_quark_from_string(fstring));
  a = lttv_attribute_find_subdir(a, LTTV_STATS_MODE_TYPES);
  a = lttv_attribute_find_subdir(a, mode);
  a = lttv_attribute_find_subdir(a, LTTV_STATS_SUBMODES);
  a = lttv_attribute_find_subdir(a, sub_mode);
  *events_tree = a;
  a = lttv_attribute_find_subdir(a, LTTV_STATS_EVENT_TYPES);
  *event_types_tree = a;
}


static void update_event_tree(LttvTracefileStats *tfcs) 
{
  LttvTraceState *ts = (LttvTraceState *)tfcs->parent.parent.t_context;
  guint cpu = tfcs->parent.cpu;
  LttvProcessState *process = ts->running_process[cpu];
  LttvExecutionState *es = process->state;

  find_event_tree(tfcs, process->pid_time,
      cpu,
			process->current_function,
      es->t, es->n, &(tfcs->current_events_tree), 
      &(tfcs->current_event_types_tree));
}


static void mode_change(LttvTracefileStats *tfcs)
{
  LttvTraceState *ts = (LttvTraceState *)tfcs->parent.parent.t_context;
  guint cpu = tfcs->parent.cpu;
  LttvProcessState *process = ts->running_process[cpu];
  LttvAttributeValue cpu_time, cum_cpu_time;

  LttTime delta;

  delta = ltt_time_sub(tfcs->parent.parent.timestamp, 
      process->state->change);

  lttv_attribute_find(tfcs->current_events_tree, LTTV_STATS_CPU_TIME, 
      LTTV_TIME, &cpu_time);
  *(cpu_time.v_time) = ltt_time_add(*(cpu_time.v_time), delta);

  process->state->cum_cpu_time = ltt_time_add(process->state->cum_cpu_time,
			delta);
}

/* Note : every mode_end must come with a cumulative cpu time update in the
 * after hook */
static void mode_end(LttvTracefileStats *tfcs)
{
  LttvTraceState *ts = (LttvTraceState *)tfcs->parent.parent.t_context;
  guint cpu = tfcs->parent.cpu;
  LttvProcessState *process = ts->running_process[cpu];
  LttvAttributeValue elapsed_time, cpu_time, cum_cpu_time; 

  LttTime delta;

  lttv_attribute_find(tfcs->current_events_tree, LTTV_STATS_ELAPSED_TIME, 
      LTTV_TIME, &elapsed_time);
  delta = ltt_time_sub(tfcs->parent.parent.timestamp, 
      process->state->entry);
  *(elapsed_time.v_time) = ltt_time_add(*(elapsed_time.v_time), delta);

  lttv_attribute_find(tfcs->current_events_tree, LTTV_STATS_CPU_TIME, 
      LTTV_TIME, &cpu_time);
  delta = ltt_time_sub(tfcs->parent.parent.timestamp, 
      process->state->change);
  *(cpu_time.v_time) = ltt_time_add(*(cpu_time.v_time), delta);
  process->state->cum_cpu_time = ltt_time_add(process->state->cum_cpu_time,
			delta);

  lttv_attribute_find(tfcs->current_events_tree, LTTV_STATS_CUMULATIVE_CPU_TIME,
      LTTV_TIME, &cum_cpu_time);
  *(cum_cpu_time.v_time) = ltt_time_add(*(cum_cpu_time.v_time), 
			process->state->cum_cpu_time);
}


static void after_mode_end(LttvTracefileStats *tfcs)
{
  LttvTraceState *ts = (LttvTraceState *)tfcs->parent.parent.t_context;
  guint cpu = tfcs->parent.cpu;
  LttvProcessState *process = ts->running_process[cpu];
  LttvAttributeValue cum_cpu_time;

  LttTime nested_delta;

  nested_delta = process->state->cum_cpu_time;
  process->state->cum_cpu_time = ltt_time_zero;	/* For after traceset hook */

  update_event_tree(tfcs);

  process->state->cum_cpu_time = ltt_time_add(process->state->cum_cpu_time,
			nested_delta);
}

static gboolean before_syscall_entry(void *hook_data, void *call_data)
{
  mode_change((LttvTracefileStats *)call_data);
  return FALSE;
}


static gboolean after_syscall_entry(void *hook_data, void *call_data)
{
  update_event_tree((LttvTracefileStats *)call_data);
  return FALSE;
}


gboolean before_syscall_exit(void *hook_data, void *call_data)
{
  mode_end((LttvTracefileStats *)call_data);
  return FALSE;
}


static gboolean after_syscall_exit(void *hook_data, void *call_data)
{
  after_mode_end((LttvTracefileStats *)call_data);
  return FALSE;
}


gboolean before_trap_entry(void *hook_data, void *call_data)
{
  mode_change((LttvTracefileStats *)call_data);
  return FALSE;
}


static gboolean after_trap_entry(void *hook_data, void *call_data)
{
  update_event_tree((LttvTracefileStats *)call_data);
  return FALSE;
}


gboolean before_trap_exit(void *hook_data, void *call_data)
{
  mode_end((LttvTracefileStats *)call_data);
  return FALSE;
}


gboolean after_trap_exit(void *hook_data, void *call_data)
{
  after_mode_end((LttvTracefileStats *)call_data);
  return FALSE;
}


gboolean before_irq_entry(void *hook_data, void *call_data)
{
  mode_change((LttvTracefileStats *)call_data);
  return FALSE;
}

gboolean after_irq_entry(void *hook_data, void *call_data)
{
  update_event_tree((LttvTracefileStats *)call_data);
  return FALSE;
}


gboolean before_irq_exit(void *hook_data, void *call_data)
{
  mode_end((LttvTracefileStats *)call_data);
  return FALSE;
}


gboolean after_irq_exit(void *hook_data, void *call_data)
{
  after_mode_end((LttvTracefileStats *)call_data);
  return FALSE;
}


gboolean before_soft_irq_entry(void *hook_data, void *call_data)
{
  mode_change((LttvTracefileStats *)call_data);
  return FALSE;
}

gboolean after_soft_irq_entry(void *hook_data, void *call_data)
{
  update_event_tree((LttvTracefileStats *)call_data);
  return FALSE;
}


gboolean before_soft_irq_exit(void *hook_data, void *call_data)
{
  mode_end((LttvTracefileStats *)call_data);
  return FALSE;
}


gboolean after_soft_irq_exit(void *hook_data, void *call_data)
{
  after_mode_end((LttvTracefileStats *)call_data);
  return FALSE;
}

gboolean before_function_entry(void *hook_data, void *call_data)
{
  mode_change((LttvTracefileStats *)call_data);
  return FALSE;
}

gboolean after_function_entry(void *hook_data, void *call_data)
{
  update_event_tree((LttvTracefileStats *)call_data);
  return FALSE;
}

gboolean before_function_exit(void *hook_data, void *call_data)
{
  mode_end((LttvTracefileStats *)call_data);
  return FALSE;
}

gboolean after_function_exit(void *hook_data, void *call_data)
{
  after_mode_end((LttvTracefileStats *)call_data);
  return FALSE;
}


gboolean before_schedchange(void *hook_data, void *call_data)
{
  LttvTracefileStats *tfcs = (LttvTracefileStats *)call_data;

  LttvTraceState *ts = (LttvTraceState*)tfcs->parent.parent.t_context;

  LttEvent *e = ltt_tracefile_get_event(tfcs->parent.parent.tf);

  LttvTraceHookByFacility *thf = (LttvTraceHookByFacility *)hook_data;

  guint pid_in, pid_out;
    
  gint state_out;

  pid_out = ltt_event_get_unsigned(e, thf->f1);
  pid_in = ltt_event_get_unsigned(e, thf->f2);
  state_out = ltt_event_get_int(e, thf->f3);

  /* compute the time for the process to schedule out */

  mode_change(tfcs);

  return FALSE;
}

gboolean after_schedchange(void *hook_data, void *call_data)
{
  LttvTracefileStats *tfcs = (LttvTracefileStats *)call_data;

  LttvTraceState *ts = (LttvTraceState*)tfcs->parent.parent.t_context;

  LttEvent *e = ltt_tracefile_get_event(tfcs->parent.parent.tf);

  LttvTraceHookByFacility *thf = (LttvTraceHookByFacility *)hook_data;

  guint pid_in, pid_out;
    
  gint state_out;

  LttvProcessState *process;

  pid_out = ltt_event_get_unsigned(e, thf->f1);
  pid_in = ltt_event_get_unsigned(e, thf->f2);
  state_out = ltt_event_get_int(e, thf->f3);

  /* get the information for the process scheduled in */
  guint cpu = tfcs->parent.cpu;
  process = ts->running_process[cpu];

  find_event_tree(tfcs, process->pid_time,
      cpu,
			process->current_function,
      process->state->t, process->state->n, &(tfcs->current_events_tree), 
      &(tfcs->current_event_types_tree));

  /* compute the time waiting for the process to schedule in */
  mode_change(tfcs);

	return FALSE;
}

gboolean process_fork(void *hook_data, void *call_data)
{
  /* nothing to do for now */
  return FALSE;
}


gboolean process_exit(void *hook_data, void *call_data)
{
  /* We should probably exit all modes here or we could do that at 
     schedule out. */
  return FALSE;
}

gboolean before_enum_process_state(void *hook_data, void *call_data)
{
  mode_end((LttvTracefileStats *)call_data);
  after_mode_end((LttvTracefileStats *)call_data);
  mode_change((LttvTracefileStats *)call_data);
  return FALSE;
}

gboolean after_enum_process_state(void *hook_data, void *call_data)
{
  update_event_tree((LttvTracefileStats *)call_data);
  return FALSE;
}

gboolean process_free(void *hook_data, void *call_data)
{
  return FALSE;
}

gboolean every_event(void *hook_data, void *call_data)
{
  LttvTracefileStats *tfcs = (LttvTracefileStats *)call_data;

  LttEvent *e = ltt_tracefile_get_event(tfcs->parent.parent.tf);

  LttvAttributeValue v;

  /* The current branch corresponds to the tracefile/process/interrupt state.
     Statistics are added within it, to count the number of events of this
     type occuring in this context. A quark has been pre-allocated for each
     event type and is used as name. */

  lttv_attribute_find(tfcs->current_event_types_tree, 
      ltt_eventtype_name(ltt_event_eventtype(e)), 
      LTTV_UINT, &v);
  (*(v.v_uint))++;
  return FALSE;
}

static void lttv_stats_cleanup_process_state(gpointer key, gpointer value,
    gpointer user_data)
{
  LttvTraceStats *tcs = (LttvTraceStats *)user_data;
  LttvTraceState *ts = (LttvTraceState *)user_data;
  LttvTracesetContext *tsc = ts->parent.ts_context;
  LttvProcessState *process = (LttvProcessState *)value;
  int i;
  LttvTracefileStats **tfs = (LttvTracefileStats **)
      &g_array_index(ts->parent.tracefiles, LttvTracefileContext*,
          process->cpu);
  int cleanup_empty = 0;
  LttTime nested_delta = ltt_time_zero;
  /* FIXME : ok, this is a hack. The time is infinite here :( */
  LttTime save_time = (*tfs)->parent.parent.timestamp;
  LttTime start, end;
  ltt_trace_time_span_get(ts->parent.t, &start, &end);
  (*tfs)->parent.parent.timestamp = end;

  do {
    if(ltt_time_compare(process->state->cum_cpu_time, ltt_time_zero) != 0) {
      find_event_tree(*tfs, process->pid_time,
          process->cpu,
          process->current_function,
          process->state->t, process->state->n, &((*tfs)->current_events_tree), 
          &((*tfs)->current_event_types_tree));
      /* if it is a running mode, we must count its cpu time */
      if(process->state->s == LTTV_STATE_RUN)
        mode_end(*tfs);
      nested_delta = process->state->cum_cpu_time;
    }
    cleanup_empty = lttv_state_pop_state_cleanup(process,
        (LttvTracefileState *)*tfs);
    process->state->cum_cpu_time = ltt_time_add(process->state->cum_cpu_time,
        nested_delta);

  } while(cleanup_empty != 1);

  (*tfs)->parent.parent.timestamp = save_time;
}

/* For each process in the state, for each of their stacked states,
 * perform sum of needed values. */
static void lttv_stats_cleanup_state(LttvTraceStats *tcs)
{
  LttvTraceState *ts = (LttvTraceState *)tcs;
  
  /* Does not work correctly FIXME. */
  g_hash_table_foreach(ts->processes, lttv_stats_cleanup_process_state,
      tcs);
}

void
lttv_stats_sum_trace(LttvTraceStats *self, LttvAttribute *ts_stats)
{
  LttvAttribute *sum_container = self->stats;

  LttvAttributeType type;

  LttvAttributeValue value;

  LttvAttributeName name;

	gboolean is_named;

  unsigned sum;

	int trace_is_summed;

  int i, j, k, l, m, nb_process, nb_cpu, nb_mode_type, nb_submode,
      nb_event_type, nf, nb_functions;

  LttvAttribute *main_tree, *processes_tree, *process_tree, *cpus_tree,
      *cpu_tree, *mode_tree, *mode_types_tree, *submodes_tree,
      *submode_tree, *event_types_tree, *mode_events_tree,
      *cpu_functions_tree,
			*function_tree,
			*function_mode_types_tree,
			*trace_cpu_tree;


  main_tree = sum_container;

  lttv_attribute_find(sum_container,
                      LTTV_STATS_SUMMED, 
                      LTTV_UINT, &value);
	trace_is_summed = *(value.v_uint);
  *(value.v_uint) = 1;

	/* First cleanup the state : sum all stalled information (never ending
	 * states). */
	if(!trace_is_summed)
		lttv_stats_cleanup_state(self);
	
  processes_tree = lttv_attribute_find_subdir(main_tree, 
                                              LTTV_STATS_PROCESSES);
  nb_process = lttv_attribute_get_number(processes_tree);

  for(i = 0 ; i < nb_process ; i++) {
    type = lttv_attribute_get(processes_tree, i, &name, &value, &is_named);
    process_tree = LTTV_ATTRIBUTE(*(value.v_gobject));

    cpus_tree = lttv_attribute_find_subdir(process_tree, LTTV_STATS_CPU);
    nb_cpu = lttv_attribute_get_number(cpus_tree);

    for(j = 0 ; j < nb_cpu ; j++) {
      type = lttv_attribute_get(cpus_tree, j, &name, &value, &is_named);
      cpu_tree = LTTV_ATTRIBUTE(*(value.v_gobject));

      trace_cpu_tree = lttv_attribute_find_subdir(main_tree, LTTV_STATS_CPU);
      trace_cpu_tree = lttv_attribute_find_subdir_unnamed(trace_cpu_tree, name);
			cpu_functions_tree = lttv_attribute_find_subdir(cpu_tree,
																											LTTV_STATS_FUNCTIONS);
    	nb_functions = lttv_attribute_get_number(cpu_functions_tree);
			
			for(nf=0; nf < nb_functions; nf++) {
				type = lttv_attribute_get(cpu_functions_tree, nf, &name, &value,
						&is_named);
				function_tree = LTTV_ATTRIBUTE(*(value.v_gobject));
				function_mode_types_tree = lttv_attribute_find_subdir(function_tree,
						LTTV_STATS_MODE_TYPES);
      	nb_mode_type = lttv_attribute_get_number(function_mode_types_tree);
				for(k = 0 ; k < nb_mode_type ; k++) {
					type = lttv_attribute_get(function_mode_types_tree, k, &name, &value,
							&is_named);
					mode_tree = LTTV_ATTRIBUTE(*(value.v_gobject));

					submodes_tree = lttv_attribute_find_subdir(mode_tree, 
							LTTV_STATS_SUBMODES);
					mode_events_tree = lttv_attribute_find_subdir(mode_tree,
							LTTV_STATS_EVENTS);
					mode_types_tree = lttv_attribute_find_subdir(mode_tree, 
							LTTV_STATS_MODE_TYPES);

					nb_submode = lttv_attribute_get_number(submodes_tree);

					for(l = 0 ; l < nb_submode ; l++) {
						type = lttv_attribute_get(submodes_tree, l, &name, &value,
								&is_named);
						submode_tree = LTTV_ATTRIBUTE(*(value.v_gobject));

						event_types_tree = lttv_attribute_find_subdir(submode_tree, 
							LTTV_STATS_EVENT_TYPES);
						nb_event_type = lttv_attribute_get_number(event_types_tree);

						sum = 0;
						for(m = 0 ; m < nb_event_type ; m++) {
							type = lttv_attribute_get(event_types_tree, m, &name, &value,
									&is_named);
							sum += *(value.v_uint);
						}
						lttv_attribute_find(submode_tree, LTTV_STATS_EVENTS_COUNT, 
								LTTV_UINT, &value);
						*(value.v_uint) = sum;

						type = lttv_attribute_get(submodes_tree, l, &name, &value,
								&is_named);
						submode_tree = LTTV_ATTRIBUTE(*(value.v_gobject));
						if(!trace_is_summed) {
							lttv_attribute_recursive_add(mode_events_tree, event_types_tree);
							lttv_attribute_recursive_add(mode_types_tree, submode_tree);
						}
					}
					if(!trace_is_summed) {
						lttv_attribute_recursive_add(function_tree, mode_types_tree);
					}
				}
				if(!trace_is_summed) {
					lttv_attribute_recursive_add(cpu_tree, function_tree);
					lttv_attribute_recursive_add(process_tree, function_tree);
					lttv_attribute_recursive_add(trace_cpu_tree, function_tree);
					lttv_attribute_recursive_add(main_tree, function_tree);
				}
				lttv_attribute_recursive_add(ts_stats, function_tree);
			}
    }
  }
}


gboolean lttv_stats_sum_traceset_hook(void *hook_data, void *call_data)
{
  lttv_stats_sum_traceset((LttvTracesetStats *)call_data);
  return 0;
}

void
lttv_stats_sum_traceset(LttvTracesetStats *self)
{
  LttvTraceset *traceset = self->parent.parent.ts;
  LttvAttribute *sum_container = self->stats;

  LttvTraceStats *tcs;

  int i, nb_trace;

  LttvAttribute *main_tree;

  LttvAttributeValue value;

  lttv_attribute_find(sum_container, LTTV_STATS_SUMMED, 
      LTTV_UINT, &value);
  if(*(value.v_uint) != 0) return;
  *(value.v_uint) = 1;

  nb_trace = lttv_traceset_number(traceset);

  for(i = 0 ; i < nb_trace ; i++) {
    tcs = (LttvTraceStats *)(self->parent.parent.traces[i]);
    lttv_stats_sum_trace(tcs, self->stats);
	//				lttv_attribute_recursive_add(sum_container, tcs->stats);
  }
}


// Hook wrapper. call_data is a traceset context.
gboolean lttv_stats_hook_add_event_hooks(void *hook_data, void *call_data)
{
   LttvTracesetStats *tss = (LttvTracesetStats*)call_data;

   lttv_stats_add_event_hooks(tss);

   return 0;
}

void lttv_stats_add_event_hooks(LttvTracesetStats *self)
{
  LttvTraceset *traceset = self->parent.parent.ts;

  guint i, j, k, l, nb_trace, nb_tracefile;

  LttvTraceStats *ts;

  LttvTracefileStats *tfs;

  GArray *hooks, *before_hooks, *after_hooks;

  LttvTraceHook *hook;

  LttvTraceHookByFacility *thf;

  LttvAttributeValue val;

  gint ret;
	gint hn;

  nb_trace = lttv_traceset_number(traceset);
  for(i = 0 ; i < nb_trace ; i++) {
    ts = (LttvTraceStats *)self->parent.parent.traces[i];

    /* Find the eventtype id for the following events and register the
       associated by id hooks. */

    hooks = g_array_sized_new(FALSE, FALSE, sizeof(LttvTraceHook), 12);
    g_array_set_size(hooks, 12);
    hn=0;

    ret = lttv_trace_find_hook(ts->parent.parent.t,
        LTT_FACILITY_KERNEL_ARCH, LTT_EVENT_SYSCALL_ENTRY,
        LTT_FIELD_SYSCALL_ID, 0, 0,
        before_syscall_entry, NULL, 
        &g_array_index(hooks, LttvTraceHook, hn++));
    if(ret) hn--;

    ret = lttv_trace_find_hook(ts->parent.parent.t,
        LTT_FACILITY_KERNEL_ARCH, LTT_EVENT_SYSCALL_EXIT,
        0, 0, 0,
        before_syscall_exit, NULL, 
        &g_array_index(hooks, LttvTraceHook, hn++));
    if(ret) hn--;

    ret = lttv_trace_find_hook(ts->parent.parent.t,
        LTT_FACILITY_KERNEL, LTT_EVENT_TRAP_ENTRY,
        LTT_FIELD_TRAP_ID, 0, 0,
        before_trap_entry, NULL, 
        &g_array_index(hooks, LttvTraceHook, hn++));
    if(ret) hn--;

    ret = lttv_trace_find_hook(ts->parent.parent.t,
        LTT_FACILITY_KERNEL, LTT_EVENT_TRAP_EXIT,
        0, 0, 0,
        before_trap_exit, NULL,
        &g_array_index(hooks, LttvTraceHook, hn++));
    if(ret) hn--;

    ret = lttv_trace_find_hook(ts->parent.parent.t,
        LTT_FACILITY_KERNEL, LTT_EVENT_IRQ_ENTRY,
        LTT_FIELD_IRQ_ID, 0, 0,
        before_irq_entry, NULL,
        &g_array_index(hooks, LttvTraceHook, hn++));
    if(ret) hn--;

    ret = lttv_trace_find_hook(ts->parent.parent.t,
        LTT_FACILITY_KERNEL, LTT_EVENT_IRQ_EXIT,
        0, 0, 0,
        before_irq_exit, NULL,
        &g_array_index(hooks, LttvTraceHook, hn++));
    if(ret) hn--;

    ret = lttv_trace_find_hook(ts->parent.parent.t,
        LTT_FACILITY_KERNEL, LTT_EVENT_SOFT_IRQ_ENTRY,
        LTT_FIELD_SOFT_IRQ_ID, 0, 0,
        before_soft_irq_entry, NULL,
        &g_array_index(hooks, LttvTraceHook, hn++));
    if(ret) hn--;

    ret = lttv_trace_find_hook(ts->parent.parent.t,
        LTT_FACILITY_KERNEL, LTT_EVENT_SOFT_IRQ_EXIT,
        0, 0, 0,
        before_soft_irq_exit, NULL,
        &g_array_index(hooks, LttvTraceHook, hn++));
    if(ret) hn--;

    ret = lttv_trace_find_hook(ts->parent.parent.t,
        LTT_FACILITY_PROCESS, LTT_EVENT_SCHEDCHANGE,
        LTT_FIELD_OUT, LTT_FIELD_IN, LTT_FIELD_OUT_STATE,
        before_schedchange, NULL, 
        &g_array_index(hooks, LttvTraceHook, hn++));
    if(ret) hn--;

    ret = lttv_trace_find_hook(ts->parent.parent.t,
        LTT_FACILITY_USER_GENERIC, LTT_EVENT_FUNCTION_ENTRY,
        LTT_FIELD_THIS_FN, LTT_FIELD_CALL_SITE, 0,
        before_function_entry, NULL,
        &g_array_index(hooks, LttvTraceHook, hn++));
    if(ret) hn--;

    ret = lttv_trace_find_hook(ts->parent.parent.t,
        LTT_FACILITY_USER_GENERIC, LTT_EVENT_FUNCTION_EXIT,
        LTT_FIELD_THIS_FN, LTT_FIELD_CALL_SITE, 0,
        before_function_exit, NULL,
        &g_array_index(hooks, LttvTraceHook, hn++));
    if(ret) hn--;

    /* statedump-related hooks */
    ret = lttv_trace_find_hook(ts->parent.parent.t,
        LTT_FACILITY_STATEDUMP, LTT_EVENT_ENUM_PROCESS_STATE,
        LTT_FIELD_PID, LTT_FIELD_PARENT_PID, LTT_FIELD_NAME,
        before_enum_process_state, NULL,
        &g_array_index(hooks, LttvTraceHook, hn++));
    if(ret) hn--;

    g_array_set_size(hooks, hn);

    before_hooks = hooks;

    hooks = g_array_sized_new(FALSE, FALSE, sizeof(LttvTraceHook), 15);
    g_array_set_size(hooks, 15);
    hn=0;

    ret = lttv_trace_find_hook(ts->parent.parent.t,
        LTT_FACILITY_KERNEL_ARCH, LTT_EVENT_SYSCALL_ENTRY,
        LTT_FIELD_SYSCALL_ID, 0, 0,
        after_syscall_entry, NULL, 
        &g_array_index(hooks, LttvTraceHook, hn++));
    if(ret) hn--;

    ret = lttv_trace_find_hook(ts->parent.parent.t,
        LTT_FACILITY_KERNEL_ARCH, LTT_EVENT_SYSCALL_EXIT,
        0, 0, 0,
        after_syscall_exit, NULL, 
        &g_array_index(hooks, LttvTraceHook, hn++));
    if(ret) hn--;

    ret = lttv_trace_find_hook(ts->parent.parent.t,
        LTT_FACILITY_KERNEL, LTT_EVENT_TRAP_ENTRY, 
        LTT_FIELD_TRAP_ID, 0, 0,
        after_trap_entry, NULL,
        &g_array_index(hooks, LttvTraceHook, hn++));
    if(ret) hn--;

    ret = lttv_trace_find_hook(ts->parent.parent.t,
        LTT_FACILITY_KERNEL, LTT_EVENT_TRAP_EXIT,
        0, 0, 0,
        after_trap_exit, NULL,
        &g_array_index(hooks, LttvTraceHook, hn++));
    if(ret) hn--;

    ret = lttv_trace_find_hook(ts->parent.parent.t,
        LTT_FACILITY_KERNEL, LTT_EVENT_IRQ_ENTRY, 
        LTT_FIELD_IRQ_ID, 0, 0,
        after_irq_entry, NULL,
        &g_array_index(hooks, LttvTraceHook, hn++));
    if(ret) hn--;

    ret = lttv_trace_find_hook(ts->parent.parent.t,
        LTT_FACILITY_KERNEL, LTT_EVENT_IRQ_EXIT,
        0, 0, 0,
        after_irq_exit, NULL,
        &g_array_index(hooks, LttvTraceHook, hn++));
    if(ret) hn--;

    ret = lttv_trace_find_hook(ts->parent.parent.t,
        LTT_FACILITY_KERNEL, LTT_EVENT_SOFT_IRQ_ENTRY, 
        LTT_FIELD_SOFT_IRQ_ID, 0, 0,
        after_irq_entry, NULL,
        &g_array_index(hooks, LttvTraceHook, hn++));
    if(ret) hn--;

    ret = lttv_trace_find_hook(ts->parent.parent.t,
        LTT_FACILITY_KERNEL, LTT_EVENT_SOFT_IRQ_EXIT,
        0, 0, 0,
        after_soft_irq_exit, NULL,
        &g_array_index(hooks, LttvTraceHook, hn++));
    if(ret) hn--;

    ret = lttv_trace_find_hook(ts->parent.parent.t,
        LTT_FACILITY_PROCESS, LTT_EVENT_SCHEDCHANGE,
        LTT_FIELD_OUT, LTT_FIELD_IN, LTT_FIELD_OUT_STATE,
        after_schedchange, NULL, 
        &g_array_index(hooks, LttvTraceHook, hn++));
    if(ret) hn--;

    ret = lttv_trace_find_hook(ts->parent.parent.t,
        LTT_FACILITY_PROCESS, LTT_EVENT_FORK, 
        LTT_FIELD_PARENT_PID, LTT_FIELD_CHILD_PID, 0,
        process_fork, NULL, 
        &g_array_index(hooks, LttvTraceHook, hn++));
    if(ret) hn--;

    ret = lttv_trace_find_hook(ts->parent.parent.t,
        LTT_FACILITY_PROCESS, LTT_EVENT_EXIT,
        LTT_FIELD_PID, 0, 0,
        process_exit, NULL,
        &g_array_index(hooks, LttvTraceHook, hn++));
    if(ret) hn--;
    
    ret = lttv_trace_find_hook(ts->parent.parent.t,
        LTT_FACILITY_PROCESS, LTT_EVENT_FREE,
        LTT_FIELD_PID, 0, 0,
        process_free, NULL,
        &g_array_index(hooks, LttvTraceHook, hn++));
    if(ret) hn--;

    ret = lttv_trace_find_hook(ts->parent.parent.t,
        LTT_FACILITY_USER_GENERIC, LTT_EVENT_FUNCTION_ENTRY,
        LTT_FIELD_THIS_FN, LTT_FIELD_CALL_SITE, 0,
        after_function_entry, NULL,
        &g_array_index(hooks, LttvTraceHook, hn++));
    if(ret) hn--;

    ret = lttv_trace_find_hook(ts->parent.parent.t,
        LTT_FACILITY_USER_GENERIC, LTT_EVENT_FUNCTION_EXIT,
        LTT_FIELD_THIS_FN, LTT_FIELD_CALL_SITE, 0,
        after_function_exit, NULL,
        &g_array_index(hooks, LttvTraceHook, hn++));
    if(ret) hn--;

    /* statedump-related hooks */
    ret = lttv_trace_find_hook(ts->parent.parent.t,
        LTT_FACILITY_STATEDUMP, LTT_EVENT_ENUM_PROCESS_STATE,
        LTT_FIELD_PID, LTT_FIELD_PARENT_PID, LTT_FIELD_NAME,
        after_enum_process_state, NULL,
        &g_array_index(hooks, LttvTraceHook, hn++));
    if(ret) hn--;

    g_array_set_size(hooks, hn);

    after_hooks = hooks;

    /* Add these hooks to each event_by_id hooks list */

    nb_tracefile = ts->parent.parent.tracefiles->len;

    for(j = 0 ; j < nb_tracefile ; j++) {
      tfs = LTTV_TRACEFILE_STATS(g_array_index(ts->parent.parent.tracefiles,
                                  LttvTracefileContext*, j));
      lttv_hooks_add(tfs->parent.parent.event, every_event, NULL, 
                     LTTV_PRIO_DEFAULT);

      for(k = 0 ; k < before_hooks->len ; k++) {
        hook = &g_array_index(before_hooks, LttvTraceHook, k);
        for(l = 0; l<hook->fac_list->len;l++) {
          thf = g_array_index(hook->fac_list, LttvTraceHookByFacility*, l);
          lttv_hooks_add(
              lttv_hooks_by_id_find(tfs->parent.parent.event_by_id, thf->id),
              thf->h,
              thf,
              LTTV_PRIO_STATS_BEFORE_STATE);
        }
      }
      for(k = 0 ; k < after_hooks->len ; k++) {
        hook = &g_array_index(after_hooks, LttvTraceHook, k);
        for(l = 0; l<hook->fac_list->len;l++) {
          thf = g_array_index(hook->fac_list, LttvTraceHookByFacility*, l);
          lttv_hooks_add(
              lttv_hooks_by_id_find(tfs->parent.parent.event_by_id, thf->id),
              thf->h,
              thf,
              LTTV_PRIO_STATS_AFTER_STATE);
        }
      }
    }
    lttv_attribute_find(self->parent.parent.a, LTTV_STATS_BEFORE_HOOKS, 
        LTTV_POINTER, &val);
    *(val.v_pointer) = before_hooks;
    lttv_attribute_find(self->parent.parent.a, LTTV_STATS_AFTER_HOOKS, 
        LTTV_POINTER, &val);
    *(val.v_pointer) = after_hooks;
  }
}

// Hook wrapper. call_data is a traceset context.
gboolean lttv_stats_hook_remove_event_hooks(void *hook_data, void *call_data)
{
   LttvTracesetStats *tss = (LttvTracesetStats*)call_data;

   lttv_stats_remove_event_hooks(tss);

   return 0;
}

void lttv_stats_remove_event_hooks(LttvTracesetStats *self)
{
  LttvTraceset *traceset = self->parent.parent.ts;

  guint i, j, k, l, nb_trace, nb_tracefile;

  LttvTraceStats *ts;

  LttvTracefileStats *tfs;

  void *hook_data;

  GArray *before_hooks, *after_hooks;

  LttvTraceHook *hook;
  
  LttvTraceHookByFacility *thf;

  LttvAttributeValue val;

  nb_trace = lttv_traceset_number(traceset);
  for(i = 0 ; i < nb_trace ; i++) {
    ts = (LttvTraceStats*)self->parent.parent.traces[i];
    lttv_attribute_find(self->parent.parent.a, LTTV_STATS_BEFORE_HOOKS, 
        LTTV_POINTER, &val);
    before_hooks = *(val.v_pointer);
    lttv_attribute_find(self->parent.parent.a, LTTV_STATS_AFTER_HOOKS, 
        LTTV_POINTER, &val);
    after_hooks = *(val.v_pointer);

    /* Remove these hooks from each event_by_id hooks list */

    nb_tracefile = ts->parent.parent.tracefiles->len;

    for(j = 0 ; j < nb_tracefile ; j++) {
      tfs = LTTV_TRACEFILE_STATS(g_array_index(ts->parent.parent.tracefiles,
                                  LttvTracefileContext*, j));
      lttv_hooks_remove_data(tfs->parent.parent.event, every_event, 
          NULL);

      for(k = 0 ; k < before_hooks->len ; k++) {
        hook = &g_array_index(before_hooks, LttvTraceHook, k);
        for(l = 0 ; l < hook->fac_list->len ; l++) {
          thf = g_array_index(hook->fac_list, LttvTraceHookByFacility*, l);
          lttv_hooks_remove_data(
              lttv_hooks_by_id_find(tfs->parent.parent.event_by_id, thf->id),
              thf->h,
              thf);
        }
      }
      for(k = 0 ; k < after_hooks->len ; k++) {
        hook = &g_array_index(after_hooks, LttvTraceHook, k);
        for(l = 0 ; l < hook->fac_list->len ; l++) {
          thf = g_array_index(hook->fac_list, LttvTraceHookByFacility*, l);
          lttv_hooks_remove_data(
              lttv_hooks_by_id_find(tfs->parent.parent.event_by_id, thf->id),
              thf->h,
              thf);
        }
      }
    }
    g_debug("lttv_stats_remove_event_hooks()");
    g_array_free(before_hooks, TRUE);
    g_array_free(after_hooks, TRUE);
  }
}


static void module_init()
{
  LTTV_STATS_PROCESS_UNKNOWN = g_quark_from_string("unknown process");
  LTTV_STATS_PROCESSES = g_quark_from_string("processes");
  LTTV_STATS_CPU = g_quark_from_string("cpu");
  LTTV_STATS_MODE_TYPES = g_quark_from_string("mode_types");
  LTTV_STATS_MODES = g_quark_from_string("modes");
  LTTV_STATS_SUBMODES = g_quark_from_string("submodes");
  LTTV_STATS_FUNCTIONS = g_quark_from_string("functions");
  LTTV_STATS_EVENT_TYPES = g_quark_from_string("event_types");
  LTTV_STATS_CPU_TIME = g_quark_from_string("cpu time");
  LTTV_STATS_CUMULATIVE_CPU_TIME = g_quark_from_string("cumulative cpu time (includes nested routines and modes)");
  LTTV_STATS_ELAPSED_TIME = g_quark_from_string("elapsed time (includes per process waiting time)");
  LTTV_STATS_EVENTS = g_quark_from_string("events");
  LTTV_STATS_EVENTS_COUNT = g_quark_from_string("events count");
  LTTV_STATS_BEFORE_HOOKS = g_quark_from_string("saved stats before hooks");
  LTTV_STATS_AFTER_HOOKS = g_quark_from_string("saved stats after hooks");
  LTTV_STATS_USE_COUNT = g_quark_from_string("stats_use_count");
  LTTV_STATS = g_quark_from_string("statistics");
  LTTV_STATS_TRACEFILES = g_quark_from_string("tracefiles statistics");
  LTTV_STATS_SUMMED = g_quark_from_string("statistics summed");
}

static void module_destroy() 
{
}


LTTV_MODULE("stats", "Compute processes statistics", \
    "Accumulate statistics for event types, processes and CPUs", \
    module_init, module_destroy, "state");

/* Change the places where stats are called (create/read/write stats)

   Check for options in batchtest.c to reduce writing and see what tests are
   best candidates for performance analysis. Once OK, commit, move to main
   and run tests. Update the gui for statistics. */
