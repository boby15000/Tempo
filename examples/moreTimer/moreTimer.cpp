#include <Arduino.h>
#include <Tempo.h>

Tempo Minuterie;
Tempo Minuterie1;
Tempo Minuterie2;

void setup() {
  // Initiale une communication série
  Serial.begin(9600);
  // Démarre deux minuteurs avec leurs paramétres
  Minuterie.Start(1, Tempo::MINUTE);
  Minuterie1.Start(10, Tempo::SECONDE);
  // Initialise le minuteur avec ses paramétres (ne démarre le compteur)
  Minuterie2.Init(2000, Tempo::MICRO);
}

String message;
String message1;
String message2;

void loop() {

  if ( Minuterie.IsEnd() )
  { message = "Compteur 1mn terminé"; }
  else
  { message = "Compteur 1mn en cours, Reste encore : " + String(Minuterie.GetTime()/1000); }

  if ( Minuterie1.IsEnd() )
  { message1 = "Compteur 10s terminé"; Minuterie2.Start();  }
  else
  { message1 = "Compteur 10s en cours, Reste encore : " + String(Minuterie1.GetTime()/1000); }

  if ( Minuterie2.IsEnd() )
  { message2 = "Compteur 2000 micro terminé"; }
  else
  { message2 = "Compteur 2000 micro en cours, Reste encore : " + String(Minuterie2.GetTime()); }

  Serial.println(message);
  Serial.println(message1);
  Serial.println(message2);
  delay(1000);
}