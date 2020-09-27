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

/**
 * \file ns_cmdline.h
 *
 * Command line library - mbedOS shell
 *
 * Usage example:
 *
 * \code
 * //simple print function
 * void myprint(const char* fmt, va_list ap){ vprintf(fmt, ap); }
 * // simple ready cb, which call next command to be execute
 * void cmd_ready_cb(int retcode) { cmd_next( retcode ); }
 *
 * // dummy command with some option
 * int cmd_dummy(int argc, char *argv[]){
 *  if( cmd_has_option(argc, argv, "o") ) {
 *    cmd_printf("This is o option");
 *  } else {
 *    return CMDLINE_RETCODE_INVALID_PARAMETERS;
 *  }
 *  return CMDLINE_RETCODE_SUCCESS;
 *}
 * // timer cb ( pseudo-timer-code )
 * void timer_ready_cb(void) {
 *   cmd_ready(CMDLINE_RETCODE_SUCCESS);
 * }
 * // long command, which need e.g. some events to finalize command execution
 * int cmd_long(int argc, char *argv[] ) {
     timer_start( 5000, timer_ready_cb );
 *   return CMDLINE_RETCODE_EXCUTING_CONTINUE;
 * }
 * void main(void) {
 *   cmd_init( &myprint );              // initialize cmdline with print function
 *   cmd_set_ready_cb( cmd_ready_cb );  // configure ready cb
 *   cmd_add("dummy", cmd_dummy, 0, 0); // add one dummy command
 *   cmd_add("long", cmd_long, 0, 0);   // add one dummy command
 *   //execute dummy and long commands
 *   cmd_exe( "dymmy;long" );
 * }
 * \endcode
 */
#ifndef _CMDLINE_H_
#define _CMDLINE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define CMDLINE_RETCODE_COMMAND_BUSY            2   //!< Command Busy
#define CMDLINE_RETCODE_EXCUTING_CONTINUE       1   //!< Execution continue in background
#define CMDLINE_RETCODE_SUCCESS                 0   //!< Execution Success
#define CMDLINE_RETCODE_FAIL                    -1  //!< Execution Fail
#define CMDLINE_RETCODE_INVALID_PARAMETERS      -2  //!< Command parameters was incorrect
#define CMDLINE_RETCODE_COMMAND_NOT_IMPLEMENTED -3  //!< Command not implemented
#define CMDLINE_RETCODE_COMMAND_CB_MISSING      -4  //!< Command callback function missing
#define CMDLINE_RETCODE_COMMAND_NOT_FOUND       -5  //!< Command not found

/**
 * typedef for print functions
 */
typedef void (cmd_print_t)(const char *, va_list);
/**
 * Initialize cmdline class.
 * This is command line editor without any commands. Application
 * needs to add commands that should be enabled.
 * usage e.g.
 * \code
    cmd_init( &default_cmd_response_out );
 * \endcode
 * \param outf  console printing function (like vprintf)
 */
void cmd_init(cmd_print_t *outf);
/** Command ready function for __special__ cases.
 * This need to be call if command implementation return CMDLINE_RETCODE_EXECUTING_CONTINUE
 * because there is some background stuff ongoing before command is finally completed.
 * Normally there is some event, which call cmd_ready().
 * \param retcode return code for command
 */
void cmd_ready(int retcode);
/** typedef for ready cb function */
typedef void (cmd_ready_cb_f)(int);
/**
 * Configure cb which will be called after commands are executed
 * or cmd_ready is called
 * \param cb    callback function for command ready
 */
void cmd_set_ready_cb(cmd_ready_cb_f *cb);
/**
 * execute next command if any
 * \param retcode last command return value
 */
void cmd_next(int retcode);
/** Free cmd class */
void cmd_free(void);
/** Reset cmdline to default values
 *  detach external commands, delete all variables and aliases
 */
void cmd_reset(void);
/** Configure command history size (default 32)
 *  \param max  maximum history size
 *  max > 0 -> configure new value
 *  max = 0 -> just return current value
 *  \return current history max-size
 */
uint8_t cmd_history_size(uint8_t max);
/** command line print function
 *  This function should be used when user want to print something to the console
 *  \param fmt   console print function (like printf)
 */
#if defined(__GNUC__)
void cmd_printf(const char *fmt, ...)  __attribute__((__format__(__printf__, 1, 2)));
#else
void cmd_printf(const char *fmt, ...);
#endif
/** command line print function
 *  This function should be used when user want to print something to the console with vprintf functionality
 *  \param fmt  The format string is a character string, beginning and ending in its initial shift state, if any. The format string is composed of zero or more directives.
 *  \param ap   list of parameters needed by format string. This must correspond properly with the conversion specifier.
 */
#if defined(__GNUC__)
void cmd_vprintf(const char *fmt, va_list ap)  __attribute__((__format__(__printf__, 1, 0)));
#else
void cmd_vprintf(const char *fmt, va_list ap);
#endif
/** Reconfigure default cmdline out function (cmd_printf)
 *  \param outf  select console print function
 */
void cmd_out_func(cmd_print_t *outf);
/** Configure function, which will be called when Ctrl+A is pressed
 * \param sohf control function which called every time when user input control keys
 */
void cmd_ctrl_func(void (*sohf)(uint8_t c));
/**
 * Configure mutex wait function
 * By default, cmd_printf calls may not be thread safe, depending on the implementation of the used output.
 * This can be used to set a callback function that will be called before each cmd_printf call.
 * The specific implementation is up to the application developer, but simple mutex locking is assumed.
 */
void cmd_mutex_wait_func(void (*mutex_wait_f)(void));
/**
 * Configure mutex wait function
 * By default, cmd_printf calls may not be thread safe, depending on the implementation of the used output.
 * This can be used to set a callback function that will be called after each cmd_printf call.
 * The specific implementation is up to the application developer, but simple mutex locking is assumed.
 */
void cmd_mutex_release_func(void (*mutex_release_f)(void));
/**
 * Retrieve output mutex lock
 * This can be used to retrieve the output mutex when multiple cmd_printf/cmd_vprintf calls must be
 * guaranteed to be grouped together in a thread safe manner. Must be released by a following call to
 * cmd_mutex_unlock()
 * For example:
 * * \code
 * cmd_mutex_lock();
   for (i = 0; i < 10; i++) {
       cmd_printf("%02x ", i);
   }
   // without locking a print from another thread could happen here
   cmd_printf("\r\n);
   cmd_mutex_unlock();
 * \endcode
 * Exact behaviour depends on the implementation of the configured mutex,
 * but counting mutexes are required.
 */
void cmd_mutex_lock(void);
/**
 * Release output mutex lock
 * This can be used to release the output mutex once it has been retrieved with cmd_mutex_lock()
 * Exact behaviour depends on the implementation of the configured mutex,
 * but counting mutexes are required.
 */
void cmd_mutex_unlock(void);
/** Refresh output */
void cmd_output(void);
/** default cmd response function, use stdout
 *  \param fmt  The format string is a character string, beginning and ending in its initial shift state, if any. The format string is composed of zero or more directives.
 *  \param ap   list of parameters needed by format string. This must correspond properly with the conversion specifier.
 */
void default_cmd_response_out(const char *fmt, va_list ap);
/** Initialize screen */
void cmd_init_screen(void);
/** Get echo state
 * \return true if echo is on otherwise false
 */
bool cmd_echo_state(void);
/** Echo off */
void cmd_echo_off(void);
/** Echo on */
void cmd_echo_on(void);
/** Enter character to console.
 * insert key pressess to cmdline called from main loop of application
 * \param u_data char to be added to console
 */
void cmd_char_input(int16_t u_data);
/*
 * Set the passthrough mode callback function. In passthrough mode normal command input handling is skipped and any
 * received characters are passed to the passthrough callback function. Setting this to null will disable passthrough mode.
 * \param passthrough_fnc The passthrough callback function
 */
typedef void (*input_passthrough_func_t)(uint8_t c);
void cmd_input_passthrough_func(input_passthrough_func_t passthrough_fnc);

/* Methods used for adding and handling of commands and aliases
 */

/** Callback called when your command is run.
 * \param argc argc is the count of arguments given in argv pointer list. value begins from 1 and this means that the 0 item in list argv is a string to name of command.
 * \param argv argv is list of arguments. List size is given in argc parameter. Value in argv[0] is string to name of command.
 */
typedef int (cmd_run_cb)(int argc, char *argv[]);
/** Add command to intepreter
 * \param name      command string
 * \param callback  This function is called when command line start executing
 * \param info      Command short description which is visible in help command, or null if not in use
 * \param man       Help page for this command. This is shown when executing command with invalid parameters or command with --help parameter. Can be null if not in use.
 */
void cmd_add(const char *name, cmd_run_cb *callback, const char *info, const char *man);

/** delete command from intepreter
 *  \param name command to be delete
 */
void cmd_delete(const char *name);
/** Command executer.
 * Command executer, which split&push command(s) to the buffer and
 * start executing commands in cmd tasklet.
 * if not, execute command directly.
 * If command implementation returns CMDLINE_RETCODE_EXCUTING_CONTINUE,
 * executor will wait for cmd_ready() before continue to next command.
 * \param str  command string, e.g. "help"
 */
void cmd_exe(char *str);
/** Add alias to interpreter.
 * Aliases are replaced with values before executing a command. All aliases must be started from beginning of line.
 * null or empty value deletes alias.
 * \code
   cmd_alias_add("print", "echo");
   cmd_exe("print \"hello world!\""); // this is now same as "echo \"hello world!\"" .
 * \endcode
 * \param alias     alias name
 * \param value     value for alias. Values can be any visible ASCII -characters.
 */
void cmd_alias_add(const char *alias, const char *value);
/** Add Variable to interpreter.
 * Variables are replaced with values before executing a command.
 * To use variables from cli, use dollar ($) -character so that interpreter knows user want to use variable in that place.
 * null or empty value deletes variable.
 * \code
   cmd_variable_add("world", "hello world!");
   cmd_exe("echo $world"); // this is now same as echo "hello world!" .
 * \endcode
 * \param variable  Variable name, which will be replaced in interpreter.
 * \param value     Value for variable. Values can contains white spaces and '"' or '"' characters.
 */
void cmd_variable_add(char *variable, char *value);
/**
 * Add integer variable to interpreter.
 * Variables are replaced with values before executing a command.
 * \code
   cmd_variable_add_int("world", 2);
   cmd_exe("echo $world"); // this is now same as 'echo 2' .
 * \endcode
 * \param variable  Variable name, which will be replaced in interpreter.
 * \param value     Value for variable

 */
void cmd_variable_add_int(char *variable, int value);
/**
 * Request screen size from host
 * Response are stored to variables:
 * COLUMNS and LINES - as integer values.
 * Note: Require terminal that handle request codes, like screen.
 */
void cmd_request_screen_size(void);

/** find command parameter index by key.
 * e.g.
 * \code
      int main(void){
            //..init cmd..
            //..
            cmd_exe("mycmd enable")
      }
      int mycmd_command(int argc, char *argv[]) {
        bool found = cmd_parameter_index( argc, argv, "enable" ) > 0;
      }
 * \endcode
 * \param argc  argc is the count of arguments given in argv pointer list. value begins from 1 and this means that the 0 item in list argv is a string to name of command.
 * \param argv  is list of arguments. List size is given in argc parameter. Value in argv[0] is string to name of command.
 * \param key   option key, which index you want to find out.
 * \return index where parameter was or -1 when not found
 */
int cmd_parameter_index(int argc, char *argv[], const char *key);
/** check if command option is present.
 * e.g. cmd: "mycmd -c"
 * \code
 *    bool on = cmd_has_option( argc, argv, "p" );
 * \endcode
 * \param argc  argc is the count of arguments given in argv pointer list. value begins from 1 and this means that the 0 item in list argv is a string to name of command.
 * \param argv  is list of arguments. List size is given in argc parameter. Value in argv[0] is string to name of command.
 * \param key   option key to be find
 * \return true if option found otherwise false
 */
bool cmd_has_option(int argc, char *argv[], const char *key);
/** find command parameter by key.
 * if exists, return true, otherwise false.
 * e.g. cmd: "mycmd enable 1"
 * \code
  int mycmd_command(int argc, char *argv[]) {
        bool value;
        bool found = cmd_parameter_bool( argc, argv, "mykey", &value );
        if( found ) return CMDLINE_RETCODE_SUCCESS;
        else return CMDLINE_RETCODE_FAIL;
    }
 * \endcode
 * \param argc  argc is the count of arguments given in argv pointer list. value begins from 1 and this means that the 0 item in list argv is a string to name of command.
 * \param argv  is list of arguments. List size is given in argc parameter. Value in argv[0] is string to name of command.
 * \param key   parameter key to be find
 * \param value parameter value to be fetch, if key not found value are untouched. "1" and "on" and "true" and "enable" and "allow" are True -value, all others false.
 * \return true if parameter key and value found otherwise false
 */
bool cmd_parameter_bool(int argc, char *argv[], const char *key, bool *value);
/** find command parameter by key and return value (next parameter).
 * if exists, return parameter pointer, otherwise null.
 * e.g. cmd: "mycmd mykey myvalue"
 * \code
    int mycmd_command(int argc, char *argv[]) {
        char *value;
        bool found = cmd_parameter_val( argc, argv, "mykey", &value );
        if( found ) return CMDLINE_RETCODE_SUCCESS;
        else return CMDLINE_RETCODE_FAIL;
    }
 * \endcode
 * \param argc  argc is the count of arguments given in argv pointer list. value begins from 1 and this means that the 0 item in list argv is a string to name of command.
 * \param argv  is list of arguments. List size is given in argc parameter. Value in argv[0] is string to name of command.
 * \param key   parameter key to be find
 * \param value pointer to pointer, which will point to cli input data when key and value found. if key or value not found this parameter are untouched.
 * \return true if parameter key and value found otherwise false
 */
bool cmd_parameter_val(int argc, char *argv[], const char *key, char **value);
/** find command parameter by key and return value (next parameter) in integer. Only whitespaces are allowed in addition to the float to be read.
 * e.g. cmd: "mycmd mykey myvalue"
 * \code
     int32_t value;
     cmd_parameter_int( argc, argv, "key", &value );
 * \endcode
 * \param argc  argc is the count of arguments given in argv pointer list. value begins from 1 and this means that the item 0 in the list argv is a string to name of command.
 * \param argv  is list of arguments. List size is given in argc parameter. Value in argv[0] is string to name of command.
 * \param key   parameter key to be found
 * \param value A pointer to a variable where to write the converted number. If value cannot be converted, it is not touched.
 * \return true if parameter key and an integer is found, otherwise return false
 */
bool cmd_parameter_int(int argc, char *argv[], const char *key, int32_t *value);
/** find command parameter by key and return value (next parameter) in float. Only whitespaces are allowed in addition to the float to be read.
 * e.g. cmd: "mycmd mykey myvalue"
 * \code
     float value;
     cmd_parameter_float( argc, argv, "key", &value );
 * \endcode
 * \param argc  argc is the count of arguments given in argv pointer list. values begin from 1 and this means that the item 0 in the list argv is a string to name of command.
 * \param argv  is list of arguments. List size is given in argc parameter. Value in argv[0] is string to name of command.
 * \param key   parameter key to be found
 * \param value A pointer to a variable where to write the converted number. If value cannot be converted, it is not touched.
 * \return true if parameter key and a float found, otherwise return false
 */
bool cmd_parameter_float(int argc, char *argv[], const char *key, float *value);
/** Get last command line parameter as string.
 * e.g.
 *     cmd: "mycmd hello world"
 *     cmd_parameter_last -> "world"
 *     cmd: "mycmd"
 *     cmd_parameter_last() -> NULL
 * \code
    cmd_parameter_last(argc, argv)
 * \endcode
 * \param argc  argc is the count of arguments given in argv pointer list. value begins from 1 and this means that the 0 item in list argv is a string to name of command.
 * \param argv  is list of arguments. List size is given in argc parameter. Value in argv[0] is string to name of command.
 * \return pointer to last parameter or NULL when there is no any parameters.
 */
char *cmd_parameter_last(int argc, char *argv[]);

/** find command parameter by key and return value (next parameter) in int64.
 * e.g. cmd: "mycmd mykey myvalue"
 * \code
     uint32_t i;
     cmd_parameter_timestamp( argc, argv, "mykey", &i );
 * \endcode
 *
 * Supports following formats:
 *  number -> direct conversion
 *  11:22:33:44:55:66:77:88 -> converts to number
 *  seconds,tics -> converts thread type timestamp to int64
 *
 * \param argc  argc is the count of arguments given in argv pointer list. value begins from 1 and this means that the 0 item in list argv is a string to name of command.
 * \param argv  is list of arguments. List size is given in argc parameter. Value in argv[0] is string to name of command.
 * \param key   parameter key to be find
 * \param value parameter value to be fetch, if key not found value are untouched.
 * \return true if parameter key and value found otherwise false
 */
bool cmd_parameter_timestamp(int argc, char *argv[], const char *key, int64_t *value);

#ifdef __cplusplus
}
#endif
#endif /*_CMDLINE_H_*/
