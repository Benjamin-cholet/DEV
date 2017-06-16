Ce programe a été réalisé dans le cadre du projet S2 à Télécom Bretagne.
Il permet de communiquer avec le controlleur RFID Mercury 5e.
Il a été testé sur Macos 10.12, Ubuntu 16.04 et Raspbian Jessi 4.4.
Il requiert un compilateur C ainsi que des library unix.

============================Configuration============================
La voie somme doit être branchée sur le port Ant 1, voie différence sur le port Ant 2.


============================Compiltion rapide============================
- affecter la variable portName du fichier serial.c au device file correspondant
(faire ls /dev/ et chercher un nom qui contient usb, i.e. /dev/TTYUSB0).

- dans un shell :
        $cd /path/to/source
        $gcc *.c -std=gnu99 -o a.out
        $chmod a+x a.out
        $./a.out

============================Explication Rapide============================
Le protocol de communication est expliqué dans la documentation : "M5eFamilyDevGuide_Jan12.pdf".

Dans la fonction main (fichier main.c), on utilise les fonctions suivantes :
- void serial_init(int *fd) :
    Ouvre le device file et établi la connection série.

- void buildPacket(unsigned char **msg, unsigned char dataLength, M5eCmd op_code, unsigned char *data)
    Création du packet : on fait pointer "msg" vers un tableau contenant le numéro de la commande "op_code" et
    les éventuelles option contenues dans "data". Les options sont du types unsigned char (codées sur 1 octet)
    "dataLength" doit être égal à la longueur du tableau "data".

- void serial_write(int fd, unsigned char *msg)
    On envoie le packet "msg" au lecteur RFID.

- void serial_read(int fd, unsigned char **msg, int *length)
    On copie la réponse dans "msg". "length" est égal à la longueur de la réponse.

- void serial_read_to_stdout(int fd)
    On affiche la réponse du lecteur dans la console.

- void init(int fd)
    On configure le lecteur avec les commandes suivantes :
        bootFirmware (sans option) : démarre le firmware du lecteur.
        setCurrentRegion (0x08 pour EU3) : choix de la région (fréquences utilisées).
        setCurrentTagProtocol (0x0005 pour Gen2) : choix du protocol RFID
        setReadTXPower (0x09C4 pour 25dBm) : puissance TX en lecture en centi dBm.
        setAntennaPort (0x02, 0x01, 0x01, 0x01, 0x02 pour altener les modes mono-static et bi-static :
            alternativement TX,RX = 0x01, 0x01 puis TX,RX = 0x01, 0x02 ;
            0x00, 0x01, 0x01 pour du monostatic sur l'antenne 1):
            configuration des antennes (doc p.153).

On utilise la commande "readTagMultiple" pour lancer une phase d'acquisition. Les 2 derniers octets correspondent
à la durée (en ms) de cette phase.
Durant cette phase, des données sont sockées dans la mémoire interne du lecteur.
Pour récupérer ces données et les afficher, on fait appel à void readBuffer(int fd).





Fait par Benjamin CHOLET, février-juin 2017
Tuteur : François GALLÉE
