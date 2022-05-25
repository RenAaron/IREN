#include <IRremote.h>
#include <Keyboard.h>

#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);
String state = "";
int flag = 0;

const int hotKey1 = 16;
const int hotKey2 = 10;
const int hotKey3 = 14;
const int hotKey4 = 15;

int piezo = 8;

int TX = 4;
int RX = 6;
int IRS = 5;

const int RECV_PIN = 7;
IRrecv irrecv(RECV_PIN);
decode_results results;

#include <IRremote.h>
IRsend irsend;

char *hkLinks[] = {"", "1", "", "2"};
int hkIrCodes[] = {0xff02fD, 0, 0xff609f, 0};

void intro(){
  for(int i = 0; i < 4; i++){
    delay(20);
    digitalWrite(A2, LOW);
    tone(8, 450*i);
    delay(50);
    noTone(8);
    digitalWrite(A2, HIGH);
  }

  for(int i = 0; i < 4; i++){
    delay(20);
    digitalWrite(A3, LOW);
    tone(8, 350*i);
    delay(50);
    noTone(8);
    digitalWrite(A3, HIGH);
  }

  for(int i = 0; i < 4; i++){
    delay(20);
    digitalWrite(A0, LOW);
    tone(8, 250*i);
    delay(50);
    noTone(8);
    digitalWrite(A0, HIGH);
  }

  for(int i = 0; i < 4; i++){
    delay(20);
    digitalWrite(A1, LOW);
    tone(8, 150*i);
    delay(50);
    noTone(8);
    digitalWrite(A1, HIGH);
  }
}

void openLink(String Link){
  // keyboard emulator stuffs here please
  statusLED(TX, 1, 100, 500);
  Keyboard.press(KEY_LEFT_GUI);
  delay(200);
  Keyboard.release(KEY_LEFT_GUI);
  delay(100);
  Keyboard.print(Link);
  delay(500);
  Keyboard.press(KEY_RETURN);
  delay(100);
  Keyboard.release(KEY_RETURN);
  Keyboard.releaseAll();
}

void keyListen(int lisKey, String link, int irCode){
  if(digitalRead(lisKey) == LOW){ //IF THE KEY IS DOWN DO THIS
    if(link == ""){ //ir code
      irsend.sendNEC(irCode, 32); 
      delay(100);
      statusLED(IRS, 3, 100, 4000);
      //example: irsend.sendNEC(0xFF02FD, 32);
    }
   
    else{
      openLink(link);
    }
  }
}

void menuText(){
  
  u8g2.setCursor(3, 15);
  u8g2.print("KEY 1 - KEY USE");
  
  u8g2.setCursor(3, 27);
  u8g2.print("KEY 2 - KEY CHNG");
  
  u8g2.setCursor(3, 39);
  u8g2.print("KEY 3 - IR LOG");
  
  u8g2.setCursor(3, 51);
  u8g2.print("KEY 4 - FUNC TEST");
  
  u8g2.sendBuffer();

}

void statusLED(int LED, int times, int speeed, int piezoTone){
  for(int i = 0; i < times; i++){
    tone(piezo, piezoTone);
    digitalWrite(LED, HIGH);
    delay(speeed);
    digitalWrite(LED, LOW);
    noTone(piezo);
    delay(speeed);
  }
}

void commandAndFunction(String command) {
  if (state.startsWith(command)) { //the string I send to the serial port
    //keyChange10xFF02FD
 
    int key = state.substring(command.length(), command.length() + 1).toInt();
    String codeLink = state.substring(command.length() + 1, state.length());
    
    if(codeLink.startsWith("0x")){
      hkIrCodes[key] = codeLink.toInt();
      Serial.print("[IR] - Changed key [" + String(key) + "] to " + codeLink);
      hkLinks[key] = "";
    }
    
    else{
      char charArrayLink[codeLink.length() + 1];  
      codeLink.toCharArray(charArrayLink, codeLink.length() + 1);
      hkLinks[key] = charArrayLink; 
      Serial.print("[LINK] - Changed key [" + String(key) + "] to " + codeLink);
      hkIrCodes[key] = 0;
    }
    
    statusLED(RX, 3, 100, 200);

    if (flag == 0) {
      state = "s"; 
      flag = 1; 
    }
  }
}

void doText(int x, int y, String text){
  u8g2.setCursor(x, y);
  u8g2.print(text);
  u8g2.sendBuffer();
}

void cb(){
  u8g2.clearBuffer();
  u8g2.sendBuffer();
}

void setup() {
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);

  Keyboard.begin();

  Serial.begin(9600);
 
  pinMode(hotKey1, INPUT_PULLUP);
  pinMode(hotKey2, INPUT_PULLUP);
  pinMode(hotKey3, INPUT_PULLUP);
  pinMode(hotKey4, INPUT_PULLUP);

  pinMode(RX, OUTPUT);
  pinMode(TX, OUTPUT);
  pinMode(IRS, OUTPUT);

  u8g2.begin();

  u8g2.setFont(u8g2_font_synchronizer_nbp_tr);

  doText(45, 32, "IREN OS");
  delay(1000);

  intro();
  delay(500);
  

  statusLED(TX, 3, 30, 555);

  statusLED(RX, 3, 30, 777);

  statusLED(IRS, 7, 30, 999);

  cb();

  menuText();
  u8g2.setFont(u8g2_font_synchronizer_nbp_tr);
  
  irrecv.enableIRIn();
  irrecv.blink13(true);
  
  //keyChange20xFF02FD

}

bool breakPhase(){
  if(digitalRead(hotKey1) == LOW and digitalRead(hotKey2) == LOW and digitalRead(hotKey3) == LOW and digitalRead(hotKey4) == LOW){
    return true;
  }

  else{
    return false;
  }
}

void keyChange(){
  while(true){
    if(Serial.available() > 0) {
      state = Serial.readString();
      flag = 0;
      //Serial.print(state);
    }

    commandAndFunction("keyChange");

    if(breakPhase()){
      statusLED(TX, 3, 100, 400);
      cb();
      menuText();
      delay(2000);
      break;
    }
    
  }
}

void irSendAndSerialListen(){
  while(true){
    if(Serial.available() > 0) {
      state = Serial.readString();
      flag = 0;
      //Serial.print(state);
    }
  
    keyListen(hotKey1, hkLinks[1], hkIrCodes[1]);
    keyListen(hotKey2, hkLinks[2], hkIrCodes[2]);
    keyListen(hotKey3, hkLinks[3], hkIrCodes[3]);
    keyListen(hotKey4, hkLinks[4], hkIrCodes[4]);
    
    commandAndFunction("keyChange");

    if(breakPhase()){
      statusLED(TX, 3, 100, 400);
      cb();
      menuText();
      delay(2000);
      break;
    }
  }
}

void funcTest(){

  while(true){
    if (irrecv.decode(&results)){
      statusLED(RX, 4, 20, 500);
      Serial.println(results.value, HEX);
      cb();

      if(String(results.value, HEX).length() > 6){
        doText(3, 10, "REC:" + String(results.value, HEX));
        doText(3, 23, "BAD READ, BRO!");
        delay(1000);
      }

      else{
        doText(3, 10, "REC:" + String(results.value, HEX));
        doText(3, 23, "Good read!");
        //delay(1000);  
      }

      doText(3, 36, "Sending...");
    
      statusLED(RX, 20, 30, 500);
      
    
      irsend.sendNEC(results.value, 32);
      irrecv.enableIRIn();
      irrecv.resume();

      statusLED(IRS, 1, 100, 1000);

      doText(3, 49, "Sent!");
    }

    if(breakPhase()){
      digitalWrite(4 , LOW);
      digitalWrite(5 , LOW);
      digitalWrite(6 , LOW);
      statusLED(TX, 3, 100, 400);
      cb();
      menuText();
      delay(2000);
      break;
    }

    if(digitalRead(16) == LOW){ tone(8, 65*10); digitalWrite(A2, HIGH);}
    else{digitalWrite(A2, LOW);}
    if(digitalRead(10) == LOW){ tone(8, 73*10); digitalWrite(A3, HIGH);}
    else{digitalWrite(A3, LOW);}
    if(digitalRead(14) == LOW){ tone(8, 82*10); digitalWrite(A0, HIGH);}
    else{digitalWrite(A0, LOW);}
    if(digitalRead(15) == LOW){ tone(8, 98*10); digitalWrite(A1, HIGH);}
    else{digitalWrite(A1, LOW);}
    if(digitalRead(16) == HIGH and digitalRead(15) == HIGH and digitalRead(10) == HIGH and digitalRead(14) == HIGH){noTone(8);}
  }
}

void menu(){

  if(digitalRead(hotKey1) == LOW){
    statusLED(RX, 5, 20, 600);
    cb();
    doText(38, 21, "KEY USE");
    irSendAndSerialListen();
  }
  
  if(digitalRead(hotKey2) == LOW){
    statusLED(RX, 5, 20, 600);
    cb();
    doText(30, 29, "KEY CHANGE");
    keyChange();
  }
  
  if(digitalRead(hotKey3) == LOW){
    statusLED(RX, 5, 20, 400);
    cb();
    doText(29, 29, "IR LOGGING");
    //funcTest();
  }

  if(digitalRead(hotKey4) == LOW){
    digitalWrite(A0, LOW);
    digitalWrite(A3, LOW);
    digitalWrite(A2, LOW);
    statusLED(RX, 5, 20, 400);
    cb();
    doText(35, 29, "FUNC TEST");
    digitalWrite(A1, LOW);
    funcTest();
  }

}

void loop() {
  menu();
}
