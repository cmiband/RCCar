# Remotely Controller Car
## Quick introduction
My goal in this project was to create my own remotely controller car. Idea of creating my own IOT/embedded project came into my mind a while ago.  I started my journey from learning ESP32 and electronics basisc from sources I found on the internet. Then, I made a decision that a project I'm going to build will be a remotely controlled car. I bought a lot of parts on the go, mainly when I discovered that I approach I have taken was invalid or not optimal(happened a few times).
## Architecture
Project consits of two main modules: 

 - Remote control
 - Card

Remote control module contains Arduino Nano ESP32 microcontrollers connected with two thumb joysticks and diodes. The microcontroller reads data from joysticks and then, when change was detected, sends information about gas and steering. Sending is performed via ESP NOW protocol. Everything is connected together via breadboard, and powered by a coverter that takes two AA batteries.



