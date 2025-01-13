#include <M5Core2.h>
#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#define DHTPIN 27
#define DHTTYPE DHT22

#include "nuage.h"
#include "soleil.h"
#include "orage.h"
#include "neige.h"
#include "grele.h"
#include "pluie.h"
#include "soleilNuage.h"
#include "soleilPluie.h"





DHT dht(DHTPIN, DHTTYPE);

// Configuration Wi-Fi
const char* ssid = "Groupe1";
const char* password = "motdepasse";

// Configuration du serveur
const char* serverIP = "192.168.4.1";
const int serverPort = 80;
const char* authToken = "1234567890abcdef";

/*          LOUIS V          */
float temperatureAPI = 0;
float humAPI = 0;
String meteoAPI = "";

float tempera = dht.readTemperature();
float hum = dht.readHumidity();



int page = 0;
const int PAGE_MAX = 2; // donc page 1 et page 2
bool modif = true;

unsigned long previousMillis = 0; // Stocke le temps précédent pour la mise à jour
const unsigned long interval = 3000; // Intervalle de 2 secondes
unsigned long errorDisplayMillis = 0; // Pour l'affichage d'erreurs
bool errorDisplayed = false; // Indique si une erreur est affichée

//// Defines gestures
Gesture swipeRight("swipe right", 160, DIR_RIGHT, 30, true);
Gesture swipeDown("swipe down", 120, DIR_DOWN, 30, true);
Gesture swipeLeft("swipe left", 160, DIR_LEFT, 30, true);
Gesture swipeUp("swipe up", 120, DIR_UP, 30, true);

// Defines the buttons. Colors in format {bg, text, outline}
ButtonColors on_clrs  = {RED, WHITE, WHITE};
ButtonColors off_clrs = {GREEN, WHITE, WHITE};
Button bl(0, 0, 0, 0, false, "bottom-left", off_clrs, on_clrs, BL_DATUM);
Button br(0, 0, 0, 0, false, "bottom-right", off_clrs, on_clrs, BR_DATUM);

// For performance measurement (Single tap on bottom-right button)
uint32_t startTime;
uint32_t times = 0;
//________________________________________________________//


/*          Hugo          */
const String serverUrl = "http://192.168.4.1:80";

/*          Louis V          */
// Positions the buttons and draws them. (Only because height and width
// change. If we were only switching between normal and upside-down we
// would only need the Buttons.draw() here.)
void doButtons() {
    int16_t hw = M5.Lcd.width() / 2;
    int16_t hh = M5.Lcd.height() / 5;
    bl.set(0, M5.Lcd.height() - hh-5, hw - 5, hh - 5); // bas gauche
    br.set(hw + 5, M5.Lcd.height() - hh-5, hw - 5, hh - 5); // bas droite
    M5.Buttons.draw();
}


void afficherImage(const uint8_t* imageData, size_t imageSize) {
  // Assuming the image is too big, we need to draw it smaller.
  // Calculate the scaling factor (if you want to resize to 50%, for example)
  int scaledWidth = 175;    // 50% of original width
  int scaledHeight = 175;  // 50% of original height

  // Set the position for the image (optional)
  int posX = M5.Lcd.width() - scaledWidth;  // Align to the right side
  int posY = 0;  // Starting from the top (adjust as needed)

  // Draw the image at the new, smaller size (scaled)
  M5.Lcd.setBrightness(100);  // Set brightness if needed
  M5.Lcd.drawJpg(imageData, imageSize, posX, posY, scaledWidth, scaledHeight, 25, 25); // Draw the scaled image
}



void toggleColor(Event& e) {
    // Just so we can type "b." instead of "e.button->"
    Button& b = *e.button;

    if (b != M5.background) {
        // Toggles the button color between black and blue
        b.off.bg = (b.off.bg == BLACK) ? BLUE : BLACK;
        b.draw();
    }
}

void showPerformance(Event& e) {
    Serial.printf("%d in %d ms, average M5.update() took %.2f microseconds\n",
                  times, millis() - startTime,
                  (float)((millis() - startTime) * 1000) / times);
    startTime = millis();
    times     = 0;
}

void eventDisplay(Event& e) {
    Serial.printf("%-12s finger%d  %-18s (%3d, %3d) --> (%3d, %3d)   ",
                  e.typeName(), e.finger, e.objName(), e.from.x, e.from.y,
                  e.to.x, e.to.y);
    Serial.printf("( dir %d deg, dist %d, %d ms )\n", e.direction(),
                  e.distance(), e.duration);
}

void showHello(Event& e) {
    // Vérifie quel bouton a été cliqué
    if (e.button == &bl) {
      page = 0;
    } else {
      page = 1;
    }
    modif = true;

}






void connectToWiFi() {
  M5.Lcd.setCursor(0, 0);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  M5.Lcd.println("\nConnecté au réseau WiFi!");
}



/*          HUGO          */
void getServerData() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String url = String(serverUrl) + "/api-weather";
    Serial.printf("URL complète : %s\n", url.c_str());
    http.begin(url);

    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String response = http.getString();

      // Afficher la réponse brute
      Serial.println("Réponse brute :");
      Serial.println(response);

      M5.Lcd.setTextSize(1);
      M5.Lcd.setCursor(10, 10);
      M5.Lcd.println("Réponse brute:");
      //M5.Lcd.println(response);

      // Parser les données JSON
      StaticJsonDocument<1024> doc;
      DeserializationError error = deserializeJson(doc, response);

      if (error) {
        M5.Lcd.println("Erreur de parsing JSON:");
        M5.Lcd.println(error.c_str());
        Serial.println("Erreur de parsing JSON:");
        Serial.println(error.c_str());
      } else {
        JsonObject main = doc["main"];
        float temp = main["temp"];
        float temps = main["humidity"];

        JsonObject mainTempe = doc["weather"][0];
        String temperatureTemp = mainTempe["main"] ;

        temperatureAPI = temp;
        humAPI = temps;
        meteoAPI = temperatureTemp;

        // M5.Lcd.setCursor(10, 30);
        // M5.Lcd.printf("Temp: %.2f C", temp);
        // Serial.printf("Température API : %.2f\n", temp);
      }
    } else {
      String errorStr = http.errorToString(httpResponseCode);
      M5.Lcd.println("Erreur lors de la récupération: ");
      M5.Lcd.println(errorStr);
      Serial.printf("Erreur HTTP : %s\n", errorStr.c_str());
    }

    http.end();
  } else {
    M5.Lcd.println("Non connecté au WiFi!");
    Serial.println("Non connecté au WiFi !");
  }
}


void displayWeatherImage() {
  // Use if/else to compare the weather condition and display the corresponding image
  if (meteoAPI == "Clouds") {
    afficherImage(iconeNuage, sizeof(iconeNuage));
  } else if (meteoAPI == "Hail") {
    afficherImage(iconeGrele, sizeof(iconeGrele));
  } else if (meteoAPI == "Snow") {
    afficherImage(iconeNeige, sizeof(iconeNeige));
  } else if (meteoAPI == "Thunderstorm") {
    afficherImage(iconeOrage, sizeof(iconeOrage));
  } else if (meteoAPI == "Rain") {
    afficherImage(iconePluie, sizeof(iconePluie));
  } else if (meteoAPI == "Clear") {
    afficherImage(iconeSoleil, sizeof(iconeSoleil));
  } else if (meteoAPI == "CloudyClear") {
    afficherImage(iconeSoleilNuage, sizeof(iconeSoleilNuage));
  } else if (meteoAPI == "SunRain") {
    afficherImage(iconeSoleilPluie, sizeof(iconeSoleilPluie));
  } else {
    // If weather type doesn't match any case
    M5.Lcd.setCursor(10, 20);
    M5.Lcd.println("Weather not recognized");
    M5.Lcd.println(meteoAPI);  // Display the value of meteoAPI

    M5.update();
  }
}



void sendTestData() {
  if (WiFi.status() == WL_CONNECTED) {
    delay(5);
    HTTPClient http;

    // Construction de l'URL
    String url = String("http://") + serverIP + ":" + serverPort + "/api";

    // Requête pour "Alarme"
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("token", authToken);

    StaticJsonDocument<200> alarmDoc;
    alarmDoc["id"] = "Alarm";
    alarmDoc["temperature"] = tempera;
    alarmDoc["humidite"] = hum;



    String alarmRequestBody;
    serializeJson(alarmDoc, alarmRequestBody);

    int alarmHttpResponseCode = http.POST(alarmRequestBody);

    if (alarmHttpResponseCode > 0) {
      String alarmResponse = http.getString();
      M5.Lcd.setCursor(0, 170);
      //M5.Lcd.printf("Reponse Alarm: %s", alarmResponse.c_str());
    } else {
      M5.Lcd.setCursor(0, 170);
      M5.Lcd.printf("Erreur POST Alarm: %d", alarmHttpResponseCode);
    }

    http.end();
  } else {
    M5.Lcd.println("Connexion WiFi perdue");
  }
}


unsigned long previousTestMillisSendTempe = 0;  // Store the last time sendTestData was called
unsigned long testIntervalSendTempe = 10000;    // Interval of 10 seconds (10000 milliseconds)

void setup() {
  M5.begin();
  /*          Louis V          */
  M5.Buttons.addHandler(showHello, E_TAP);
  M5.Buttons.addHandler(showHello, E_TAP);
  M5.Buttons.addHandler(showHello, E_TAP);
  br.addHandler(showPerformance, E_TAP);
  br.repeatDelay = 1000;
  doButtons();
  startTime = millis();
  //________________________________________________

  M5.Lcd.setTextSize(2);
  M5.Lcd.println("Initialisation...");
  
  Serial.begin(115200);
  connectToWiFi();
  dht.begin();
  M5.Lcd.clear();

  sendTestData();

  getServerData();
}



void loop() {

  tempera = dht.readTemperature();
  hum = dht.readHumidity();

  unsigned long currentMillis = millis();

  // Call sendTestData every 10 seconds
  if (currentMillis - previousTestMillisSendTempe >= testIntervalSendTempe) {
    sendTestData();
    getServerData();
    previousTestMillisSendTempe = currentMillis; 
  }


  switch (page) {
      case 0: // Page avec sonde de température
          if (isnan(tempera) || isnan(hum)) {
              // Affiche une erreur si nécessaire
              if (!errorDisplayed || (currentMillis - errorDisplayMillis > interval) || modif) {
                  M5.Lcd.clear();
                  M5.Lcd.setCursor(10, 20);
                  M5.Lcd.println("Erreur lecture DHT !");
                  errorDisplayMillis = currentMillis;
                  errorDisplayed = true;
                  modif = false;
              }
              return;
          } else {
              // Met à jour les valeurs toutes les `interval` millisecondes
              if ((currentMillis - previousMillis > interval) || modif) {
                  previousMillis = currentMillis;
                  M5.Lcd.clear();
                  M5.Lcd.setCursor(10, 20);
                  M5.Lcd.printf("Données de la sonde");
                  M5.Lcd.setCursor(10, 60);
                  M5.Lcd.printf("Temp : %.2f C", tempera);
                  M5.Lcd.setCursor(10, 100);
                  M5.Lcd.printf("Hum : %.2f %%", hum);

                  modif = false;
              }
              errorDisplayed = false; // Réinitialise l'état d'erreur
          }
          break;

      case 1: // Une autre page
          if ((currentMillis - previousMillis > interval) || modif) {
            previousMillis = currentMillis;
            M5.Lcd.clear();
            M5.Lcd.setCursor(10, 20);
            M5.Lcd.printf("Données de l'API");
            M5.Lcd.setCursor(10, 60);
            M5.Lcd.printf("Temp : %.2f C", temperatureAPI);
            M5.Lcd.setCursor(10, 100);
            M5.Lcd.printf("Hum : %.2f %%", humAPI);

            displayWeatherImage();

            modif = false;
          }
          break;
  }



  M5.update();
  times++;


  doButtons();



}