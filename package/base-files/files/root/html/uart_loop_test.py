# run this to test whether serial works well
import paho.mqtt.client as mqtt
import string, random, re, traceback, json, time, os, sys

REMOTE_IP = "192.168.0.104"

"""
usage:

the server will listen 3 topic for uart1: (the same for uart2)
    "ldscar/uart1/send"
    "ldscar/uart1/flush"
    "ldscar/uart1/recvdelay/set_ms"
and will publish to
    "ldscar/uart1/recv"
if bytes are received

the "recvdelay/set_ms" is used to set delay time after at least one byte is available, in millisecond
a maximum value of 10000ms is allowed. you wouldn't need larger than 10s delay time, are you?
in this demo program, we will not set this.

"""






client_id = ''.join(random.sample(string.ascii_letters + string.digits, 8))
print("client_id:", client_id)

def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))
    client.subscribe("ldscar/uart1/recv")

def on_subscribe(client, userdata, mid, granted_qos):
    print("subscribed, wait 3s...")
    time.sleep(1)
    print("publish...")
    client.publish("ldscar/uart1/send", "*")

cnt = 0
def on_message(client, userdata, msg):
    global cnt
    try:
        print(msg.topic + " " + ":" + str(msg.payload))
        # time.sleep(1)
        client.publish("ldscar/uart1/send", "%d" % cnt)
        cnt += 1
    except Exception as e:
        traceback.print_exc()

def on_log(client, userdata, level, buf):
    print(buf)
    pass

client = mqtt.Client(client_id)
client.on_connect = on_connect
client.on_message = on_message
client.on_log = on_log
client.on_subscribe = on_subscribe
client.connect(REMOTE_IP, 1883, 60)
client.loop_forever()
