#ifndef LDSCAR_H
#define LDSCAR_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include "serial.h"
#include "mosquitto.h"
#include <pthread.h>
#include <fcntl.h>
#include <string>
using std::vector;
using serial::Serial;
using serial::PortInfo;
using serial::list_ports;
using std::string;

#define VERSION_STR "ldscar v0.0.5"

extern void help_exit();
extern void list_exit();
extern void server_loop();
extern void log_callback(struct mosquitto *mosq, void *userdata, int level, const char *str);
extern void message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message);
extern void connect_callback(struct mosquitto *mosq, void *userdata, int result);
extern void subscribe_callback(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos);

extern struct mosquitto* mosq;
extern const char* HOST;
extern int PORT;
extern const char* USERNAME;
extern const char* PASSWORD;

#define TEST_THREAD 0
#define TEST_UART1 1
extern void run_test_case(int test_case);

#endif
