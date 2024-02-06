
# DSA_Assignment_06

A basic command-line application for a user management system with features such as user registration, login, profile management, points transactions, and admin functionalities.

This is the homework intended to submit per the requirement of the course Diploma in Data Structures and Algorithms namely.

All the user data is write back to a file accordingly so the data can be persistent. There is no need to recreate the account after restarting the program.


## Features

- User and Admin Structure:
The program defines a User structure to store user information, including ID, name, email, password, ASM (Assessment) points, and a transaction message.
An Admin account is created at the beginning of the program with fixed IDs and names.

- File Handling:
User data is stored in a binary file (db.bin), and functions readUserFromFile and writeUserToFile are used to read and write user data to/from the file.
File paths are defined using a constant (filePath), and file handling operations are performed with FILE pointers.

- Menu System:
The program features a menu system with options for sign-in, sign-up, user profile, giving ASM points, deleting users, searching users, and logging out or exiting.
Separate menus are provided for regular users and admin users.

- User Authentication:
The program implements a simple email/password authentication system for user sign-in.
Password strength is checked during registration using the isStrongPasswd function.
Email validation is performed using the isValidEmail function.

- Points Transaction System:
Users can give ASM points to other users, and the transactions are recorded in the user's transaction message.
The ptsProcess and ptsGive functions handle the point-giving process.

- Online User Tracking:
An array (onlineUserIDs) is used to track online users, allowing the program to manage user sessions.

- Initialization:
The createAdminAcc function initializes three admin accounts during program startup.

- Code Organization:
Functions are organized to handle specific tasks, improving code readability and maintainability.
Some code sections are commented for clarity.

- Error Handling:
Error messages are displayed when file operations encounter issues.
Some input validation is implemented to ensure data integrity.

- Code Structure:
The program structure includes a continuous loop in the main function for the start menu, creating an infinite loop until the program is exited.

- Code Quality:
The code includes comments and uses meaningful function and variable names, contributing to its readability.


## 🚀 About Me
I'm a full fledge hobbyist seeking knowledge and experience 
to become a so called Maker... 


## License

MIT License

Copyright (c) 2024 Pyisoe-Thame

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

