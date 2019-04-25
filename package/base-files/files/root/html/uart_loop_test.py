# run this to test whether serial works well
import paho.mqtt.client as mqtt
import string, random, re, traceback, json, time, os, sys

REMOTE_IP = "192.168.0.104"

client_id = ''.join(random.sample(string.ascii_letters + string.digits, 8))
print("client_id:", client_id)

def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))
    client.subscribe("ldscar/uart1/recv")

def on_subscribe(client, userdata, mid, granted_qos):
    print("subscribed, wait 3s...")
    time.sleep(3)
    print("publish...")
    client.publish("ldscar/uart1/send", "*")

def on_message(client, userdata, msg):
    try:
        print(msg.topic + " " + ":" + str(msg.payload))
        client.publish("ldscar/uart1/send", "*")
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
