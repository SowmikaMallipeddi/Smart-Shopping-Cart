#include "WiFiProv.h"
#include "WiFi.h"

#define ar1 12
#define ar2 13

const char *pop = "abcd1234";
const char *service_name = "PROV_123";
const char *service_key = NULL;
bool reset_provisioned = true;

void SysProvEvent(arduino_event_t *sys_event) {
  switch (sys_event->event_id) {
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      Serial.print("\nConnected IP address : ");
      Serial.println(IPAddress(sys_event->event_info.got_ip.ip_info.ip.addr));
      Serial.print("RSSI: ");
      Serial.println(WiFi.RSSI());
      break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      Serial.println("\nDisconnected. Connecting to the AP again... ");
      break;
    case ARDUINO_EVENT_PROV_START:
      Serial.println("\nProvisioning started\nGive Credentials of your access point using smartphone app");
      break;
    case ARDUINO_EVENT_PROV_CRED_RECV:
      Serial.println("\nReceived Wi-Fi credentials");
      Serial.print("\tSSID : ");
      Serial.println((const char *)sys_event->event_info.prov_cred_recv.ssid);
      Serial.print("\tPassword : ");
      Serial.println((char const *)sys_event->event_info.prov_cred_recv.password);
      break;
    case ARDUINO_EVENT_PROV_CRED_FAIL:
      Serial.println("\nProvisioning failed!\nPlease reset to factory and retry provisioning\n");
      break;
    case ARDUINO_EVENT_PROV_CRED_SUCCESS:
      Serial.println("\nProvisioning Successful");
      break;
    case ARDUINO_EVENT_PROV_END:
      Serial.println("\nProvisioning Ends");
      break;
    default:
      break;
  }
}

void setup() {


  pinMode(ar1, OUTPUT);
  pinMode(ar2, OUTPUT);

  digitalWrite(ar1, LOW);
  digitalWrite(ar2, LOW);
  
  Serial.begin(115200);
  WiFi.begin();
  WiFi.onEvent(SysProvEvent);

#if CONFIG_BLUEDROID_ENABLED && !defined(USE_SOFT_AP)
  Serial.println("Begin Provisioning using BLE");
  uint8_t uuid[16] = {0xb4, 0xdf, 0x5a, 0x1c, 0x3f, 0x6b, 0xf4, 0xbf, 0xea, 0x4a, 0x82, 0x03, 0x04, 0x90, 0x1a, 0x02};
  WiFiProv.beginProvision(
    NETWORK_PROV_SCHEME_BLE, NETWORK_PROV_SCHEME_HANDLER_FREE_BLE, NETWORK_PROV_SECURITY_1, pop, service_name, service_key, uuid, reset_provisioned
  );
  log_d("ble qr");
  WiFiProv.printQR(service_name, pop, "ble");
#else
  Serial.println("Begin Provisioning using Soft AP");
  WiFiProv.beginProvision(NETWORK_PROV_SCHEME_SOFTAP, NETWORK_PROV_SCHEME_HANDLER_NONE, NETWORK_PROV_SECURITY_1, pop, service_name, service_key);
  log_d("wifi qr");
  WiFiProv.printQR(service_name, pop, "softap");
#endif
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Current RSSI: ");
    Serial.println(WiFi.RSSI());
    if(WiFi.RSSI() < -50 )
    {
     digitalWrite(ar2, HIGH);
     digitalWrite(ar1, LOW);
     Serial.println("Motors should run:");
     delay(1000);
     }
    else
    {
      digitalWrite(ar1, HIGH);
      digitalWrite(ar2, LOW);
      Serial.println("Products scanning:");
      delay(1000);
    }
  }
}
