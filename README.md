# Remotely Controller Car
## Quick introduction
My goal in this project was to create my own remotely controller car. Idea of creating my own IOT/embedded project came into my mind a while ago.  I started my journey from learning ESP32 and electronics basics from sources I found on the internet. Then, I made a decision that a project I'm going to build will be a remotely controlled car. I bought a lot of parts on the go, mainly when I discovered that I approach I have taken was invalid or not optimal(happened a few times).
## Video presenting car movement
[https://youtu.be/bGT8Jp7CBA4](https://www.youtube.com/watch?v=bGT8Jp7CBA4)
## Architecture overview
Project consists of two main modules: 

 - Remote control
 - Car

### Remote Control
Remote control module contains Arduino Nano ESP32 microcontrollers connected with two thumb joysticks and diodes. The microcontroller reads data from joysticks and then, when change was detected, sends information about gas and steering. Sending is performed via ESP NOW protocol. Everything is connected together via breadboard, and powered by a converter that takes two AA batteries.

![-vr4cYT3](https://github.com/user-attachments/assets/fcdb0917-2525-4a23-b754-cc16fa2f7b63)

### Car
Car module is undoubtedly more complex. ESP32(ESP-WROOM-32) microcontroller is the brain of the module. It's responsible for reading data sent by remote control and powering up selected PINs to perform logic behind car movement. Car can perform three main operations: 

 - Drive forward
 - Drive backward
 - Steer(left or right)

Logic behind powering engines is performed using MOSFET transistors and a logic converter(I will explain how it works in next paragraphs), and steering is performed by Servo mechanisms, which are connected with car engines and rotate them. Car is powered by six AA batteries(quite a lot I know), using three converters. First converter powers only ESP microcontroller, second powers up two engines and one Servo mechanism, and third converter powers another two engines and another Servo. 

![image](https://github.com/user-attachments/assets/463b956e-b5b9-4a05-afdf-45a5ed53b6c3)

## Architecture details
### Car forward and backward movement
