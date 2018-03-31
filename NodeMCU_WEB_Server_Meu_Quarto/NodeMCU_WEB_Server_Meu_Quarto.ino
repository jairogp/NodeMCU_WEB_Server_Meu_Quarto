  /* 
  Projeto original de Pedro Minatel.
  Retirado do site: http://pedrominatel.com.br/pt/arduino/web-server-com-esp8266-e-ide-arduino/
  */

  #include <ESP8266WiFi.h>

  const char* ssid = "Vitorelo";
  const char* password = "setembro1309";

  IPAddress ip(192,168,0,175);
  IPAddress gateway(192,168,0,1);
  IPAddress subnet(255,255,255,0);
  
  WiFiServer server(80);

  #define led1 D0
  #define led2 D1
  
  #define botao1 D5
    
  int estado_ch1 = 0;
  
  void setup() {

  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);  

  Serial.begin(9600);
  delay(10);
    
  // prepare GPIOs
  pinMode(led1, OUTPUT);
  digitalWrite(led1, 0);
  pinMode(led2, OUTPUT);
  digitalWrite(led2, 0);
  pinMode(botao1, INPUT_PULLUP);
    
  conectar();
  }

  void conectar() {
  Serial.println();
  Serial.println();
  Serial.print("Connectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  delay(2000);
  Serial.print(".");
  Serial.println("");
  Serial.println("Conectado com sucesso na rede.");
  // Start the server
  server.begin();
  Serial.println("IP Obtido: ");
  Serial.print(WiFi.localIP());
  }}

  void Ligadesliga() {
  Canal_01:
  if(digitalRead(botao1)== 0 && estado_ch1 == 0)
  { digitalWrite(led1,!digitalRead(led1));
    delay(400);
    estado_ch1 = 1;}
  if(digitalRead(botao1) == 1 && estado_ch1 == 1)
  { delay(400);
    estado_ch1 = 0;}
  }

  void loop() {
  Ligadesliga();
     
  if (WiFi.status() != WL_CONNECTED) {conectar();}  // Caso wifi tenha caido conecta novamente
  WiFiClient client = server.available();
  // Volta ao loop aguardando nova conexao do cliente.
  if (!client) { 
    return; 
  }     
  
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  String buf = "";
 
  buf += "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
  buf += "<html lang=\"pt\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/>\r\n";
  buf += "<link rel='icon' type='image/png' href='https://icon-icons.com/icons2/474/PNG/512/light-bulb_46866.png'/>"; 
  buf += "<body style=background-color:#000000>";
  buf += "<title>CONTROLE ILUMINACAO</title>";
  buf += "<h1><font color=#0000FF> CONTROLE ILUMINACAO</h1></font>";
  buf += "<font color=#FFFFFF>";
  buf += "<style>.c{text-align: center;} div,input{padding:5px;font-size:1em;} input{width:80%;} body{text-align: center;font-family:verdana;} button{border:0;border-radius:0.3rem;background-color:#FFA500;color:#FFFFF;line-height:2.4rem;font-size:1.2rem;width:49%;} .q{float: right;width: 64px;text-align: right;}</style>";
  buf += "<p><h3>LAMPADA QUARTO</h3></p> <a href=\"?function=D0_on\"><button>LIGA</button></a> <a href=\"?function=D0_off\"><button>DESLIGA</button></a></p>";
  buf += "<p><h3>CANAL 02</h3></p> <a href=\"?function=D1_on\"><button>LIGA</button></a> <a href=\"?function=D1_off\"><button>DESLIGA</button></a></p>";
  
  client.print(buf);
  delay(10);
  client.flush();

  if (req.indexOf("D0_on") != -1)
    digitalWrite(led1, 1);
  else if (req.indexOf("D0_off") != -1)
    digitalWrite(led1, 0);
  else if (req.indexOf("D1_on") != -1)
    digitalWrite(D1, 1);
  else if (req.indexOf("D1_off") != -1)
    digitalWrite(D1, 0);
  else {
    Serial.println("invalid request");
    client.stop();
  }
  Serial.println("Client disonnected");
  }

