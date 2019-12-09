# Descrição dos dispositivos

## Raspberry		
### IP(s)
* 192.168.31.197	na interface wlan0 
* 10.8.0.2 na interface tun0
### Funcionalidade(s)
* Executa o OpenPLC Master na porta 8080
* Executa o cliente VPN para acessar o supervisório desenvolvido no ScadaBR no AWS

## ESP32Atuador
### IP(s)
* 192.168.31.61
### Funcionalidade(s)
* Aplica tensão no circuito RC

## ESP32Sensor
### IP(s)
* 192.168.31.91
### Funcionalidade(s)
* Mede a tensão do capacitor no circuito RC

## Máquina virtual rodando no AWS
### IP(s)
* 10.8.0.1
### Funcionalidade(s)
* Executa o servidor VPN usando UDP na porta 1194
* Executa o sistema supervisório no ScadaBR usando a porta 9090

# Funcionamento
Inicialmente deve-se ligar o access-point que conecta os ESP32 e a Raspberry. Em seguida, deve-se ligar os ESPs e a Raspberry, inicializar o programa no OpenPLC, acessar o ScadaBR. O usuário com poderes de administrador pode alterar os seguintes valores do processo:
  * Ganhos do controlador
  * Setpoint
  * Ligar/Desligar controlador
  * Abrir/Fechar a malha
  * Sinal de controle (modo malha aberta apenas)
O usuáruo 'guest' (senha: 'guest'), pode apenas visualizar a interface gráfica do Scada. Os valores de setpoint podem variar de 0 a 100%.
