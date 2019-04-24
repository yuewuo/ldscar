#include "ldscar.h"

struct mosquitto* mosq = NULL;
const char* HOST = "localhost";
int PORT = 1883;
const char* USERNAME = NULL;
const char* PASSWORD = NULL;

int main(int argc, char *argv[])
{
    char ch;
    bool run_server = false;
    int test_case = -1;
    if (argc == 1) help_exit();  // print help information
    while((ch = getopt(argc, argv, "lhsH:P:u:p:t:")) != EOF) {
        switch(ch) {
            case 'l':
                list_exit();  // will print port under system with its hardware_id, then exit
            case 'h': default:
                help_exit();
            case 's': run_server = true; break;
            case 'H': HOST = optarg; break;
            case 'P': PORT = atoi(optarg); break;
            case 'u': USERNAME = optarg; break;
            case 'p': PASSWORD = optarg; break;
            case 't': test_case = atoi(optarg); break;
        }
    }
    if (test_case >= 0) run_test_case(test_case);
    if (run_server) server_loop();
    return 0;
    
}

void server_loop() {
    // see https://blog.csdn.net/danwuxie/article/details/86517568
    mosquitto_lib_init();
    bool session = true;  // TODO what's this
    mosq = mosquitto_new(NULL, session, NULL);
    if(!mosq){
        fprintf(stderr, "create mqtt client failed..\n");
        mosquitto_lib_cleanup();
        exit(1);
    }
    mosquitto_log_callback_set(mosq, log_callback);
    mosquitto_connect_callback_set(mosq, connect_callback);
    mosquitto_message_callback_set(mosq, message_callback);
    if (USERNAME != NULL && PASSWORD != NULL) {
        mosquitto_username_pw_set(mosq, USERNAME, PASSWORD);
    }
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
    printf("version: %s\n", VERSION_STR);
    printf("usage: ldscar <args>\n");
    printf("possible arguments:\n");
    printf("    -l: list serial ports of the system\n");
    printf("    -s: run full function server\n");
    printf("    -t <testcase>: run test-cases, possible values are:\n");
    printf("        %d: test thread\n", TEST_THREAD);
    printf("        %d: test uart1 by sending \"hello\\n\" per second and print whatever received\n", TEST_UART1);
    printf("\n");
    printf("MQTT client setup (no password allowed):\n");
    printf("    -H <hostname>: MQTT broker hostname\n");
    printf("    -P <port>: MQTT broker port\n");
    printf("    -u <username>: MQTT username (if applicapble)\n");
    printf("    -p <password>: MQTT password (if applicapble)\n");
    printf("\n");
    exit(0);
}

void list_exit() {
    vector<PortInfo> ports = list_ports();
	for (vector<PortInfo>::iterator it = ports.begin(); it != ports.end(); ++it) {
		printf("port: \"%s\", fname: \"%s\", hardware_id: \"%s\"\n", it->port.c_str(), it->description.c_str(), it->hardware_id.c_str());
	}
    exit(0);
}
