
# ⏱️ Tempo – Minuterie simple pour Arduino

`Tempo` est une bibliothèque Arduino conçue pour faciliter la gestion des temporisations non bloquantes, avec un support natif des **callbacks** et une option de **redémarrage automatique**. Elle permet d’exécuter facilement une action après un certain délai, sans bloquer le programme avec `delay()`.

---

## 🚀 Fonctionnalités

- ✅ Temporisation non bloquante
- 🔁 Redémarrage automatique optionnel
- 🧠 Support des **callbacks** à la fin du compte à rebours
- ⏳ Unités supportées : microsecondes, millisecondes, secondes, minutes, heures
- 🔧 Facile à intégrer, légère et optimisée pour les petits microcontrôleurs

---

## 📦 Installation

1. Télécharge ou clone ce dépôt :
   ```bash
   git clone https://github.com/ton-utilisateur/Tempo.git
   ```

2. Copie le dossier `Tempo` dans ton répertoire `libraries/` d’Arduino :
   - Sous Windows : `Documents/Arduino/libraries`
   - Sous Linux/macOS : `~/Arduino/libraries/`

3. Redémarre l’IDE Arduino.

---

## 🛠️ Utilisation de base

```cpp
#include <Tempo.h>

Tempo Minuterie;

void setup() {
  // Initiale une communication série
  Serial.begin(9600);
  // Démarre le minuteur avec ses paramétres
  Minuterie.Start(5, Tempo::SECONDE);
}

String message;

void loop() {
  // Si IsEnd() utilisé pas necessaire d'utiliser Update()
  if ( Minuterie.IsEnd() )
  { message = "Compteur 5s terminé"; }
  else
  { message = "Compteur 5s en cours, Reste encore : " + String(Minuterie.GetTime()/1000); }

  Serial.println(message);
  delay(1000);
}
```

---

## ⌚ Méthodes principales

| Méthode                       | Description |
|-------------------------------|-------------|
| `Start(valeur, unité, autoRestart)`  | Démarre la temporisation. |
| `Update()`                    | À appeler régulièrement dans `loop()` pour que la tempo fonctionne *(Inutile si IsEnd() ou GetTime() utilisé)*. |
| `OnEnd(callback)`             | Définit une fonction à appeler à la fin de la Tempo. |
| `IsStart()`                   | Renvoie `true` si la tempo est en cours. |
| `IsEnd()`                     | Renvoie `true` si la tempo est en terminé. |
| `GetTime()`                   | Renvoie le temps restant en Millis ou Micro selon l'unité utilisé. |
| `Stop()`                      | Arrête la tempo manuellement. |

---

## 📌 Unités de temps disponibles

```cpp
Tempo::MICRO
Tempo::MILLIS
Tempo::SECONDE
Tempo::MINUTE
Tempo::HEURE
```

---

## 🧪 Exemples fournis

- `oneTimer.cpp` – Exemple de base
- `moreTimer.cpp` – Exemple avec plusieurs Timers
- `pauseTimer.cpp` – Exemple avec pause de du Timer
- `CallbackTimer.cpp` – Exemple de CallBack avec Option Redémarrage Auto

---

## 🧰 Compatibilité

- ATmega328 (Uno, Nano)
- ATtiny85 
- ESP8266, ESP32
- Arduino Mega

---

## 📜 Changelog

### v1.6.1
Refonte interne de la classe `Tempo` : l'état (actif / en pause / terminé) est désormais représenté par une machine à états explicite au lieu de plusieurs champs booléens combinés, ce qui corrige plusieurs bugs latents. **Aucun changement de l'API publique** — le comportement observable reste identique, hormis les corrections ci-dessous.

Corrections :
- `Start()` appelé sans argument pour reprendre après une `Pause()` n'écrase plus silencieusement l'option `autoRestart`.
- `Pause()` ne peut plus produire une valeur aberrante (dépassement arithmétique) si elle est appelée juste après l'expiration réelle de la tempo.
- `GetTime()` ne renvoie plus une valeur obsolète pendant une pause.
- Les champs internes de la tempo sont désormais toujours initialisés, y compris pour une instance `Tempo` locale (pas seulement globale).
- Documentation corrigée : `GetTime()` renvoie toujours le temps restant en millisecondes (ou microsecondes si l'unité `MICRO` est utilisée), jamais dans l'unité d'origine (secondes/minutes/heures) ; le tableau des méthodes mentionnait par erreur `IsRunning()` au lieu de `IsStart()`.

---

## 📄 Licence

MIT – Utilisation libre avec attribution.

---

## 🤝 Contribuer

Les contributions sont les bienvenues ! Forkez le dépôt, créez une branche, puis ouvrez une *pull request*.

---

> Mainteneur : Nicolas Fourgheon
>  
> Pour toute question, ouvrez un [Issue](https://github.com/boby15000/Tempo/issues).
