Hang Heat est un projet de porte-manteau chauffant conçu pour accélérer le séchage des vêtements grâce à un chauffage PTC piloté par Arduino.

📌 Ce dépôt GitHub correspond à la partie programmation (code Arduino).
Le projet complet, lui, inclut aussi la conception et la construction du porte-manteau de A à Z, notamment :

la conception du support,

le choix des matériaux,

la découpe/assemblage du bois,

l’intégration du système chauffant et de l’électronique dans la structure.

🎯 Objectif du projet

Proposer un porte-manteau chauffant fonctionnel et simple d’utilisation

Adapter le séchage selon plusieurs modes (humidité, température, durée)

Afficher les informations sur un écran LCD et permettre un contrôle par boutons

🧰 Matériel utilisé

Arduino Uno

LCD Keypad Shield 16x2

Capteur température/humidité HTU21D / HTU21DF

Module relais 1 canal

Chauffage PTC + alimentation adaptée

Structure en bois (porte-manteau fabriqué et assemblé dans le cadre du projet)

📚 Librairies Arduino

Adafruit_HTU21DF

Adafruit Unified Sensor

Wire

LiquidCrystal

⚙️ Fonctionnement général

Au démarrage, le système affiche “Systeme Pret”, puis un menu de sélection de programme.
Navigation via les boutons du LCD Keypad Shield.

Boutons

UP : naviguer vers le haut

DOWN : naviguer vers le bas

RIGHT : valider / lancer

LEFT : arrêter le programme (mode Calcul)

🧪 Programmes disponibles

Le menu propose 4 modes :

🌧️ Grosse pluie

Active le chauffage tant que l’humidité mesurée est supérieure au seuil défini.
Affiche l’humidité en temps réel.

⚡ Rapide

Chauffage actif pendant 30 minutes (durée fixe).

🌡️ Normal

Chauffage actif tant que la température est inférieure à 29°C.
Affiche la température en temps réel.

⏱️ Calcul

Permet de choisir un temps de séchage (en minutes) :

UP/DOWN : ajuster le temps

RIGHT : démarrer

LEFT : arrêter en cours d’exécution
