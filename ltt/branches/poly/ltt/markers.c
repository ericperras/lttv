/*
 * Marker support code.
 *
 * Mathieu Desnoyers, August 2007
 * License: LGPL.
 */

#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ltt/compiler.h>
#include <ltt/markers.h>
#include <ltt/ltt-private.h>

#define DEFAULT_MARKERS_NUM   100
#define DEFAULT_FIELDS_NUM    1
#define MAX_NAME_LEN          1024

static inline const char *parse_trace_type(struct marker_info *info,
    const char *fmt,
    char *trace_size, enum ltt_type *trace_type,
    unsigned long *attributes)
{
  int qualifier;    /* 'h', 'l', or 'L' for integer fields */
        /* 'z' support added 23/7/1999 S.H.    */
        /* 'z' changed to 'Z' --davidm 1/25/99 */
        /* 't' added for ptrdiff_t */

  /* parse attributes. */
  repeat:
    switch (*fmt) {
      case 'b':
        *attributes |= LTT_ATTRIBUTE_COMPACT;
        ++fmt;
        goto repeat;
      case 'n':
        *attributes |= LTT_ATTRIBUTE_NETWORK_BYTE_ORDER;
        ++fmt;
        goto repeat;
    }

  /* get the conversion qualifier */
  qualifier = -1;
  if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L' ||
      *fmt =='Z' || *fmt == 'z' || *fmt == 't' ||
      *fmt == 'S' || *fmt == '1' || *fmt == '2' ||
      *fmt == '4' || *fmt == 8) {
    qualifier = *fmt;
    ++fmt;
    if (qualifier == 'l' && *fmt == 'l') {
      qualifier = 'L';
      ++fmt;
    }
  }

  switch (*fmt) {
    case 'c':
      *trace_type = LTT_TYPE_UNSIGNED_INT;
      *trace_size = sizeof(char);
      goto parse_end;
    case 's':
      *trace_type = LTT_TYPE_STRING;
      goto parse_end;
    case 'p':
      *trace_type = LTT_TYPE_UNSIGNED_INT;
      *trace_size = info->pointer_size;
      goto parse_end;
    case 'd':
    case 'i':
      *trace_type = LTT_TYPE_SIGNED_INT;
      break;
    case 'o':
    case 'u':
    case 'x':
    case 'X':
      *trace_type = LTT_TYPE_UNSIGNED_INT;
      break;
    default:
      if (!*fmt)
        --fmt;
      goto parse_end;
  }
  switch (qualifier) {
  case 'L':
    *trace_size = sizeof(long long);
    break;
  case 'l':
    *trace_size = info->long_size;
    break;
  case 'Z':
  case 'z':
    *trace_size = info->size_t_size;
    break;
  case 't':
    *trace_size = info->pointer_size;
    break;
  case 'h':
    *trace_size = sizeof(short);
    break;
  case '1':
    *trace_size = sizeof(uint8_t);
    break;
  case '2':
    *trace_size = sizeof(guint16);
    break;
  case '4':
    *trace_size = sizeof(uint32_t);
    break;
  case '8':
    *trace_size = sizeof(uint64_t);
    break;
  default:
    *trace_size = info->int_size;
  }

parse_end:
  return fmt;
}

/*
 * Restrictions:
 * Field width and precision are *not* supported.
 * %n not supported.
 */
__attribute__((no_instrument_function))
static inline const char *parse_c_type(struct marker_info *info,
    const char *fmt,
    char *c_size, enum ltt_type *c_type)
{
  int qualifier;    /* 'h', 'l', or 'L' for integer fields */
        /* 'z' support added 23/7/1999 S.H.    */
        /* 'z' changed to 'Z' --davidm 1/25/99 */
        /* 't' added for ptrdiff_t */

  /* process flags : ignore standard print formats for now. */
  repeat:
    switch (*fmt) {
      case '-':
      case '+':
      case ' ':
      case '#':
      case '0':
        ++fmt;
        goto repeat;
    }

  /* get the conversion qualifier */
  qualifier = -1;
  if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L' ||
      *fmt =='Z' || *fmt == 'z' || *fmt == 't' ||
      *fmt == 'S') {
    qualifier = *fmt;
    ++fmt;
    if (qualifier == 'l' && *fmt == 'l') {
      qualifier = 'L';
      ++fmt;
    }
  }

  switch (*fmt) {
    case 'c':
      *c_type = LTT_TYPE_UNSIGNED_INT;
      *c_size = sizeof(unsigned char);
      goto parse_end;
    case 's':
      *c_type = LTT_TYPE_STRING;
      goto parse_end;
    case 'p':
      *c_type = LTT_TYPE_UNSIGNED_INT;
      *c_size = info->pointer_size;
      goto parse_end;
    case 'd':
    case 'i':
      *c_type = LTT_TYPE_SIGNED_INT;
      break;
    case 'o':
    case 'u':
    case 'x':
    case 'X':
      *c_type = LTT_TYPE_UNSIGNED_INT;
      break;
    default:
      if (!*fmt)
        --fmt;
      goto parse_end;
  }
  switch (qualifier) {
  case 'L':
    *c_size = sizeof(long long);
    break;
  case 'l':
    *c_size = info->long_size;
    break;
  case 'Z':
  case 'z':
    *c_size = info->size_t_size;
    break;
  case 't':
    *c_size = info->pointer_size;
    break;
  case 'h':
    *c_size = sizeof(short);
    break;
  default:
    *c_size = info->int_size;
  }

parse_end:
  return fmt;
}

static inline long add_type(struct marker_info *info,
    long offset, const char *name,
    char trace_size, enum ltt_type trace_type,
    char c_size, enum ltt_type c_type, unsigned long attributes,
    unsigned int field_count)
{
  struct marker_field *field;
  char tmpname[MAX_NAME_LEN];

  info->fields = g_array_set_size(info->fields, info->fields->len+1);
  field = &g_array_index(info->fields, struct marker_field,
            info->fields->len-1);
  if (name)
    field->name = g_quark_from_string(name);
  else {
    snprintf(tmpname, MAX_NAME_LEN-1, "field %u", field_count);
    field->name = g_quark_from_string(tmpname);
  }
  field->type = trace_type;

  switch (trace_type) {
  case LTT_TYPE_SIGNED_INT:
  case LTT_TYPE_UNSIGNED_INT:
    field->size = trace_size;
    field->alignment = trace_size;
    field->attributes = attributes;
    if (offset == -1) {
      field->offset = -1;
      field->static_offset = 0;
      return -1;
    } else {
      field->offset = offset + ltt_align(offset, field->alignment,
                                         info->alignment);
      field->static_offset = 1;
      return field->offset + trace_size;
    }
  case LTT_TYPE_STRING:
    field->offset = offset;
    field->size = 0;  /* Variable length, size is 0 */
    field->alignment = 1;
    if (offset == -1)
      field->static_offset = 0;
    else
      field->static_offset = 1;
    return -1;
  default:
    g_error("Unexpected type"); //FIXME: compact type
    return 0;
  }
}

long marker_update_fields_offsets(struct marker_info *info, const char *data)
{
  struct marker_field *field;
  unsigned int i;
  long offset = 0;

  for (i = 0; i < info->fields->len; i++) {
    field = &g_array_index(info->fields, struct marker_field, i);

    if (field->static_offset)
      continue;

    switch (field->type) {
    case LTT_TYPE_SIGNED_INT:
    case LTT_TYPE_UNSIGNED_INT:
      field->offset = offset + ltt_align(offset, field->alignment,
                                          info->alignment);
      offset = field->offset + field->size;
      break;
    case LTT_TYPE_STRING:
      field->offset = offset;
      offset = offset + strlen(&data[offset]) + 1;
      // not aligning on pointer size, breaking genevent backward compatibility.
      break;
    default:
      g_error("Unexpected type"); //FIXME: compact type
      return -1;
    }
  }
  return offset;
}

static void format_parse(const char *fmt, struct marker_info *info)
{
  char trace_size = 0, c_size = 0;  /*
             * 0 (unset), 1, 2, 4, 8 bytes.
             */
  enum ltt_type trace_type = LTT_TYPE_NONE, c_type = LTT_TYPE_NONE;
  unsigned long attributes = 0;
  long offset = 0;
  const char *name_begin = NULL, *name_end = NULL;
  char *name = NULL;
  unsigned int field_count = 1;

  for (; *fmt ; ++fmt) {
    switch (*fmt) {
    case '#':
      /* tracetypes (#) */
      ++fmt;      /* skip first '#' */
      if (*fmt == '#')  /* Escaped ## */
        break;
      attributes = 0;
      fmt = parse_trace_type(info, fmt, &trace_size, &trace_type,
        &attributes);
      break;
    case '%':
      /* c types (%) */
      ++fmt;      /* skip first '%' */
      if (*fmt == '%')  /* Escaped %% */
        break;
      fmt = parse_c_type(info, fmt, &c_size, &c_type);
      /*
       * Output c types if no trace types has been
       * specified.
       */
      if (!trace_size)
        trace_size = c_size;
      if (trace_type == LTT_TYPE_NONE)
        trace_type = c_type;
      if (c_type == LTT_TYPE_STRING)
        trace_type = LTT_TYPE_STRING;
      /* perform trace write */
      offset = add_type(info, offset, name, trace_size,
            trace_type, c_size, c_type, attributes, field_count++);
      trace_size = c_size = 0;
      trace_type = c_size = LTT_TYPE_NONE;
      attributes = 0;
      name_begin = NULL;
      if (name) {
        g_free(name);
        name = NULL;
      }
      break;
    case ' ':
      if (!name_end) {
        name_end = fmt;
        if (name)
          g_free(name);
        name = g_new(char, name_end - name_begin + 1);
        memcpy(name, name_begin, name_end - name_begin);
        name[name_end - name_begin] = '\0';
      }
      break;  /* Skip white spaces */
    default:
      if (!name) {
        name_begin = fmt;
        name_end = NULL;
      }
    }
  }
  info->size = offset;
  if (name)
    g_free(name);
}

int marker_parse_format(const char *format, struct marker_info *info)
{
  if (info->fields)
    g_array_free(info->fields, TRUE);
  info->fields = g_array_sized_new(FALSE, TRUE,
                    sizeof(struct marker_field), DEFAULT_FIELDS_NUM);
  format_parse(format, info);
}

int marker_format_event(LttTrace *trace, GQuark name, const char *format)
{
  struct marker_info *info;
  
  info = g_hash_table_lookup(trace->markers_hash, (gconstpointer)name);
  if (!info)
    g_error("Got marker format %s, but marker name %s has no ID yet. "
            "Kernel issue.",
            format, name);
  for (; info != NULL; info = info->next) {
    if (info->format)
      g_free(info->format);
    info->format = g_new(char, strlen(format)+1);
    strcpy(info->format, format);
    if (marker_parse_format(format, info))
      g_error("Error parsing marker format %s for marker %s", format,
        g_quark_to_string(name));
  }
}

int marker_id_event(LttTrace *trace, GQuark name, guint16 id,
  uint8_t int_size, uint8_t long_size, uint8_t pointer_size,
  uint8_t size_t_size, uint8_t alignment)
{
  struct marker_info *info, *head;

  if (trace->markers->len < id)
    trace->markers = g_array_set_size(trace->markers, id+1);
  info = &g_array_index(trace->markers, struct marker_info, id);
  info->name = name;
  info->int_size = int_size;
  info->long_size = long_size;
  info->pointer_size = pointer_size;
  info->size_t_size = size_t_size;
  info->alignment = alignment;
  info->next = NULL;
  head = g_hash_table_lookup(trace->markers_hash, (gconstpointer)name);
  if (!head)
    g_hash_table_insert(trace->markers_hash, (gpointer)name, info);
  else
    head->next = info;
}

int allocate_marker_data(LttTrace *trace)
{
  /* Init array to 0 */
  trace->markers = g_array_sized_new(FALSE, TRUE,
                    sizeof(struct marker_info), DEFAULT_MARKERS_NUM);
  if (!trace->markers)
    return -ENOMEM;
  trace->markers_hash = g_hash_table_new(g_direct_hash, g_direct_equal);
  if (!trace->markers_hash)
    return -ENOMEM;
  return 0;
}

void destroy_marker_data(LttTrace *trace)
{
  unsigned int i;
  struct marker_info *info;

  for (i=0; i<trace->markers->len; i++) {
    info = &g_array_index(trace->markers, struct marker_info, i);
    if (info->format)
      g_free(info->format);
    if (info->fields)
      g_array_free(info->fields, TRUE);
  }
  g_array_free(trace->markers, TRUE);
  g_hash_table_destroy(trace->markers_hash);
}
