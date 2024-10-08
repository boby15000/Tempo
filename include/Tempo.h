#if ARDUINO >= 100    
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#ifndef TEMPO_INCLUDED
#define TEMPO_INCLUDED

class Tempo
{
    public:
        static const int Micro = 1;
        static const int Millis = 2;
        static const int Seconde = 3;
        static const int Minute = 4;
        static const int Heure = 5;

        Tempo(); // Declare la Tempo
        void Init(long value, int unite); // Initialise la Tempo
        void Start(long value = 0, int unite = 0); // Démarre la Tempo si il n'est pas en fonction
        void Stop(); // Arrete la Tempo.
        bool IsStart(); // Indique si la Tempo est en fonction.
        int End(); // Indique la fin de la tempo.
        unsigned long GetTime(); // Retourne le temps restant en Millis ou Micro si Micro utilisé en tant qu'unité

    private:
        long ConversionUnite(long unite, long seuil);
        unsigned long _tempo[6];
        /*
        Tempo[0] => Etat du compteur
        Tempo[1] => Seuil du compteur
        Tempo[2] => Unite du compteur
        Tempo[3] => Temps initiale du compteur au démarrage
        Tempo[4] => Etat fin de compteur
        Tempo[5] => Temps restant
        */
};
#endif