#include <WiFi.h>
#include <PubSubClient.h>
#include <ESPAsyncWebServer.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include "kit1.xbm"
#include "kit2.xbm"
#include "kit3.xbm"
#include "kit4.xbm"
#include "pag.xbm"
#define TRIG_PIN 21
#define ECHO_PIN 22

//distancia
const int valordis = 35;

const char* ssid = "Alvarado";
const char* password = "29542433";
const char* mqttServer = "13.58.157.125";
const int mqttPort = 1883;

// Definición pines EnA y EnB para el control de la velocidad
int VelocidadMotor1y2 = 13;
int VelocidadMotor3y4 = 12;

// Definición de los pines de control de giro de los motores In1, In2, In3 e In4
int Motor1 = 14;
int Motor2 = 27;
int Motor3 = 26;
int Motor4 = 25;

//variables para el ultrasonido
long duration;
float distance;

// Configura la IP estática, puerta de enlace y máscara de subred
IPAddress ip_local(192, 168, 20, 69);  //ip de mi esp32
IPAddress gateway(192, 168, 20, 1);
IPAddress subnet(255, 255, 255, 0);
// Crea el servidor en el puerto 80
AsyncWebServer server(80);

WiFiClient espClient;
PubSubClient client(espClient);

TFT_eSPI tft = TFT_eSPI();

//manejo de los mensajes de bancolombia
String codigoqr = "";
String nombre = "";
String monto = "";
String cuenta = "";
String fecha_hora = "";
//=====================
void setup() {
  Serial.begin(115200);

  // Configuración de los pines de control de motores
  pinMode(Motor1, OUTPUT);
  pinMode(Motor2, OUTPUT);
  pinMode(Motor3, OUTPUT);
  pinMode(Motor4, OUTPUT);

  // Configurar los pines
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  //MOTORES
  analogWrite(VelocidadMotor1y2, 70);
  analogWrite(VelocidadMotor3y4, 75);
  digitalWrite(Motor1, LOW);
  digitalWrite(Motor2, LOW);
  digitalWrite(Motor3, LOW);
  digitalWrite(Motor4, LOW);

  //tft
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_WHITE);

  // Configura la IP estática
  /*if (!WiFi.config(ip_local, gateway, subnet)) {
    Serial.println("Fallo al configurar la IP estática");
  }*/
  // Conectar al Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando al WiFi...");
  }
  Serial.println("Conectado al WiFi!");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Mostrar la dirección IP en la pantalla TFT
  tft.fillScreen(TFT_WHITE); // Limpiar la pantalla
  tft.setTextColor(TFT_BLACK); // Establecer el color de texto
  tft.setTextSize(2); // Establecer el tamaño del texto
  tft.setCursor(10, 10); // Establecer la posición del cursor
  tft.print("Dirección IP: ");
  tft.println(WiFi.localIP());

  // Ruta para la raíz del servidor
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/plain", "Bienvenido al servidor ESP32!");  // Responde a la solicitud
  });
  //=======================================================================================================================================
  //=======================================================================================================================================
  //=======================================================================================================================================
  //RECEPCION DE COMPROBANTE DE PAGO Y EXTRACCION DEL MENSAJE Y SUS VARIABLES IMPORTANTES

  // Configura la ruta /notificacion para recibir el mensaje
  server.on("/notificacion", HTTP_GET, [](AsyncWebServerRequest* request) {
    String mensaje = request->getParam("mensaje")->value();  // Lee el valor del parámetro "mensaje"
    Serial.println("Mensaje recibido: " + mensaje);          // Muestra el mensaje en el monitor serie

    // Extraer el nombre
    int startIndex = mensaje.indexOf("de ") + 3;  // Encontrar el índice después de "de "
    int endIndex = mensaje.indexOf("por");
    if (startIndex != -1 && endIndex != -1) {
      nombre = mensaje.substring(startIndex, endIndex - 1);  // Extraer el nombre
    }

    // Extraer el monto
    startIndex = mensaje.indexOf("por $") + 5;  // Encontrar el índice después de "por $"
    endIndex = mensaje.indexOf("en la cuenta");
    if (startIndex != -1 && endIndex != -1) {
      monto = mensaje.substring(startIndex, endIndex - 1);  // Extraer el monto
    }

    // Extraer la cuenta
    startIndex = mensaje.indexOf("cuenta *") + 8;  // Encontrar el índice después de "cuenta *"
    endIndex = mensaje.indexOf(".");
    if (startIndex != -1 && endIndex != -1) {
      cuenta = mensaje.substring(startIndex, endIndex);  // Extraer la cuenta
    }

    // Extraer la fecha y hora
    startIndex = mensaje.indexOf(".") + 2;    // Encontrar el índice después de ". "
    endIndex = mensaje.indexOf("Dudas") - 1;  // Buscar "Dudas"
    if (startIndex != -1 && endIndex != -1) {
      fecha_hora = mensaje.substring(startIndex, endIndex);  // Extraer la hora
    }

    // Mostrar la información extraída
    Serial.println("Nombre: " + nombre);
    Serial.println("Monto: " + monto);
    Serial.println("Cuenta: " + cuenta);
    Serial.println("Fecha y hora: " + fecha_hora);

    request->send(200, "text/plain", "Mensaje recibido y procesado.");  // Responde a la solicitud
  });

  // Inicia el servidor
  server.begin();
  Serial.println("Servidor iniciado");

  //=======================================================================================================================================
  //=======================================================================================================================================
  //=======================================================================================================================================
  
  client.setServer(mqttServer, mqttPort);
  client.setCallback(mqttCallback);
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println("Mensaje recibido: " + message);
  //MANEJO DE LAS IMAGENES
  if (message != "") {
    //MANEJO DE LAS IMAGENES
    if (message == "5") {
      tft.fillScreen(TFT_WHITE);
      tft.drawXBitmap(0, 0, pag, 320, 240, TFT_BLACK);
      message = "0";
    }
    if (message == "4") {
      tft.fillScreen(TFT_WHITE);
      tft.drawXBitmap(0, 0, kit4, 320, 240, TFT_BLACK);
      message = "0";
    }
    if (message == "3") {
      tft.fillScreen(TFT_WHITE);
      tft.drawXBitmap(0, 0, kit3, 320, 240, TFT_BLACK);
      message = "0";
    }
    if (message == "2") {
      tft.fillScreen(TFT_WHITE);
      tft.drawXBitmap(0, 0, kit2, 320, 240, TFT_BLACK);
      message = "0";
    }
    if (message == "1") {
      tft.fillScreen(TFT_WHITE);
      tft.drawXBitmap(0, 0, kit1, 320, 240, TFT_BLACK);
      message = "0";
    }
  } else {
    tft.fillScreen(TFT_WHITE);
    tft.drawXBitmap(0, 0, pag, 320, 240, TFT_BLACK);
    message = "0";
  }
  //==================================================
  //==================================================
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  //Manejo de los motores para la dispensacion de productos.
  if (monto != "") {
    distance = medirDistancia();
    if (monto == "6,000") {
      //ENCENDER Motor1
      digitalWrite(Motor1, HIGH);
      if (distance < valordis) {
        digitalWrite(Motor1, LOW);
        Serial.println("stop 1");

        monto = "";
      }
    }
    if (monto == "7,500") {
      //ENCENDER Motor2
      Serial.println("7500");
      digitalWrite(Motor2, HIGH);
      if (distance < valordis) {
        digitalWrite(Motor2, LOW);
        Serial.println("stop 2");

        monto = "";
      }
    }
    if (monto == "6,500") {
      //ENCENDER Motor3
      Serial.println("6500");
      digitalWrite(Motor3, HIGH);
      if (distance < valordis) {
        digitalWrite(Motor3, LOW);
        Serial.println("stop 3");

        monto = "";
      }
    }
    if (monto == "7,000") {
      //ENCENDER Motor4
      Serial.println("7000");
      digitalWrite(Motor4, HIGH);
      if (distance < valordis) {
        digitalWrite(Motor4, LOW);
        Serial.println("stop 4");
        monto = "";
      }
    }
  } else {
    //Serial.println("Esperando comprobante...");
  }
}

int medirDistancia() {

  // Enviar pulso de 10µs
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Leer el tiempo que tarda el pulso
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calcular distancia (velocidad del sonido = 343 m/s)
  distance = (duration * 0.034) / 2;
  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(50);
  return distance;
}


void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando a MQTT...");
    if (client.connect("ESP32")) {
      Serial.println("Conectado");
      client.subscribe("detronix/compra");  // Suscribirse al tema
      Serial.println("Suscrito a detronix/compra");
    } else {
      Serial.print("Falló, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}