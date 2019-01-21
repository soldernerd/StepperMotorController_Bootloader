/* 
 * File:   hardware_config.h
 * Author: luke
 *
 * Created on 3. Oktober 2018, 17:19
 */

#ifndef HARDWARE_CONFIG_H
#define	HARDWARE_CONFIG_H

/*
 * Define your board revision here
 */

#define BOARD_REVISION_B


/******************************************************************************
 * Nothing should need to be changed below here                               *
 ******************************************************************************/

/*
 * Replacement for some depreciated PLIB macros
 */

#define  PPSUnLock()    {EECON2 = 0b01010101; EECON2 = 0b10101010; PPSCONbits.IOLOCK = 0;}
#define  PPSLock() 		{EECON2 = 0b01010101; EECON2 = 0b10101010; PPSCONbits.IOLOCK = 1;}

/*
 * General definitions for setting pin functions
 */

#define PIN_INPUT           1
#define PIN_OUTPUT          0
#define PIN_DIGITAL         1
#define PIN_ANALOG          0

/*
 * Physical properties, pinout
 */

#define _XTAL_FREQ 8000000

#define PPS_FUNCTION_SPI2_DATA_OUTPUT_VALUE 10
#define PPS_FUNCTION_SPI2_DATA_INPUT_REGISTER RPINR21
#define PPS_FUNCTION_SPI2_CLOCK_OUTPUT_VALUE 11
#define PPS_FUNCTION_SPI2_CLOCK_INPUT_REGISTER RPINR22
#define PPS_FUNCTION_SPI2_CHIPSELECT_OUTPUT_VALUE 12
#define PPS_FUNCTION_SPI2_CHIPSELECT_INPUT_REGISTER RPINR23
#define PPS_FUNCTION_CCP1_A_OUTPUT_VALUE 14
#define PPS_FUNCTION_CCP1_B_OUTPUT_VALUE 15

#define DISP_RESET_TRIS TRISAbits.TRISA3
#define DISP_RESET_PIN LATAbits.LA3
#define DISP_BACKLIGHT_TRIS TRISCbits.TRISC1
#define DISP_BACKLIGHT_PIN LATCbits.LC1

#define FAN_ENABLE_TRIS TRISCbits.TRISC6
#define FAN_ENABLE_PIN LATCbits.LC6
#define BRAKE_ENABLE_TRIS TRISAbits.TRISA5
#define BRAKE_ENABLE_PIN LATAbits.LA5

#define SPI_MISO_TRIS TRISDbits.TRISD5
#define SPI_MISO_PIN LATDbits.LD5
#define SPI_MISO_RP_INPUT_VALUE 22
#define SPI_MISO_RP_OUTPUT_REGISTER RPOR22
#define SPI_MOSI_TRIS TRISDbits.TRISD4
#define SPI_MOSI_PIN LATDbits.LD4
#define SPI_MOSI_RP_INPUT_VALUE 21
#define SPI_MOSI_RP_OUTPUT_REGISTER RPOR21
#define SPI_SCLK_TRIS TRISDbits.TRISD6
#define SPI_SCLK_PIN LATDbits.LD6
#define SPI_SCLK_RP_INPUT_VALUE 23
#define SPI_SCLK_RP_OUTPUT_REGISTER RPOR23
#define SPI_SS1_TRIS TRISDbits.TRISD7
#define SPI_SS1_PORT PORTDbits.RD7
#define SPI_SS1_PIN LATDbits.LD7
#define SPI_SS1_RP_INPUT_VALUE 24
#define SPI_SS1_RP_OUTPUT_REGISTER RPOR24

#define ENCODER1_PB_TRIS TRISEbits.TRISE1
#define ENCODER1_PB_PIN PORTEbits.RE1
#define ENCODER1_PB_LAT LATEbits.LE1
#define ENCODER1_A_TRIS TRISEbits.TRISE2
#define ENCODER1_A_PIN PORTEbits.RE2
#define ENCODER1_B_TRIS TRISEbits.TRISE0
#define ENCODER1_B_PIN PORTEbits.RE0

#define ENCODER2_PB_TRIS TRISAbits.TRISA1
#define ENCODER2_PB_PIN PORTAbits.RA1
#define ENCODER2_PB_LAT LATAbits.LA1
#define ENCODER2_PB_ANCON ANCON0bits.PCFG1
#define ENCODER2_PB_PPS 1
#define ENCODER2_A_TRIS TRISAbits.TRISA0
#define ENCODER2_A_PIN PORTAbits.RA0
#define ENCODER2_A_PPS 0
#define ENCODER2_A_ANCON ANCON0bits.PCFG0
#define ENCODER2_B_TRIS TRISAbits.TRISA2
#define ENCODER2_B_PIN PORTAbits.RA2
#define ENCODER2_B_PPS 2
#define ENCODER2_B_ANCON ANCON0bits.PCFG2

#define TEMPERATURE_INTERNAL_TRIS TRISBbits.TRISB3
#define TEMPERATURE_INTERNAL_ANCON ANCON1bits.PCFG9
#define TEMPERATURE_INTERNAL_CHANNEL 0b1001
#define TEMPERATURE_EXTERNAL_TRIS TRISCbits.TRISC2
#define TEMPERATURE_EXTERNAL_ANCON ANCON1bits.PCFG11
#define TEMPERATURE_EXTERNAL_CHANNEL 0b1011

#endif	/* HARDWARE_CONFIG_H */
