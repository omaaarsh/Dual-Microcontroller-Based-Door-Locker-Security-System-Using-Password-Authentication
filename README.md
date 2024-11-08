# Dual-Microcontroller-Based-Door-Locker-Security-System-Using-Password-Authentication
Designed a smart door locking system for secure access control. The HMI_ECU handles user interaction through an LCD and keypad for password input, while the Control_ECU manages password verification and door operation

Dual Microcontroller-Based Door Locker Security System Using Password Authentication
Objective: Developed a secure, smart door control system using two microcontrollers, HMI_ECU and Control_ECU, which communicate via UART. The system features password authentication with data stored in external EEPROM, along with enhanced functionality through components like a PIR sensor, H-bridge motor control, and a buzzer.

Project Overview:
Designed a smart door locking system for secure access control. The HMI_ECU handles user interaction through an LCD and keypad for password input, while the Control_ECU manages password verification and door operation. Key features include motor-driven door control, motion detection, alarm activation on unauthorized access attempts, and password management options. This project demonstrates a layered model architecture for embedded systems with modular drivers for GPIO, UART, I2C, LCD, Keypad, and PWM.

Key Features:

	•	Password Protection: Passwords are verified and stored securely in external EEPROM.
	•	User Interface: LCD and keypad allow easy password entry and management.
	•	Communication: UART facilitates data transfer between HMI_ECU and Control_ECU.
	•	Security Features: Buzzer alerts on unauthorized access attempts, and the system locks temporarily after multiple failed attempts.
	•	Motion Detection: PIR sensor controls door operation based on detected motion.
	•	Motorized Door Control: H-bridge controls motor movement for locking/unlocking.
	•	Password Change and Security Lock: Supports password update and locks after incorrect attempts.

Hardware and Drivers:

	•	Microcontroller: ATmega32 at 8 MHz.
	•	Components: LCD, keypad, PIR sensor, motor via H-bridge, EEPROM, buzzer.
	•	Drivers: Custom drivers for GPIO, UART, I2C, LCD, keypad, and PWM as per course standards.

Technical Challenges:

	•	Achieved efficient and secure data handling through external EEPROM.
	•	Integrated motion-controlled door locking for automated access.
	•	Developed robust password protection, enhancing overall security.

Outcome: Successfully implemented a functional and secure smart door system with a comprehensive user interface, reliable security features, and real-time access control based on password authentication and motion detection.
