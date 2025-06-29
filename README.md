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

Logic behind powering motors is performed using MOSFET transistors and a logic converter(I will explain how it works in next paragraphs), and steering is performed by Servo mechanisms, which are connected with car motors and rotate them. Car is powered by six AA batteries(quite a lot I know), using three converters. First converter powers only ESP microcontroller, second powers up two motors and one Servo mechanism, and third converter powers another two motors and another Servo. 

![image](https://github.com/user-attachments/assets/463b956e-b5b9-4a05-afdf-45a5ed53b6c3)

## Architecture details
### Car forward and backward movement
Car gas is controlled by four ESP pins:

 - Two pins for forward movement(one for passing current into the motor, and one for passing ground into the motor)
 - Two pins for backward movement(same configuration as described above)
 
Signals coming from PINs are passed through logic level converter. They have to be lifted up to 5V, because 3.3V that ESP provides is not enough for MOSFET transistor that I bought to fully open up. MOSFET transistors are components of H-bridges, which I preferred to create on my own instead of buying ready-made ones. H-bridges were essential for proper passing of current to correct voltage poles. Each H-bridge consists of four transistors, two P-MOSFET for passing current and two N-MOSFET for passing ground. So, in total I have used 16 transistors for entire module.
I have also tried to use bipolar transistors, but ESP did not manage to pass enough current to them, so they stayed unstable.
### Steering
Steering is controlled by two Servo mechanisms. Steering is non-binary, you can specify angle of steer by using steering joystick carefully. Angle ranges from -45 degrees to 45 degrees. Servo mechanisms are glued(using super glue) to front motors. 
### Wireless communication
Communication between boards is performed via ESP NOW protocol. Remote control module reads data from joysticks, normalizes it and sends it to car module, so all calculations are performed on the "client-side". ESP NOW handles communication really easily, all I needed was MAC address of my ESP microcontroller that was responsible by controlling car.
## Obtained knowledge
Key skills that I learned during this project:
 - soldering
 - how transistors work
 - differences between transistor types
 - ESP NOW protocol
 - how electric motors work
 - how Servo mechanisms work
 - ESP programming
 - logic behind H-bridges

## Components that I used

 - ESP32(ESP-WROOM-32)
 - Arduino Nano ESP
 - some basic LED diodes
 - eight IRFZ44N N-MOSFET transistors
 - eight IRF9540 P-MOSFET transistors
 - Zhaoxin 936AH soldering station
 - four electric motors with wheels
 - two Iduino Thumb Joysticks
 - eight-channel, bi-directional logic level converter
 - four converters for powering up modules with AA batteries
 - cardboard boxes serving as frames for modules
 - a lot of cables
