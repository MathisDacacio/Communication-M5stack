# Projet : Station de Communication M5Stack

Ce projet consiste à créer une station de communication basée sur la plateforme M5Stack, utilisant le langage de programmation C++. L'objectif est de gérer différents dispositifs (porte, détecteur de mouvement, alarme, lumière, météo, capteur de température et d'humidité, etc.) et d'afficher les informations sur un écran LCD, tout en permettant une interaction via une interface web.

## Table des Matières

- [Aperçu](#aperçu)
- [Fonctionnalités](#fonctionnalités)
- [Prérequis](#prérequis)
- [Installation](#installation)
- [Comment utiliser le projet](#comment-utiliser-le-projet)

## Aperçu

Ce projet comprend plusieurs modules permettant de contrôler et d'afficher des informations sur la station M5Stack :

1. **Contrôle de la porte** : Gestion de l'état d'une porte (ouverte/fermée).
2. **Détecteur de mouvement et alarme** : Un système d'alarme qui se déclenche en cas d'intrusion détectée.
3. **Contrôle de la lumière** : Un code qui permet de moduler la luminosité de la lumière.
4. **Météo et capteurs** : Un module qui interagit avec une API pour récupérer les informations météorologiques, ainsi que les données d'un capteur de température et d'humidité.
5. **Écran LCD et interface web** : Affichage des informations collectées sur un écran LCD, ainsi qu'une interface web pour la connexion Wi-Fi.

## Fonctionnalités

### 1. **Contrôle de la porte**
   - Suivi de l'état de la porte (ouverte ou fermée) via un capteur.

### 2. **Détecteur de mouvement et alarme**
   - Détection de mouvement en temps réel.
   - Activation d'une alarme sonore ou visuelle en cas d'intrusion.

### 3. **Contrôle de la lumière**
   - Modification de la luminosité de la lumière via un code C++.

### 4. **Météo et capteurs**
   - Récupération des données météo via une API en ligne.
   - Collecte des mesures de température et d'humidité grâce à des capteurs.

### 5. **Affichage sur écran LCD et interface web**
   - Affichage des informations (état de la porte, mouvement, température, humidité, météo) sur l'écran LCD de la M5Stack.
   - Interface web permettant de connecter l'appareil à un réseau Wi-Fi et de consulter les informations via un navigateur.

## Prérequis

- **M5Stack** ou toute autre plateforme compatible.
- **Arduino IDE** pour la programmation de la plateforme.
- Bibliothèques nécessaires :
  - `M5Stack` pour gérer la plateforme M5Stack.
  - `WiFi` pour la connexion Wi-Fi.
  - `HTTPClient` pour interagir avec les API.
  - `DHT` ou `Adafruit_Sensor` pour gérer les capteurs de température et d'humidité.
  - Autres bibliothèques spécifiques pour l'API météo et l'écran LCD.

## Installation

1. Téléchargez et installez **Arduino IDE** (version 1.8 ou supérieure).
2. Installez les bibliothèques nécessaires à travers le gestionnaire de bibliothèques d'Arduino :
   - `M5Stack`
   - `WiFi`
   - `HTTPClient`
   - `DHT` (ou autre bibliothèque de capteurs)
3. Ouvrez le projet dans **Arduino IDE**.
4. Branchez la plateforme M5Stack à votre ordinateur via USB.
5. Sélectionnez la bonne carte et le bon port dans **Outils** > **Type de carte** et **Port**.
6. Chargez le code sur la plateforme.

## Comment utiliser le projet

1. Connectez le M5Stack à une source d'alimentation.
2. Connectez le M5Stack au réseau Wi-Fi en accédant à l'interface web via un navigateur. La page web vous permettra de visualiser les informations de la station.
3. Vous pouvez modifier les paramètres, surveiller l'état de la porte, la température, l'humidité, ainsi que l'éclairage de la station via l'interface web.
4. L'écran LCD affichera en temps réel les informations des capteurs et l'état des dispositifs.
