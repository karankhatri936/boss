#include <stdio.h>
#include <string.h>
#include <windows.h>

int main() {
    HANDLE hSerial;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};

    char command[100];

    // Step 1: Read command from file
    FILE *fp = fopen("command.txt", "r");
    if (fp == NULL) {
        printf("Error: Cannot open command file.\n");
        return 1;
    }

    fgets(command, sizeof(command), fp);
    fclose(fp);

    printf("Command received: %s\n", command);

    // Step 2: Open COM Port (change COM3 if needed)
    hSerial = CreateFile("COM3",
                         GENERIC_READ | GENERIC_WRITE,
                         0,
                         NULL,
                         OPEN_EXISTING,
                         0,
                         NULL);

    if (hSerial == INVALID_HANDLE_VALUE) {
        printf("Error opening serial port\n");
        return 1;
    }

    // Step 3: Configure Serial Port
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) {
        printf("Error getting state\n");
        CloseHandle(hSerial);
        return 1;
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity   = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        printf("Error setting serial parameters\n");
        CloseHandle(hSerial);
        return 1;
    }

    // Step 4: Set Timeouts
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    SetCommTimeouts(hSerial, &timeouts);

    // Step 5: Decide command and send to Arduino
    char *dataToSend;


    if (strstr(command, "light on") || strstr(command, "on the light")) {
        dataToSend = "LED_ON\n";
        printf("Action: Turning ON light\n");
    }
    else if (strstr(command, "light off") || strstr(command, "off the light")) {
        dataToSend = "LED_OFF\n";
        printf("Action: Turning OFF light\n");
    }
    else if (strstr(command, "fan on") || strstr(command, "on the fan")) {
        dataToSend = "FAN_ON\n";
        printf("Action: Turning ON fan\n");
    }
    else if (strstr(command, "fan off") || strstr(command, "off the fan")) {
        dataToSend = "FAN_OFF\n";
        printf("Action: Turning OFF fan\n");
    }
    else {
        dataToSend = "UNKNOWN\n";
        printf("Unknown command\n");
    }

    // Step 6: Send data
    DWORD bytesWritten;
    WriteFile(hSerial, dataToSend, strlen(dataToSend), &bytesWritten, NULL);

    printf("Sent to Arduino: %s\n", dataToSend);

    // Step 7: Close port
    CloseHandle(hSerial);

    return 0;
}
