	component unsaved is
		port (
			accelerometer_spi_external_interface_I2C_SDAT      : inout std_logic                     := 'X';             -- I2C_SDAT
			accelerometer_spi_external_interface_I2C_SCLK      : out   std_logic;                                        -- I2C_SCLK
			accelerometer_spi_external_interface_G_SENSOR_CS_N : out   std_logic;                                        -- G_SENSOR_CS_N
			accelerometer_spi_external_interface_G_SENSOR_INT  : in    std_logic                     := 'X';             -- G_SENSOR_INT
			clk_clk                                            : in    std_logic                     := 'X';             -- clk
			in_h_external_connection_export                    : in    std_logic_vector(31 downto 0) := (others => 'X'); -- export
			in_l_external_connection_export                    : in    std_logic_vector(31 downto 0) := (others => 'X'); -- export
			led_external_connection_export                     : out   std_logic_vector(9 downto 0);                     -- export
			out0_external_connection_export                    : out   std_logic_vector(31 downto 0);                    -- export
			out1_external_connection_export                    : out   std_logic_vector(31 downto 0);                    -- export
			reset_reset_n                                      : in    std_logic                     := 'X';             -- reset_n
			sample_clk_external_connection_export              : out   std_logic                                         -- export
		);
	end component unsaved;

	u0 : component unsaved
		port map (
			accelerometer_spi_external_interface_I2C_SDAT      => CONNECTED_TO_accelerometer_spi_external_interface_I2C_SDAT,      -- accelerometer_spi_external_interface.I2C_SDAT
			accelerometer_spi_external_interface_I2C_SCLK      => CONNECTED_TO_accelerometer_spi_external_interface_I2C_SCLK,      --                                     .I2C_SCLK
			accelerometer_spi_external_interface_G_SENSOR_CS_N => CONNECTED_TO_accelerometer_spi_external_interface_G_SENSOR_CS_N, --                                     .G_SENSOR_CS_N
			accelerometer_spi_external_interface_G_SENSOR_INT  => CONNECTED_TO_accelerometer_spi_external_interface_G_SENSOR_INT,  --                                     .G_SENSOR_INT
			clk_clk                                            => CONNECTED_TO_clk_clk,                                            --                                  clk.clk
			in_h_external_connection_export                    => CONNECTED_TO_in_h_external_connection_export,                    --             in_h_external_connection.export
			in_l_external_connection_export                    => CONNECTED_TO_in_l_external_connection_export,                    --             in_l_external_connection.export
			led_external_connection_export                     => CONNECTED_TO_led_external_connection_export,                     --              led_external_connection.export
			out0_external_connection_export                    => CONNECTED_TO_out0_external_connection_export,                    --             out0_external_connection.export
			out1_external_connection_export                    => CONNECTED_TO_out1_external_connection_export,                    --             out1_external_connection.export
			reset_reset_n                                      => CONNECTED_TO_reset_reset_n,                                      --                                reset.reset_n
			sample_clk_external_connection_export              => CONNECTED_TO_sample_clk_external_connection_export               --       sample_clk_external_connection.export
		);

