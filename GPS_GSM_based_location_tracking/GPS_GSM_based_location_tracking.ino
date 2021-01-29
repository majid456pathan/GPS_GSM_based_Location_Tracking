/* CODE BY MAJID PATHAN
 * DATE: 19TH FEB 2020 */
 
#include<SoftwareSerial.h>
#include<TinyGPS.h>

#define GPS_TX_PIN 5
#define GPS_RX_PIN 6
#define GSM_TX_PIN 10
#define GSM_RX_PIN 11

TinyGPS GPS;
SoftwareSerial SGPS(GPS_TX_PIN,GPS_RX_PIN);
SoftwareSerial SIM900A(GSM_TX_PIN,GSM_RX_PIN);

String URL;                 //database server URL
char URL_DATA[115];
String LATLONG;
char LOCATION[25];

void gprs_init()
{
  SIM900A.write("AT+CMGF=1\r\n");
  delay(2000);
  SIM900A.write("AT+CNMI=2,2,2,0,0,0\r\n");
  delay(2000);
  SIM900A.write("AT+CPIN?\r\n");
  delay(2000);
  SIM900A.write("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n");
  delay(2000);
  SIM900A.write("AT+SAPBR=3,1,\"APN\",\"airtelgprs.com\"\r\n");
  delay(2000);
  SIM900A.write("AT+SAPBR=3,1,\"USER\",\"\"\r\n");
  delay(2000);
  SIM900A.write("AT+SAPBR=3,1,\"PWD\",\"\"\r\n");
  delay(2000);
  SIM900A.write("AT+SAPBR=1,1\r\n");
  delay(2000);
  Serial.println(" Setup Complete! GPRS is Ready!");
}

void gprs_send_data(char * LOCATION)
{
  Serial.println(" sending data...");
   SIM900A.write("AT+HTTPINIT\r\n");
  delay(2000);
  String URL=String("AT+HTTPPARA=\"URL\",\"http://majidgpsdata.000webhostapp.com/appdata/database.php?latlong=");
  URL+=String(LOCATION);
  URL+=String("\"\r\n");
  URL.toCharArray(URL_DATA, 115);
  Serial.println(" DATA: ");
  Serial.println(LOCATION);
  Serial.println(URL_DATA);
  SIM900A.write(URL_DATA); 
  delay(1000);
  SIM900A.write("AT+HTTPACTION=0\r\n");
  delay(1000);
  SIM900A.write("AT+HTTPTERM\r\n");
  delay(1000);
  Serial.println(" DATA SENT SUCCESSFULLY ");
  delay(10000);  
}

void setup() 
{  
  Serial.begin(9600);
  while(!Serial);
  SGPS.begin(9600);
  Serial.println(" SIM28ML(GPS) started at 9600");
  SIM900A.begin(9600); 
  Serial.println(" SIM900A(GSM) started at 9600");
  delay(1000);
  gprs_init();
}

void loop() 
{
  SGPS.begin(9600);
  while(SGPS.available())
 {
    int C=SGPS.read();
    if(GPS.encode(C))
    {
      float slat,slon;
      GPS.f_get_position(&slat, &slon);
      LATLONG+=String(slat, 6);
      LATLONG+=F(",");
      LATLONG+=String(slon, 6);
      Serial.println(LATLONG);
      LATLONG.toCharArray(LOCATION, 20);
      gprs_send_data(LOCATION);
    }
  }  
}
 
