/* This file is part of the Linux Trace Toolkit viewer
 * Copyright (C) 2009 Benjamin Poirier <benjamin.poirier@polymtl.ca>
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

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <lttv/module.h>
#include <lttv/option.h>

#include "sync_chain.h"


#ifndef g_info
#define g_info(format...) g_log (G_LOG_DOMAIN, G_LOG_LEVEL_INFO, format)
#endif


static void init();
static void destroy();

static void gfAppendAnalysisName(gpointer data, gpointer user_data);
static void gfAddModuleOption(gpointer data, gpointer user_data);
static void gfRemoveModuleOption(gpointer data, gpointer user_data);

GQueue processingModules= G_QUEUE_INIT;
GQueue matchingModules= G_QUEUE_INIT;
GQueue analysisModules= G_QUEUE_INIT;
GQueue moduleOptions= G_QUEUE_INIT;

static char* argHelpNone= "none";
static ModuleOption optionSync= {
	.longName= "sync",
	.hasArg= NO_ARG,
	{.present= false},
	.optionHelp= "synchronize the time between the traces",
};
static char graphsDir[20];
static ModuleOption optionSyncStats= {
	.longName= "sync-stats",
	.hasArg= NO_ARG,
	{.present= false},
	.optionHelp= "print statistics about the time synchronization",
};
static ModuleOption optionSyncNull= {
	.longName= "sync-null",
	.hasArg= NO_ARG,
	{.present= false},
	.optionHelp= "read the events but do not perform any processing",
};
static GString* analysisModulesNames;
static ModuleOption optionSyncAnalysis= {
	.longName= "sync-analysis",
	.hasArg= REQUIRED_ARG,
	.optionHelp= "specify the algorithm to use for event analysis",
};
static ModuleOption optionSyncGraphs= {
	.longName= "sync-graphs",
	.hasArg= NO_ARG,
	{.present= false},
	.optionHelp= "output gnuplot graph showing synchronization points",
};
static ModuleOption optionSyncGraphsDir= {
	.longName= "sync-graphs-dir",
	.hasArg= REQUIRED_ARG,
	.optionHelp= "specify the directory where to store the graphs",
};

/*
 * Module init function
 *
 * This function is declared to be the module initialization function. Event
 * modules are registered with a "constructor (102)" attribute except one in
 * each class (processing, matching, analysis) which is chosen to be the
 * default and which is registered with a "constructor (101)" attribute.
 * Constructors with no priority are called after constructors with
 * priorities. The result is that the list of event modules is known when this
 * function is executed.
 */
static void init()
{
	int retval;

	g_debug("\t\t\tXXXX sync init\n");

	g_assert(g_queue_get_length(&analysisModules) > 0);
	optionSyncAnalysis.arg = ((AnalysisModule*)
		g_queue_peek_head(&analysisModules))->name;
	analysisModulesNames= g_string_new("");
	g_queue_foreach(&analysisModules, &gfAppendAnalysisName,
		analysisModulesNames);
	// remove the last ", "
	g_string_truncate(analysisModulesNames, analysisModulesNames->len - 2);
	optionSyncAnalysis.argHelp= analysisModulesNames->str;

	retval= snprintf(graphsDir, sizeof(graphsDir), "graphs-%d", getpid());
	if (retval > sizeof(graphsDir) - 1)
	{
		graphsDir[sizeof(graphsDir) - 1]= '\0';
	}
	optionSyncGraphsDir.arg= graphsDir;
	optionSyncGraphsDir.argHelp= graphsDir;

	g_queue_push_head(&moduleOptions, &optionSyncGraphsDir);
	g_queue_push_head(&moduleOptions, &optionSyncGraphs);
	g_queue_push_head(&moduleOptions, &optionSyncAnalysis);
	g_queue_push_head(&moduleOptions, &optionSyncNull);
	g_queue_push_head(&moduleOptions, &optionSyncStats);
	g_queue_push_head(&moduleOptions, &optionSync);

	g_queue_foreach(&moduleOptions, &gfAddModuleOption, NULL);

}


/*
 * Module unload function
 */
static void destroy()
{
	g_debug("\t\t\tXXXX sync destroy\n");

	g_queue_foreach(&moduleOptions, &gfRemoveModuleOption, NULL);
	g_string_free(analysisModulesNames, TRUE);

	g_queue_clear(&processingModules);
	g_queue_clear(&matchingModules);
	g_queue_clear(&analysisModules);
	g_queue_clear(&moduleOptions);
}


/*
 * Calculate a traceset's drift and offset values based on network events
 *
 * The individual correction factors are written out to each trace.
 *
 * Args:
 *   traceSetContext: traceset
 */
void syncTraceset(LttvTracesetContext* const traceSetContext)
{
	SyncState* syncState;
	struct timeval startTime, endTime;
	struct rusage startUsage, endUsage;
	GList* result;
	FILE* graphsStream;
	int retval;

	if (!optionSync.present)
	{
		g_debug("Not synchronizing traceset because option is disabled");
		return;
	}

	if (optionSyncStats.present)
	{
		gettimeofday(&startTime, 0);
		getrusage(RUSAGE_SELF, &startUsage);
	}

	// Initialize data structures
	syncState= malloc(sizeof(SyncState));
	syncState->traceNb= lttv_traceset_number(traceSetContext->ts);

	if (optionSyncStats.present)
	{
		syncState->stats= true;
	}
	else
	{
		syncState->stats= false;
	}

	if (optionSyncGraphs.present)
	{
		syncState->graphs= optionSyncGraphsDir.arg;
	}
	else
	{
		syncState->graphs= NULL;
	}

	// Identify and initialize processing module
	syncState->processingData= NULL;
	if (optionSyncNull.present)
	{
		result= g_queue_find_custom(&processingModules, "LTTV-null",
			&gcfCompareProcessing);
	}
	else
	{
		result= g_queue_find_custom(&processingModules, "LTTV-standard",
			&gcfCompareProcessing);
	}
	g_assert(result != NULL);
	syncState->processingModule= (ProcessingModule*) result->data;

	graphsStream= NULL;
	if (syncState->graphs &&
		syncState->processingModule->writeProcessingGraphsPlots != NULL)
	{
		char* cwd;
		int graphsFp;

		// Create the graph directory right away in case the module initialization
		// functions have something to write in it.
		cwd= changeToGraphDir(syncState->graphs);

		if ((graphsFp= open("graphs.gnu", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR |
				S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH
				| S_IWOTH | S_IXOTH)) == -1)
		{
			g_error(strerror(errno));
		}
		if ((graphsStream= fdopen(graphsFp, "w")) == NULL)
		{
			g_error(strerror(errno));
		}

		retval= chdir(cwd);
		if (retval == -1)
		{
			g_error(strerror(errno));
		}
		free(cwd);
	}

	// Identify matching and analysis modules
	result= g_queue_find_custom(&matchingModules, "TCP", &gcfCompareMatching);
	g_assert(result != NULL);
	syncState->matchingModule= (MatchingModule*) result->data;

	result= g_queue_find_custom(&analysisModules, optionSyncAnalysis.arg,
		&gcfCompareAnalysis);
	if (result != NULL)
	{
		syncState->analysisModule= (AnalysisModule*) result->data;
	}
	else
	{
		g_error("Analysis module '%s' not found", optionSyncAnalysis.arg);
	}

	syncState->processingModule->initProcessing(syncState, traceSetContext);

	syncState->matchingData= NULL;
	syncState->analysisData= NULL;

	if (!optionSyncNull.present)
	{
		syncState->matchingModule->initMatching(syncState);
		syncState->analysisModule->initAnalysis(syncState);
	}

	// Process traceset
	lttv_process_traceset_seek_time(traceSetContext, ltt_time_zero);
	lttv_process_traceset_middle(traceSetContext, ltt_time_infinite,
		G_MAXULONG, NULL);
	lttv_process_traceset_seek_time(traceSetContext, ltt_time_zero);

	syncState->processingModule->finalizeProcessing(syncState);

	// Write graphs file
	if (graphsStream != NULL)
	{
		unsigned int i, j;

		fprintf(graphsStream,
			"#!/usr/bin/gnuplot\n\n"
			"set terminal postscript eps color size 8in,6in\n");

		// Cover the upper triangular matrix, i is the reference node.
		for (i= 0; i < syncState->traceNb; i++)
		{
			for (j= i + 1; j < syncState->traceNb; j++)
			{
				long pos;

				fprintf(graphsStream,
					"\nset output \"%03d-%03d.eps\"\n"
					"plot \\\n", i, j);

				syncState->processingModule->writeProcessingGraphsPlots(graphsStream,
					syncState, i, j);

				// Remove the ", \\\n" from the last graph plot line
				fflush(graphsStream);
				pos= ftell(graphsStream);
				if (ftruncate(fileno(graphsStream), pos - 4) == -1)
				{
					g_error(strerror(errno));
				}
				if (fseek(graphsStream, 0, SEEK_END) == -1)
				{
					g_error(strerror(errno));
				}

				fprintf(graphsStream,
					"\nset output \"%1$03d-%2$03d.eps\"\n"
					"set key inside right bottom\n"
					"set title \"\"\n"
					"set xlabel \"Clock %1$u\"\n"
					"set xtics nomirror\n"
					"set ylabel \"Clock %2$u\"\n"
					"set ytics nomirror\n", i, j);

				syncState->processingModule->writeProcessingGraphsOptions(graphsStream,
					syncState, i, j);

				fprintf(graphsStream,
					"replot\n");
			}
		}

		if (fclose(graphsStream) != 0)
		{
			g_error(strerror(errno));
		}
	}

	if (syncState->processingModule->printProcessingStats != NULL)
	{
		syncState->processingModule->printProcessingStats(syncState);
	}

	syncState->processingModule->destroyProcessing(syncState);
	if (syncState->matchingModule != NULL)
	{
		syncState->matchingModule->destroyMatching(syncState);
	}
	if (syncState->analysisModule != NULL)
	{
		syncState->analysisModule->destroyAnalysis(syncState);
	}

	free(syncState);

	if (optionSyncStats.present)
	{
		gettimeofday(&endTime, 0);
		retval= getrusage(RUSAGE_SELF, &endUsage);

		timeDiff(&endTime, &startTime);
		timeDiff(&endUsage.ru_utime, &startUsage.ru_utime);
		timeDiff(&endUsage.ru_stime, &startUsage.ru_stime);

		printf("Synchronization time:\n");
		printf("\treal time: %ld.%06ld\n", endTime.tv_sec, endTime.tv_usec);
		printf("\tuser time: %ld.%06ld\n", endUsage.ru_utime.tv_sec,
			endUsage.ru_utime.tv_usec);
		printf("\tsystem time: %ld.%06ld\n", endUsage.ru_stime.tv_sec,
			endUsage.ru_stime.tv_usec);
	}
}


/*
 * Calculate the elapsed time between two timeval values
 *
 * Args:
 *   end:          end time, result is also stored in this structure
 *   start:        start time
 */
void timeDiff(struct timeval* const end, const struct timeval* const start)
{
		if (end->tv_usec >= start->tv_usec)
		{
			end->tv_sec-= start->tv_sec;
			end->tv_usec-= start->tv_usec;
		}
		else
		{
			end->tv_sec= end->tv_sec - start->tv_sec - 1;
			end->tv_usec= end->tv_usec - start->tv_usec + 1e6;
		}
}


/*
 * A GCompareFunc for g_slist_find_custom()
 *
 * Args:
 *   a:            ProcessingModule*, element's data
 *   b:            char*, user data to compare against
 *
 * Returns:
 *   0 if the processing module a's name is b
 */
gint gcfCompareProcessing(gconstpointer a, gconstpointer b)
{
	const ProcessingModule* processingModule;
	const char* name;

	processingModule= (const ProcessingModule*) a;
	name= (const char*) b;

	return strncmp(processingModule->name, name,
		strlen(processingModule->name) + 1);
}


/*
 * A GCompareFunc for g_slist_find_custom()
 *
 * Args:
 *   a:            MatchingModule*, element's data
 *   b:            char*, user data to compare against
 *
 * Returns:
 *   0 if the matching module a's name is b
 */
gint gcfCompareMatching(gconstpointer a, gconstpointer b)
{
	const MatchingModule* matchingModule;
	const char* name;

	matchingModule= (const MatchingModule*) a;
	name= (const char*) b;

	return strncmp(matchingModule->name, name, strlen(matchingModule->name) +
		1);
}


/*
 * A GCompareFunc for g_slist_find_custom()
 *
 * Args:
 *   a:            AnalysisModule*, element's data
 *   b:            char*, user data to compare against
 *
 * Returns:
 *   0 if the analysis module a's name is b
 */
gint gcfCompareAnalysis(gconstpointer a, gconstpointer b)
{
	const AnalysisModule* analysisModule;
	const char* name;

	analysisModule= (const AnalysisModule*) a;
	name= (const char*) b;

	return strncmp(analysisModule->name, name, strlen(analysisModule->name) +
		1);
}


/*
 * A GFunc for g_queue_foreach()
 *
 * Concatenate analysis module names.
 *
 * Args:
 *   data:         AnalysisModule*
 *   user_data:    GString*, concatenated names
 */
static void gfAppendAnalysisName(gpointer data, gpointer user_data)
{
	g_string_append((GString*) user_data, ((AnalysisModule*) data)->name);
	g_string_append((GString*) user_data, ", ");
}


/*
 * Change to the directory used to hold graphs. Create it if necessary.
 *
 * Args:
 *   graph:        name of directory
 *
 * Returns:
 *   The current working directory before the execution of the function. The
 *   string must be free'd by the caller.
 */
char* changeToGraphDir(const char* const graphs)
{
	int retval;
	char* cwd;

	cwd= getcwd(NULL, 0);
	if (cwd == NULL)
	{
		g_error(strerror(errno));
	}
	while ((retval= chdir(graphs)) != 0)
	{
		if (errno == ENOENT)
		{
			retval= mkdir(graphs, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP |
				S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH);
			if (retval != 0)
			{
				g_error(strerror(errno));
			}
		}
		else
		{
			g_error(strerror(errno));
		}
	}

	return cwd;
}


/*
 * A GFunc for g_queue_foreach()
 *
 * Args:
 *   data:         ModuleOption*
 *   user_data:    NULL
 */
static void gfAddModuleOption(gpointer data, gpointer user_data)
{
	ModuleOption* option;
	LttvOptionType conversion[]= {
		[NO_ARG]= LTTV_OPT_NONE,
		[REQUIRED_ARG]= LTTV_OPT_STRING,
	};

	g_assert_cmpuint(sizeof(conversion) / sizeof(*conversion), ==,
		HAS_ARG_COUNT);
	option= (ModuleOption*) data;
	lttv_option_add(option->longName, '\0', option->optionHelp,
		option->argHelp ? option->argHelp : argHelpNone,
		conversion[option->hasArg], &option->arg, NULL, NULL);
}


/*
 * A GFunc for g_queue_foreach()
 *
 * Args:
 *   data:         ModuleOption*
 *   user_data:    NULL
 */
static void gfRemoveModuleOption(gpointer data, gpointer user_data)
{
	lttv_option_remove(((ModuleOption*) data)->longName);
}


LTTV_MODULE("sync", "Synchronize traces", \
	"Synchronizes a traceset based on the correspondance of network events", \
	init, destroy, "option")
