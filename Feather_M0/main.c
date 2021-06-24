#include <atmel_start.h>
#include <stdio.h>

/* Author : Gert van Biljon
 * Email : gertvb@gmail.com
 * Date created : 2017/06/28
 * Latest Modification : 2017/12/04
 *
 * What am I : An Atmel Studio 7 example project for using the ADAFruit Feather M0 with Start.Atmel (ASF4), using the USB as serial/com port, writing ADC values to something like putty
 *
 * ASF4 API Reference (As used by Atmel Start) :
 *
 * Downloadable PDF : http://ww1.microchip.com/downloads/en/DeviceDoc/50002633A.pdf
 *
 * Online :http://atmel-studio-doc.s3-website-us-east-1.amazonaws.com/webhelp/GUID-2A8AADED-413E-4021-AF0C-D99E61B8160D-en-US-1/index.html?GUID-819376C9-5C19-4F1E-9320-ED29CF2A0627
 *
 *
 * 1. Install the ADAFruit USB Driver for Windows
 * ==============================================
 *
 * When the Feather M0 is running the bootloader (after double pressing the reset button), the Feather M0 USB is started as an "ADAFruit Feather M0 device", and Windows needs to know
 * what drivers to use to communicate with the Feather M0 when it is running the bootloader.
 *
 * Follow the instructions at : https://learn.adafruit.com/adafruit-arduino-ide-setup/windows-setup and install the Windows drivers for the Adafruit USB device
 *
 *
 * 2. Ofsetting the start address of your program
 * ==============================================
 *
 * You need to cater for the ADAFruit bootloader on your Feather M0 occupying the start of memory on your Feather M0 and Atmel Studio building your program for 
 * the same address, by moving the start of your program to a higher address in memory than at 0, so that your program and the ADAFruit bootloader can coexist peacefully
 * next to each other in memory.
 *
 * Solution Explorer -> Right Click on your project (Feather M0 in BOLD) -> Properties (right at the bottom) -> Toolchain -> Arm/Gnu linker -> Miscellaneous -> Linker Flags 
 * modify from : -Tsamd21g18a_flash.ld to: -Wl,--section-start=.text=0x2000 -Tsamd21g18a_flash.ld
 *
 * Further info : https://reprapdad.wordpress.com/2016/08/19/adafruit-feather-m0-with-atmel-studio/
 *
 *
 * 3. Installing the Bossac command line programmer
 * ================================================
 *
 * Bossac is needed to write your compiled program to your Feather M0, it will download your program to the Feather M0 when the Feather M0 is
 * running the bootloader.  The bootloader is started on the Feather M0 by double-pressing the reset button on the Feather M0
 *
 * download : bossac-1.7.0-mingw32.tar.gz from : https://github.com/shumatech/BOSSA/releases
 * extract bossac.exe to :  C:\Program Files (x86)\Bossac
 *
 * To create an entry for bossa in the Atmel Studio Tools menu, follow the instructions at : http://www.elecrom.com/program-arduino-due-atmel-studio/
 * Note : do not add the line "mode %1......" to your batch file, it is for the dedicated serial port available on the Arduino Due
 *
 * Before you download the program to your Feather M0, you have to start the bootloader on your Feather M0 by double pressing the reset switch on the Feather M0!!
 *
 * More info on bossa : http://www.shumatech.com/web/products/bossa
 *
 * For those of us who hail from the command line era (and don't you dare laugh at our age!!) : bossac.exe -i -d --port=COM10 -U true -i -e -w -v LED_TOGGLE1.bin -R
 *
 *
 * 4. Start.Atmel project creation
 * ===============================
 *
 * I really battled to get the USB serial port stuff going on my Feather M0 from an Atmel Studio Atmel Start project, my first breakthrough came when I built 
 * an AtSamd21 explained pro example project for the HID Mouse, and later for the CDC echo.
 *
 * It turns out that the culprit is in the clock setup, your USB needs to run at 48MHz, and in some cases Atmel Start didn't automatically set the USB clock
 * to 48MHz
 *
 * To confound things further, the ADC, has a max frequency of 2100kHz (Atmel-42181G–SAM-D21_Datasheet–09/2015 page 956)
 *
 * After experimenting with many different clock combinations, I've settled for the following values : CPU at 24MHz, USB at 48MHz and the ADC at 2.086MHz (48MHz/23)
 *
 * To see how this project's clocks are configured:
 * 
 * See the Atmel start screen capture Clock_config_4_ADC_and_USB.png in the source directory 
 *
 * Solution Explorer -> Right Click on your project (Feather M0 in BOLD) -> Reconfigure Atmel Start Project -> wait -> click on the clock icon on the left
 *
 *
 * 5. Diff tool for comparing files
 * ================================
 *
 * Install WinMerge
 *
 * in Tools -> Options -> Atmel Software framework -> File compare -> Path of application used for comparing files
 *
 *
 * 6. Select different ADC Inputs
 * ==============================
 *
 * C:\Users\user\Documents\Persoonlik\embedded\feather\Projects\Light sensor\Light sensor\Config\hpl_adc_config.h
 *
 * adc_sync_set_inputs(&ADC_0, 0x02, 0x19, 0);//AIN02 with io ground
 *
 * Positive Mux Input Selection
 * 0x00=>ADC AIN0 pin
 * 0x01=>ADC AIN1 pin
 * ..
 * 0x09=>ADC AIN9 pin
 * 0x0A=>ADC AIN10 pin
 * 0x0B=>ADC AIN11 pin
 * 0x0C=>ADC AIN12 pin
 * 0x0D=>ADC AIN13 pin
 * 0x0E=>ADC AIN14 pin
 * 0x0F=>ADC AIN15 pin
 * 0x10=>ADC AIN16 pin
 * 0x11=>ADC AIN17 pin
 * 0x12=>ADC AIN18 pin
 * 0x13=>ADC AIN19 pin
 * 0x18=>Temperature reference
 * 0x19=>Bandgap voltage
 * 0x1A=>1/4 scaled core supply
 * 0x1B=>1/4 scaled I/O supply
 * 0x1C=>DAC output
 *
 * Negative Mux Input Selection
 * 0x00=>ADC AIN0 pin
 * 0x01=>ADC AIN1 pin
 * ..
 * 0x07=>ADC AIN7 pin
 * 0x18=>Internal ground
 * 0x19=>I/O ground
 *
 *
 * 7. Physical Connections
 * =======================
 *
 * 7.1 Variable resistor (2kOhm) - Analogue Input
 * ----------------------------------------------
 *
 * 3.314V o-------o 
 *                |
 *                |
 *                \
 *                /
 *                \
 *                / (1.5kOhm Fixed)
 *                \
 *                /
 *                |
 *                |
 *                \
 *                / (2.0kOhm variable)
 *                \ 
 *                / <----^^^^^^^^(3.3kOhm Fixed)--------o AIN2 / PB08 (Value 0V - 1.928V)
 *                \
 *                /
 *                \
 *                / 
 *                |
 *                |
 * GND  o---------o
 *
 *
 * 7.2 USB_COM_Port_Enable_Switch - GPIO Input
 * -------------------------------------------
 *
 * 3.3V o---------o
 *                 \
 *                  o----------^^^^^^^^(3kOhm)--------o PA16
 *
 * GND  o---------o
 *
 *
 * 7.3 Frequency measurement - GPIO Output
 * ---------------------------------------
 *
 * PA15 o---------o
 *                |
 *                Multimeter
 *                |
 * GND  o---------o
 *
 *
 * 8. ADC / A2D : Analog to digital converters
 * ===========================================
 *
 * http://drechsler-it.de/Electronics_Adafruit_FeatherM0_ASF_Tutorials_05_ADC.html
 *
 * Using 12bit mode
 *
 * 0V < VInput < Vref
 *
 * VRef Internal = 3.314V/1.48 = 2.239V
 *
 * Theoretical & measured values
 *
 *    0 @     0V   Measured :   26-50
 * 3526 @ 1.928V   Measured : 3549-3567
 * 3802 @ 2.079C   Measured : 3705-3860
 * 4095 @ 2.239V with VRef = 2.239V
 *
 * NOTE : With the ADC in SYNC mode, the measured values makes sense, however with the ASYNC mode, it looks like every 3rd or 4th 
 *        value is valid, with the others garbage, and I have discarded all my efforts with starting async conversions in timers
 *        and reading the values in callback functions
 *
 *
 * 
 * Hope this helps someone
 *
 * Enjoy plenty!!
 *
 * Regards
 *
 * Gert
 */

int main(void)
{
    char usb_serial_port_write_buffer[200];
    uint16_t batt_voltage_ACD_value;
	uint16_t variable_resistor_voltage_ADC_value;

	/* Initializes MCU, drivers and middleware */
	system_init();
	adc_sync_enable_channel(&ADC_0, 0);//Seems that the concept of channels is undefined on the samd21 ADC, thus using channel 0 throughout with the ADC
	adc_sync_set_inputs(&ADC_0,	0x02, 0x19, 0);//from hpl_adc_config, read AIN2/A1/PB08 (0x02) referenced to Internal Ground (0x18)

	/* Replace with your application code */
	while (1) 
	{
		//If the user's usb on switch is on, then enable the usb com port, and write data on the USB com port as debug
		if (gpio_get_pin_level(USB_COM_Port_Enable_Switch))
        {
			//This is an instance of a USB CDC (Communications Device Class) subset ACM (Abstract Control Model - Serial Port)
			//USB serial port
	        usb_init();

	        // wait for cdc acm to be enabled
	        while ((!cdcdf_acm_is_enabled()) && (gpio_get_pin_level(USB_COM_Port_Enable_Switch)))
	        {
		        gpio_toggle_pin_level(Led);
		        delay_ms(1);
	        };

	        //As long as the User keeps the usb switch on, keep on writing values to the com port
			while (gpio_get_pin_level(USB_COM_Port_Enable_Switch))
	        {
				//Get the ADC values for the Battery and the variable resistor
				batt_voltage_ACD_value = 0;
				adc_sync_set_inputs(&ADC_0, 0x07, 0x19, 0);//Read AIN7/A7/PA07 (0x07) referenced to Internal Ground (0x18)
		        adc_sync_read_channel(&ADC_0, 0, (uint8_t*)&batt_voltage_ACD_value, 2);

				variable_resistor_voltage_ADC_value = 0;
				adc_sync_set_inputs(&ADC_0,	0x02, 0x19, 0);//from hpl_adc_config, read AIN2/A1/PB08 (0x02) referenced to Internal Ground (0x18)
			    adc_sync_read_channel(&ADC_0, 0, (uint8_t*)&variable_resistor_voltage_ADC_value, 2);
		
				//USB write, you can read this text on a PC with a terminal program like putty
				snprintf(usb_serial_port_write_buffer, sizeof(usb_serial_port_write_buffer), "ADC Values : Variable Resistor : %04d, Battery : %04d \n\r", variable_resistor_voltage_ADC_value, batt_voltage_ACD_value);
		        cdcdf_acm_write((uint8_t *)usb_serial_port_write_buffer, strlen(usb_serial_port_write_buffer));

				//Measuring 1.715kHz : Samples per second = 1.715 * 2 (toggling) * 2 (batt & var resistor) = 6860sps
		        gpio_toggle_pin_level(Led);
	        }

	        usbdc_deinit();
        }//if (gpio_get_pin_level(USB_COM_Port_Enable_Switch))
        else
        {//the actual program should end up in this block
			adc_sync_read_channel(&ADC_0, 0, (uint8_t*)&variable_resistor_voltage_ADC_value, 2);

			//Measuring 15.70kHz : Samples per second = 15.70 * 2 (toggling) = 31.4ksps an order of magnitude less than the possible 350ksps??
			gpio_toggle_pin_level(Led);
        }//else
	}//while (1)
}