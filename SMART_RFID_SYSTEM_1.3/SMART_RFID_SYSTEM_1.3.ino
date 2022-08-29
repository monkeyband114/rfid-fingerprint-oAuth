#include <SoftwareSerial.h>
#include <FPM.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <LiquidCrystal.h>
#include <WiFi.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

#define I2C_SDA 33
#define I2C_SCL 32

#define SS_PIN  21
#define RST_PIN 22
SoftwareSerial fserial(17, 16);
LiquidCrystal_I2C lcd(0x27, 16, 2);  
FPM finger(&fserial);
FPM_System_Params params;
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
const char* ssid     = "TECNO C15 primer";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "125566@@4";   
const char* serverName = "http://tikaghd.000webhostapp.com/post-esp-data.php";

// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key 
String apiKeyValue = "tPmAT5Ab3j7F9";

String nameArray[] ={ "OKE DENNIS ", "ONUKOKWO JAMES ", "ODINAKA JESICCA ", "EMORDI FRANCES ", "ARIHIRE MATHEW", "OBI CUBANNA ","JAMES MVAVOY", "MICHAEL ADAWAREN ", "ODOMERO MATTEW ", "OVIE LUCKY FELIX", "NO DATA"};
String matNo[] = {"C0S/1845/2019","COS/1075/2019", "COS/0045/2019", "COS/1285/2019", "C0S/3045/2019", "C0S/2019/2019", "C0S/3422/2019", "C0S/0012/2019","C0S/0712/2019", "NO DATA "};
String courseArray[] = {"CSC224", "CSC211", "MTH213", "MTH219", "CSC211", "CSC213", "CSC210", "MTH225", "CSC215", "CSC310", "CSC228", "NO DATA"};
String alert = "ACCESS DENIED WRONG CARD INPUTED";
String NONE = "";
String sensorName = "COMPUTER SCIENCE";
String sensorLocation = "FUPRE";
uint16_t match_score;


void setup() 
{
  
  Serial.begin(115200); 
  fserial.begin(57600);
  Wire.begin(I2C_SDA, I2C_SCL);
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  Serial.println("print below");
  lcd.println("RFID ATTENDANCE ACTIVE");
  delay(3000);
  lcd.clear();
    
    if (finger.begin()) {
        finger.readParams(&params);
        Serial.println("Found fingerprint sensor!");
        Serial.print("Capacity: "); Serial.println(params.capacity);
        Serial.print("Packet length: "); Serial.println(FPM::packet_lengths[params.packet_len]);
    } else {
        Serial.println("Did not find fingerprint sensor :(");
        while (1) yield();
    }
  // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("SWIPE YOUR CARD");
  Serial.println();
  
  ledcSetup(0,1E5,12);
  ledcAttachPin(25,0);
  delay(10);
  Serial.println('\n');
  
  WiFi.begin(ssid, password);             // Connect  to the network
  Serial.print("Connecting to ");
  Serial.print(ssid);

  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(500);
    Serial.print('.');
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
  lcd.print("WIFI CONNECTED");
    delay(4000);
    lcd.clear();
    lcd.print("SWIPE YOUR CARD");
    
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());   

}
void loop(){
   if(WiFi.status()== WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;
 lcd.print("ENTER RFID SMARTCARD"); 
    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);
  
    if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    lcd.clear();
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
    
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  
  content.toUpperCase();
 lcd.print("ENTER FINGERPRINT");
 int16_t fid = 0;
  
 match_prints(fid);
 lcd.clear();
 delay(3000);
   
 if (content.substring(1) == "E2 EB 58 2C"  ){
      Serial.println("Authorized access");
      Serial.println();
      Serial.println(nameArray[0]);
      lcd.print("ACCESS GRANTED");
      delay(5000);
      ledcDetachPin(25);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
     
      // Prepare your HTTP POST request data
      String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + sensorName
                         + "&location=" + sensorLocation + "&value1=" + nameArray[0]
                         + "&value2=" + matNo[0] + "&value3=" + courseArray[0] + "";
    //  String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=RFIDSC&location=FUPRE&value1=OKE&value2=345&value3=224";
                  
      Serial.print("httpRequestData: ");
      Serial.println(httpRequestData);
    
      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);
      if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
     }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
     }
      // Free resources
      http.end();
      }
      
 else  if (content.substring(1) == "09 40 8B B2" && match_score > 50 ){
      Serial.println("Authorized access");
      Serial.println();
      Serial.println(nameArray[1]);
      
      ledcDetachPin(25);
      lcd.print("ACCESS GRANTED");
      delay(5000);
   
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
      // Prepare your HTTP POST request data
      String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + sensorName
                         + "&location=" + sensorLocation + "&value1=" + nameArray[1]
                         + "&value2=" + matNo[1] + "&value3=" + courseArray[1] + "";
    //  String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=RFIDSC&location=FUPRE&value1=OKE&value2=345&value3=224";
                  
      Serial.print("httpRequestData: ");
      Serial.println(httpRequestData);
    
      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);
      if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
     }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
     }
      // Free resources
      http.end();
     
  
      }
      
 else if (content.substring(1) == "27 1B 76 62" && match_score > 50 ){
      Serial.println("Authorized access");
      Serial.println();
       Serial.println(nameArray[2]);
      ledcDetachPin(25);
      lcd.print("ACCESS GRANTED");
      delay(5000);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
      // Prepare your HTTP POST request data
      String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + sensorName
                         + "&location=" + sensorLocation + "&value1=" + nameArray[2]
                         + "&value2=" + matNo[2] + "&value3=" + courseArray[2] + "";
    //  String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=RFIDSC&location=FUPRE&value1=OKE&value2=345&value3=224";
                  
      Serial.print("httpRequestData: " && match_score > 50 );
      Serial.println(httpRequestData);
    
      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);
      if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
     }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
     }
      // Free resources
      http.end();
    
      }
    
 else if (content.substring(1) == "A9 FA 75 B3" && match_score > 50 ){
      Serial.println("Authorized access");
      Serial.println();
       Serial.println(nameArray[3]);
      ledcDetachPin(25);
      lcd.print("ACCESS GRANTED");
      delay(5000);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
      // Prepare your HTTP POST request data
      String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + sensorName
                         + "&location=" + sensorLocation + "&value1=" + nameArray[3]
                         + "&value2=" + matNo[3] + "&value3=" + courseArray[3] + "";
    //  String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=RFIDSC&location=FUPRE&value1=OKE&value2=345&value3=224";
                  
      Serial.print("httpRequestData: ");
      Serial.println(httpRequestData);
    
      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);
      if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
     }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
     }
      // Free resources
      http.end();
      }

 else if (content.substring(1) == "C9 27 78 B3" && match_score > 50 ){
      Serial.println("Authorized access");
      Serial.println();
       Serial.println(nameArray[4]);
      ledcDetachPin(25);
      lcd.print("ACCESS GRANTED");
      delay(5000);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
      // Prepare your HTTP POST request data
      String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + sensorName
                         + "&location=" + sensorLocation + "&value1=" + nameArray[4]
                         + "&value2=" + matNo[4] + "&value3=" + courseArray[4] + "";
    //  String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=RFIDSC&location=FUPRE&value1=OKE&value2=345&value3=224";
                  
      Serial.print("httpRequestData: ");
      Serial.println(httpRequestData);
    
      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);
      if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
     }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
     }
      // Free resources
      http.end();
     
      }

 else if (content.substring(1) == "59 E6 3E B3" && match_score > 50){
      Serial.println("Authorized access");
      Serial.println();
       Serial.println(nameArray[5]);
      ledcDetachPin(25);
      lcd.print("ACCESS GRANTED");
      delay(5000);
     http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
      // Prepare your HTTP POST request data
      String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + sensorName
                         + "&location=" + sensorLocation + "&value1=" + nameArray[5]
                         + "&value2=" + matNo[5] + "&value3=" + courseArray[5] + "";
    //  String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=RFIDSC&location=FUPRE&value1=OKE&value2=345&value3=224";
                  
      Serial.print("httpRequestData: ");
      Serial.println(httpRequestData);
    
      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);
      if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
     }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
     }
      // Free resources
      http.end();
     
      }

 else if (content.substring(1) == "B9 F3 87 B3" && match_score > 50 ){
      Serial.println("Authorized access");
      Serial.println();
      Serial.println(nameArray[6]);
      ledcDetachPin(25);
      lcd.print("ACCESS GRANTED");
      delay(5000);
     http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
      // Prepare your HTTP POST request data
      String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + sensorName
                         + "&location=" + sensorLocation + "&value1=" + nameArray[6]
                         + "&value2=" + matNo[6] + "&value3=" + courseArray[6] + "";
    //  String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=RFIDSC&location=FUPRE&value1=OKE&value2=345&value3=224";
                  
      Serial.print("httpRequestData: ");
      Serial.println(httpRequestData);
    
      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);
      if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
     }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
     }
      // Free resources
      http.end();
    
     
      }

 else if (content.substring(1) == "29 7A 76 B3" && match_score > 50 ){
      Serial.println("Authorized access");
      Serial.println();
      Serial.println(nameArray[7]);
      ledcDetachPin(25);
      lcd.print("ACCESS GRANTED");
      delay(5000);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
      // Prepare your HTTP POST request data
      String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + sensorName
                         + "&location=" + sensorLocation + "&value1=" + nameArray[7]
                         + "&value2=" + matNo[7] + "&value3=" + courseArray[7] + "";
    //  String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=RFIDSC&location=FUPRE&value1=OKE&value2=345&value3=224";
                  
      Serial.print("httpRequestData: ");
      Serial.println(httpRequestData);
    
      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);
      if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
     }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
     }
      // Free resources
      http.end();
     
      }
  
 else {
     lcd.print("ACCESS DENIED");
      delay(5000);
     ledcAttachPin(25,0);
     Serial.println(" ACCESS DENIED WRONG CARD INPUTED  ");
     ledcWriteTone(0,900);
     delay(4000);
     uint8_t octave = 1;
     ledcWriteNote(0,NOTE_C,octave);  
     delay(1000);
     http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
      // Prepare your HTTP POST request data
      String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + sensorName
                         + "&location=" + sensorLocation + "&value1=" + nameArray[10]
                         + "&value2=" + matNo[9] + "&value3=" + courseArray[11] + "";
    //  String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=RFIDSC&location=FUPRE&value1=OKE&value2=345&value3=224";
                  
      Serial.print("httpRequestData: ");
      Serial.println(httpRequestData);
    
      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);
      if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
     }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
     }
      // Free resources
      
     }
    
    
}
 else {
    Serial.println("WiFi Disconnected");
  }

  
  //Send an HTTP POST request every 30 seconds
 
  
  }


  void match_prints(int16_t fid) {
    int16_t p = -1;

    /* first get the finger image */
    Serial.println("Waiting for valid finger");
    while (p != FPM_OK) {
        p = finger.getImage();
        switch (p) {
            case FPM_OK:
                Serial.println("Image taken");
                break;
            case FPM_NOFINGER:
                Serial.println(".");
                break;
            case FPM_PACKETRECIEVEERR:
                Serial.println("Communication error");
                return;
            default:
                Serial.println("Unknown error");
                return;
        }
        yield();
    }

    /* convert it and place in slot 1*/
    p = finger.image2Tz(1);
    switch (p) {
        case FPM_OK:
            Serial.println("Image converted");
            break;
        case FPM_IMAGEMESS:
            Serial.println("Image too messy");
            return;
        case FPM_PACKETRECIEVEERR:
            Serial.println("Communication error");
            return;
        default:
            Serial.println("Unknown error");
            return;
    }

    Serial.println("Remove finger");
    p = 0;
    while (p != FPM_NOFINGER) {
        p = finger.getImage();
        yield();
    }
    Serial.println();

    /* read template into slot 2 */
    p = finger.loadModel(fid, 2);
    switch (p) {
        case FPM_OK:
            Serial.print("Template "); Serial.print(fid); Serial.println(" loaded.");
            break;
        case FPM_PACKETRECIEVEERR:
            Serial.println("Communication error");
            return;
        case FPM_DBREADFAIL:
            Serial.println("Invalid template");
            return;
        default:
            Serial.print("Unknown error "); Serial.println(p);
            return;
    }
    
     match_score = 0;
    p = finger.matchTemplatePair(&match_score);
    switch (p) {
        case FPM_OK:
            Serial.print("Prints matched. Score: "); Serial.println(match_score);
            break;
        case FPM_NOMATCH:
            Serial.println("Prints did not match.");
            break;
        default:
            Serial.println("Unknown error");
            return;
    }
}

 void match_print(int16_t fid) {
    int16_t p = -1;

    /* first get the finger image */
    Serial.println("Waiting for valid finger");
    while (p != FPM_OK) {
        p = finger.getImage();
        switch (p) {
            case FPM_OK:
                Serial.println("Image taken");
                break;
            case FPM_NOFINGER:
                Serial.println(".");
                break;
            case FPM_PACKETRECIEVEERR:
                Serial.println("Communication error");
                return;
            default:
                Serial.println("Unknown error");
                return;
        }
        yield();
    }

    /* convert it and place in slot 1*/
    p = finger.image2Tz(1);
    switch (p) {
        case FPM_OK:
            Serial.println("Image converted");
            break;
        case FPM_IMAGEMESS:
            Serial.println("Image too messy");
            return;
        case FPM_PACKETRECIEVEERR:
            Serial.println("Communication error");
            return;
        default:
            Serial.println("Unknown error");
            return;
    }

    Serial.println("Remove finger");
    p = 0;
    while (p != FPM_NOFINGER) {
        p = finger.getImage();
        yield();
    }
    Serial.println();

    /* read template into slot 2 */
    p = finger.loadModel(fid, 2);
    switch (p) {
        case FPM_OK:
            Serial.print("Template "); Serial.print(fid); Serial.println(" loaded.");
            break;
        case FPM_PACKETRECIEVEERR:
            Serial.println("Communication error");
            return;
        case FPM_DBREADFAIL:
            Serial.println("Invalid template");
            return;
        default:
            Serial.print("Unknown error "); Serial.println(p);
            return;
    }
    
     match_score = 0;
    p = finger.matchTemplatePair(&match_score);
    switch (p) {
        case FPM_OK:
            Serial.print("Prints matched. Score: "); Serial.println(match_score);
            break;
        case FPM_NOMATCH:
            Serial.println("Prints did not match.");
            break;
        default:
            Serial.println("Unknown error");
            return;
    }
}
