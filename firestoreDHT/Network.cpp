#include "Network.h"
#include "addons/Token"

#define WIFI_SSID "HUAWEI-E5330-9920"
#define WIFI_PASSWORD "14et72by"

#define API_KEY ""
#define FIREBSAE_PROJECT_ID ""
#define USER_EMAIL ""
#define USER_PASSWORD ""
 
static Network *instance = NULL;

Network::Network(){
  instance = this;
}

void WiFEventConnected(WiFiEvent_t event, WiFiEvent_t info){
  Serial.println("WIFI CONNECTED WAIT FOR IP");
}
void WiFEventGetIP(WiFiEvent_t event, WiFiEvent_t info){
  Serial.println("LOCAL IP ADRESS: ");
  Serial.println(WiFi.localIP());
  instnace->firebaseInit();
}

void WiFEventDisconnected(WiFiEvent_t event, WiFiEvent_t info){
  Serial.println("WIFI DISCONNECTED!");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void Network::initWIFI(){
  WIFI.disconected();
  WiFi.onEvent(WiFEventConnected, SYSTEM_EVENT_STA_CONNECTED);
  WiFi.onEvent(WiFEventGetIP, SYSTEM_EVENT_STA_GET_IP);
  WiFi.onEvent(WiFEventDisconnected, SYSTEM_EVENT_STA_DISCONNECTED);

  WIFI.begin(WIFI_SSID, WIFI_PASSWORD);
}

void Network::firebaseInit(){
config.api.key = API_KEY;

auth.user.email = USER_EMAIL;
auth.user.password = USER_PASSWORD;

Firebase.begin(&config, &auth);
}

void Network::firestoreDataUpdate(double temp, double humid){
  if(WiFi.Status() = WL_CONNECTED && Firebase.ready()){
    string documentPath = "SensorData";

    FirebaseJson content;

    content.set("fields/temperature/doubleValue", string(temp).c_stlr());
    content.set("fields/humid/doubleValue", string(humid).c_stlr());

    if(Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c.str(),content.raw()))
    Serial.println("ok\n%s\n\n", fbdo.payload().c.str())
    return;
  }else{
    serial.println(fbdo.errorReason());
  }
  
}


