#ifndef LDSCAR_H
#define LDSCAR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include "serial.h"
#include "mosquitto.h"
#include <pthread.h>
#include <fcntl.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include "assert.h"
using std::vector;
using serial::Serial;
using serial::PortInfo;
using serial::list_ports;
using std::string;

#define VERSION_STR "ldscar v0.1.2, compiled at " __TIME__ ", " __DATE__ 

extern void help_exit();
extern void list_exit();
extern void server_loop();
extern void log_callback(struct mosquitto *mosq, void *userdata, int level, const char *str);
extern void message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message);
extern void connect_callback(struct mosquitto *mosq, void *userdata, int result);
extern void subscribe_callback(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos);
extern void start_serial_server(Serial** uart_ptr, const char* device, pthread_t* thptr, void** args);

extern struct mosquitto* mosq;
extern const char* HOST;
extern int PORT;
extern const char* USERNAME;
extern const char* PASSWORD;

#define TEST_THREAD 0
#define TEST_UART1 1
#define TEST_LEGACY_UART1 2
extern void run_test_case(int test_case);

extern Serial* uart1;
extern Serial* uart2;

#define USEQOS 2

#endif
