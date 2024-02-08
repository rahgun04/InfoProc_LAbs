
module unsaved (
	accelerometer_spi_external_interface_I2C_SDAT,
	accelerometer_spi_external_interface_I2C_SCLK,
	accelerometer_spi_external_interface_G_SENSOR_CS_N,
	accelerometer_spi_external_interface_G_SENSOR_INT,
	clk_clk,
	in_h_external_connection_export,
	in_l_external_connection_export,
	led_external_connection_export,
	out0_external_connection_export,
	out1_external_connection_export,
	reset_reset_n,
	sample_clk_external_connection_export);	

	inout		accelerometer_spi_external_interface_I2C_SDAT;
	output		accelerometer_spi_external_interface_I2C_SCLK;
	output		accelerometer_spi_external_interface_G_SENSOR_CS_N;
	input		accelerometer_spi_external_interface_G_SENSOR_INT;
	input		clk_clk;
	input	[31:0]	in_h_external_connection_export;
	input	[31:0]	in_l_external_connection_export;
	output	[9:0]	led_external_connection_export;
	output	[31:0]	out0_external_connection_export;
	output	[31:0]	out1_external_connection_export;
	input		reset_reset_n;
	output		sample_clk_external_connection_export;
endmodule
