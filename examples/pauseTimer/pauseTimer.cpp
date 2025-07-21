#include <Arduino.h>
#include <Tempo.h>

Tempo Minuterie;
Tempo Minuterie1;

void setup() {
  Serial.begin(9600);
  // Démarre le minuteur avec ses paramétres
  Minuterie.Start(30, Tempo::SECONDE);
}


void loop() {

  // Démarre le compteur si celui-ci n'est pas déjà en fonction
  Serial.println(Minuterie.GetTime());
  Serial.println(Minuterie1.GetTime());

  if ( Minuterie.GetTime() < 23000 && !Minuterie1.IsStart() ) 
  {
    Minuterie.Pause();
    Minuterie1.Start(10, Tempo::SECONDE);
  }

  if ( Minuterie1.IsEnd() ) Minuterie.Start();

  delay(1000);
}