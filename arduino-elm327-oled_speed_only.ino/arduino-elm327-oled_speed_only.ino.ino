#include <U8g2lib.h>
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R2, /* reset=*/ U8X8_PIN_NONE);

byte inData;
char inChar;
String RS="";
int obdSPD;

void setup() {
  u8g2.begin();
  Serial.begin(9600); 
  Serial1.begin(38400);
}

void loop() {  
  getSpeed();
  delay(100);
}

void getSpeed(){ // RS = "41 0D 82"; // SPD - 130
  getData("01 0D"); 
  u8g2.clearBuffer();
  
  u8g2.setFont(u8g2_font_fur17_tf); 
  u8g2.setCursor(0, 18);
  u8g2.print("Rychlost:");
//  obdSPD = 100;
  u8g2.setFont(u8g2_font_logisoso32_tf); 
  if(obdSPD < 10){
    u8g2.setCursor( 55, 60);
    u8g2.print(String(obdSPD));
  }
  if(obdSPD >= 10 && obdSPD < 100){
    u8g2.setCursor( 40, 60);
    u8g2.print(String(obdSPD));
  }
  if(obdSPD >= 100){
    u8g2.setCursor( 31, 60);
    u8g2.print(String(obdSPD));
  }  
  
  u8g2.sendBuffer();
}

void getData(const char *commnad){ 
  String WSA="";
  String WSB="";
  sendCommand(commnad);  
//  RS = " 41 0D 82"; // SPD - 130
  WSA = "0x" + RS.substring(7,9);
  obdSPD = strtol(WSA.c_str(),NULL,0);    
}

void sendCommand(const char *commnad){
  RS = "";  
  Serial1.println(String(commnad));  
  while(Serial1.available() > 0)
  {
    inData=0;
    inChar=0;
    inData = Serial1.read();
    inChar=char(inData);
    RS = RS + inChar;
  }  
  Serial.println(String(commnad)); 
  Serial.println(String(RS));  
  RS.replace(commnad,"");
  RS.replace(">","");
  RS.replace("OK","");
  RS.replace("STOPPED","");
  RS.replace("SEARCHING","");
  RS.replace("NO DATA","");
  RS.replace("?","");
  RS.replace(",","");
}

void initOBD(){
  sendCommand("ATZ");
  delay(200);
  sendCommand("ATSP0");
  delay(200);
  sendCommand("0100");
  delay(200);
  sendCommand("0120");
  delay(200);
  sendCommand("0140");
  delay(200);
}
