/*
  Erstellt von:             Johannes Beißwenger, Simon Gaiser
  Erstellt im Zeitraum:     Oktober 2017
  Aufgaben des Programms:   Das Programm Überwacht den Druckablauf des 3D-Druckes der staatlichen Majolika.
                          Nach dem Fertigstellen eines Druckauftrages wird ein Förderband angesteuert, welches das fertige Tonobjekt aus dem Druckraum fährt.
                          Anschließend kann ein neuer Druckauftrag gestartet werden.
*/

// Inkludierung der Bibliotheken
//------------------------------------------------------------------------------//
#include <Wire.h>                 // I2C
#include <Adafruit_GFX.h>         // OLED
#include <Adafruit_SSD1306.h>     // Graphic

//Definition der Pinbelegung / Namensgebung
//-------------------------------------------------------------------------------//
#define SignalDrucker 25                 // Endschalter am 3D-Drucker
#define SignalFoerderband 32             // Lichtschranke am Förderband
#define FoerderbandFahren 5              // Steuersignal für Förderband fahren

#define OLED_RESET 4                     // not used / nicht genutzt bei diesem Display

// Initialisierungen
//------------------------------------------------------------------------------//
Adafruit_SSD1306 display(OLED_RESET);

// Funktionen
//------------------------------------------------------------------------------//
void FoerderbandVoll();
void DruckBetrieb();

// Definition der Pins
//------------------------------------------------------------------------------//
void setup()
{
  Serial.begin(115200);           
  // serielle Schnittstelle

  //Oled Initialisierung
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(INVERSE);

  //Ausgänge
  pinMode(FoerderbandFahren, OUTPUT);           // Steuersignal Förderband fahren

  //Eingänge
  pinMode(SignalDrucker, INPUT_PULLUP);         // Eingangssignal 3D-Drucker, Achtung PULLUP -> Taster gegen Masse
  pinMode(SignalFoerderband, INPUT);     // Eingangssignal Lichtschranke Förderband, Achtung PULLUP ->  Taster gegen Masse
}

// Globale Variablen
//-----------------------------------------------------------------------------//
unsigned int TextS = 1;       // Textgroeße

//-----------------------------------------------------------------------------//
void loop()
{ 
  Serial.println("Loop");
  //Serial.println("Taster");
  //Serial.println(digitalRead(SignalDrucker));
  digitalWrite(FoerderbandFahren, HIGH);
  if (digitalRead(SignalFoerderband) == HIGH)
  {
       FoerderbandVoll();
  }
  else
  {
        DruckBetrieb();
  }
}

//-----------------------------------------------------------------------------//
void FoerderbandVoll()
{
  Serial.println("Foerderband Voll");
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(TextS);
  display.println("Foerderband Voll");
  display.println();
  display.println("Bitte Tonobjekte von Foerderband entnehmen");
  display.display();
}

void DruckBetrieb()
{
    int val;
  int buttonState = digitalRead(SignalDrucker);     // Anfangszustand lesen
  int buttonPresses = 0;

  while (digitalRead(SignalFoerderband) == LOW)
  { 
    val = digitalRead(SignalDrucker);               // Eingabewert lesen und in val speichern
    if (val != buttonState)                         // Der Zustand des Schalters hat sich verändert
    {
      if (val == LOW)                               // Ist der Schalter gedrückt?
      {
        
       buttonPresses++;                            // Tastendrücke zählen
      Serial.println("Schalter");
      }
    }
    buttonState = val;                              // Den Zustand merken
    if (buttonPresses == 0)                         // kein Druckersignal erhalten
    { // hier ggf die Zahlen ändern um zuerst hoch und dann runter zu fahren
      Serial.println("Anlage Bereit");
      display.clearDisplay();
      display.setCursor(0, 0);
      display.setTextSize(TextS);
      display.println("Anlage Bereit");
      display.display();
      Serial.println("Schalter0");
    }
    else if (buttonPresses == 1)                    // Drucker fährt Grundstellung
    {
      Serial.println("Nullposition");
      display.clearDisplay();
      display.setCursor(0, 0);
      display.setTextSize(TextS);
      display.println("Nullposition");
      //display.println("??Druckzeit??");
      display.display();
      Serial.println("Schalter1");
    }
    else if (buttonPresses == 2)                    // Drucker fährt von Grundstellung in Arbeitsposition
    {
      Serial.println("Druckbetrieb");
      display.clearDisplay();
      display.setCursor(0, 0);
      display.setTextSize(TextS);
      display.println("Druckbetrieb");
      //display.println("??Druckzeit??");
      display.display();
      Serial.println("Schalter2");
    }
    else if (buttonPresses == 3)                    // Drucker ist fertig, fährt von Arbeitsposition in Grundstellung
    {
      Serial.println("Druck beendet.... Foerderband fahren");
      Serial.println("Schalter3");
      display.clearDisplay();
      display.setCursor(0, 0);
      display.setTextSize(TextS);
      display.println("Druck beendet");
      display.println("Foerderband fahren");
      display.display();
      digitalWrite(FoerderbandFahren, LOW);        // Förderband ansteuern
      delay(2000);                                  // 2 s --> 15cm Verfahrweg
      digitalWrite(FoerderbandFahren, HIGH);
      buttonPresses = 1;
    }
  }
}

