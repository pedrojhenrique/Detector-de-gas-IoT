# Detector-de-gas-IoT
Meu projeto técnico de TCC realizado na Escola SENAI "Mariano Ferraz".
O projeto foi desenvolvido em C++ utilizando a IDE Arduino.

##

# Introdução
A função do projeto é apresentar um dispositivo de detecção e monitoramento de gases inflamáveis e fumaça, com um alerta sonoro emitido por um buzzer, notificações através do aplicativo Blynk e também por e-mail, dessa forma, obtendo um monitoramento remoto que facilita 
o controle do local, aumentando a segurança de locais e pessoas. 
Com um display LCD em conjunto com o microcontrolador ESP32, é permitida uma leitura 
rápida apenas por observar em sua tela a quantidade de gás no local em ppm. 
Além dos benefícios de segurança proporcionados pelo detector de gás, também é possível verificar os níveis de leituras anteriores através do aplicativo Blynk, a fim de obter-se um maior controle em relação a aumentos de vazamentos.

## <br> Escopo
O projeto tem por sua finalidade detectar fumaça ou vazamento de gases - a partir de seu sensor integrado -, por meio da utilização do aplicativo Blynk, o qual viabiliza que o usuário seja informado via internet. 

ESP32: A ESP32 é um microcontrolador que possui conectividade bluetooth e WiFi já integrados à placa.

![image](https://user-images.githubusercontent.com/105826347/169413742-62352147-40b7-431a-98d4-c0218ceed732.png)


<br> Blynk: aplicativo disponível para Android e iOS que permite ao usuário integrar e monitorar programações, foi criado para ser utilizado em projetos Iot (Internet of Things ou Internet das coisas).

![image](https://user-images.githubusercontent.com/105826347/169413326-3814f08c-3efd-4d5b-9c5b-536f8c944c50.png)

<br> A aplicação deste detector pode ser tanto em ambientes residenciais, quanto em industriais.

![image](https://user-images.githubusercontent.com/105826347/169414280-44edf638-332f-47e3-ba21-f8aa1a883729.png)

##

# <br> Fluxograma
Fluxograma que exibe as etapas de detecção e alertas do projeto.

![image](https://user-images.githubusercontent.com/105826347/169414573-47b9827e-1df6-4d32-8e47-6ae999442e3d.png) 

# <br> Breve vídeo demonstrativo
O sistema considera a partir de 600 ppm (parte por milhão) como "nível alto" de presença de gás, porém, este valor foi determinado através da programação apenas como parâmetros de testes, podendo ter seu valor alterado a depender do ambiente que for utilizado.  

https://user-images.githubusercontent.com/105826347/172036339-aacd3d7a-577e-4ebf-a699-4e93dad53002.mp4






