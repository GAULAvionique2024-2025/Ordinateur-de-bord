# Drivers de l'ordinateur de bord

Voici une description des drivers programmés pour l'implémentation des composantes avec le IMU.

# Driver GPIO:

Ce driver permet de gérer les ports GPIO (General Purpose Input/Output) sur un microcontrôleur STM32. Il fournit des fonctions pour initialiser les broches GPIO, lire leur état, écrire des valeurs et basculer leur état.

- Définition broche GPIO

Le driver utilise plusieurs constantes pour définir la configuration des GPIO :

Direction:
Détermine le mode d'une broche GPIO (Input, Output).

Options d'une broche Input:
Détermine l'option d'une broche de type Input selon le besoin (Analogique, Push-Pull).

Option d'une broche Output:
Détermine l'option d'une broche de type Output selon le besoin (Push-Pull, Open-drain, Alternative).

États:
LOW : État bas
HIGH : État haut

- Fonctions et explications du driver

Voici une présentation des fonctions utilisées.

```
void Init_GPIO(GPIO_TypeDef *port, unsigned short pin, unsigned short dir, unsigned short opt)
```
Cette fonction initialise un port GPIO en spécifiant le port, la broche, la direction et l'option souhaitées.
```
Exemple
Init_GPIO(GPIOA, 5, OUT10, O_GP_PP);
```
Ici, la broche PA5 est configurée pour être une sortie à une vitesse de 10MHz en mode push-pull.

```
int Read_GPIO(GPIO_TypeDef *port, unsigned short pin)
```
Cette fonction lit l'état d'une broche GPIO en mode Input et renvoie HIGH(1) ou LOW(0).
```
Exemple
int state = Read_GPIO(GPIOA, 5);
if (state == HIGH) {
    // L'état est HIGH
}
```
Ici, on compare l'état de la broche PA5 à HIGH, par exemple, lorsqu'on appuie sur un bouton.

```
void Write_GPIO(GPIO_TypeDef *port, unsigned short pin, unsigned short state)
```
Cette fonction écrit un état HIGH(1) ou LOW(0) sur une broche GPIO.
```
Exemple
Write_GPIO(GPIOA, 5, HIGH);
```
Dans cet exemple, l'état de PA5 est mis à HIGH.


# Driver SPI:

Ce driver permet de configurer et d'utiliser le protocole de communication SPI (Serial Peripheral Interface) sur un microcontrôleur STM32. Il fournit des fonctions pour initialiser le périphérique SPI, transmettre des données, recevoir des données, et effectuer des opérations de transmission/réception simultanées.

- Fonctions et explications du driver

```
void SPI_Init(SPI_TypeDef *SPIx)
```
Cette fonction initialise le périphérique SPI spécifié (SPI1 ou SPI2). Elle configure les broches GPIO nécessaires pour le fonctionnement du SPI, y compris le CS (Chip Select), le CLK (horloge), le MISO (Master In Slave Out) et le MOSI (Master Out Slave In).
```
Exemple
SPI_Init(SPI1);
```
Dans cet exemple, le périphérique SPI1 est initialisé avec les broches appropriées.

```
int SPI_TX(SPI_TypeDef *SPIx, uint8_t *data, int size)
```
Cette fonction transmet un tableau de données via SPI. Elle envoie chaque octet de données en attendant que le buffer TX soit vide avant d'écrire le prochain octet.
```
Exemple
uint8_t txData[] = {0xAA, 0xBB, 0xCC};
int result = SPI_TX(SPI1, txData, sizeof(txData));
if (result == 0) {
    // Transmission réussie
} else {
    // Erreur de transmission
}
```

```
int SPI_RX(SPI_TypeDef *SPIx, uint8_t *data, int size)
```
Cette fonction reçoit des données via SPI. Elle envoie un octet "dummy" pour générer l'horloge nécessaire à la réception des données.
```
Exemple
uint8_t rxData[3];
int result = SPI_RX(SPI1, rxData, sizeof(rxData));
if (result == 0) {
    // Réception réussie, les données sont dans rxData
} else {
    // Erreur de réception
}
```

```
int SPI_TransmitReceive(SPI_TypeDef *SPIx, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size, uint32_t Timeout)
```
Cette fonction permet de transmettre et de recevoir des données en même temps. Elle utilise les buffers TX et RX pour envoyer et recevoir des octets simultanément.
```
uint8_t txData[] = {0x01, 0x02, 0x03};
uint8_t rxData[3];
int result = SPI_TransmitReceive(SPI1, txData, rxData, sizeof(txData), 200);
if (result == 0) {
    // Transmission et réception réussies, les données reçues sont dans rxData
} else {
    // Erreur lors de la transmission/réception
}
```

# Driver USART

Ce driver permet de gérer les communications série via les périphériques USART (Universal Synchronous/Asynchronous Receiver-Transmitter) sur un microcontrôleur STM32. Il fournit des fonctions pour initialiser le périphérique USART, envoyer et recevoir des données en mode de sondage (polling).

- Fonctions et explications du driver

```
void USART_Init(USART_TypeDef *USARTx, uint16_t baudrate, uint16_t frequency_MHz)
```
Cette fonction initialise un périphérique USART avec les paramètres spécifiés : taux de transmission (baud rate) et fréquence d'horloge du système. Elle configure également les broches GPIO associées pour la transmission (TX) et la réception (RX).
```
USART_Init(USART1, 9600, 8);
```
Avec cet exemple, on initialise le périphérique connecté à UART1 en définissant son baudrate (9600bps) et la vitesse de transfert (8MHz).

```
int8_t USART_TX(USART_TypeDef *USARTx, uint8_t *data, uint16_t size)
```
Cette fonction transmet des données via USART en mode de sondage. Elle attend que le registre de données soit prêt à accepter de nouvelles données avant de les écrire. Elle retourne 0 si la transmission est réussie et -1 en cas de timeout.
```
uint8_t message[] = "Hello, USART!";
if (USART_TX(USART1, message, sizeof(message)) < 0) {
    // Gérer l'erreur de transmission
}
```

```
int8_t USART_RX(USART_TypeDef *USARTx, uint8_t *data, uint16_t size)
```
Cette fonction reçoit des données via USART en mode de sondage. Elle attend que des données soient disponibles dans le registre de données avant de les lire. Elle retourne 0 si la réception est réussie et -1 en cas de timeout.
```
uint8_t buffer[20];
if (USART_RX(USART1, buffer, sizeof(buffer)) < 0) {
    // Gérer l'erreur de réception
}
```

- Détails techniques

Le registre de contrôle (CR1) est configuré pour activer la transmission (TE) et la réception (RE).
La vitesse de transmission est définie par le registre BRR, calculée à partir de la fréquence d'horloge et du baudrate spécifié.
Le format de données est configuré pour 8 bits sans parité, avec un bit d'arrêt.
Un timeout est utilisé pour éviter que le programme ne se bloque indéfiniment lors de l'attente de la disponibilité des registres.
Des messages d'erreur peuvent être gérés en vérifiant la valeur de retour des fonctions de transmission et de réception.

# Driver CRC

Ce driver permet de calculer un code de contrôle de redondance cyclique sur 16 bits (CRC16) sur un tableau de données. Le CRC est une méthode utilisée pour détecter les erreurs dans les données transmises ou stockées.

- Fonctions et explications du driver

```
uint16_t CRC16_Calculate(uint8_t *data, uint8_t size)
```
Cette fonction calcule le CRC16 d'un tableau de données. Elle initialise le registre CRC, applique le CRC à chaque octet des données, et renvoie le résultat dans un entier non-signé de 16 bits.
```
int8_t message[] = {0x01, 0x02, 0x03, 0x04};
uint16_t crc = CRC16_Calculate(message, sizeof(message));
```

Retour à la [page principale](../README.md)