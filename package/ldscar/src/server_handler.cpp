#include "ldscar.h"

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

    // setup serial port
    pthread_t thread_uart1;
    void* args_uart1[2] = { NULL, (void*)"ldscar/uart1/recv" };
    start_serial_server(&uart1, "/dev/ttyS1", &thread_uart1, (void**)&args_uart1);
    pthread_t thread_uart2;
    void* args_uart2[2] = { NULL, (void*)"ldscar/uart2/recv" };
    start_serial_server(&uart2, "/dev/ttyS1", &thread_uart2, (void**)&args_uart2);

    fflush(stdout);

    mosquitto_loop_forever(mosq, -1, 1);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    pthread_join(thread_uart1, NULL);
    pthread_join(thread_uart2, NULL);
    printf("exit gracefully\n");
}

void* serial_receiver(void* data) {
    void** args = (void**)data;
    Serial& uart = *(Serial*)args[0];
    const char* topic = (const char*)args[1];
    printf("serial_listener start, publish on %s\n", topic);
    uint8_t buf[1024];
    while (1) {
        // size_t len = uart.read(buf, sizeof(buf));
        size_t len = uart.read(buf, 1);
        // printf("receive timeout\n");
        if (len) {  // received
            printf("recv %d\n", len);
            mosquitto_publish(mosq, NULL, topic, len, buf, 0, 0);
        }
    }
}

void* serial_sender(void* data) {
    void** args = (void**)data;
    Serial& uart = *(Serial*)args[0];
    while (1) {
        sleep(5);
        uart.write(string("sender"));
    }
}

void start_serial_server(Serial** uart_ptr, const char* device, pthread_t* thptr, void** args) {
    printf("opening %s...\n", device);
    *uart_ptr = new Serial("/dev/ttyS1", 115200, serial::Timeout::simpleTimeout(1000));
    Serial& uart = **uart_ptr;
    if (uart.isOpen()) printf("uart opened\n");
    else { printf("open uart failed, exit\n"); exit(-1); }
    args[0] = (void*)*uart_ptr;
    // pthread_create(thptr, NULL, serial_sender, (void*)args);
    pthread_create(thptr, NULL, serial_receiver, (void*)args);
}

void log_callback(struct mosquitto *mosq, void *userdata, int level, const char *str) {
    printf("%s\n", str);
}

void message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message) {
    if (strcmp(message->topic, "ldscar/query") == 0) {
        // reply to query
        mosquitto_publish(mosq, NULL, "ldscar/info", strlen(VERSION_STR), VERSION_STR, 0, 0);
    } else if (strcmp(message->topic, "ldscar/uart1/send") == 0) {
        printf("payloadlen = %d, payload= %s\n", message->payloadlen, message->payload);
        uart1->write((const uint8_t*)message->payload, message->payloadlen);
        // uart1->flush();
    } else if (strcmp(message->topic, "ldscar/uart2/send") == 0) {
        uart2->write((const uint8_t*)message->payload, message->payloadlen);
        // uart2->flush();
    }
}

void connect_callback(struct mosquitto *mosq, void *userdata, int result) {
    int i;
    if(!result){
        /* Subscribe to broker information topics on successful connect. */
        mosquitto_subscribe(mosq, NULL, "ldscar/query", 2);
        mosquitto_subscribe(mosq, NULL, "ldscar/uart1/send", 2);
        mosquitto_subscribe(mosq, NULL, "ldscar/uart2/send", 2);
    }else{
        fprintf(stderr, "Connect failed\n");
    }
}
 
void subscribe_callback(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos)
{
    // int i;
    // printf("Subscribed (mid: %d): %d", mid, granted_qos[0]);
    // for(i=1; i<qos_count; i++){
    //     printf(", %d", granted_qos[i]);
    // }
    // printf("\n");
}
