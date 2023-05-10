#ifdef Network_H_
#define Network_H_

#include <WiFi.h>
#include <Firebase_ESP_Client.h>
class Network{
private: 
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfic config;



void firebaseInit();
  friend void WiFEventConnected(WiFiEvent_t event, WiFiEvent_t info);
  friend void WiFEventGetIP(WiFiEvent_t event, WiFiEvent_t info);
  friend void WiFEventDisconnected(WiFiEvent_t event, WiFiEvent_t info);
  friend void firestoreTokenStatusCallback(tokenInfo info);

public:
  Network();
  void initWIFI();
  void firestoreDatabaseUpdate(double temp. double humid); 
};

#endif