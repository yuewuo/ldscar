#include "ldscar.h"

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
        mosquitto_subscribe(mosq, NULL, "", 2);
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
