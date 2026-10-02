# Bomb Disposal Robot

A remotely operated 4-wheel robotic platform designed for **hazardous-object handling and remote inspection**. The robot combines a rugged mobile base, 6-DOF robotic arm, live ESP32-CAM video feed, obstacle detection, and a custom Android control application.

> **Project Type:** Robotics / Mechanical Engineering / Embedded Systems
> **Controller:** Arduino Mega 2560
> **Communication:** Bluetooth (HC-05)
> **Control Interface:** Custom Android application developed using Android Studio

---

## 🚀 Project Overview

The robot was designed as a compact remotely controlled platform capable of:

* Moving through different terrains using rugged wheels
* Remotely operating a 6-servo robotic arm
* Providing live visual feedback through an ESP32-CAM
* Detecting obstacles in front of the robot
* Automatically stopping when an obstacle is detected within approximately 20 cm
* Operating the mobile base and robotic arm through a single custom Android application

The system is powered by a **12 V, 7 Ah lead-acid battery** with separate regulated power supplies for the robotic arm and ESP32-CAM.

---

## ⚙️ Hardware

### Mobile Platform

| Component                | Specification                           |
| ------------------------ | --------------------------------------- |
| Controller               | Arduino Mega 2560                       |
| Drive Motors             | 4 × 12 V Johnson 1000 RPM geared motors |
| Motor Drivers            | 2 × BTS motor drivers                   |
| Wheels                   | Rugged wheels                           |
| Main Battery             | 12 V 7 Ah Lead-Acid                     |
| Communication            | HC-05 Bluetooth module                  |
| Obstacle Sensor          | Ultrasonic sensor                       |
| Obstacle Detection Range | ~20 cm                                  |

### Robotic Arm

The robot carries a **6-servo robotic arm** mounted on the top of the mobile platform.

* 6 × MG-series servo motors
* Separate 6 V buck converter for servo power
* Arm controlled through the same Bluetooth connection
* Commands sent from the custom Android application

### Vision System

An **ESP32-CAM** is mounted behind the robotic arm to provide a live camera feed for remote operation and visual inspection.

* ESP32-CAM
* Dedicated 5 V buck converter
* Live video feed

---

## 🔌 Power System

The robot uses a **12 V, 7 Ah lead-acid battery** as its main power source.

The power system uses separate buck converters:

```text
12 V 7 Ah Lead-Acid Battery
             │
             ├──► BTS Motor Drivers ──► 4 × 12 V Motors
             │
             ├──► 6 V Buck Converter ──► 6 × MG Servos
             │
             └──► 5 V Buck Converter ──► ESP32-CAM
```

Separating the servo and camera power supplies helps provide appropriate operating voltages for the different subsystems.

---

## 📱 Android Control System

A custom Android application was developed using **Android Studio**.

The application communicates with the Arduino Mega through an **HC-05 Bluetooth module**.

The same application provides control of:

* Forward / reverse movement
* Left / right movement
* Robotic arm movements
* Servo-controlled arm functions

This allows the operator to control the mobile platform and robotic arm from a single interface.

---


## 🛡️ Obstacle Detection

An ultrasonic sensor is positioned at the front of the robot.

When an obstacle is detected within approximately **20 cm**, the robot is programmed to stop the drive system. This provides an additional safety layer during remote operation.

---

## 🛠️ Key Engineering Features

* 4-wheel high-speed drive system
* 4 × 12 V Johnson 1000 RPM motors
* Dual BTS motor-driver configuration
* Arduino Mega-based control
* Bluetooth wireless operation
* Custom Android control application
* 6-servo robotic arm
* ESP32-CAM live video system
* Front ultrasonic obstacle detection
* Separate regulated power rails for servos and camera
* 12 V 7 Ah portable power system

---


## 🔮 Future Improvements

Possible future development includes:

* Improved camera positioning
* Higher-resolution video streaming
* Better obstacle detection
* Wireless communication with greater range
* Improved arm payload and precision
* Additional sensors for remote inspection
* Improved chassis protection for rough environments
* More advanced operator controls

---

## 👨‍💻 My Contribution

### Mechanical Engineering

* Designed and assembled the mobile robotic platform
* Integrated the robotic arm with the mobile base
* Selected and integrated the drive motors and rugged wheels
* Worked on mechanical integration of the robotic subsystems

### Electronics & Embedded Systems

* Integrated Arduino Mega, BTS motor drivers and HC-05 Bluetooth communication
* Implemented motor and servo control
* Integrated ultrasonic obstacle detection
* Designed the power distribution using separate buck converters

### Software

* Worked with the Arduino control system
* Developed/integrated the Bluetooth command system
* Developed a custom Android control application using Android Studio
* Integrated ESP32-CAM for live visual feedback

---

## 📚 Technologies Used

**Hardware**

* Arduino Mega 2560
* HC-05 Bluetooth
* BTS motor drivers
* ESP32-CAM
* Ultrasonic sensor
* MG-series servo motors
* 12 V Johnson motors
* Buck converters
* 12 V 7 Ah lead-acid battery

**Software**

* Arduino IDE
* Android Studio
* Arduino/C++
* Android

---

## ⚠️ Disclaimer

This repository documents an **educational robotics prototype** developed for engineering and remote-operation applications. It is not a certified explosive ordnance disposal system and should not be used for handling real explosives or hazardous devices without appropriate professional equipment, training, and safety procedures.

---

## 📄 License

This project is intended primarily for educational and portfolio purposes.

See the repository license for terms of reuse.
