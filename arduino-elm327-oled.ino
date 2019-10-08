#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#define SerialMon Serial

int obdRequestDelay = 500/3;

// "MEGA"
#define obdSerial Serial2 // MEGA

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R2, /* reset=*/ U8X8_PIN_NONE);

char str[32];

void setup() {
  u8g2.begin();
  SerialMon.begin(115200);
  obdSerial.begin(38400);
}

void loop() {
  draw();
}

 const char *getPid(String cmd)
{
  String request, response;
  byte inData;
  char inChar;
  request = cmd;
  obdSerial.println(cmd);
  
  delay(obdRequestDelay);
  
  while(obdSerial.available() > 0)
  {
    inData=0;
    inChar=0;
    inData = obdSerial.read();
    inChar=char(inData);
    response = response + inChar;
  }
    
  const char *value = "---";
  response.replace(request,"");
  response.replace(">","");
  response.replace("OK","");
  response.replace("STOPPED","");
  response.replace("SEARCHING","");
  response.replace("NO DATA","");
  response.replace("?","");
  response.replace(",","");
  
  String WSA = "0x" + response.substring(7,9);
  int A = strtol(WSA.c_str(),NULL,0);  
  
  String WSB = "0x" + response.substring(10,11);
  int B = strtol(WSB.c_str(),NULL,0); 
  
//  SerialMon.print("REQ: '" + request + "' RESP: '" + response +  "' A: '" + WSA + "' B: '" + WSB + "'");
  int val = 0;
  // ENGINE_RPM
  if (request == "01 0C"){
    val = ((A * 256)+B)/4;
  }
  
  // SPEED => 41 0C 1B E0
  else if (request == "01 0D"){
    val = A;
  }
  
  // Coolant Temp 
  else if (request == "01 05"){
    val = A - 40;
  }
  
  // Turbo
  else if(request == "01 0B"){   
    val = A;
  }
   
  // Air Intake Temp 
  else if (request == "01 0F"){
    val = A;
  }
  
  // Air flow Rate
  else if (request == "01 10"){ 
    val = ((A * 256)+B)/100;
  }
  
  //Ambient Temp 
  else if (request == "01 46"){
    val = A;
  }
  
  //Throttle position 
  else if (request == "01 11"){
    val = A;
  }
  
  //Barometric pressure
  else if (request == "01 33"){
    val = A;
  }
  
  //Engine Load
  else if (request == "01 04"){
    val = A/2.55;
  }
  
  //Fuel Tank Level
  else if (request == "01 2F"){
    val = A/2.55;
  }
  
  //Throttle Possition
  else if (request == "01 11"){
    val = A/2.55;
  }
  
  value = dtostrf(val, 4, 0, str); 
  
//  SerialMon.println(" VAL: '" + String(val) + "'");
  return value;
}

void draw()
{
  int width = 0;
  int offset = 0;
  u8g2.clearBuffer();
    char *spd = getPid("01 0D");
    u8g2.setFont(u8g2_font_logisoso38_tn);
    width = u8g2.getStrWidth(spd);
    offset =128/2 - width/2;
    u8g2.drawStr(offset, 38, spd);
    
    char *tmp = getPid("01 05");
    u8g2.setFont(u8g2_font_fur14_tf);
    width = u8g2.getStrWidth(tmp);
    offset = 128/4*3 - width/2;
    u8g2.drawStr(offset, 57, tmp);    
    
    char *rpm = getPid("01 0C");
    u8g2.setFont(u8g2_font_fur14_tf);
    width = u8g2.getStrWidth(rpm);
    offset =128/4 - width/2;
    u8g2.drawStr(offset, 57, rpm);
  u8g2.sendBuffer();
}
