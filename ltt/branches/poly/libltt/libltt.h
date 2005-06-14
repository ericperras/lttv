/* libltt header file
 *
 * Copyright 2005-
 *     Mathieu Desnoyers <mathieu.desnoyers@polymtl.ca>
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *
 * Inspired from iptables, by James Morris <jmorris@intercode.com.au>.
 * 
 */

#ifndef _LIBLTT_H
#define _LIBLTT_H

#include <linux/limits.h>

enum trace_op {
	OP_CREATE,
	OP_DESTROY,
	OP_START,
	OP_STOP
};

enum trace_mode {
	TRACE_NORMAL,
	TRACE_FLIGHT
};


struct lttctl_handle
{
  int fd;
  u_int8_t blocking;
  struct sockaddr_nl local;
  struct sockaddr_nl peer;
};

typedef struct lttctl_peer_msg {
	char trace_name[NAME_MAX];
	enum trace_op op;
	union {
		enum trace_mode mode;
	} args;
} lttctl_peer_msg_t;


struct lttctl_handle *lttctl_create_handle(u_int32_t flags);

int lttctl_destroy_handle(struct lttctl_handle *h);


int lttctl_create_trace(char *name, enum trace_mode mode);

int lttctl_destroy_trace(char *name);

int lttctl_start(char *name);

int lttctl_stop(char *name);

#define LTTCTLM_BASE	0x10
#define LTTCTLM_CONTROL	(LTTCTLM_BASE + 1)	/* LTT control message */


#endif //_LIBLTT_H
