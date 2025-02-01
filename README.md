# Projeto: Controle de Matriz de LEDs WS2812 com Raspberry Pi Pico
### Autor
**Wendel S. Souza**  
## Descrição
Este projeto implementa um sistema de controle para uma matriz de LEDs 5x5 WS2812 utilizando a Raspberry Pi Pico. A matriz exibe números de 0 a 9 e é controlada por dois botões, permitindo incrementar ou decrementar o número exibido. Além disso, um LED RGB pode ser controlado, e um LED vermelho pisca continuamente a uma taxa de 5 vezes por segundo.

## Recursos
- Controle de uma matriz de LEDs 5x5 WS2812.
- Incremento e decremento do número exibido através de dois botões.
- Pisca-pisca do LED vermelho a uma frequência de 5 Hz.
- Configuração de cores para os LEDs da matriz.
- Uso do PIO para comunicação eficiente com os LEDs WS2812.

## Hardware Utilizado
- **Raspberry Pi Pico**
- **Matriz de LEDs WS2812** (5x5)
- **LED RGB** (pinos individuais para vermelho, verde e azul)
- **2 botões** (para incrementar e decrementar o número exibido)

## Pinos Utilizados
| Componente | Pino GPIO |
|------------|----------|
| LED Vermelho  | GPIO 13 |
| LED Verde    | GPIO 11 |
| LED Azul    | GPIO 12 |
| Botão A    | GPIO 5  |
| Botão B    | GPIO 6  |
| Matriz WS2812 | GPIO 7  |

## Instalação e Configuração
1. Clone este repositório:
   ```sh
   git clone https://github.com/seu-usuario/seu-repositorio.git
   ```
2. Configure o ambiente de desenvolvimento para a Raspberry Pi Pico:
   - Instale o **SDK do Pico**.
   - Configure o **CMake** e o **Toolchain para ARM**.
3. Compile e carregue o código na Raspberry Pi Pico:
   ```sh
   mkdir build
   cd build
   cmake ..
   make
   ```
4. Copie o arquivo gerado (`.uf2`) para a Pico conectada via USB.

## Uso
- Pressione o **botão A** para incrementar o número exibido na matriz.
- Pressione o **botão B** para decrementar o número exibido na matriz.
- O LED vermelho pisca automaticamente a uma taxa de 5 Hz.

## Simulação Wokwi
![image](https://github.com/user-attachments/assets/c344de92-076f-42ef-b22c-5dfec3c4d0f8)


## Video
https://youtu.be/dmD81KwU-cU

