#include <ezTime.h>

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>

#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

#include <ArduinoJson.h>

#include <Wire.h>

#include <PCF8574.h>

#include "htmlstring_PCF8574.h"

//Timezone
Timezone myTZ;

// struttura di appoggio dei dati relativi allo stato delle porte del PCF8574
//PCF8574::DigitalInput PortStatus;

volatile bool keyPressed = false;

// Function interrupt - va dichiarata prima del suo utilizzo (l'istruzione ICACHE_RAM_ATTR serve per eseguire il codice in RAM invece che dalla FLASH
void ICACHE_RAM_ATTR keyPressedOnPCF8574();

// Set i2c address
//PCF8574(uint8_t address, uint8_t sda, uint8_t scl, uint8_t interruptPin,  void (*interruptFunction)());
PCF8574 pcf8574(0x20, 0, 2, 3, keyPressedOnPCF8574);

uint8_t SessionId=0;

unsigned long BUZSendTime = 0;        // L'ultimo istante in cui ho acceso il buzzer
unsigned long BUZinterval = 300;          // intervallo tra una accenzione e l'altra del buzzer

#define ALLLEDOFF 15 // in binario equivale a 4 bit tutti attivi a uno
uint8_t ledcounter=ALLLEDOFF; 

String page; // stringa generica per la costruzione della pagina html

int bresult=0;

std::unique_ptr<ESP8266WebServer> server;

////////////////////////////
// handleNotFound()
////////////////////////////
void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server->uri();
  message += "\nMethod: ";
  message += (server->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server->args();
  message += "\n";
  for (uint8_t i = 0; i < server->args(); i++)
  {
    message += " " + server->argName(i) + ": " + server->arg(i) + "\n";
  }
  server->send(404, "text/plain", message);
}

////////////////////////////
// handleRoot()
////////////////////////////
void handleRoot()
{
 Serial.println("handle root");
 page = FPSTR(HTML_head);
 page += FPSTR(HTML_body);
 page += FPSTR(HTML_script);
 page += FPSTR(HTML_tail);

 server->send(200, "text/html", page);
}

////////////////////////////
// handleUpdateData()
////////////////////////////
void handleUpdateData()
{
//
// il messaggio è sempre definito come segue
// il server deve inviare i dati al browser con il Content-Type associato alla risposta = text/event-stream.
// una riga che inizia con data: e termina con due fine riga consecutivi:
// data: messaggio
//
// messaggi multiriga si possono inviare, ma tutte devono iniziare con data:
// data: questo è un messaggio
// data: su più righe
//
// oppure con JSON
// data: { "codice": "123",
// data: "msg": "messaggio dal server" }
//
// È possibile associare un identificatore a ciascun messaggio includendo una riga che inizia con id:
// id: 12345
// data: { "codice": "123",
// data: "msg": "messaggio dal server" }
// 
// La presenza di un identificatore consente al browser di tenere traccia dell’ultimo messaggio ricevuto e di
// risincronizzarsi con il server in caso di caduta della connessione, in modo da ricevere eventuali messaggi
// inviati nel frattempo dal server.
// Lato server è anche possibile assegnare un nome d’evento a ciascun messaggio. Questo consente di differenziare
// i messaggi ricevuti sul browser e gestirli eventualmente in maniera appropriata.
// data: { "codice": "123",
// data: "msg": "messaggio dal server" }
// event: update
// data: { "codice": "567",
// data: "msg": "Nuovo messaggio dal server" }
//
// Il primo messaggio sarà gestito dal gestore generico che intercetta l’evento standard message, 
// mentre il secondo messaggio verrà gestito dal seguente gestore: 
//
// eventSource.addEventListener("update", function(e) {
//	var myDiv = document.getElementById("myDiv");
//	var messaggio = JSON.parse(e.data);
//	myDiv.innerHTML = "Nuovo aggiornamento: " + messaggio.msg;
// });


 Serial.println("handleUpdateData");

 page ="";
 
 page+="id: "+String(SessionId)+"\n";
 page+="data: { ";
 page+="\"timestamp\": \""+String(dateTime(myTZ.now(), "H:i:s.v"))+"\",";
 page+="\"id\": "+String(SessionId)+",";
 page+="\"l1\": "+String(pcf8574.digitalRead(0))+",";
 page+="\"l2\": "+String(pcf8574.digitalRead(1))+",";
 page+="\"l3\": "+String(pcf8574.digitalRead(2))+",";
 page+="\"l4\": "+String(pcf8574.digitalRead(3))+",";
 page+="\"rb1\": "+String(pcf8574.digitalRead(4))+",";
 page+="\"rb2\": "+String(pcf8574.digitalRead(5))+",";
 page+="\"rb3\": "+String(pcf8574.digitalRead(6))+",";
 page+="\"bz1\": "+String(pcf8574.digitalRead(7))+",";

 page+="\"bresult\": \""+String(bresult)+"\"";
 page+="}\n\n";

 server->send(200, "text/event-stream", page);

 Serial.print("page=");
 Serial.println(page);
 
 SessionId++;
 
 bresult=0;
}


////////////////////////////
// handleDataSave()
////////////////////////////
void handleDataSave()
{
 byte bmask = 0b00000000;
 
 String appo;
 
 String message = "<center>";
 
 String data = server->arg("plain");
 
 Serial.print("handleDataSave -> ");
 Serial.println(data);
 
 StaticJsonDocument<1024> jBuffer;
 auto error = deserializeJson(jBuffer, data);
 
 if (error)
 {
  Serial.println("lettura json fallita");
  message += "lettura json fallita";
 }
 else
 {
  appo = String(jBuffer["l1"]);
  
  if(appo.toInt())
   pcf8574.digitalWrite(0, HIGH);
  else   
   pcf8574.digitalWrite(0, LOW);
  
  appo = String(jBuffer["l2"]);

  if(appo.toInt())
   pcf8574.digitalWrite(1, HIGH);
  else
   pcf8574.digitalWrite(1, LOW);
  
  appo = String(jBuffer["l3"]);

  if(appo.toInt())
   pcf8574.digitalWrite(2, HIGH);
  else
   pcf8574.digitalWrite(2, LOW);
  
  appo = String(jBuffer["l4"]);

  if(appo.toInt())
   pcf8574.digitalWrite(3, HIGH);
  else
   pcf8574.digitalWrite(3, LOW);

  appo = String(jBuffer["bz1"]);

  if(appo.toInt())
   pcf8574.digitalWrite(7, HIGH);
  else
   pcf8574.digitalWrite(7, LOW);

  appo = String(jBuffer["rb1"]);
  if (appo.toInt())
   bitSet(bmask, 0);
  else
   bitClear(bmask, 0);

  appo = String(jBuffer["rb2"]);
  if (appo.toInt())
   bitSet(bmask, 1);
  else
   bitClear(bmask, 1);

  appo = String(jBuffer["rb3"]);
  if (appo.toInt())
   bitSet(bmask, 2);
  else
   bitClear(bmask, 2);
  
  handleButtons(bmask);
  
  message += "Dati inviati";
 }
  
 message += "</center>";
 server->send(200, "text/html", message);       //Risponde alla richiesta HTTP
 
 handleUpdateData(); // aggiorno lato flusso dati
}

/////////////////////////////
// handleButtons()
/////////////////////////////
void handleButtons(byte buttonsMask)
{
 Serial.print("handleButtons ->");
 Serial.println(buttonsMask);

 bresult=(int)buttonsMask;

// qui gestisco l'uso dei tasti che mi aspetto possano essere gestiti considerando anche pressioni contemporanee dei bottoni

 switch(bresult)
 {
  case 0: //0b00000000
   break;
  case 1: //0b00000001
   break;
  case 2: //0b00000010
   break;
  case 3: //0b00000011
   break;
  case 4: //0b00000100
   break;
  case 5: //0b00000101
   break;
  case 6: //0b00000110
   break;
  case 7: //0b00000111
   break;
  default:
   break;
 }

// inserito SOLO per viasualizzare le modifiche successive all'uso dei bottoni

// if (ledcounter>0)
//  ledcounter--; // decremento il contatore per l'iterazione successiva
// else
//  ledcounter = ALLLEDOFF; // ripristino il valore iniziale

// handleLEDs();
}

////////////////////////////////
// handleLEDs()
////////////////////////////////
void handleLEDs()
{
 uint8_t ledcopy = ledcounter; // per gestire il filtro a bit del contatore e accendere i LED
 String appo="";

 if ((ledcopy & 1) == 1) // eseguo un and a bit per eliminare i valori superiori a quello contenuto nel bit meno significativo
 {
  appo += "1";
  pcf8574.digitalWrite(P3, HIGH);
 }
 else
 {
  appo += "0";
  pcf8574.digitalWrite(P3, LOW);
 }
 
 ledcopy=ledcopy/2; // in questo modo faccio lo shift a bit del valore iniziale

 if ((ledcopy & 1) == 1)
 {
  appo += "1";
  pcf8574.digitalWrite(P2, HIGH);
 }
 else
 {
  appo += "0";
  pcf8574.digitalWrite(P2, LOW);
 }
 
 ledcopy=ledcopy/2; 

 if ((ledcopy & 1) == 1) 
 {
  appo += "1";
  pcf8574.digitalWrite(P1, HIGH);
 }
 else
 {
  appo += "0";
  pcf8574.digitalWrite(P1, LOW);
 }
 
 ledcopy=ledcopy/2; 

 if ((ledcopy & 1) == 1) 
 {
  appo += "1";
  pcf8574.digitalWrite(P0, HIGH);
 }
 else
 {
  appo += "0";
  pcf8574.digitalWrite(P0, LOW);
 }

 Serial.print("LED: "); 
 Serial.println(appo);
}

////////////////////////////////
// setup()
////////////////////////////////
void setup()
{
  Serial.begin(115200);
  
  delay(5000); // solo per il debug
 
  Wire.begin(0, 2);                  // sembra necessario benché l'istruzione è pure presente nella funzione lcd.begin()

  /////////////////////////////////////////////
  // INDIRIZZI I2C
  //
  // BME280 (sensore) 0x76
  // display          0x3C
  // PCF8574          0x20
  //
  // SDA = GPIO0 = 0
  // SCL = GPIO2 = 2
  // INT = GPIO3 = RXD = 3
  /////////////////////////////////////////////

  ////////////////////////////////////////////////
  // inizializzo il PCF8574
  ////////////////////////////////////////////////
  
  pcf8574.pinMode(0, OUTPUT, HIGH);  // L1
  pcf8574.pinMode(1, OUTPUT, HIGH);  // L2
  pcf8574.pinMode(2, OUTPUT, HIGH);  // L3
  pcf8574.pinMode(3, OUTPUT, HIGH);  // L4

  pcf8574.pinMode(4, INPUT_PULLUP); // RB1
  pcf8574.pinMode(5, INPUT_PULLUP); // RB2
  pcf8574.pinMode(6, INPUT_PULLUP); // RB3

  pcf8574.pinMode(7, OUTPUT, LOW);  // BZ1

  Serial.println(".");

  Serial.print("Init pcf8574...");
  if (pcf8574.begin())
   Serial.println("OK");
  else
   Serial.println("KO");

  Serial.println(".");
  
  //Serial.setDebugOutput(true);
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  //reset saved settings
  //  wifiManager.resetSettings();

  //fetches ssid and pass from eeprom and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  //wifiManager.autoConnect("AutoConnectAP");
  //or use this for auto generated name ESP + ChipID
  wifiManager.autoConnect();

  //if you get here you have connected to the WiFi
  Serial.println("connected");
 
  // ezTime
  setDebug(INFO);
  waitForSync();
  myTZ.setLocation(F("IT")); // imposto il timezone per myTZ
 
  // attivazione del web server
  
  // DEVE rimanere "server." e non "server->"
  server.reset(new ESP8266WebServer(WiFi.localIP(), 80));

  server->on("/inline", []() 
             {
              server->send(200, "text/plain", "this works as well");
             }
      );

  server->onNotFound(handleNotFound);

  server->on("/", handleRoot);

  server->on("/updatedata", handleUpdateData);

  server->on("/datasave", handleDataSave);

  server->begin();
  Serial.print("HTTP server started -> ");
  Serial.println(WiFi.localIP());
  
}

void loop()
{
 byte bmask = 0b00000000;

 // gestore degli eventi di ezTime  
// events();

 if (keyPressed)
 {
  Serial.println("key pressed");
  
  uint8_t val4 = pcf8574.digitalRead(4);
  uint8_t val5 = pcf8574.digitalRead(5);
  uint8_t val6 = pcf8574.digitalRead(6);
  
  if((val4+val5+val6)<3) // fa suonare il buzzer se è stato premuto almeno un tasto
  {
   BUZSendTime = millis(); 
   pcf8574.digitalWrite(P7, HIGH);
  }
   
//  PortStatus = PCF8574.digitalReadAll();
  
  Serial.print("val4,val5,val6=");
  Serial.print(val4);
  Serial.print(",");
  Serial.print(val5);
  Serial.print(",");
  Serial.println(val6);

  if (val4)
   bitSet(bmask, 0);
  else
   bitClear(bmask, 0);

  if (val5)
   bitSet(bmask, 1);
  else
   bitClear(bmask, 1);

  if (val6)
   bitSet(bmask, 2);
  else
   bitClear(bmask, 2);


  Serial.print("bmask=");
  Serial.println(bmask);

  handleButtons(bmask);
  
  handleUpdateData();
  keyPressed= false;
 }

 if (millis() - BUZSendTime > BUZinterval) // verifico se sono trascorsi i BUZinterval millisecondi dall'ultima accensione del buzzer per poterlo spegnere
  pcf8574.digitalWrite(P7, LOW);
  
 server->handleClient();
 
 delay(100); // per verifica se il fatto di avere un loop così stressato influisca con l'invio dei dati via http
}


void keyPressedOnPCF8574()
{
  keyPressed=true;
}
