def main():
    # Write to the driver
    with open('/proc/my_driver', 'w') as driver_handle:
        message_to_kernel = "Hello from user space!\n"
        driver_handle.write(message_to_kernel)
        print(f"Sent to kernel space: {message_to_kernel}")

    # Read from the driver
    with open('/proc/my_driver', 'r') as driver_handle:
        message_from_kernel_space = driver_handle.readline()
        print(f"Received from kernel space: {message_from_kernel_space}")

if __name__ == "__main__":
    main()
