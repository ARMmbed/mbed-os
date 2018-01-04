#include "io.h"

AnalogInManager analogInManager(8, MUX_0, MUX_1, MUX_2, SPI_MISO, SPI_SCK, SPI_CS0);
AnalogOutManager analogOutManager(SPI_MOSI, SPI_SCK, SPI_CS1);
OneWire oneWire(ONEWIRE_Rx, ONEWIRE_Tx, ONEWIRE_TxH);

RoutingmaxIO routingmax_io = {
	.DINs = {
		SensorDigitalIn(In1), 
		SensorDigitalIn(In2),
		SensorDigitalIn(In3),
		SensorDigitalIn(In4),
		SensorDigitalIn(In5),
		SensorDigitalIn(In6),
		SensorDigitalIn(In7),
		SensorDigitalIn(In8),
		SensorDigitalIn(In9),
		SensorDigitalIn(In10)
	},
		
	.AINs = {
		SensorCurrentLoopIn(&analogInManager, 0),
		SensorCurrentLoopIn(&analogInManager, 1),
		SensorCurrentLoopIn(&analogInManager, 2),
		SensorCurrentLoopIn(&analogInManager, 3),
		SensorCurrentLoopIn(&analogInManager, 4),
		SensorCurrentLoopIn(&analogInManager, 5)
	},
	
	.PTs = {
		SensorPlatinumTemperature(&analogInManager, 6, SensorPlatinumTemperature::PT1000),
		SensorPlatinumTemperature(&analogInManager, 7, SensorPlatinumTemperature::PT100)
	},
	
	.OneWireDS18B20 = DS18B20(&oneWire),
	
	.DOUTs = {
		ActorDigitalOut(Out1),
		ActorDigitalOut(Out2),
		ActorDigitalOut(Out3)
	},
	
	.AOUTs = {
		ActorCurrentLoopOut(&analogOutManager, 0),
		ActorCurrentLoopOut(&analogOutManager, 1)
	}
	
};

uint8_t board_hw_version = 
	((uint8_t) DigitalIn(Version16) << 4) |
	((uint8_t) DigitalIn(Version8) << 3) |
	((uint8_t) DigitalIn(Version4) << 2) |
	((uint8_t) DigitalIn(Version2) << 1) |
	((uint8_t) DigitalIn(Version1));