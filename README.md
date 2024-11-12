# Custom Proc-Based Linux Device Driver

## Overview
This project implements a Linux kernel module that creates a `/proc/my_driver` entry with read and write capabilities. This module demonstrates basic user-kernel communication by allowing messages to be sent from user space to kernel space and back.


## Features
- **Read and Write Operations**: Interact with the kernel via the proc filesystem using standard read and write system calls.
- **User-Space Application**: Example Python script to send and receive messages from the kernel.


## Project Structure
- **ldd.c**: Kernel module code for creating a proc file with read and write capabilities.
- **user-app.py**: User-space application to communicate with the kernel module by writing to and reading from `/proc/my_driver`.
- **Makefile**: Build file to compile and load the kernel module.


## Instructions
1. Compile and load the module:
    ```bash
    make
    sudo insmod ldd.ko
    ```
2. Run the user application:
    ```bash
    python3 user-app.py
    ```
3. Check `dmesg` for kernel logs:
    ```bash
    dmesg | tail
    ```
4. Unload the module when done:
    ```bash
    sudo rmmod ldd
    ```


## Output
Sent to kernel space: Hello from user space!

Received from kernel space: Hello from user space!


## Output with dmesg
my_init: Entry
my_init: Exit
my_write
Data received from user space: Hello from user space!

my_read
my_exit: Entry
my_exit: Exit 
