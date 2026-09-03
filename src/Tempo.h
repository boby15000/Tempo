/**
 * @file Tempo.h
 * @author Nicolas Fourgheon
 * @page https://github.com/boby15000/Tempo
 * @brief Tempo est une bibliothèque qui vise à fournir une fonctionnalité de délai non bloquante.
 * @version v1.6.1
 * @date 2023-05-24
 */

#if ARDUINO >= 100    
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#ifndef TEMPO_INCLUDED
#define TEMPO_INCLUDED

/**
 * @brief Pointeur vers une fonction de rappel (callback) appelée à la fin de la tempo.
 */
typedef void (*Callback)();

/**
 * @class Tempo
 * @brief Classe permettant de gérer un délai non bloquant (millis/micros) avec des fonctions de contrôle.
 */
class Tempo
{
    public:
        /**
         * @brief Enumération des unités de temps utilisables pour la tempo.
         */
        enum BaseTemps {
            MICRO = 1,   ///< Microsecondes
            MILLIS = 2,  ///< Millisecondes
            SECONDE = 3, ///< Secondes
            MINUTE = 4,  ///< Minutes
            HEURE = 5    ///< Heures
        };

        /**
         * @brief Constructeur de la classe Tempo.
         */
        Tempo();

        /**
         * @brief Initialise les paramètres de la tempo sans la démarrer.
         * @param value Durée souhaitée.
         * @param unite Unité de temps.
         * @param autoRestart Redémarrage automatique après la fin si true.
         */
        void Init(unsigned long value, BaseTemps unite, bool autoRestart = false);

        /**
         * @brief Démarre la tempo (ou redémarre si elle est terminée).
         * @param value Durée (0 = garde la durée précédente).
         * @param unite Unité de temps.
         * @param autoRestart Si vrai, la tempo redémarre automatiquement.
         */
        void Start(unsigned long value = 0, BaseTemps unite = MICRO, bool autoRestart = false);

        /**
         * @brief Redémarre la tempo à zéro avec les mêmes paramètres.
         */
        void ReStart();

        /**
         * @brief Met en pause la tempo en sauvegardant le temps restant.
         */
        void Pause();

        /**
         * @brief Arrête complètement la tempo.
         */
        void Stop();

        /**
         * @brief Définit une fonction callback appelée automatiquement à la fin de la tempo.
         * @param cb Pointeur vers la fonction callback.
         */
        void OnEnd(Callback cb);

        /**
         * @brief Met à jour l’état de la tempo. À appeler régulièrement dans `loop()`.
         */
        void Update();

        /**
         * @brief Vérifie si la tempo est active (en cours de décompte).
         * @return true si la tempo est active.
         */
        bool IsStart() const;

        /**
         * @brief Vérifie si la tempo est en pause.
         * @return true si la tempo est en pause.
         */
        bool IsPause() const;

        /**
         * @brief Vérifie si la tempo est terminée (et met à jour son état).
         * @return true si la tempo est terminée.
         */
        bool IsEnd();

        /**
         * @brief Retourne le temps restant avant la fin de la tempo.
         * @return Temps restant, toujours en unité interne : millisecondes, ou
         *         microsecondes si la tempo a été initialisée avec Tempo::MICRO
         *         (jamais dans l’unité d’origine SECONDE/MINUTE/HEURE).
         */
        unsigned long GetTime();

    private:
        Callback onEndCallback = nullptr; ///< Fonction callback appelée à la fin de la tempo.

        /**
         * @brief États possibles d’une tempo.
         */
        enum class State : uint8_t {
            IDLE,    ///< Jamais démarrée, ou arrêtée via Stop().
            RUNNING, ///< En cours de décompte.
            PAUSED,  ///< En pause, temps restant conservé.
            EXPIRED  ///< Décompte terminé.
        };

        /**
         * @brief Convertit la durée selon l’unité spécifiée en millisecondes ou microsecondes.
         * @param unite Unité de temps.
         * @param seuil Durée à convertir.
         * @return Durée convertie en unités internes (millis ou micros).
         */
        unsigned long ConversionUnite(BaseTemps unite, unsigned long seuil) const;

        /**
         * @brief Horodatage courant dans l’unité interne de la tempo (micros() ou millis()).
         */
        unsigned long Now() const;

        /**
         * @brief Structure interne contenant les données de la tempo.
         */
        struct TempoData {
            State state = State::IDLE;         ///< État courant de la tempo.
            unsigned long duration = 0;        ///< Durée configurée (unité interne), immuable hors reconfiguration.
            BaseTemps unite = MICRO;           ///< Unité de temps utilisée.
            unsigned long depart = 0;          ///< Timestamp de départ, valide si state == RUNNING.
            unsigned long remainingAtPause = 0;///< Temps restant figé, valide si state == PAUSED.
            bool autoRestart = false;          ///< Si vrai, redémarre automatiquement.
        };

        TempoData tempo; ///< Données internes de la tempo.
};

#endif // TEMPO_INCLUDED
