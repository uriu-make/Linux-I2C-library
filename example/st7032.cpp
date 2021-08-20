/*
Program to display characters on LCD using ST7032
The display digits are set by #define SIZE.
When executing
$ ./st7032.o "string"
or
$ ./st7032.o "string1" "string2"
Type at the terminal.
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <i2clib.h>

// #define SIZE 8 //8x2
#define SIZE 16  //16x2

char DisplayON = 0x0c, ClearDisplay = 0x01, ReturnHome = 0x02;

void writeData(const char *t_data);
void writeDataReg(char DisplayAddress, const char *t_data);  // Write with register specified.
void writeCommand(char t_command);
void contrast_max();
void init_oled();

i2c st7032("/dev/i2c-1", 0x3c);

int main(int argc, char *argv[]) {
  if (st7032.Setup() < 0) return -1;
  contrast_max();
  init_oled();

  if (argc == 2) {
    char *s = argv[1];
    int size = strlen(s);

    if (size <= SIZE) {
      char str0[size];
      strcpy(str0, s);
      writeCommand(0x80 + 0x00);
      writeData(str0);
    } else {
      char str0[SIZE + 1];
      size = (size > SIZE * 2) ? SIZE * 2 : size;
      char str1[size - SIZE + 1];
      strncpy(str0, s, SIZE);
      strncpy(str1, s + SIZE, size - SIZE);
      str0[SIZE] = '\0';
      str1[size - SIZE] = '\0';
      writeCommand(0x80 + 0x00);
      writeData(str0);
      writeCommand(0x80 + 0x20);
      writeData(str1);
    }
  } else if (argc == 3) {
    char *str0 = argv[1];
    char *str1 = argv[2];
    writeCommand(0x80 + 0x00);
    writeData(str0);
    writeCommand(0x80 + 0x20);
    writeData(str1);
  }
  st7032.Close();
  return 0;
}

void contrast_max() {
  char data[] = {0x2A, 0x79, 0x81, 0xFF, 0x78, 0x28};
  for (int i = 0; i < 6; i++) writeCommand(data[i]);
}

void init_oled() {
  char data[] = {ClearDisplay, ReturnHome, DisplayON, ClearDisplay};
  for (int i = 0; i < 4; i++) {
    writeCommand(data[i]);
  }
}

void writeCommand(char t_command) {
  st7032.WriteReg(0x00, &t_command, 1);
}

void writeDataReg(char DisplayAddress, char *t_data) {
  writeCommand(0x80 + DisplayAddress);
  for (int i = 0; i < strlen(t_data); i++) {
    st7032.WriteReg(0x40, &t_data[i], 1);
  }
}

void writeData(const char *t_data) {
  for (int i = 0; i < strlen(t_data); i++) {
    st7032.WriteReg(0x40, &t_data[i], 1);
  }
}
