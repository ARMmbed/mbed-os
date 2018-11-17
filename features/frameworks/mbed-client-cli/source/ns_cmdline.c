/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#if defined(_WIN32) || defined(__unix__) || defined(__unix) || defined(unix) || defined(MBED_CONF_RTOS_PRESENT) || defined(__APPLE__)
#include <stdlib.h> //malloc
#ifndef MEM_ALLOC
#define MEM_ALLOC malloc
#endif
#ifndef MEM_FREE
#define MEM_FREE free
#endif
#else
#include "nsdynmemLIB.h"
#ifndef MEM_ALLOC
#define MEM_ALLOC ns_dyn_mem_temporary_alloc
#endif
#ifndef MEM_FREE
#define MEM_FREE ns_dyn_mem_free
#endif
#endif

// force traces for this module
//#define FEA_TRACE_SUPPORT


#ifdef YOTTA_CFG
#include "ns_list_internal/ns_list.h"
#include "mbed-client-cli/ns_cmdline.h"
#else
#include "ns_list.h"
#include "ns_cmdline.h"
#endif
#include "mbed-trace/mbed_trace.h"

//#define TRACE_DEEP
//#define TRACE_PRINTF

#ifdef TRACE_PRINTF
#undef tr_debug
#define tr_debug(...) printf( __VA_ARGS__);printf("\r\n")
#endif

// #define MBED_CLIENT_CLI_TRACE_ENABLE
// MBED_CLIENT_CLI_TRACE_ENABLE is to enable the traces for debugging,
// By default all debug traces are disabled.
#ifndef MBED_CLIENT_CLI_TRACE_ENABLE
#undef tr_error
#define tr_error(...)
#undef tr_warn
#define tr_warn(...)
#undef tr_debug
#define tr_debug(...)
#undef tr_info
#define tr_info(...)
#endif

#ifdef TRACE_DEEP
#define tr_deep   tr_debug
#else
#define tr_deep(...)
#endif

#define TRACE_GROUP "cmdL"

#ifndef MBED_CMDLINE_BOOT_MESSAGE
#define MBED_CMDLINE_BOOT_MESSAGE "ARM Ltd\r\n"
#endif
#define ESCAPE(x) "\x1b" x
#define CR_S "\r"
#define LF_S "\n"
#define CLEAR_ENTIRE_LINE ESCAPE("[2K")
#define CLEAR_ENTIRE_SCREEN ESCAPE("[2J")
#define ENABLE_AUTO_WRAP_MODE ESCAPE("[7h")
#define MOVE_CURSOR_LEFT_N_CHAR ESCAPE("[%dD")

#define SET_TOP_AND_BOTTOM_LINES ESCAPE("[;r")
#define MOVE_CURSOR_TO_BOTTOM_RIGHT ESCAPE("[999;999H")
#define STORE_CURSOR_POS ESCAPE("7")
#define RESTORE_CURSOR_POS ESCAPE("8")
#define GET_CURSOR_POSITION ESCAPE("[6n")

#define REQUEST_SCREEN_SIZE STORE_CURSOR_POS SET_TOP_AND_BOTTOM_LINES MOVE_CURSOR_TO_BOTTOM_RIGHT GET_CURSOR_POSITION RESTORE_CURSOR_POS

/*ASCII defines*/
#define ESC 0x1B
#define DEL 0x7F
#define BS  0x08
#define ETX 0x03
#define ETB 0x17
#define TAB 0x09
#define CAN 0x18

#define DEFAULT_RETFMT "retcode: %i\r\n"
#define DEFAULT_PROMPT "/>"
#define VAR_PROMPT "PS1"
#define VAR_RETFMT "RETFMT"

// Maximum length of input line
#ifdef MBED_CMDLINE_MAX_LINE_LENGTH
#define MAX_LINE MBED_CMDLINE_MAX_LINE_LENGTH
#else
#define MAX_LINE 2000
#endif
// Maximum number of arguments in a single command
#ifdef MBED_CMDLINE_ARGUMENTS_MAX_COUNT
#define MAX_ARGUMENTS MBED_CMDLINE_ARGUMENTS_MAX_COUNT
#else
#define MAX_ARGUMENTS 30
#endif
// Maximum number of commands saved in history
#ifdef MBED_CMDLINE_HISTORY_MAX_COUNT
#define HISTORY_MAX_COUNT MBED_CMDLINE_HISTORY_MAX_COUNT
#else
#define HISTORY_MAX_COUNT 32
#endif
//include manuals or not (save memory a little when not include)
#ifndef MBED_CMDLINE_INCLUDE_MAN
#define MBED_CMDLINE_INCLUDE_MAN 1
#endif


typedef struct cmd_history_s {
    char *command_ptr;
    ns_list_link_t link;
} cmd_history_t;

typedef struct cmd_command_s {
    const char *name_ptr;
    const char *info_ptr;
    const char *man_ptr;
    cmd_run_cb *run_cb;
    bool        busy;
    ns_list_link_t link;
} cmd_command_t;

typedef struct cmd_alias_s {
    char *name_ptr;
    char *value_ptr;
    ns_list_link_t link;
} cmd_alias_t;

union Data {
    char *ptr;
    int i;
};
typedef enum value_type_s {
    VALUE_TYPE_STR,
    VALUE_TYPE_INT
} value_type_t;

typedef struct cmd_variable_s {
    char *name_ptr;
    union Data value;
    value_type_t type;
    ns_list_link_t link;
} cmd_variable_t;

typedef enum operator_s {
    OPERATOR_SEMI_COLON,  //default
    OPERATOR_AND,
    OPERATOR_OR,
    OPERATOR_BACKGROUND,
    OPERATOR_PIPE
} operator_t;
typedef struct cmd_exe_s {
    char          *cmd_s;
    operator_t     operator;
    ns_list_link_t link;
} cmd_exe_t;
typedef NS_LIST_HEAD(cmd_exe_t, link) cmd_list_t;
typedef NS_LIST_HEAD(cmd_history_t, link) history_list_t;
typedef NS_LIST_HEAD(cmd_command_t, link) command_list_t;
typedef NS_LIST_HEAD(cmd_alias_t, link) alias_list_t;
typedef NS_LIST_HEAD(cmd_variable_t, link) variable_list_t;

typedef struct cmd_class_s {
    char input[MAX_LINE];             // input data
    char escape[10];                  // escape data
    int16_t history;                  // history position
    int16_t cursor;                   // cursor position
    int16_t escape_index;             // escape index
    history_list_t history_list;      // input history
    uint8_t history_max_count;        // history max size
    command_list_t command_list;      // commands list
    alias_list_t alias_list;          // alias list
    variable_list_t variable_list;    // variables list
    bool vt100_on;                    // control characters
    bool init;                        // true when lists are initialized already
    bool escaping;                    // escaping input
    bool insert;                      // insert enabled
    int  tab_lookup;                  // originally lookup characters count
    int  tab_lookup_cmd_n;            // index in command list
    int  tab_lookup_n;                //
    bool prev_cr;                     // indicate if cr was last char
    bool echo;                        // echo inputs
    cmd_ready_cb_f *ready_cb;         // ready cb function
    cmd_list_t  cmd_buffer;
    cmd_exe_t  *cmd_buffer_ptr;
    cmd_command_t  *cmd_ptr;
    int8_t      tasklet_id;
    int8_t      network_tasklet_id;
    bool        idle;

    cmd_print_t *out;                  // print cb function
    void (*ctrl_fnc)(uint8_t c);      // control cb function
    void (*mutex_wait_fnc)(void);         // mutex wait cb function
    void (*mutex_release_fnc)(void);      // mutex release cb function
    input_passthrough_func_t passthrough_fnc; // input passthrough cb function
} cmd_class_t;

cmd_class_t cmd = {
    .init = false,
    .cmd_ptr = NULL,
    .mutex_wait_fnc = NULL,
    .mutex_release_fnc = NULL,
    .passthrough_fnc = NULL
};

/* Function prototypes
 */
static void             cmd_init_base_commands(void);
static void             cmd_replace_alias(char *input);
static void             cmd_replace_variables(char *input);
static int              cmd_parse_argv(char *string_ptr, char **argv);
static void             cmd_execute(void);
static void             cmd_line_clear(int from);
static void             cmd_history_save(int16_t index);
static void             cmd_history_get(uint16_t index);
static void             cmd_history_clean_overflow(void);
static void             cmd_history_clean(void);
static void             cmd_echo(bool on);
static cmd_history_t   *cmd_history_find(int16_t index);
static bool             cmd_tab_lookup(void);
static void             cmd_clear_last_word(void);
static void             cmd_move_cursor_to_last_space(void);
static void             cmd_move_cursor_to_next_space(void);
static const char      *cmd_input_lookup(char *name, int namelength, int n);
static char            *cmd_input_lookup_var(char *name, int namelength, int n);
static cmd_command_t   *cmd_find(const char *name);
static cmd_command_t   *cmd_find_n(char *name, int nameLength, int n);
static cmd_alias_t     *alias_find(const char *alias);
static cmd_alias_t     *alias_find_n(char *alias, int aliaslength, int n);
static cmd_variable_t  *variable_find(char *variable);
static cmd_variable_t  *variable_find_n(char *variable, int length, int n);
static void             cmd_print_man(cmd_command_t *command_ptr);
static void             goto_end_of_history(void);
static void             goto_beginning_of_history(void);
static void             cmd_set_input(const char *str, int cur);
static char            *next_command(char *string_ptr, operator_t *mode);
/** Run single command through cmd intepreter
 * \param string_ptr    command string with parameters
 * \ret  command return code (CMDLINE_RETCODE_*)
 */
static int              cmd_run(char *string_ptr);
static cmd_exe_t       *cmd_next_ptr(int retcode);
static void             cmd_split(char *string_ptr);
static void             cmd_push(char *cmd_str, operator_t oper);

/*internal shell commands
 */
int true_command(int argc, char *argv[]);
int false_command(int argc, char *argv[]);
int echo_command(int argc, char *argv[]);
int alias_command(int argc, char *argv[]);
int unset_command(int argc, char *argv[]);
int set_command(int argc, char *argv[]);
int clear_command(int argc, char *argv[]);
int help_command(int argc, char *argv[]);
int history_command(int argc, char *argv[]);

/** Internal helper functions
 */
static const char *find_last_space(const char *from, const char *to);
static int replace_string(
    char *str, int str_len,
    const char *old_str, const char *new_str);

void default_cmd_response_out(const char *fmt, va_list ap)
{
    vprintf(fmt, ap);
    fflush(stdout);
}
void cmd_printf(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    cmd_vprintf(fmt, ap);
    va_end(ap);
}
void cmd_vprintf(const char *fmt, va_list ap)
{
    if (cmd.mutex_wait_fnc) {
        cmd.mutex_wait_fnc();
    }
    cmd.out(fmt, ap);
    if (cmd.mutex_release_fnc) {
        cmd.mutex_release_fnc();
    }
}
/* Function definitions
 */
void cmd_init(cmd_print_t *outf)
{
    if (!cmd.init) {
        ns_list_init(&cmd.alias_list);
        ns_list_init(&cmd.history_list);
        ns_list_init(&cmd.command_list);
        ns_list_init(&cmd.variable_list);
        ns_list_init(&cmd.cmd_buffer);
        cmd.init = true;
    }
    cmd.out = outf ? outf : default_cmd_response_out;
    cmd.ctrl_fnc = NULL;
    cmd.echo = true;
    cmd.escaping = false;
    cmd.insert = true;
    cmd.cursor = 0;
    cmd.prev_cr = false;
    cmd.vt100_on = true;
    cmd.history_max_count = HISTORY_MAX_COUNT;
    cmd.tab_lookup = 0;
    cmd.tab_lookup_cmd_n = 0;
    cmd.tab_lookup_n = 0;
    cmd.cmd_buffer_ptr = 0;
    cmd.idle = true;
    cmd.ready_cb = cmd_next;
    cmd.passthrough_fnc = NULL;
    cmd_variable_add(VAR_PROMPT, DEFAULT_PROMPT);
    cmd_variable_add_int("?", 0);
    //cmd_alias_add("auto-on", "set PS1=\r\nretcode=$?\r\n&&echo off");
    //cmd_alias_add("auto-off", "set PS1="DEFAULT_PROMPT"&&echo on");
    cmd_line_clear(0);            // clear line
    cmd_history_save(0);          // the current line is the 0 item
    cmd_init_base_commands();
    cmd_init_screen();
    return;
}
void cmd_request_screen_size(void)
{
    cmd_printf(REQUEST_SCREEN_SIZE);
}

const char *cmdline_get_prompt(void)
{
    cmd_variable_t *var_ptr = variable_find(VAR_PROMPT);
    return var_ptr && var_ptr->type == VALUE_TYPE_STR ? var_ptr->value.ptr : "";
}
const char *cmd_get_retfmt(void)
{
    cmd_variable_t *var_ptr = variable_find(VAR_RETFMT);
    return var_ptr && var_ptr->type == VALUE_TYPE_STR ? var_ptr->value.ptr : 0;
}

#if MBED_CMDLINE_INCLUDE_MAN == 1
#define MAN_ECHO    "Displays messages, or turns command echoing on or off\r\n"\
                    "echo <data_to_be_print>\r\n"\
                    "some special parameters:\r\n"\
                    "<bool>                 On/Off echo input characters\r\n"
#define MAN_ALIAS   "alias <theAlias> <command (parameters)>\r\n"
#define MAN_UNSET   "unset <var_name>\r\n"
#define MAN_SET     "set <var_name> <value>\r\n"\
                    "some special parameters\r\n"\
                    "--vt100 <bool>         On/Off vt100 controls\r\n"\
                    "--retcode <bool>       On/Off retcode print after execution\r\n"\
                    "--retfmt <format>      Return print format. Default: \"retcode: %i\\n\"\r\n"

#define MAN_CLEAR   "Clears the display"
#define MAN_HISTORY "Show commands history\r\n"\
                    "history (<optio>)\r\n"\
                    "clear                  Clear history\r\n"
#else
#define MAN_ECHO    NULL
#define MAN_ALIAS   NULL
#define MAN_SET     NULL
#define MAN_CLEAR   NULL
#define MAN_HISTORY NULL
#endif

static void cmd_init_base_commands(void)
{
    cmd_add("help",     help_command,     "This help",            NULL);
    cmd_add("echo",     echo_command,     "Echo controlling",     MAN_ECHO);
    cmd_add("alias",    alias_command,    "Handle aliases",       MAN_ALIAS);
    cmd_add("unset",    unset_command,    "unset variables",      MAN_UNSET);
    cmd_add("set",      set_command,      "print or set variables", MAN_SET);
    cmd_add("clear",    clear_command,    "Clears the display",   MAN_CLEAR);
    cmd_add("history",  history_command,  "View your command Line History", MAN_HISTORY);
    cmd_add("true",     true_command, 0, 0);
    cmd_add("false",    false_command, 0, 0);
}
void cmd_reset(void)
{
    cmd_free();
    cmd_init_base_commands();
}
void cmd_free(void)
{
    ns_list_foreach_safe(cmd_command_t, cur_ptr, &cmd.command_list) {
        cmd_delete(cur_ptr->name_ptr);
    }
    ns_list_foreach_safe(cmd_alias_t, cur_ptr, &cmd.alias_list) {
        cmd_alias_add(cur_ptr->name_ptr, NULL);
    }
    ns_list_foreach_safe(cmd_variable_t, cur_ptr, &cmd.variable_list) {
        if (cur_ptr->type == VALUE_TYPE_STR) {
            cmd_variable_add(cur_ptr->value.ptr, NULL);
        }
    }
    ns_list_foreach_safe(cmd_history_t, cur_ptr, &cmd.history_list) {
        MEM_FREE(cur_ptr->command_ptr);
        ns_list_remove(&cmd.history_list, cur_ptr);
        MEM_FREE(cur_ptr);
    }
    cmd.mutex_wait_fnc = NULL;
    cmd.mutex_release_fnc = NULL;
}

void cmd_input_passthrough_func(input_passthrough_func_t passthrough_fnc)
{
    cmd.passthrough_fnc = passthrough_fnc;
}

void cmd_exe(char *str)
{
    cmd_split(str);
    if (cmd.cmd_buffer_ptr == 0) {
        //execution buffer is empty
        cmd.idle = false; //not really, but fake it
        cmd_ready(CMDLINE_RETCODE_SUCCESS);
    } else {
        tr_debug("previous cmd is still in progress");
    }
}
void cmd_set_ready_cb(cmd_ready_cb_f *cb)
{
    cmd.ready_cb = cb;
}
void cmd_ready(int retcode)
{
    if (cmd.cmd_ptr && cmd.cmd_ptr->busy) {
        //execution finished
        cmd.cmd_ptr->busy = false;
    }
    if (!cmd.idle) {
        if (cmd.cmd_buffer_ptr == NULL) {
            tr_debug("goto next command");
        } else {
            tr_debug("cmd '%s' executed with retcode: %i", cmd.cmd_buffer_ptr->cmd_s, retcode);
        }
        if (cmd.ready_cb == NULL) {
            tr_warn("Missing ready_cb! use cmd_set_ready_cb()");
        } else {
            cmd.ready_cb(retcode);
        }
    } else {
        tr_warn("Someone call cmd_ready(%i) even there shouldn't be any running cmd", retcode);
        if (cmd.echo) {
            cmd_output();    //refresh if this happens
        }
    }
}
void cmd_next(int retcode)
{
    cmd.idle = true;
    //figure out next command
    cmd.cmd_buffer_ptr = cmd_next_ptr(retcode);
    if (cmd.cmd_buffer_ptr) {
        cmd.idle = false;
        //yep there was some -> lets execute it
        retcode = cmd_run(cmd.cmd_buffer_ptr->cmd_s);
        //check if execution goes to the backend or not
        if (retcode == CMDLINE_RETCODE_EXCUTING_CONTINUE) {
            if ((NULL != cmd.cmd_buffer_ptr) && cmd.cmd_buffer_ptr->operator == OPERATOR_BACKGROUND) {
                //execution continue in background, but operator say that it's "ready"
                cmd_ready(CMDLINE_RETCODE_SUCCESS);
            } else {
                //command execution phase continuous in background
                tr_debug("Command execution continuous in background..");
            }
        } else {
            //execution finished -> call ready function with retcode
            cmd_ready(retcode);
        }
    } else {
        const char *retfmt = cmd_get_retfmt();
        if (retfmt) {
            cmd_printf(retfmt, retcode);
        }
        cmd_line_clear(0);
        if (cmd.echo) {
            cmd_output();    //ready
        }
    }
}
static cmd_exe_t *cmd_pop(void)
{
    cmd_exe_t *cmd_ptr = ns_list_get_first(&cmd.cmd_buffer),
               *next_cmd = ns_list_get_next(&cmd.cmd_buffer, cmd_ptr);

    if (cmd.cmd_buffer_ptr == 0) {
        //was first in bool
        next_cmd = ns_list_get_first(&cmd.cmd_buffer);
    } else {
        MEM_FREE(cmd_ptr->cmd_s);
        ns_list_remove(&cmd.cmd_buffer, cmd_ptr);
        MEM_FREE(cmd_ptr);
    }
    return next_cmd;
}

static cmd_exe_t *cmd_next_ptr(int retcode)
{
    cmd_exe_t *next_cmd = cmd.cmd_buffer_ptr;
    if (ns_list_is_empty(&cmd.cmd_buffer)) {
        return NULL;
    }
    if (cmd.cmd_buffer_ptr == NULL) {
        return cmd_pop();
    }
    switch (cmd.cmd_buffer_ptr->operator) {
        case (OPERATOR_AND):
            if (retcode != CMDLINE_RETCODE_SUCCESS) {
                //if fails, go to next command, which not have AND operator
                while ((next_cmd->operator == OPERATOR_AND) && ((next_cmd = cmd_pop()) != 0));
            } else {
                next_cmd = cmd_pop();
            }
            break;
        case (OPERATOR_OR):
            if (retcode == CMDLINE_RETCODE_SUCCESS) {
                //if fails, go to next command, which not have OR operator
                while ((next_cmd->operator == OPERATOR_OR) && ((next_cmd = cmd_pop()) != 0));
            } else {
                next_cmd = cmd_pop();
            }
            break;
        case (OPERATOR_BACKGROUND):
            next_cmd = cmd_pop();
            break;
        case (OPERATOR_PIPE):
            cmd_printf("pipe is not supported\r\n");
            while ((next_cmd = cmd_pop()) != 0);
            break;
        case (OPERATOR_SEMI_COLON):
        default:
            //get next command to be execute (might be null if there is no more)
            next_cmd = cmd_pop();
            break;
    }

    //return next command if any
    return next_cmd;
}
static void cmd_split(char *string_ptr)
{
    char *ptr = string_ptr, *next;
    operator_t oper = OPERATOR_SEMI_COLON;
    do {
        next = next_command(ptr, &oper);
        cmd_push(ptr, oper);
        ptr = next;
        if (next && !*next) {
            break;
        }
    } while (ptr != 0);
}

static void cmd_push(char *cmd_str, operator_t oper)
{
    //store this command to the stack
    cmd_exe_t *cmd_ptr = MEM_ALLOC(sizeof(cmd_exe_t));
    if (cmd_ptr == NULL) {
        tr_error("mem alloc failed in cmd_push");
        return;
    }
    cmd_ptr->cmd_s = MEM_ALLOC(strlen(cmd_str) + 1);
    if (cmd_ptr->cmd_s == NULL) {
        MEM_FREE(cmd_ptr);
        tr_error("mem alloc failed in cmd_push cmd_s");
        return;
    }
    strcpy(cmd_ptr->cmd_s, cmd_str);
    cmd_ptr->operator = oper;
    ns_list_add_to_end(&cmd.cmd_buffer, cmd_ptr);
}
void cmd_out_func(cmd_print_t *outf)
{
    cmd.out = outf;
}
void cmd_ctrl_func(void (*sohf)(uint8_t c))
{
    cmd.ctrl_fnc = sohf;
}

void cmd_mutex_wait_func(void (*mutex_wait_f)(void))
{
    cmd.mutex_wait_fnc = mutex_wait_f;
}
void cmd_mutex_release_func(void (*mutex_release_f)(void))
{
    cmd.mutex_release_fnc = mutex_release_f;
}

void cmd_mutex_lock()
{
    if (cmd.mutex_wait_fnc) {
        cmd.mutex_wait_fnc();
    }
}

void cmd_mutex_unlock()
{
    if (cmd.mutex_release_fnc) {
        cmd.mutex_release_fnc();
    }
}
void cmd_init_screen()
{
    if (cmd.vt100_on) {
        cmd_printf(CR_S CLEAR_ENTIRE_SCREEN); /* Clear screen */
        cmd_printf(ENABLE_AUTO_WRAP_MODE); /* enable line wrap */
    }
    cmd_printf(MBED_CMDLINE_BOOT_MESSAGE);
    cmd_output();
}
uint8_t cmd_history_size(uint8_t max)
{
    if (max > 0) {
        cmd.history_max_count = max;
        cmd_history_clean_overflow();
    }
    return cmd.history_max_count;
}
static void cmd_echo(bool on)
{
    cmd.echo = on;
}
bool cmd_echo_state(void)
{
    return cmd.echo;
}
static cmd_command_t *cmd_find_n(char *name, int nameLength, int n)
{
    cmd_command_t *cmd_ptr = NULL;
    if (name != NULL && nameLength != 0) {
        int i = 0;
        ns_list_foreach(cmd_command_t, cur_ptr, &cmd.command_list) {
            if (strncmp(name, cur_ptr->name_ptr, nameLength) == 0) {
                if (i == n) {
                    cmd_ptr = cur_ptr;
                    break;
                }
                i++;
            }
        }
    }
    return cmd_ptr;
}
static const char *cmd_input_lookup(char *name, int namelength, int n)
{
    const char *str = NULL;
    cmd_command_t *cmd_ptr = cmd_find_n(name, namelength, n);
    if (cmd_ptr) {
        str = cmd_ptr->name_ptr;
        cmd.tab_lookup_n = n + 1;
    } else {
        n -= cmd.tab_lookup_n;
        cmd_alias_t *alias = alias_find_n(name, namelength, n);
        if (alias) {
            str = (const char *)alias->name_ptr;
        }
    }

    return str;
}
static char *cmd_input_lookup_var(char *name, int namelength, int n)
{
    char *str = NULL;
    cmd_variable_t *var = variable_find_n(name, namelength, n);
    if (var) {
        str = var->name_ptr;
    }
    return str;
}
static cmd_command_t *cmd_find(const char *name)
{
    cmd_command_t *cmd_ptr = NULL;
    if (name == NULL || strlen(name) == 0) {
        tr_error("cmd_find invalid parameters");
        return NULL;
    }

    ns_list_foreach(cmd_command_t, cur_ptr, &cmd.command_list) {
        if (strcmp(name, cur_ptr->name_ptr) == 0) {
            cmd_ptr = cur_ptr;
            break;
        }
    }
    return cmd_ptr;
}

void cmd_add(const char *name, cmd_run_cb *callback, const char *info, const char *man)
{
    cmd_command_t *cmd_ptr;

    if (name == NULL || callback == NULL || strlen(name) == 0) {
        tr_warn("cmd_add invalid parameters");
        return;
    }
    cmd_ptr = (cmd_command_t *)MEM_ALLOC(sizeof(cmd_command_t));
    if (cmd_ptr == NULL) {
        tr_error("mem alloc failed in cmd_add");
        return;
    }
    cmd_ptr->name_ptr = name;
    cmd_ptr->info_ptr = info;
#if MBED_CMDLINE_INCLUDE_MAN == 1
    cmd_ptr->man_ptr = man;
#else
    cmd_ptr->man_ptr = 0;
#endif
    cmd_ptr->run_cb = callback;
    cmd_ptr->busy = false;
    ns_list_add_to_end(&cmd.command_list, cmd_ptr);
    return;
}

void cmd_delete(const char *name)
{
    cmd_command_t *cmd_ptr;
    cmd_ptr = cmd_find(name);
    if (cmd_ptr == NULL) {
        return;
    }
    ns_list_remove(&cmd.command_list, cmd_ptr);
    MEM_FREE(cmd_ptr);
    return;
}
static void replace_escapes(char *string_ptr)
{
    while ((string_ptr = strchr(string_ptr, '\\')) != NULL) {
        memmove(string_ptr, string_ptr + 1, strlen(string_ptr + 1) + 1);
        string_ptr++;
    }
}
static int cmd_parse_argv(char *string_ptr, char **argv)
{
    tr_deep("cmd_parse_argv(%s, ..)\r\n", string_ptr);
    int argc = 0;
    char *str_ptr, *end_quote_ptr = NULL;

    if (string_ptr == NULL || strlen(string_ptr) == 0) {
        tr_error("Invalid parameters");
        return 0;
    }
    str_ptr = string_ptr;
    do {
        argv[argc] = str_ptr;
        // tr_deep("parsing.. argv[%d]: %s\r\n", argc, str_ptr);
        if (*str_ptr != '\\') {
            if (*str_ptr == '"') {
                // check if end quote
                end_quote_ptr = str_ptr;
                do {
                    end_quote_ptr = strchr(end_quote_ptr + 1, '\"');
                    if (end_quote_ptr == NULL) {
                        break;
                    }
                } while (*(end_quote_ptr - 1) == '\\');
                if (end_quote_ptr != NULL) {
                    // remove quotes give as one parameter
                    argv[argc]++;
                    str_ptr = end_quote_ptr;
                } else {
                    str_ptr = strchr(str_ptr, ' ');
                }
            } else {
                str_ptr = strchr(str_ptr, ' ');
            }
        } else {
            str_ptr = strchr(str_ptr, ' ');
        }
        argc++; // one argument parsed
        if (str_ptr == NULL) {
            break;
        }
        if (argc > MAX_ARGUMENTS) {
            tr_warn("Maximum arguments (%d) reached", MAX_ARGUMENTS);
            break;
        }
        *str_ptr++ = 0;
        replace_escapes(argv[argc - 1]);
        // tr_deep("parsed argv[%d]: %s\r\n", argc-1, argv[argc-1]);
        while (*str_ptr == ' ') {
            str_ptr++;  // skip spaces
        };
    } while (*str_ptr != 0);
    return argc;
}
static void cmd_print_man(cmd_command_t *command_ptr)
{
    if (command_ptr->man_ptr) {
        cmd_printf("%s\r\n", command_ptr->man_ptr);
    }
}
static void cmd_set_input(const char *str, int cur)
{
    cmd_line_clear(cur);
    strcpy(cmd.input + cur, str);
    cmd.cursor = strlen(cmd.input);
}
/**
 * If oper is not null, function set null pointers
 */
static char *next_command(char *string_ptr, operator_t *oper)
{
    char *ptr = string_ptr;
    bool quote = false;
    while (*ptr != 0) {
        if (quote) {
            if (*ptr == '"') {
                quote = false;
            }
        } else {
            //skip if previous character is '\'
            if ((ptr == string_ptr) || (*(ptr - 1) != '\\')) {
                switch (*ptr) {
                    case ('"'): {
                        quote = true;
                        break;
                    }
                    case (';'):  //default operator
                        if (oper) {
                            *oper = OPERATOR_SEMI_COLON;
                            *ptr = 0;
                        }
                        return ptr + 1;
                    case ('&'):
                        if (ptr[1] == '&') {
                            if (oper) {
                                *oper = OPERATOR_AND;
                                *ptr = 0;
                            }
                            return ptr + 2;
                        } else {
                            if (oper) {
                                *oper = OPERATOR_BACKGROUND;
                                *ptr = 0;
                            }
                            return ptr + 1;
                        }
                    case ('|'):
                        if (ptr[1] == '|') {
                            if (oper) {
                                *oper = OPERATOR_OR;
                                *ptr = 0;
                            }
                            return ptr + 2;
                        } else {
                            tr_warn("pipe operator not supported");
                            if (oper) {
                                *oper = OPERATOR_PIPE;
                                *ptr = 0;
                            }
                            return ptr + 1;
                        }
                    default:
                        break;
                }
            }
        }
        ptr++;
    }
    return 0;
}
static int cmd_run(char *string_ptr)
{
    char *argv[MAX_ARGUMENTS];
    int argc, ret;

    tr_info("Executing cmd: '%s'", string_ptr);
    char *command_str = MEM_ALLOC(MAX_LINE);
    if (command_str == NULL) {
        tr_error("mem alloc failed in cmd_run");
        return CMDLINE_RETCODE_FAIL;
    }
    while (isspace((unsigned char) *string_ptr) &&
            *string_ptr != '\n' &&
            *string_ptr != 0) {
        string_ptr++; //skip white spaces
    }
    strcpy(command_str, string_ptr);
    tr_deep("cmd_run('%s') ", command_str);
    cmd_replace_alias(command_str);
    cmd_replace_variables(command_str);
    tr_debug("Parsed cmd: '%s'", command_str);

    argc = cmd_parse_argv(command_str, argv);

    cmd.cmd_ptr = cmd_find(argv[0]);

    if (cmd.cmd_ptr == NULL) {
        cmd_printf("Command '%s' not found.\r\n", argv[0]);
        MEM_FREE(command_str);
        return CMDLINE_RETCODE_COMMAND_NOT_FOUND;
    }
    if (cmd.cmd_ptr->run_cb == NULL) {
        tr_error("Command callback missing");
        MEM_FREE(command_str);
        return CMDLINE_RETCODE_COMMAND_CB_MISSING;
    }

    if (argc == 2 &&
            (cmd_has_option(argc, argv, "h") || cmd_parameter_index(argc, argv, "--help") > 0)) {
        MEM_FREE(command_str);
        cmd_print_man(cmd.cmd_ptr);
        return CMDLINE_RETCODE_SUCCESS;
    }

    if (cmd.cmd_ptr->busy) {
        MEM_FREE(command_str);
        return CMDLINE_RETCODE_COMMAND_BUSY;
    }

    // Run the actual callback
    cmd.cmd_ptr->busy = true;
    ret = cmd.cmd_ptr->run_cb(argc, argv);
    cmd_variable_add_int("?", ret);
    cmd_alias_add("_", string_ptr); // last executed command
    MEM_FREE(command_str);
    switch (ret) {
        case (CMDLINE_RETCODE_COMMAND_NOT_IMPLEMENTED):
            tr_warn("Command not implemented");
            break;
        case (CMDLINE_RETCODE_INVALID_PARAMETERS):
            tr_warn("Command parameter was incorrect");
            cmd_printf("Invalid parameters!\r\n");
            cmd_print_man(cmd.cmd_ptr);
            break;
        default:
            break;
    }
    return ret;
}
void cmd_escape_start(void)
{
    cmd.escaping = true;
    memset(cmd.escape, 0, sizeof(cmd.escape));
    cmd.escape_index = 0;
}
static void cmd_arrow_right()
{
    /* @todo handle shift
    if(strncmp(cmd.escape+1, "1;2", 3) == 0) {
        tr_debug("Shift pressed");
        shift = true;
    }
    */
    if ((cmd.escape_index == 1 && cmd.escape[0] == 'O') ||
            (cmd.escape_index == 4 && strncmp(cmd.escape + 1, "1;5", 3) == 0)) {
        cmd_move_cursor_to_next_space();
    } else {
        cmd.cursor ++;
    }
    if ((int)cmd.cursor > (int)strlen(cmd.input)) {
        cmd.cursor = strlen(cmd.input);
    }
}
static void cmd_arrow_left()
{
    /* @todo handle shift
    if(strncmp(cmd.escape+1, "1;2", 3) == 0) {
        tr_debug("Shift pressed");
        shift = true;
    }
    */
    if ((cmd.escape_index == 1 && cmd.escape[0] == 'O') ||
            (cmd.escape_index == 4 && strncmp(cmd.escape + 1, "1;5", 3) == 0)) {
        cmd_move_cursor_to_last_space();
    } else {
        cmd.cursor --;
    }
    if (cmd.cursor < 0) {
        cmd.cursor = 0;
    }
}
static void cmd_arrow_up()
{
    int16_t old_entry = cmd.history++;
    if (NULL == cmd_history_find(cmd.history)) {
        cmd.history = old_entry;
    }
    if (old_entry != cmd.history) {
        cmd_history_save(old_entry);
        cmd_history_get(cmd.history);
    }
}
static void cmd_arrow_down(void)
{
    int16_t old_entry = cmd.history--;
    if (cmd.history < 0) {
        cmd.history = 0;
    }

    if (old_entry != cmd.history) {
        cmd_history_save(old_entry);
        cmd_history_get(cmd.history);
    }
}
void cmd_escape_read(int16_t u_data)
{
    tr_debug("cmd_escape_read: %02x '%c' escape_index: %d: %s",
             u_data,
             (isprint(u_data) ? u_data : '?'),
             cmd.escape_index,
             cmd.escape);

    if (u_data == 'D') {
        cmd_arrow_left();
    } else if (u_data == 'C') {
        cmd_arrow_right();
    } else if (u_data == 'A') {
        cmd_arrow_up();
    } else if (u_data == 'B') {
        cmd_arrow_down();
    } else if (u_data == 'Z') {
        // Shift+TAB
        if (cmd.tab_lookup > 0) {
            cmd.cursor = cmd.tab_lookup;
            cmd.input[cmd.tab_lookup] = 0;
            if (cmd.tab_lookup_cmd_n > 0) {
                cmd.tab_lookup_cmd_n--;
            }
        }
        cmd_tab_lookup();
    } else if (u_data == 'b') {
        cmd_move_cursor_to_last_space();
    } else if (u_data == 'f') {
        cmd_move_cursor_to_next_space();
    } else if (u_data == 'n') {
        if (cmd.escape[1] == '5') {
            // Device status report
            // Response: terminal is OK
            cmd_printf("%c0n", ESC);
        } else if (cmd.escape[1] == '6') {
            // Get cursor position
            cmd_printf(ESCAPE("%d%d"), 0, cmd.cursor);
        }
    } else if (u_data == 'R') {
        // response for Get cursor position (<esc>[6n)
        // <esc>[<lines>;<cols>R
        char *ptr;
        int lines = strtol(cmd.escape + 1, &ptr, 10);
        if (ptr == NULL) {
            tr_warn("Invalid response: <esc>%s%c", cmd.escape, u_data);
        } else {
            int cols = strtol(ptr + 1, 0, 10);
            tr_debug("Lines: %d, cols: %d", lines, cols);
            cmd_variable_add_int("LINES", lines);
            cmd_variable_add_int("COLUMNS", cols);
        }
    } else if (u_data == 'H') {
        // Xterm support
        cmd.cursor =  0;
    } else if (u_data == 'F') {
        // Xterm support
        cmd.cursor = strlen(cmd.input);
    } else if (isdigit((int)cmd.escape[cmd.escape_index - 1]) && u_data == '~') {
        switch (cmd.escape[cmd.escape_index - 1]) {
            case ('1'): //beginning-of-line     # Home key
                cmd.cursor =  0;
                break;
            case ('2'): //quoted-insert         # Insert key
                cmd.insert = !cmd.insert;
                break;
            case ('3'): //delete-char           # Delete key
                if ((int)strlen(cmd.input) > (int)cmd.cursor) {
                    memmove(&cmd.input[cmd.cursor], &cmd.input[cmd.cursor + 1], strlen(&cmd.input[cmd.cursor + 1]) + 1);
                }
                break;
            case ('4'): //end-of-line           # End key
                cmd.cursor = strlen(cmd.input);
                break;
            case ('5'): //beginning-of-history  # PageUp key
                goto_end_of_history();
                break;
            case ('6'): //end-of-history        # PageDown key
                goto_beginning_of_history();
                break;
            default:
                break;
        }
    } else if (isprint(u_data)) {  //IS_NUMBER || IS_CONTROL
        cmd.escape[cmd.escape_index++] = u_data;
        return;
    }
    cmd_output();
    cmd.escaping = false;
    return;
}
static void goto_end_of_history(void)
{
    // handle new input if any and verify that
    // it is not already in beginning of history or current position
    bool allowStore = strlen(cmd.input) != 0; //avoid store empty lines to history
    cmd_history_t *entry_ptr;
    if (cmd.history > 0 && allowStore) {
        entry_ptr = cmd_history_find(cmd.history);
        if (entry_ptr) {
            if (strcmp(entry_ptr->command_ptr, cmd.input) == 0) {
                // current history contains contains same text as input
                allowStore = false;
            }
        }
    } else if (allowStore && (entry_ptr = cmd_history_find(0)) != NULL) {
        if (strcmp(entry_ptr->command_ptr, cmd.input) == 0) {
            //beginning of history was same text as input
            allowStore = false;
        }
    }
    if (allowStore) {
        cmd_history_save(0);  // new is saved to place 0
        cmd_history_save(-1); // new is created to the current one
    }

    cmd_history_t *cmd_ptr = ns_list_get_last(&cmd.history_list);
    cmd_set_input(cmd_ptr->command_ptr, 0);
    cmd.history =  ns_list_count(&cmd.history_list) - 1;
}
static void goto_beginning_of_history(void)
{
    cmd_history_t *cmd_ptr = ns_list_get_first(&cmd.history_list);
    cmd_set_input(cmd_ptr->command_ptr, 0);
    cmd.history = 0;
}
static void cmd_reset_tab(void)
{
    cmd.tab_lookup = 0;
    cmd.tab_lookup_cmd_n = 0;
    cmd.tab_lookup_n = 0;
}
void cmd_char_input(int16_t u_data)
{
    if (cmd.prev_cr && u_data == '\n') {
        // ignore \n if previous character was \r ->
        // that triggers execute so \n does not need to anymore
        cmd.prev_cr = false; // unset to ensure we doesn't go here anymore
        return;
    }
    /*Handle passthrough*/
    if (cmd.passthrough_fnc != NULL) {
        cmd.passthrough_fnc(u_data);
        return;
    }
    /*handle ecape command*/
    if (cmd.escaping == true) {
        cmd_escape_read(u_data);
        return;
    }

    tr_debug("input char:      %02x '%c', cursor: %i, input: \"%s\"", u_data, (isprint(u_data) ? u_data : ' '), cmd.cursor,  cmd.input);

    /*Normal character input*/
    if (u_data == '\r' || u_data == '\n') {
        cmd.prev_cr = u_data == '\r';
        cmd_reset_tab();
        if (strlen(cmd.input) == 0) {
            if (cmd.echo) {
                cmd_printf("\r\n");
                cmd_output();
            }
        } else {
            if (cmd.echo) {
                cmd_printf("\r\n");
            }
            cmd_execute();
        }
    } else if (u_data == ESC) {
        cmd_escape_start();
    } else if (u_data == BS || u_data == DEL) {
        cmd_reset_tab();
        cmd.cursor--;
        if (cmd.cursor < 0) {
            cmd.cursor = 0;
            return;
        }
        memmove(&cmd.input[cmd.cursor], &cmd.input[cmd.cursor + 1], strlen(&cmd.input[cmd.cursor + 1]) + 1);
        if (cmd.echo) {
            cmd_output();
        }
    } else if (u_data == ETX || u_data == CAN) {
        //ctrl+c (End of text) or ctrl+x (cancel)
        cmd_reset_tab();
        cmd_line_clear(0);
        if (!cmd.idle) {
            cmd_ready(CMDLINE_RETCODE_FAIL);
        }
        if (cmd.echo) {
            cmd_output();
        }
    } else if (u_data == ETB) {
        //ctrl+w (End of xmit block)
        tr_debug("ctrl+w - remove last word to cursor");
        cmd_clear_last_word();
        if (cmd.echo) {
            cmd_output();
        }
    } else if (u_data == TAB) {
        bool inc = false;
        if (cmd.tab_lookup > 0) {
            cmd.cursor = cmd.tab_lookup;
            cmd.input[cmd.tab_lookup] = 0;
            cmd.tab_lookup_cmd_n++;
            inc = true;
        } else {
            cmd.tab_lookup = strlen(cmd.input);
        }

        if (!cmd_tab_lookup()) {
            if (inc) {
                cmd.tab_lookup_cmd_n--;
            }
            memset(cmd.input + cmd.tab_lookup, 0, MAX_LINE - cmd.tab_lookup);
        }
        if (cmd.echo) {
            cmd_output();
        }

    } else if (iscntrl(u_data)) {
        if (cmd.ctrl_fnc) {
            cmd.ctrl_fnc(u_data);
        }
    } else {
        cmd_reset_tab();
        tr_deep("cursor: %d, inputlen: %lu, u_data: %c\r\n", cmd.cursor, strlen(cmd.input), u_data);
        if ((strlen(cmd.input) >= MAX_LINE - 1) || (cmd.cursor >= MAX_LINE - 1)) {
            tr_warn("input buffer full");
            if (cmd.echo) {
                cmd_output();
            }
            return;
        }
        if (cmd.insert) {
            memmove(&cmd.input[cmd.cursor + 1], &cmd.input[cmd.cursor], strlen(&cmd.input[cmd.cursor]) + 1);
        }
        cmd.input[cmd.cursor++] = u_data;
        if (cmd.echo) {
            cmd_output();
        }
    }
}
static int check_variable_keylookup_size(char **key, int *keysize)
{
    if (cmd.cursor > 0 && cmd.tab_lookup > 0) {
        //printf("tab_lookup: %i\r\n", cmd.tab_lookup);
        char *ptr = cmd.input + cmd.tab_lookup;
        do {
            //printf("varkey lookup: %c\r\n", *ptr);
            if (*ptr == ' ') {
                return 0;
            }
            if (*ptr == '$') {
                int varlen = cmd.tab_lookup - (ptr - cmd.input) - 1;
                *key = ptr;
                *keysize = varlen;
                //printf("varkey size: %i\r\n", varlen);
                return (ptr - cmd.input);
            }
            ptr--;
        } while (ptr > cmd.input);
    }
    return 0;
}
bool cmd_tab_lookup(void)
{
    int len = strlen(cmd.input);
    if (len == 0) {
        return false;
    }
    char *variable_keypart;
    int lookupSize;
    int varpos = check_variable_keylookup_size(&variable_keypart, &lookupSize);

    const char *str = NULL;
    if (varpos) {
        str = cmd_input_lookup_var(variable_keypart + 1, lookupSize, cmd.tab_lookup_cmd_n);
        if (str) {
            cmd_set_input(str, varpos + 1);
            return true;
        }
    } else {
        str = cmd_input_lookup(cmd.input, len, cmd.tab_lookup_cmd_n);
        if (str != NULL) {
            cmd_set_input(str, 0);
            return true;
        }
    }
    return false;
}
static void cmd_move_cursor_to_last_space(void)
{
    if (cmd.cursor) {
        cmd.cursor--;
    } else {
        return;
    }
    const char *last_space = find_last_space(cmd.input + cmd.cursor, cmd.input);
    if (last_space) {
        cmd.cursor = last_space - cmd.input;
    } else {
        cmd.cursor = 0;
    }
}
static void cmd_move_cursor_to_next_space(void)
{
    while (cmd.input[cmd.cursor] == ' ') {
        cmd.cursor++;
    }
    const char *next_space = strchr(cmd.input + cmd.cursor, ' ');
    if (next_space) {
        cmd.cursor = next_space - cmd.input;
    } else {
        cmd.cursor = (int)strlen(cmd.input);
    }
}
static void cmd_clear_last_word()
{
    if (!cmd.cursor) {
        return;
    }
    char *ptr = cmd.input + cmd.cursor - 1;
    while (*ptr == ' ' && ptr >= cmd.input) {
        ptr--;
    }
    const char *last_space = find_last_space(ptr, cmd.input);
    if (last_space) {
        memmove((void *)last_space, &cmd.input[cmd.cursor], strlen(cmd.input + cmd.cursor) + 1);
        cmd.cursor = last_space - cmd.input;
    } else {
        memmove((void *)cmd.input, &cmd.input[cmd.cursor], strlen(cmd.input + cmd.cursor) + 1);
        cmd.cursor = 0;
    }
}
void cmd_output(void)
{
    if (cmd.vt100_on && cmd.idle) {
        int curpos = (int)strlen(cmd.input) - cmd.cursor + 1;
        cmd_printf(CR_S CLEAR_ENTIRE_LINE "%s%s " MOVE_CURSOR_LEFT_N_CHAR,
                   cmdline_get_prompt(), cmd.input, curpos);
    }
}
void cmd_echo_off(void)
{
    cmd_echo(false);
}
void cmd_echo_on(void)
{
    cmd_echo(true);
}
// alias
int replace_alias(char *str, const char *old_str, const char *new_str)
{
    int old_len = strlen(old_str),
        new_len = strlen(new_str);
    if ((strncmp(str, old_str, old_len) == 0) &&
            ((str[ old_len ] == ' ') || (str[ old_len ] == 0) ||
             (str[ old_len ] == ';') || (str[ old_len ] == '&'))) {
        memmove(str + new_len, str + old_len, strlen(str + old_len) + 1);
        memcpy(str, new_str, new_len);
        return new_len - old_len;
    }
    return 0;
}
static void cmd_replace_alias(char *input)
{
    ns_list_foreach(cmd_alias_t, cur_ptr, &cmd.alias_list) {
        replace_alias(input, cur_ptr->name_ptr, cur_ptr->value_ptr);
    }
}
//variable
static void replace_variable(char *str, cmd_variable_t *variable_ptr)
{
    const char *name = variable_ptr->name_ptr;
    int name_len = strlen(variable_ptr->name_ptr);
    char *value;
    char valueLocal[11];
    if (variable_ptr->type == VALUE_TYPE_STR) {
        value = variable_ptr->value.ptr;
    } else {
        value = valueLocal;
        snprintf(value, 11, "%d", variable_ptr->value.i);
    }
    char *tmp = MEM_ALLOC(name_len + 2);
    if (tmp == NULL) {
        tr_error("mem alloc failed in replace_variable");
        return;
    }
    tmp[0] = '$';
    strcpy(tmp + 1, name);
    replace_string(str, MAX_LINE, tmp, value);
    MEM_FREE(tmp);
}
static void cmd_replace_variables(char *input)
{
    ns_list_foreach(cmd_variable_t, cur_ptr, &cmd.variable_list) {
        replace_variable(input, cur_ptr);
    }
}
//history
static void cmd_history_item_delete(cmd_history_t *entry_ptr)
{
    ns_list_remove(&cmd.history_list, entry_ptr);
    MEM_FREE(entry_ptr->command_ptr);
    MEM_FREE(entry_ptr);
}

static cmd_history_t *cmd_history_find(int16_t index)
{
    cmd_history_t *entry_ptr = NULL;
    int16_t count = 0;
    ns_list_foreach(cmd_history_t, cur_ptr, &cmd.history_list) {
        if (count == index) {
            entry_ptr = cur_ptr;
            break;
        }
        count++;
    }
    return entry_ptr;
}

static void cmd_history_clean_overflow(void)
{
    while (ns_list_count(&cmd.history_list) > cmd.history_max_count) {
        cmd_history_t *cmd_ptr = ns_list_get_last(&cmd.history_list);
        tr_debug("removing older history (%s)", cmd_ptr->command_ptr);
        cmd_history_item_delete(cmd_ptr);
    }
}
static void cmd_history_clean(void)
{
    while (ns_list_count(&cmd.history_list) > 0) {
        tr_debug("removing older history");
        cmd_history_item_delete(ns_list_get_last(&cmd.history_list));
    }
}
static void cmd_history_save(int16_t index)
{
    /*if entry true save it to first item which is the one currently edited*/
    cmd_history_t *entry_ptr;
    int16_t len;

    len = strlen(cmd.input);

    tr_debug("saving history item %d", index);
    entry_ptr = cmd_history_find(index);

    if (entry_ptr == NULL) {
        /*new entry*/
        entry_ptr = (cmd_history_t *)MEM_ALLOC(sizeof(cmd_history_t));
        if (entry_ptr == NULL) {
            tr_error("mem alloc failed in cmd_history_save");
            return;
        }
        entry_ptr->command_ptr = NULL;
        ns_list_add_to_start(&cmd.history_list, entry_ptr);
    }

    if (entry_ptr->command_ptr != NULL) {
        MEM_FREE(entry_ptr->command_ptr);
    }
    entry_ptr->command_ptr = (char *)MEM_ALLOC(len + 1);
    if (entry_ptr->command_ptr == NULL) {
        tr_error("mem alloc failed in cmd_history_save command_ptr");
        cmd_history_item_delete(entry_ptr);
        return;
    }
    strcpy(entry_ptr->command_ptr, cmd.input);

    cmd_history_clean_overflow();
}
static void cmd_history_get(uint16_t index)
{
    cmd_history_t *entry_ptr;

    tr_debug("getting history item %d", index);

    entry_ptr = cmd_history_find(index);

    if (entry_ptr != NULL) {
        memset(cmd.input, 0, MAX_LINE);
        cmd_set_input(entry_ptr->command_ptr, 0);
    }
}

static void cmd_line_clear(int from)
{
    memset(cmd.input + from, 0, MAX_LINE - from);
    cmd.cursor = from;
}

static void cmd_execute(void)
{
    if (strlen(cmd.input) != 0) {
        bool noduplicates = true;
        cmd_history_t *entry_ptr = cmd_history_find(0);
        if (entry_ptr) {
            if (strcmp(entry_ptr->command_ptr, cmd.input) == 0) {
                noduplicates = false;
            }
        }
        if (noduplicates) {
            cmd_history_save(0);  // new is saved to place 0
            cmd_history_save(-1); // new is created to the current one
        }
    }
    cmd.history = 0;

    tr_deep("cmd_execute('%s') ", cmd.input);
    cmd_exe(cmd.input);
    cmd_line_clear(0);
}


static cmd_alias_t *alias_find(const char *alias)
{
    cmd_alias_t *alias_ptr = NULL;
    if (alias == NULL || strlen(alias) == 0) {
        tr_error("alias_find invalid parameters");
        return NULL;
    }

    ns_list_foreach(cmd_alias_t, cur_ptr, &cmd.alias_list) {
        if (strcmp(alias, cur_ptr->name_ptr) == 0) {
            alias_ptr = cur_ptr;
            break;
        }
    }
    return alias_ptr;
}

static cmd_alias_t *alias_find_n(char *alias, int aliaslength, int n)
{
    cmd_alias_t *alias_ptr = NULL;
    int i = 0;
    if (alias == NULL || strlen(alias) == 0) {
        tr_error("alias_find invalid parameters");
        return NULL;
    }

    ns_list_foreach(cmd_alias_t, cur_ptr, &cmd.alias_list) {
        if (strncmp(alias, cur_ptr->name_ptr, aliaslength) == 0) {
            if (i == n) {
                alias_ptr = cur_ptr;
                break;
            }
            i++;
        }
    }
    return alias_ptr;
}
static cmd_variable_t *variable_find(char *variable)
{
    cmd_variable_t *variable_ptr = NULL;
    if (variable == NULL || strlen(variable) == 0) {
        tr_error("variable_find invalid parameters");
        return NULL;
    }

    ns_list_foreach(cmd_variable_t, cur_ptr, &cmd.variable_list) {
        if (strcmp(variable, cur_ptr->name_ptr) == 0) {
            variable_ptr = cur_ptr;
            break;
        }
    }
    return variable_ptr;
}
static cmd_variable_t *variable_find_n(char *variable, int length, int n)
{
    cmd_variable_t *variable_ptr = NULL;
    if (variable == NULL || strlen(variable) == 0) {
        tr_error("variable_find invalid parameters");
        return NULL;
    }
    int i = 0;
    ns_list_foreach(cmd_variable_t, cur_ptr, &cmd.variable_list) {
        if (strncmp(variable, cur_ptr->name_ptr, length) == 0) {
            if (i == n) {
                variable_ptr = cur_ptr;
                break;
            }
            i++;
        }
    }
    return variable_ptr;
}
static void cmd_alias_print_all(void)
{
    ns_list_foreach(cmd_alias_t, cur_ptr, &cmd.alias_list) {
        if (cur_ptr->name_ptr != NULL) {
            cmd_printf("%-18s'%s'\r\n", cur_ptr->name_ptr, cur_ptr->value_ptr ? cur_ptr->value_ptr : "");
        }
    }
    return;
}
static void cmd_variable_print_all(void)
{
    ns_list_foreach(cmd_variable_t, cur_ptr, &cmd.variable_list) {
        if (cur_ptr->type == VALUE_TYPE_STR) {
            cmd_printf("%s='%s'\r\n", cur_ptr->name_ptr, cur_ptr->value.ptr ? cur_ptr->value.ptr : "");
        } else if (cur_ptr->type == VALUE_TYPE_INT) {
            cmd_printf("%s=%d\r\n", cur_ptr->name_ptr, cur_ptr->value.i);
        }
    }
    return;
}

void cmd_alias_add(const char *alias, const char *value)
{
    cmd_alias_t *alias_ptr;
    if (alias == NULL || strlen(alias) == 0) {
        tr_warn("cmd_alias_add invalid parameters");
        return;
    }
    alias_ptr = alias_find(alias);
    if (alias_ptr == NULL) {
        if (value == NULL) {
            return;    // no need to add new null one
        }
        if (strlen(value) == 0) {
            return;    // no need to add new empty one
        }
        alias_ptr = (cmd_alias_t *)MEM_ALLOC(sizeof(cmd_alias_t));
        if (alias_ptr == NULL) {
            tr_error("Mem alloc fail in cmd_alias_add");
            return;
        }
        alias_ptr->name_ptr = (char *)MEM_ALLOC(strlen(alias) + 1);
        if (alias_ptr->name_ptr == NULL) {
            MEM_FREE(alias_ptr);
            tr_error("Mem alloc fail in cmd_alias_add name_ptr");
            return;
        }
        ns_list_add_to_end(&cmd.alias_list, alias_ptr);
        strcpy(alias_ptr->name_ptr, alias);
        alias_ptr->value_ptr = NULL;
    }
    if (value == NULL || strlen(value) == 0) {
        // delete this one
        ns_list_remove(&cmd.alias_list, alias_ptr);
        MEM_FREE(alias_ptr->name_ptr);
        MEM_FREE(alias_ptr->value_ptr);
        MEM_FREE(alias_ptr);
    } else {
        // add new or modify
        if (alias_ptr->value_ptr != NULL) {
            MEM_FREE(alias_ptr->value_ptr);
        }
        alias_ptr->value_ptr = (char *)MEM_ALLOC(strlen(value) + 1);
        if (alias_ptr->value_ptr == NULL) {
            cmd_alias_add(alias, NULL);
            tr_error("Mem alloc fail in cmd_alias_add value_ptr");
            return;
        }
        strcpy(alias_ptr->value_ptr, value);
    }
    return;
}
static cmd_variable_t *cmd_variable_add_prepare(char *variable, char *value)
{
    if (variable == NULL || strlen(variable) == 0) {
        tr_warn("cmd_variable_add invalid parameters");
        return NULL;
    }
    cmd_variable_t *variable_ptr = variable_find(variable);
    if (variable_ptr == NULL) {
        if (value == NULL) {
            return NULL;    //  adding null variable
        }
        if (strlen(value) == 0) {
            return NULL;    // no need to add new empty one
        }
        variable_ptr = (cmd_variable_t *)MEM_ALLOC(sizeof(cmd_variable_t));
        if (variable_ptr == NULL) {
            tr_error("Mem alloc failed cmd_variable_add");
            return NULL;
        }
        variable_ptr->name_ptr = (char *)MEM_ALLOC(strlen(variable) + 1);
        if (variable_ptr->name_ptr == NULL) {
            MEM_FREE(variable_ptr);
            tr_error("Mem alloc failed cmd_variable_add name_ptr");
            return NULL;
        }

        ns_list_add_to_end(&cmd.variable_list, variable_ptr);
        strcpy(variable_ptr->name_ptr, variable);
        variable_ptr->value.ptr = NULL;
    }
    if (value == NULL || strlen(value) == 0) {
        // delete this one
        tr_debug("Remove variable: %s", variable);
        ns_list_remove(&cmd.variable_list, variable_ptr);
        MEM_FREE(variable_ptr->name_ptr);
        if (variable_ptr->type == VALUE_TYPE_STR) {
            MEM_FREE(variable_ptr->value.ptr);
        }
        MEM_FREE(variable_ptr);
        return NULL;
    }
    return variable_ptr;
}
void cmd_variable_add_int(char *variable, int value)
{
    cmd_variable_t *variable_ptr = cmd_variable_add_prepare(variable, " ");
    if (variable_ptr == NULL) {
        return;
    }
    if (variable_ptr->value.ptr != NULL &&
            variable_ptr->type == VALUE_TYPE_STR) {
        // free memory
        MEM_FREE(variable_ptr->value.ptr);
    }
    variable_ptr->type = VALUE_TYPE_INT;
    variable_ptr->value.i = value;
}
void cmd_variable_add(char *variable, char *value)
{
    cmd_variable_t *variable_ptr = cmd_variable_add_prepare(variable, value);
    if (variable_ptr == NULL) {
        return;
    }
    int value_len = strlen(value);
    replace_string(value, value_len, "\\n", "\n");
    replace_string(value, value_len, "\\r", "\r");

    // add new or modify
    int new_len = strlen(value) + 1;
    int old_len = 0;
    if (variable_ptr->value.ptr != NULL &&
            variable_ptr->type == VALUE_TYPE_STR) {
        // free memory if required
        old_len = strlen(variable_ptr->value.ptr) + 1;
        if (old_len != new_len) {
            MEM_FREE(variable_ptr->value.ptr);
        }
    }
    if (old_len != new_len) {
        variable_ptr->value.ptr = (char *)MEM_ALLOC(new_len);
        if (variable_ptr->value.ptr == NULL) {
            cmd_variable_add(variable, NULL);
            tr_error("Mem alloc failed cmd_variable_add value_ptr");
            return;
        }
    }
    variable_ptr->type = VALUE_TYPE_STR;
    strcpy(variable_ptr->value.ptr, value);
    return;
}

static bool is_cmdline_commands(char *command)
{
    if ((strncmp(command, "alias", 5) == 0) ||
            (strcmp(command, "echo") == 0) ||
            (strcmp(command, "set") == 0) ||
            (strcmp(command, "clear") == 0) ||
            (strcmp(command, "help") == 0)) {
        return true;
    }
    return false;
}
/*Basic commands for cmd line
 * alias
 * echo
 * set
 * clear
 * help
 */
int alias_command(int argc, char *argv[])
{
    if (argc == 1) {
        // print all alias
        cmd_printf("alias:\r\n");
        cmd_alias_print_all();
    } else if (argc == 2) {
        // print alias
        if (is_cmdline_commands(argv[1])) {
            cmd_printf("Cannot overwrite default commands with alias\r\n");
            return -1;
        }
        tr_debug("Deleting alias %s", argv[1]);
        cmd_alias_add(argv[1], NULL);
    } else {
        // set alias
        tr_debug("Setting alias %s = %s", argv[1], argv[2]);
        cmd_alias_add(argv[1], argv[2]);
    }
    return 0;
}
int unset_command(int argc, char *argv[])
{
    if (argc != 2) {
        return CMDLINE_RETCODE_INVALID_PARAMETERS;
    }
    tr_debug("Deleting variable %s", argv[1]);
    cmd_variable_add(argv[1], NULL);
    return 0;
}
int set_command(int argc, char *argv[])
{
    if (argc == 1) {
        // print all alias
        cmd_printf("variables:\r\n");
        cmd_variable_print_all();
    } else if (argc == 2) {
        char *separator_ptr = strchr(argv[1], '=');
        if (!separator_ptr) {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
        *separator_ptr = 0;
        cmd_variable_add(argv[1], separator_ptr + 1);
    } else {
        // set alias
        tr_debug("Setting variable %s = %s", argv[1], argv[2]);
        //handle special cases: vt100 on|off
        bool state;
        if (cmd_parameter_bool(argc, argv, "--vt100", &state)) {
            cmd.vt100_on = state;
            return 0;
        }
        if (cmd_parameter_bool(argc, argv, "--retcode", &state)) {
            cmd_variable_add(VAR_RETFMT, state ? DEFAULT_RETFMT : NULL);
            return 0;
        }
        char *str;
        if (cmd_parameter_val(argc, argv, "--retfmt", &str)) {
            cmd_variable_add(VAR_RETFMT, str);
            return 0;
        }
        cmd_variable_add(argv[1], argv[2]);
    }
    return 0;
}
int echo_command(int argc, char *argv[])
{
    bool printEcho = false;
    if (argc == 1) {
        printEcho = true;
    } else if (argc == 2) {
        if (strcmp(argv[1], "off") == 0) {
            cmd_echo(false);
            printEcho = true;
        } else if (strcmp(argv[1], "on") == 0) {
            cmd_echo(true);
            printEcho = true;
        }
    }
    if (printEcho) {
        cmd_printf("ECHO is %s\r\n", cmd.echo ? "on" : "off");
    } else {
        for (int n = 1; n < argc; n++) {
            tr_deep("ECHO: %s\r\n", argv[n]);
            cmd_printf("%s ", argv[n]);
        }
        cmd_printf("\r\n");
    }
    return 0;
}

int clear_command(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    cmd_echo(true);
    cmd_init_screen();
    return 0;
}
int help_command(int argc, char *argv[])
{
    cmd_printf("Commands:\r\n");
    if (argc == 1) {
        ns_list_foreach(cmd_command_t, cur_ptr, &cmd.command_list) {
            cmd_printf("%-16s%s\r\n", cur_ptr->name_ptr, (cur_ptr->info_ptr ? cur_ptr->info_ptr : ""));
        }
    } else if (argc == 2) {
        cmd_command_t *cmd_ptr = cmd_find(argv[1]);
        if (cmd_ptr) {
            cmd_printf("Command: %s\r\n", cmd_ptr->name_ptr);
            if (cmd_ptr->man_ptr) {
                cmd_printf("%s\r\n", cmd_ptr->man_ptr);
            } else if (cmd_ptr->info_ptr) {
                cmd_printf("%s\r\n", cmd_ptr->info_ptr);
            }
        } else {
            cmd_printf("Command '%s' not found", argv[1]);
        }
    }
    return 0;
}
int true_command(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    return CMDLINE_RETCODE_SUCCESS;
}
int false_command(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    return CMDLINE_RETCODE_FAIL;
}
int history_command(int argc, char *argv[])
{
    if (argc == 1) {
        int history_size = (int)ns_list_count(&cmd.history_list);
        cmd_printf("History [%i/%i]:\r\n", history_size - 1, cmd.history_max_count - 1);
        int i = 0;
        ns_list_foreach_reverse(cmd_history_t, cur_ptr, &cmd.history_list) {
            if (i != history_size - 1) {
                cmd_printf("[%i]: %s\r\n", i++, cur_ptr->command_ptr);
            }
        }
    } else if (argc == 2) {
        if (strcmp(argv[1], "clear") == 0) {
            cmd_history_clean();
        } else {
            cmd_history_size(strtoul(argv[1], 0, 10));
        }
    }
    return 0;
}

/** Parameter helping functions
 */
int cmd_parameter_index(int argc, char *argv[], const char *key)
{
    int i = 0;
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], key) == 0) {
            return i;
        }
    }
    return -1;
}
bool cmd_has_option(int argc, char *argv[], const char *key)
{
    int i = 0;
    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-' &&  argv[i][1] != '-') {
            if (strstr(argv[i], key) != 0) {
                return true;
            }
        }
    }
    return false;
}
bool cmd_parameter_bool(int argc, char *argv[], const char *key, bool *value)
{
    int i = cmd_parameter_index(argc, argv, key);
    if (i > 0) {
        if (argc > (i + 1)) {
            if (strcmp(argv[i + 1], "on") == 0 ||
                    strcmp(argv[i + 1], "1") == 0 ||
                    strcmp(argv[i + 1], "true") == 0 ||
                    strcmp(argv[i + 1], "enable") == 0 ||
                    strcmp(argv[i + 1], "allow") == 0) {
                *value = true;
            } else {
                *value = false;
            }
            return true;
        }
    }
    return false;
}
bool cmd_parameter_val(int argc, char *argv[], const char *key, char **value)
{
    int i = cmd_parameter_index(argc, argv, key);
    if (i > 0) {
        if (argc > (i + 1)) {
            *value = argv[i + 1];
            return true;
        }
    }
    return false;
}
bool cmd_parameter_int(int argc, char *argv[], const char *key, int32_t *value)
{
    int i = cmd_parameter_index(argc, argv, key);
    char *tailptr;
    if (i > 0) {
        if (argc > (i + 1)) {
            *value = strtol(argv[i + 1], &tailptr, 10);
            if (0 == *tailptr) {
                return true;
            }
            if (!isspace((unsigned char) *tailptr)) {
                return false;
            } else {
                return true;
            }
        }
    }
    return false;
}
bool cmd_parameter_float(int argc, char *argv[], const char *key, float *value)
{
    int i = cmd_parameter_index(argc, argv, key);
    char *tailptr;
    if (i > 0) {
        if (argc > (i + 1)) {
            *value = strtof(argv[i + 1], &tailptr);
            if (0 == *tailptr) {
                return true;    //Should be correct read always
            }
            if (!isspace((unsigned char) *tailptr)) {
                return false;   //Garbage in tailptr
            } else {
                return true;    //Spaces are fine after float
            }
        }
    }
    return false;
}
// convert hex string (eg. "76 ab ff") to binary array
static int string_to_bytes(const char *str, uint8_t *buf, int bytes)
{
    int len = strlen(str);
    if (len <= (3 * bytes - 1)) {
        int i;
        for (i = 0; i < bytes; i++) {
            if (i * 3 < len) {
                buf[i] = (uint8_t)strtoul(str + i * 3, 0, 16);
            } else {
                buf[i] = 0;
            }
        }
        return 0;
    }
    return -1;
}

static uint64_t read_64_bit(const uint8_t data_buf[__static 8])
{
    uint64_t temp_64;
    temp_64 = (uint64_t)(*data_buf++) << 56;
    temp_64 += (uint64_t)(*data_buf++) << 48;
    temp_64 += (uint64_t)(*data_buf++) << 40;
    temp_64 += (uint64_t)(*data_buf++) << 32;
    temp_64 += (uint64_t)(*data_buf++) << 24;
    temp_64 += (uint64_t)(*data_buf++) << 16;
    temp_64 += (uint64_t)(*data_buf++) << 8;
    temp_64 += *data_buf++;
    return temp_64;
}

bool cmd_parameter_timestamp(int argc, char *argv[], const char *key, int64_t *value)
{
    int i = cmd_parameter_index(argc, argv, key);
    if (i > 0) {
        if (argc > (i + 1)) {
            if (strchr(argv[i + 1], ',') != 0) {
                // Format seconds,tics
                const char splitValue[] = ", ";
                char *token;
                token = strtok(argv[i + 1], splitValue);
                if (token) {
                    *value = strtoul(token, 0, 10) << 16;
                }
                token = strtok(NULL, splitValue);
                if (token) {
                    *value |= (0xffff & strtoul(token, 0, 10));
                }
            } else if (strchr(argv[i + 1], ':') != 0) {
                // Format 00:00:00:00:00:00:00:00
                uint8_t buf[8];
                if (string_to_bytes(argv[i + 1], buf, 8) == 0) {
                    *value = read_64_bit(buf);
                } else {
                    cmd_printf("timestamp should be 8 bytes long\r\n");
                }
            } else {
                // Format uint64
                *value = strtol(argv[i + 1], 0, 10);
            }
            return true;
        }
    }
    return false;
}
char *cmd_parameter_last(int argc, char *argv[])
{
    if (argc > 1) {
        return argv[ argc - 1 ];
    }
    return NULL;
}
/**
 * find last space but ignore nulls and first spaces.
 * used only internally to find out previous word
 * e.g.
 * const char str[] = "aaaab aa bbb\0\0";
 * const char* tmp = last_space(str+strlen(str), str);
 * printf(tmp); // prints "bbb"
 */
static const char *find_last_space(const char *from, const char *to)
{
    if (from <= to) {
        return 0;
    }
    while ((from > to) && ((*from == 0) || (*from == ' '))) {
        from--;
    }
    while (from > to) {
        if (*from == ' ') {
            return from + 1;
        }
        from--;
    }
    return 0;
}
static int replace_string(
    char *str, int str_len,
    const char *old_str, const char *new_str)
{
    char *ptr = str;
    char *end = str + str_len;
    int old_len = strlen(old_str);
    int new_len = strlen(new_str);
    if (old_len > 0) {
        tr_deep("find: '%s'\r\n", old_str);
        while ((ptr = strstr(ptr, old_str)) != 0) {
            if (ptr + new_len > end) {
                tr_warn("Buffer was not enough for replacing\r\n");
                return 1;
            }
            tr_warn("old_str found\r\n");
            if (old_len != new_len) {
                tr_warn("memmove\r\n");
                memmove(ptr + new_len, ptr + old_len, strlen(ptr + old_len) + 1);
            }
            memcpy(ptr, new_str, new_len);
            ptr += new_len;
        }
    }
    return 0;
}
