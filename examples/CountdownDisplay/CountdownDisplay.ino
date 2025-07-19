/**
 * Exemple : Affichage d'un décompte en secondes avec la bibliothèque Tempo
 * Démarre un timer de 20 secondes et affiche le temps restant chaque seconde
 */

#include <Arduino.h>
#include <Tempo.h>

Tempo Minuterie;

void setup() {
  Serial.begin(115200);
  Minuterie.Start(20, Tempo::SECONDE);
}

void loop() {
  static int last = -1;
  unsigned long sec = (Minuterie.GetTime() + 500) / 1000; // arrondi correct

  if (Minuterie.IsEnd()) {
    Serial.println("Compteur 20s terminé");
  } else if ((int)sec != last) {
    last = sec;
    Serial.print("Compteur 20s en cours. Reste encore : ");
    Serial.println(sec);
  }

  delay(100);
}
