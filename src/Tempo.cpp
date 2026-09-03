/**
 * @file Tempo.cpp
 * @author Nicolas Fourgheon
 * @page https://github.com/boby15000/Tempo
 * @brief Tempo est une bibliothèque qui vise à fournir une fonctionnalité de délai non bloquante.
 * @version v1.6.1
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
    tempo.duration = this->ConversionUnite(unite, value);
    tempo.unite = unite;
    tempo.autoRestart = autoRestart;
    tempo.state = State::IDLE;
}

/**
 * @brief Démarre ou redémarre la tempo si elle n’est pas active.
 * @param value Durée (en fonction de l’unité choisie). 0 = garde la configuration précédente.
 * @param unite Unité de temps utilisée (ignorée si value == 0).
 * @param autoRestart Si vrai, la tempo redémarre automatiquement après la fin (ignoré si value == 0,
 *                    pour ne pas écraser silencieusement l’autoRestart en cours lors d’une reprise).
 */
void Tempo::Start(unsigned long value, BaseTemps unite, bool autoRestart) {
    if (tempo.state == State::RUNNING) return;

    bool resuming = (tempo.state == State::PAUSED);
    if (value > 0) {
        this->Init(value, unite, autoRestart);
        resuming = false;
    }

    tempo.depart = resuming ? (this->Now() - (tempo.duration - tempo.remainingAtPause)) : this->Now();
    tempo.state = State::RUNNING;
}

/**
 * @brief Redémarre la tempo depuis le début avec les mêmes paramètres.
 */
void Tempo::ReStart() {
    tempo.depart = this->Now();
    tempo.state = State::RUNNING;
}

/**
 * @brief Met en pause la tempo et sauvegarde le temps restant.
 */
void Tempo::Pause() {
    if (tempo.state != State::RUNNING) return;
    unsigned long elapsed = this->Now() - tempo.depart;
    tempo.remainingAtPause = (elapsed >= tempo.duration) ? 0 : (tempo.duration - elapsed);
    tempo.state = State::PAUSED;
}

/**
 * @brief Arrête totalement la tempo et réinitialise les paramètres internes.
 */
void Tempo::Stop() {
    tempo.state = State::IDLE;
    tempo.depart = 0;
    tempo.remainingAtPause = 0;
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
    if (tempo.state != State::RUNNING) return;

    unsigned long elapsed = this->Now() - tempo.depart;
    if (elapsed < tempo.duration) return;

    tempo.state = State::EXPIRED;
    if (onEndCallback != nullptr) onEndCallback();
    if (tempo.autoRestart) this->ReStart();
}

/**
 * @brief Indique si la tempo est active.
 * @return true si active, false sinon.
 */
bool Tempo::IsStart() const {
    return tempo.state == State::RUNNING;
}

/**
 * @brief Indique si la tempo est en pause.
 * @return true si en pause, false sinon.
 */
bool Tempo::IsPause() const {
    return tempo.state == State::PAUSED;
}

/**
 * @brief Vérifie si la tempo est terminée. Met à jour l’état.
 * @return true si terminée.
 */
bool Tempo::IsEnd() {
    this->Update();
    return tempo.state == State::EXPIRED;
}

/**
 * @brief Retourne le temps restant avant la fin de la tempo.
 * @return Temps restant (millisecondes ou microsecondes selon l’unité).
 */
unsigned long Tempo::GetTime() {
    this->Update();
    switch (tempo.state) {
        case State::RUNNING: {
            unsigned long elapsed = this->Now() - tempo.depart;
            return (elapsed >= tempo.duration) ? 0 : (tempo.duration - elapsed);
        }
        case State::PAUSED:  return tempo.remainingAtPause;
        case State::EXPIRED: return 0;
        default:              return tempo.duration; // IDLE : durée configurée, pas encore démarrée
    }
}

/**
 * @brief Convertit la durée selon l’unité en millisecondes ou microsecondes.
 * @param unite Unité de base.
 * @param seuil Valeur à convertir.
 * @return Valeur convertie.
 */
unsigned long Tempo::ConversionUnite(BaseTemps unite, unsigned long seuil) const {
    switch (unite) {
        case SECONDE: return (seuil * 1000);
        case MINUTE: return (seuil * 60000);
        case HEURE:  return (seuil * 3600000);
        default:     return seuil;
    }
}

/**
 * @brief Horodatage courant dans l’unité interne de la tempo (micros() ou millis()).
 */
unsigned long Tempo::Now() const {
    return (tempo.unite == MICRO) ? micros() : millis();
}
