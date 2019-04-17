#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include "serial.h"
#include "mosquitto.h"
using std::vector;
using serial::PortInfo;
using serial::list_ports;

void help_exit();
void list_exit();
void server_loop();
struct mosquitto* mosq = NULL;
const char* HOST = "localhost";
int PORT = 1883;
bool session = true;  // TODO what's this
void log_callback(struct mosquitto *mosq, void *userdata, int level, const char *str);
void message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message);
void connect_callback(struct mosquitto *mosq, void *userdata, int result);
void subscribe_callback(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos);

int main(int argc, char *argv[])
{
    char ch;
    bool run_server = false;
    while((ch = getopt(argc, argv, "hlsn:p:")) != EOF) {
        switch(ch) {
        case 's': run_server = true; break;
        case 'n': HOST = optarg; break;
        case 'p': PORT = atoi(optarg); break;
        case 'l':
            list_exit();  // will print port under system with its hardware_id, then exit
        case 'h': default:
            help_exit();
        }
    }
    if (run_server) server_loop();
    // printf("hello world, my first self package \n\n");
    return 0;
    
}

void server_loop() {
    // see https://blog.csdn.net/danwuxie/article/details/86517568
    mosquitto_lib_init();
    mosq = mosquitto_new(NULL, session, NULL);
    if(!mosq){
        fprintf(stderr, "create mqtt client failed..\n");
        mosquitto_lib_cleanup();
        exit(1);
    }
    mosquitto_log_callback_set(mosq, log_callback);
    mosquitto_connect_callback_set(mosq, connect_callback);
    mosquitto_message_callback_set(mosq, message_callback);
    printf("mqtt client connecting to %s:%d\n", HOST, PORT);
    if(mosquitto_connect(mosq, HOST, PORT, 60)){
        fprintf(stderr, "Unable to connect.\n");
        return;
    }
    printf("mqtt client connect success\n");
    // mosquitto_loop_forever(mosq, -1, 1);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    printf("exit gracefully\n");
}

void help_exit(){
    int major, minor, revision;
    mosquitto_lib_version(&major, &minor, &revision);
    printf("ldscar utility with mosquitto %d.%d.%d\n", major, minor, revision);
    printf("usage: ldscar <args>\n");
    printf("possible arguments:\n");
    printf("    -l: list serial ports of the system\n");
    exit(0);
}

void list_exit() {
    vector<PortInfo> ports = list_ports();
	for (vector<PortInfo>::iterator it = ports.begin(); it != ports.end(); ++it) {
		printf("port: \"%s\", fname: \"%s\", hardware_id: \"%s\"\n", it->port.c_str(), it->description.c_str(), it->hardware_id.c_str());
	}
    exit(0);
}

void log_callback(struct mosquitto *mosq, void *userdata, int level, const char *str) {
    printf("%s\n", str);
}

void message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message) {
    if(message->payloadlen){
        printf("%s %s", message->topic, message->payload);
    }else{
        printf("%s (null)\n", message->topic);
    }
    fflush(stdout);
}
 
void connect_callback(struct mosquitto *mosq, void *userdata, int result) {
    int i;
    if(!result){
        /* Subscribe to broker information topics on successful connect. */
        mosquitto_subscribe(mosq, NULL, "Gai爷:", 2);
    }else{
        fprintf(stderr, "Connect failed\n");
    }
}
 
void subscribe_callback(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos)
{
    int i;
    printf("Subscribed (mid: %d): %d", mid, granted_qos[0]);
    for(i=1; i<qos_count; i++){
        printf(", %d", granted_qos[i]);
    }
    printf("\n");
}
