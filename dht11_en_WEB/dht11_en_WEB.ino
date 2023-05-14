#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <DHT.h>

// Reemplazar con tus credenciales Wifi
const char* ssid     = "Totalplay-319F_2.4Gnormal";
const char* password = "PASS_2023";

// Se crea una instancia del servidor web
WebServer server(80);

// Se crea instancia del sensor DHT11
#define DHTPIN 14     // Pin del esp32 al que se conecta el pin DATA del dht11
#define DHTTYPE DHT11 // DHT 11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Inicio de comunicación serial
  Serial.begin(115200);

  // Conexión a la red Wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a la red Wifi...");
  }

  // Imprime dirección IP a la que se conecta (página web)
  Serial.println(WiFi.localIP());

  // Se inicia el sensor DHT11
  dht.begin();

  // Ruta para root / Creación página web
  server.on("/", [](){
  // lecturas de temperatura y humedad del sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

// Creación de la página HTML con los valores de tempertura y humedad
String html = "<html><head><style>h1{color:blue;font-size:30px;}p{color:green;font-size:20px;}</style></head><body>";
html += "<h1>Datos del sensor DHT11</h1>";
html += "<p>Temperatura: " + String(temperature) + " &deg;C</p>";
html += "<p>Humedad: " + String(humidity) + " %</p>";
html += "<script>setInterval(function(){location.reload();}, 5000);</script>"; // Refresh página cada 5 segundos
html += "</body></html>";

  // Send the HTML page to the client
  server.send(200, "text/html", html);
});


  // Inicia el servidor web
  server.begin();
}

void loop() {
  // Handle  client requests
  server.handleClient();
}
