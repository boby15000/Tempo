/**
 * @file Tempo.h
 * @author Nicolas Fourgheon
 * @page https://github.com/boby15000/Tempo
 * @brief Tempo est une bibliothèque qui vise à fournir une fonctionnalité de délai non bloquante.
 * @version v1.5.4
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
        bool IsStart();

        /**
         * @brief Vérifie si la tempo est en pause.
         * @return true si la tempo est en pause.
         */
        bool IsPause();

        /**
         * @brief Vérifie si la tempo est terminée (et met à jour son état).
         * @return true si la tempo est terminée.
         */
        bool IsEnd();

        /**
         * @brief Retourne le temps restant avant la fin de la tempo.
         * @return Temps restant (en millisecondes ou selon l’unité choisie).
         */
        unsigned long GetTime();

    private:
        Callback onEndCallback = nullptr; ///< Fonction callback appelée à la fin de la tempo.

        /**
         * @brief Convertit la durée selon l’unité spécifiée en millisecondes ou microsecondes.
         * @param unite Unité de temps.
         * @param seuil Durée à convertir.
         * @return Durée convertie en unités internes (millis ou micros).
         */
        unsigned long ConversionUnite(BaseTemps unite, unsigned long seuil);

        /**
         * @brief Structure interne contenant les données de la tempo.
         */
        struct TempoData {
            bool actif;               ///< Indique si la tempo est active.
            unsigned long seuil;     ///< Durée initiale convertie.
            BaseTemps unite;         ///< Unité de temps utilisée.
            unsigned long depart;    ///< Timestamp de départ.
            bool fini;               ///< Indique si la tempo est terminée.
            unsigned long restant;   ///< Temps restant.
            unsigned long pauseSeuil;///< Sauvegarde du seuil en cas de pause.
            bool autoRestart = false;///< Si vrai, redémarre automatiquement.
        };

        TempoData tempo; ///< Données internes de la tempo.
};

#endif // TEMPO_INCLUDED
