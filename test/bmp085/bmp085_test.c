#include <stdio.h>
#include "bus/i2c.h"
#include "sensor/bmp085.h"
#include "delay.h"

int main(int argc, char **argv)
{
	int bus, temp, pressure, version, id;
	bmp085_device sensor;
	
	if ((bus = i2c_open(0)) < 0) {
		perror("Unable to open i2c device file");
		return 1;
	}

	if (bmp085_init(bus, &sensor) < 0) {
		perror("Failed to initialize sensor");
	}
	
	id = bmp085_read_chip_id(&sensor);
	version = bmp085_read_chip_version(&sensor);
	printf("Chip Id: %d, Version: %d\n", id, version);
	
	temp = bmp085_read_temperature(&sensor);
	pressure = bmp085_read_pressure(&sensor);
	
	printf("Temperature: %.1f C\n", temp / 10.0);
	printf("Pressure: %.2f hPa\n", pressure / 100.0);
	i2c_close(bus);
	return 0;
}
