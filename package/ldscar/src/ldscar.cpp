#include "ldscar.h"

struct mosquitto* mosq = NULL;
const char* HOST = "localhost";
int PORT = 1883;
const char* USERNAME = NULL;
const char* PASSWORD = NULL;
Serial* uart1;
Serial* uart2;

int main(int argc, char *argv[])
{
    char ch;
    bool run_server = false;
    int test_case = -1;
    if (argc == 1) help_exit();  // print help information
    while((ch = getopt(argc, argv, "lhsH:P:u:p:t:o:")) != EOF) {
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
            case 'o': freopen(optarg, "w" ,stdout); break;
        }
    }
    if (test_case >= 0) run_test_case(test_case);
    if (run_server) server_loop();
    return 0;
    
}

void help_exit(){
    int major, minor, revision;
    mosquitto_lib_version(&major, &minor, &revision);
    printf("ldscar utility with mosquitto %d.%d.%d\n", major, minor, revision);
    printf("version: \e[0;32m%s\e[0m\n", VERSION_STR);
    printf("usage: ldscar <args>\n");
    printf("possible arguments:\n");
    printf("    -l: list serial ports of the system\n");
    printf("    -s: run full function server\n");
    printf("    -t <testcase>: run test-cases, possible values are:\n");
    printf("        %d: test thread\n", TEST_THREAD);
    printf("        %d: test uart1 by sending \"hello\\n\" per second and print whatever received\n", TEST_UART1);
    printf("        %d: test legacy uart1 read-write, the same behaviour with above\n", TEST_LEGACY_UART1);
    printf("    -o <filename>: redirect stdout and stderr to file\n");
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
