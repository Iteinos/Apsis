#include "ESP8266WiFi.h"
#include <vector>
#include <U8g2lib.h>
#include <Adafruit_AHTX0.h>
Adafruit_AHTX0 aht;
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
float gf_temp = 0; 
float gf_humi = 0; 
//DECIPHER SSID LIST
String gs_id; //identity
void s_audire(String s_str) { //SSID audition
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  String s_metg[32]={""};
  int n_indx=s_str.indexOf(",");
  while (n_indx>0){
    s_metg[32];
    String s_sbst=s_str.substring(0,n_indx);
    s_str=s_str.substring(n_indx+1);
    Serial.println(s_sbst);
    for (int n_loc=0;n_loc<32;n_loc++)if(n_loc[s_metg]==""){
        n_loc[s_metg]=s_sbst;
        break;
    }
    if (s_str.indexOf(",")<0)return ;
    s_audire(s_str);
    return ;
  }
}

void v_appalere(String s_appl) {
  WiFi.mode(WIFI_AP);
  WiFi.disconnect();
  WiFi.softAP((":" + gs_id + "," + s_appl + ",").c_str(), "");
}
void v_apsificare(){
  Serial.println("scan start");
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) Serial.println("no networks found");
  else
    for (int i = 0; i < n; ++i) s_audire(WiFi.SSID(i));
  Serial.println("______________");
}
void v_scandere(){
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
  gf_temp=temp.temperature;
  gf_humi=humidity.relative_humidity;
}
void v_displicare(){
  v_scandere();
  u8g2.firstPage();
  u8g2.setFontPosTop();
  u8g2.setCursor(0, 0);
  u8g2.print("ID: "+gs_id);
  u8g2.setCursor(0, 16);
  u8g2.print(String(gf_temp)+" degC");
  u8g2.setCursor(0, 32);
  u8g2.print(String(gf_humi)+" %RH");
  //u8g2.setCursor(0, 48);
  //u8g2.print(gs_id);
  u8g2.nextPage();
}
void setup() {
  Serial.begin(115200);
  aht.begin();
  u8g2.begin();
  u8g2.setFont(u8g2_font_doomalpha04_te);
  u8g2.enableUTF8Print();
  gs_id = WiFi.macAddress().substring(0, 2) + WiFi.macAddress().substring(15);
}

void loop() {
  v_displicare();
  v_appalere("t:"+String(gf_temp)+",h:"+String(gf_humi));
  delay(500);
}
