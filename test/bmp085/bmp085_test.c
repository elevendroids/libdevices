#include <stdio.h>
#include "bus/i2c.h"
#include "sensor/bmp085.h"
#include "delay.h"

int main(int argc, char **argv)
{
	int bus, temp, pressure, version, id;
	I2cDevice sensor = { .address = 0x77 };
	
	if ((bus = I2c_Open(0)) < 0) {
		perror("Unable to open i2c device file");
		return 1;
	}
	sensor.bus = bus;

	id = Bmp085_ReadChipId(&sensor);
	version = Bmp085_ReadChipVersion(&sensor);
	printf("Chip Id: %d, Version: %d\n", id, version);
	
	temp = Bmp085_ReadTemperature(&sensor);
	pressure = Bmp085_ReadPressure(&sensor, BMP085_SAMPLES_8);
	
	printf("Temperature: %.1f C\n", temp / 10.0);
	printf("Pressure: %.2f hPa\n", pressure / 100.0);
	I2c_Close(bus);
	return 0;
}
