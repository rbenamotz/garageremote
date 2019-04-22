#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include "user_config.h"
#include "common.h"

ESP8266WiFiMulti wifiMulti;
String wifi_networks[] = WIFI_NETWORKS;

static void onStationModeConnected(const WiFiEventStationModeConnected &event)
{
    Serial.print("WiFi Connected. SSID=");
    Serial.println(event.ssid);
}

static void onStationModeDisconnected(const WiFiEventStationModeDisconnected &event)
{
    Serial.print("WiFi Disconnected. Reason code=");
    Serial.println(event.reason);
    globalIsWifiConnected = false;
}

static void onStationModeGotIP(const WiFiEventStationModeGotIP &event)
{
    //   MDNS.begin(HOST_NAME);
    Serial.print("WiFi Got IP. localIP=");
    Serial.print(event.ip);
    Serial.print(", hostName=");
    Serial.println(WiFi.hostname());
    globalIsWifiConnected = true;
}

static void onStationModeDHCPTimeout()
{
    Serial.println("WiFi DHCP timed out.");
    globalIsWifiConnected = false;
}

void setupWifi()
{
    static WiFiEventHandler e1 = WiFi.onStationModeConnected(onStationModeConnected);
    static WiFiEventHandler e2 = WiFi.onStationModeDisconnected(onStationModeDisconnected);
    static WiFiEventHandler e3 = WiFi.onStationModeGotIP(onStationModeGotIP);
    static WiFiEventHandler e4 = WiFi.onStationModeDHCPTimeout(onStationModeDHCPTimeout);
    WiFi.hostname(HOST_NAME);
    for (int i = 0; i < NUMBER_OF_WIFI_NETWORKS * 2; i += 2)
    {
        wifiMulti.addAP(wifi_networks[i].c_str(), wifi_networks[i + 1].c_str());
    }
}
void loopWifi()
{
    wifiMulti.run();
}
