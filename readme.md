|DISPOSITIVO		|IP				|FUNCIONALIDADE
|-----------------------|-------------------------------|-------------------------------------------------------|
|Raspberry		|192.168.31.197	at wlan0 e	|OpenPLC Master na porta 8080 e cliente VPN para acessar|
|			|10.8.0.2 at tun0		|o supervisório desenvolvido no ScadaBR, que executa no |
|			|				| AWS, usando a porta 9090.				|
|ESP32Atuador		|192.168.31.61			|Aplica tensão no circuito RC				|
|ESP32Sensor		|192.168.31.91			|Mede a tensão do capacitor no circuito RC		|


