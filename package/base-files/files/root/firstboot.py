#!/usr/bin/python3

import sys, os

# you can setup your SSID and password here
wifi_sta_ssid = "wulinger"
wifi_sta_password = "number502daxinke"
wan_ports = [80, 22, 8080, 1883]






nginx_conf = """
user nobody nogroup;
worker_processes 1;
events {
    worker_connections  1024;
}
http {
    include mime.types;
    sendfile on;
    root /root/html;
    keepalive_timeout  65;
    server {
        listen 80;
        server_name ldscar;
        location / {
            index index.html index.htm;
        }
        location ~ ^\/mqtt$ {
            proxy_pass http://127.0.0.1:9000;
            proxy_http_version 1.1;
            proxy_set_header Upgrade $http_upgrade;
            proxy_set_header Connection "upgrade";
        }
    }
}
"""
print("setting nginx reverse proxy...")
with open("/etc/nginx/nginx.conf", "w") as f:
    f.write(nginx_conf)
os.system("/etc/init.d/nginx restart")

mosquitto_conf = """
port 1883
protocol mqtt
listener 9000
protocol websockets
"""
print("setting mosquitto server with websocket enabled...")
with open("/etc/mosquitto/mosquitto.conf", "w") as f:
    f.write(mosquitto_conf)
os.system("/etc/init.d/mosquitto restart")

def firewall_conf_append(port):
    return """
config rule
    option src wan
    option dest_port %d
    option target ACCEPT
    option proto TCP
""" % port
print("setting firewall enable", wan_ports, "ports...")
with open("/etc/config/firewall", "a") as f:
    for port in wan_ports:
        f.write(firewall_conf_append(port))
os.system("/etc/init.d/firewall restart")

wireless_conf = """
config wifi-device 'ra0'
    option type 'rt2860v2'
    option mode '7'
    option channel 'auto'
    option txpower '100'
    option ht '40'
    option country 'CN'
    option antnum '2'
    option disabled '0'

config wifi-iface 'ap'
    option disabled '1'
    option device 'ra0'
    option network 'lan'
    option mode 'ap'
    option ssid 'YDH-2.4GHz-2F0EFC'
    option encryption 'psk2'
    option key '12345678'

config wifi-iface 'sta'
    option disabled '0'
    option device 'ra0'
    option network 'wan'
    option mode 'sta'
    option ssid '""" + wifi_sta_ssid + """'
    option encryption 'psk2'
    option key '""" + wifi_sta_password + """'
"""
print("setting wireless network with sta enabled...")
with open("/etc/config/wireless", "w") as f:
    f.write(wireless_conf)
os.system("/etc/init.d/network restart")

