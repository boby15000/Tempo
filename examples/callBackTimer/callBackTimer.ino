#include <Arduino.h>
#include <Tempo.h>

Tempo Minuterie;

void FinDeTempo();

void setup() {
  Serial.begin(9600);
  Minuterie.OnEnd(FinDeTempo); // Ajout d'une fonction de CallBack 
  Minuterie.Start(20, Tempo::SECONDE, true); // Ajout de l'AutoRestart (redémarrage auto de la Tempo)
}


void loop() {

  Minuterie.Update();
  Serial.println("En cours");
  
  delay(1000);
}

// Fonction appelé automatique à la fin de la Tempo
void FinDeTempo()
{
  Serial.println("Tempo Terminé");
  delay(5000);
}