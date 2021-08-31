#include<stdio.h>
#include <windows.h>

int main() {
  // Open serial port
  HANDLE serialHandle;

  serialHandle = CreateFile("\\\\.\\COM4", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
  
  // Do some basic settings
  DCB serialParams = { 0 };
  serialParams.DCBlength = sizeof(serialParams);

  GetCommState(serialHandle, &serialParams);
  serialParams.BaudRate = 115200;
  serialParams.ByteSize = 8;
  serialParams.StopBits = TWOSTOPBITS;
  serialParams.Parity = NOPARITY;
  SetCommState(serialHandle, &serialParams);

  // Set timeouts
  COMMTIMEOUTS timeout = { 0 };
  timeout.ReadIntervalTimeout = 50;
  timeout.ReadTotalTimeoutConstant = 50;
  timeout.ReadTotalTimeoutMultiplier = 50;
  timeout.WriteTotalTimeoutConstant = 50;
  timeout.WriteTotalTimeoutMultiplier = 10;

  SetCommTimeouts(serialHandle, &timeout);

  char buffer[8 + 1] = {0};
  char oldBuffer[8 + 1] = {0};
  DWORD dwBytesRead = 0;

  while(1)
  { 
    // Reading lines
    ReadFile(serialHandle, buffer, 8, &dwBytesRead, NULL);

    // Reading caracters
    for (size_t i = 0; i < sizeof(buffer); i++)
    { 
      INPUT inputs;
      ZeroMemory(&inputs, sizeof(inputs));

      // Filtering unwanted data
      if (buffer[i] == '0' || buffer[i] == 'G' || buffer[i] == 'R' || buffer[i] == 'Y' || buffer[i] == 'B' || buffer[i] == 'O' || buffer[i] == 'U' || buffer[i] == 'D')
      {
        // Press keyboard key
        if (buffer[i] != '0' && oldBuffer[i] == '0')
        {
          inputs.type = INPUT_KEYBOARD;
          inputs.ki.wScan = 0;
          inputs.ki.wVk = buffer[i];
          inputs.ki.dwFlags = 0;
          inputs.ki.time = 0;
          inputs.ki.dwExtraInfo = 0;
          SendInput(1, &inputs, sizeof(INPUT));
          printf("press : %c\n", buffer[i]);
        }

        // Release keyboard key
        if (buffer[i] == '0' && oldBuffer[i] != '0')
        {
          inputs.type = INPUT_KEYBOARD;
          inputs.ki.wScan = 0;
          inputs.ki.wVk = oldBuffer[i];
          inputs.ki.dwFlags = KEYEVENTF_KEYUP;
          inputs.ki.time = 0;
          inputs.ki.dwExtraInfo = 0;
          SendInput(1, &inputs, sizeof(INPUT));
          printf("release : %c\n", inputs.ki.wVk);
        }
      }
    }

    // Set current buffer in old buffer
    for (size_t i = 0; i < sizeof(buffer); i++)
    {
      oldBuffer[i] = buffer[i];
    }
  }
}
