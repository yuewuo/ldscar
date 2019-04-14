import paho.mqtt.publish as publish

host = "192.168.1.1"  # your ldscar ip
port = 1883
username = ""
password = ""
topic = "ldscar"

command = "hello lds!"

print(command)

publish.single(topic, command, hostname=host, port=port)
