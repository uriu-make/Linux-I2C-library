#include <stdio.h>
#include <wire.h>

wire bme280(I2C1, 0x77);

unsigned int hum_raw, temp_raw, pres_raw;
int t_fine;

unsigned int dig_T1;
int dig_T2;
int dig_T3;
unsigned int dig_P1;
int dig_P2;
int dig_P3;
int dig_P4;
int dig_P5;
int dig_P6;
int dig_P7;
int dig_P8;
int dig_P9;
char dig_H1;
int dig_H2;
char dig_H3;
int dig_H4;
int dig_H5;
char dig_H6;

void readTrim();
void readData();
int calibration_T(int adc_T);
unsigned int calibration_P(int adc_P);
unsigned int calibration_H(int adc_H);

int main(void) {
  unsigned char osrs_t = 1;   //Temperature oversampling x 1
  unsigned char osrs_p = 1;   //Pressure oversampling x 1
  unsigned char osrs_h = 1;   //Humidity oversampling x 1
  unsigned char mode = 3;     //Normal mode
  unsigned char t_sb = 5;     //Tstandby 1000ms
  unsigned char filter = 2;   //Filter off
  unsigned char spi3w_en = 0; //3-wire SPI Disable

  unsigned char ctrl_meas_reg = (osrs_t << 5) | (osrs_p << 2) | mode;
  unsigned char config_reg = (t_sb << 5) | (filter << 2) | spi3w_en;
  unsigned char ctrl_hum_reg = osrs_h;

  double temp_act = 0.0, press_act = 0.0, hum_act = 0.0;
  int temp_cal;
  unsigned int press_cal, hum_cal;

  if (bme280.Setup() == 1) {
    bme280.Close();
    return -1;
  }

  bme280.WriteReg(0xF2, &ctrl_hum_reg);
  bme280.WriteReg(0xF4, &ctrl_meas_reg);
  bme280.WriteReg(0xF5, &config_reg);
  readTrim();

  while (1) {
    sleep(1);
    readData();

    temp_cal = calibration_T(temp_raw);
    press_cal = calibration_P(pres_raw);
    hum_cal = calibration_H(hum_raw);
    temp_act = (double)temp_cal / 100.0;
    press_act = (double)press_cal / 100.0;
    hum_act = (double)hum_cal / 1024.0;
    printf("TEMP :%g°C,PRESS :%ghPa  HUM :%g%%\n", temp_act, press_act, hum_act);
  }
  bme280.Close();
  return 0;
}

void readTrim() {
  unsigned char data[32], i = 0;
  bme280.Write(0x88);
  bme280.Read(&data[i], 24);
  i += 24;
  bme280.Write(0xA1);
  bme280.Read(&data[i]);
  i += 1;
  bme280.Write(0xE1);
  bme280.Read(&data[i], 7);
  i += 7;

  dig_T1 = (data[1] << 8) | data[0];
  dig_T2 = (data[3] << 8) | data[2];
  dig_T3 = (data[5] << 8) | data[4];
  dig_P1 = (data[7] << 8) | data[6];
  dig_P2 = (data[9] << 8) | data[8];
  dig_P3 = (data[11] << 8) | data[10];
  dig_P4 = (data[13] << 8) | data[12];
  dig_P5 = (data[15] << 8) | data[14];
  dig_P6 = (data[17] << 8) | data[16];
  dig_P7 = (data[19] << 8) | data[18];
  dig_P8 = (data[21] << 8) | data[20];
  dig_P9 = (data[23] << 8) | data[22];
  dig_H1 = data[24];
  dig_H2 = (data[26] << 8) | data[25];
  dig_H3 = data[27];
  dig_H4 = (data[28] << 4) | (0x0F & data[29]);
  dig_H5 = (data[30] << 4) | ((data[29] >> 4) & 0x0F);
  dig_H6 = data[31];
}

void readData() {
  int i = 0;
  unsigned char data[8];
  bme280.ReadReg(0xF7, &data, 8, true);
  for (int i = 0; i < 8; i++) {
    data[i] = data[i] & 0xFF;
  }
  pres_raw = data[0];
  pres_raw = (pres_raw << 8) | data[1];
  pres_raw = (pres_raw << 4) | (data[2] >> 4);

  temp_raw = data[3];
  temp_raw = (temp_raw << 8) | data[4];
  temp_raw = (temp_raw << 4) | (data[5] >> 4);

  hum_raw = data[6];
  hum_raw = (hum_raw << 8) | data[7];
}

int calibration_T(int adc_T) {
  int var1, var2, T;
  var1 = ((((adc_T >> 3) - ((int)dig_T1 << 1))) * ((int)dig_T2)) >> 11;
  var2 = (((((adc_T >> 4) - ((int)dig_T1)) * ((adc_T >> 4) - ((int)dig_T1))) >> 12) * ((int)dig_T3)) >> 14;

  t_fine = var1 + var2;
  T = (t_fine * 5 + 128) >> 8;
  return T;
}

unsigned int calibration_P(int adc_P) {
  int var1, var2;
  unsigned int P;
  var1 = (((int)t_fine) >> 1) - (int)64000;
  var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * ((int)dig_P6);
  var2 = var2 + ((var1 * ((int)dig_P5)) << 1);
  var2 = (var2 >> 2) + (((int)dig_P4) << 16);
  var1 = (((dig_P3 * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3) + ((((int)dig_P2) * var1) >> 1)) >> 18;
  var1 = ((((32768 + var1)) * ((int)dig_P1)) >> 15);
  if (var1 == 0) {
    return 0;
  }
  P = (((unsigned int)(((int)1048576) - adc_P) - (var2 >> 12))) * 3125;
  if (P < 0x80000000) {
    P = (P << 1) / ((unsigned int)var1);
  } else {
    P = (P / (unsigned int)var1) * 2;
  }
  var1 = (((int)dig_P9) * ((int)(((P >> 3) * (P >> 3)) >> 13))) >> 12;
  var2 = (((int)(P >> 2)) * ((int)dig_P8)) >> 13;
  P = (unsigned int)((int)P + ((var1 + var2 + dig_P7) >> 4));
  return P;
}

unsigned int calibration_H(int adc_H) {
  int v_x1;

  v_x1 = (t_fine - ((int)76800));
  v_x1 = (((((adc_H << 14) - (((int)dig_H4) << 20) - (((int)dig_H5) * v_x1)) + ((int)16384)) >> 15) *
          (((((((v_x1 * ((int)dig_H6)) >> 10) * (((v_x1 * ((int)dig_H3)) >> 11) + ((int)32768))) >> 10) +
             ((int)2097152)) *
                ((int)dig_H2) +
            8192) >>
           14));
  v_x1 = (v_x1 - (((((v_x1 >> 15) * (v_x1 >> 15)) >> 7) * ((int)dig_H1)) >> 4));
  v_x1 = (v_x1 < 0 ? 0 : v_x1);
  v_x1 = (v_x1 > 419430400 ? 419430400 : v_x1);
  return (unsigned int)(v_x1 >> 12);
}