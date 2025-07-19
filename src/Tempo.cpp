/**
 * @file Tempo.cpp
 * @author Nicolas Fourgheon
 * @page https://github.com/boby15000/Tempo
 * @brief Tempo est une bibliothèque qui vise à fournir une fonctionnalité de délai non bloquante.
 * @version v1.5.4
 * @date 2023-05-24
 */

#include "Tempo.h"
#include <Arduino.h>

/**
 * @brief Constructeur de la classe Tempo.
 */
Tempo::Tempo() {}

/**
 * @brief Initialise les paramètres de la tempo sans la démarrer.
 * @param value Durée souhaitée.
 * @param unite Unité de temps.
 * @param autoRestart Si vrai, la tempo redémarre automatiquement après la fin.
 */
void Tempo::Init(unsigned long value, BaseTemps unite, bool autoRestart) {
    tempo.actif = 0;
    tempo.seuil = this->ConversionUnite(unite, value);
    tempo.unite = unite;
    tempo.autoRestart = autoRestart;
}

/**
 * @brief Démarre ou redémarre la tempo si elle n’est pas active.
 * @param value Durée (en fonction de l’unité choisie).
 * @param unite Unité de temps utilisée.
 * @param autoRestart Si vrai, la tempo redémarre automatiquement après la fin.
 */
void Tempo::Start(unsigned long value, BaseTemps unite, bool autoRestart) {
    if (tempo.actif && !tempo.fini) return;
    if (value > 0) this->Init(value, unite);
    tempo.actif = 1;
    tempo.depart = (tempo.unite == MICRO) ? micros() : millis();
    tempo.fini = 0;
    tempo.autoRestart = autoRestart;
}

/**
 * @brief Redémarre la tempo depuis le début avec les mêmes paramètres.
 */
void Tempo::ReStart() {
    tempo.actif = 1;
    tempo.depart = (tempo.unite == MICRO) ? micros() : millis();
    tempo.fini = 0;
}

/**
 * @brief Met en pause la tempo et sauvegarde le temps restant.
 */
void Tempo::Pause() {
    if (!tempo.actif || tempo.fini) return;
    tempo.pauseSeuil = tempo.seuil;
    tempo.actif = 0;
    tempo.seuil = tempo.seuil - (((tempo.unite == MICRO) ? micros() : millis()) - tempo.depart);
}

/**
 * @brief Arrête totalement la tempo et réinitialise les paramètres internes.
 */
void Tempo::Stop() {
    tempo.actif = 0;
    tempo.depart = 0;
    tempo.pauseSeuil = 0;
}

/**
 * @brief Définit une fonction callback appelée automatiquement à la fin de la tempo.
 * @param cb Fonction à appeler en fin de tempo.
 */
void Tempo::OnEnd(Callback cb) {
    onEndCallback = cb;
}

/**
 * @brief Met à jour l'état de la tempo. Doit être appelée régulièrement dans loop().
 */
void Tempo::Update() {
    if (tempo.actif && !tempo.fini) {
        unsigned long current = (tempo.unite == MICRO) ? micros() : millis();
        unsigned long elapsed = current - tempo.depart;
        tempo.fini = (elapsed >= tempo.seuil);
        tempo.restant = tempo.fini ? 0 : tempo.seuil - elapsed;

        if (tempo.fini && onEndCallback != nullptr) onEndCallback();

        if (tempo.pauseSeuil > 0 && tempo.fini) {
            tempo.seuil = tempo.pauseSeuil;
            tempo.pauseSeuil = 0;
        }

        if (tempo.fini && tempo.autoRestart) this->ReStart();
    }
}

/**
 * @brief Indique si la tempo est active.
 * @return true si active, false sinon.
 */
bool Tempo::IsStart() {
    return tempo.actif;
}

/**
 * @brief Indique si la tempo est en pause.
 * @return true si en pause, false sinon.
 */
bool Tempo::IsPause() {
    return tempo.pauseSeuil > 0 && !tempo.actif;
}

/**
 * @brief Vérifie si la tempo est terminée. Met à jour l’état.
 * @return true si terminée.
 */
bool Tempo::IsEnd() {
    this->Update();
    return tempo.fini;
}

/**
 * @brief Retourne le temps restant avant la fin de la tempo.
 * @return Temps restant (millisecondes ou microsecondes selon l’unité).
 */
unsigned long Tempo::GetTime() {
    this->Update();
    return tempo.restant;
}

/**
 * @brief Convertit la durée selon l’unité en millisecondes ou microsecondes.
 * @param unite Unité de base.
 * @param seuil Valeur à convertir.
 * @return Valeur convertie.
 */
unsigned long Tempo::ConversionUnite(BaseTemps unite, unsigned long seuil) {
    switch (unite) {
        case SECONDE: return (seuil * 1000);
        case MINUTE: return (seuil * 60000);
        case HEURE:  return (seuil * 3600000);
        default:     return seuil;
    }
}
