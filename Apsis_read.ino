#include "WiFi.h"
#include <vector>
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
int gun_moit = 0;
String gs_id, s_nomini, s_contentus;
String gs_contentus[12][32];
struct Node {
  int nodeId;
  bool isRoot;
  std::vector<Node> children;
};
float g_n_t = 0;
float g_n_h = 0;
float g_n_m = 0;
float g_n_l = 0;

void v_subdividere(String s_str){ //this function subdivides colon message 
String s_as=s_str.substring(0,s_str.indexOf(":"));
String s_ps=s_str.substring(s_str.indexOf(":")+1);
if (s_as=="")s_nomini+=s_ps+",";
else if(s_as=="t"){s_contentus+=s_ps+" degC\n";g_n_t=s_ps.toFloat();}
else if(s_as=="h"){s_contentus+=s_ps+" %RH\n";g_n_h=s_ps.toFloat();}
else if(s_as=="m"){s_contentus+=s_ps+" %MC\n";g_n_m=s_ps.toFloat();}
else if(s_as=="l"){s_contentus+=s_ps+" lx\n";g_n_l=s_ps.toFloat();}
else s_contentus+="NAN";
Serial.println("["+s_as+"]="+s_ps);
}
void s_audire(String s_str) { //decipher the ssid string
  String s_metg[32];
  int n_indx=s_str.indexOf(",");
  while (n_indx>0){
    String s_sbst=s_str.substring(0,n_indx);
    s_str=s_str.substring(n_indx+1);
    Serial.println(s_sbst);
    v_subdividere(s_sbst);
    for (int n_loc=0;n_loc<32;n_loc++)if(n_loc[s_metg]==""){
        n_loc[s_metg]=s_sbst;
        break;
    }
    if (s_str.indexOf(",")<0)return;
    s_audire(s_str);
    return;
  }
}

void v_appalere(String s_appl) {
  WiFi.mode(WIFI_AP);
  WiFi.disconnect();
  WiFi.softAP((":" + gs_id + "," + s_appl + ",").c_str(), "");
}

void v_apsificare(){
  s_nomini="";
  s_contentus="";
  Serial.println("scan start");
  int n = WiFi.scanNetworks(); //available number of networks
  Serial.println("scan done");
  for (int i = 0; i < n; ++i) if(WiFi.SSID(i).substring(0,1)==":")s_audire(WiFi.SSID(i)); //node ssid begins with a colon, so only decipher these ssids.
  Serial.println("______________");
}
void v_scandere(){

}
void v_displicare(){
  u8g2.firstPage();
  if(s_nomini!=""){
  u8g2.setCursor(0, 0);
  u8g2.setFont(u8g2_font_doomalpha04_te);
  u8g2.print(s_nomini);
  Serial.print(s_contentus);
    for (int i=0;;i++) { //automatic newline (carriage return)
      if(s_contentus.indexOf("\n")<=0) break;
      String s_cart_retr = s_contentus.substring(0,s_contentus.indexOf("\n"));
      s_contentus = s_contentus.substring(s_contentus.indexOf("\n")+1);
      u8g2.setCursor(0, i * 15 + 16);
      u8g2.print(s_cart_retr);
    }
  u8g2.setCursor(0, 32);
  //u8g2.print(s_contentus);
  }else{
  u8g2.setCursor(0, 0);
  u8g2.print("No Signal");
  }
u8g2.nextPage();
}
void v_irrigare(){
pinMode(26,OUTPUT);
digitalWrite(26,1);
delay(5000);
digitalWrite(26,0);
}
void setup() {
  Serial.begin(115200);
  u8g2.begin();
  u8g2.setFont(u8g2_font_doomalpha04_te);
  u8g2.enableUTF8Print();
  u8g2.setFontPosTop();
  u8g2.firstPage();
  u8g2.setCursor(0, 0);
  u8g2.print("SERVER");
  u8g2.sendBuffer();
  u8g2.nextPage();
  gs_id = WiFi.macAddress().substring(0, 2) + WiFi.macAddress().substring(15);
}

void loop() {
  v_apsificare();
  v_displicare();
}
