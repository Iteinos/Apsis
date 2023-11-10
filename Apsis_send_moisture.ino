#include "WiFi.h"
#include <vector>
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
int gn_moit = 0; //moisture
//DECIPHER SSID LIST
String gs_id; //identity
String s_audire(String s_str) { //SSID audition
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  String s_metg[32];
  int n_indx=s_str.indexOf(",");
  while (n_indx>0){
    String s_sbst=s_str.substring(0,n_indx);
    s_str=s_str.substring(n_indx+1);
    Serial.println(s_sbst);
    for (int n_loc=0;n_loc<32;n_loc++)if(n_loc[s_metg]==""){
        n_loc[s_metg]=s_sbst;
        break;
    }
    if (s_str.indexOf(",")<0)return s_metg[32];
    s_audire(s_str);
    return s_metg[32];
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
  gn_moit=map(analogRead(34),2900,1100,0,99);
  if(gn_moit<0)gn_moit=0;
  if(gn_moit>99)gn_moit=99;
}
void v_displicare(){
  v_scandere();
  u8g2.firstPage();
  u8g2.setFontPosTop();
    u8g2.setCursor(0, 32);
  u8g2.print("MOISTURE");
  u8g2.setCursor(0, 16);
  u8g2.print("SOIL");
  u8g2.setCursor(0, 0);
  u8g2.print(String(gn_moit)+" %");
  u8g2.nextPage();
}
void setup() {
  Serial.begin(115200);
  pinMode(34,INPUT);
  u8g2.begin();
  u8g2.setFont(u8g2_font_commodore64_tr);//u8g2_font_commodore64_tr  u8g2_font_doomalpha04_te
  u8g2.enableUTF8Print();
  //gs_id = WiFi.macAddress().substring(0, 2) + WiFi.macAddress().substring(15);
}

void loop() {
  v_displicare();
  //v_appalere("m:"+String(gn_moit));
  ESP.deepSleep(2e6); 
}
