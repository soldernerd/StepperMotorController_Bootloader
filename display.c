
#include <xc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "display.h"
#include "i2c.h"
#include "os.h"
#include "spi.h"
#include "bootloader.h"
#include "application_config.h"

//Custom characters
#define DISPLAY_CC_VERTICALBAR_ADDRESS 0x00
#define DISPLAY_CC_VERTICALBAR_BIT_PATTERN {0b00000100,0b00000100,0b00000100,0b00000100,0b00000100,0b00000100,0b00000100,0b00000100}
const uint8_t bit_pattern_verticalbar[8] = DISPLAY_CC_VERTICALBAR_BIT_PATTERN;

#define DISPLAY_CC_DEGREE_ADDRESS 0x01
#define DISPLAY_CC_DEGREE_BIT_PATTERN {0b00011000, 0b00011000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000}
const uint8_t bit_pattern_degree[8] = DISPLAY_CC_DEGREE_BIT_PATTERN;

#define DISPLAY_CC_ae_ADDRESS 0x02
#define DISPLAY_CC_ae_BIT_PATTERN {0b00001010, 0b00000000, 0b00001110, 0b00000001, 0b00001111, 0b00010001, 0b00001111, 0b00000000}
const uint8_t bit_pattern_ae[8] = DISPLAY_CC_ae_BIT_PATTERN;

#define DISPLAY_CC_ue_ADDRESS 0x03
#define DISPLAY_CC_ue_BIT_PATTERN {0b00001010, 0b00000000, 0b00010001, 0b00010001, 0b00010001, 0b00010011, 0b00001101, 0b00000000}
const uint8_t bit_pattern_ue[8] = DISPLAY_CC_ue_BIT_PATTERN;

#define DISPLAY_CC_oe_ADDRESS 0x04
#define DISPLAY_CC_oe_BIT_PATTERN {0b00001010, 0b00000000, 0b00001110, 0b00010001, 0b00010001, 0b00010001, 0b00001110, 0b00000000}
const uint8_t bit_pattern_oe[8] = DISPLAY_CC_oe_BIT_PATTERN;

#ifdef LANGUAGE_ENGLISH

const char start_line1[] = "Bootloader Mode";
const char start_line2[] = "Version ";
const char start_line3[] = "";
const char start_line4[] = "soldernerd.com";

const char search_line1[] = "Bootloader Mode";
const char search_line2[] = "Looking for file";
const char search_line3[] = "FIRMWARE.HEX on USB";
const char search_line4[] = "drive...";

const char found_line1[] = "Bootloader Mode";
const char found_line2[] = "FIRMWARE.HEX found";
const char found_line3[] = "Size: ";
const char found_line3b[] = " bytes";
const char found_line4[] = "Press to use file";

const char verify_line1[] = "Bootloader Mode";
const char verify_line2[] = "Verifying...";
const char verify_line3[] = "Record";

const char checked_line1[] = "Bootloader Mode";
const char checked_line2[] = "File check completed";
const char checked_line3[] = "records";
const char checked_line4[] = "Press to program";

const char failed_line1[] = "Bootloader Mode";
const char failed_line2[] = "File check failed";
const char failed_line3_startCode[] = "Missing start code";
const char failed_line3_noNextRecord[] = "No next record";
const char failed_line3_checksum[] = "Checksum error";
const char failed_line3_dataTooLong[] = "Data too long";
const char failed_line3_addressRange[] = "Addr. outside range";
const char failed_line4[] = "Record ";

const char programming_line1[] = "Bootloader Mode";
const char programming_line2[] = "Programming flash";
const char programming_line3[] = "Entry ";
const char programming_line4[] = "Pages written: ";

const char done_line1[] = "Bootloader Mode";
const char done_line2[] = "Done!";
const char done_line3[] = "Pages written: ";
const char done_line4[] = "Press to reboot";

const char suspended_line1[] = "Bootloader Mode";
const char suspended_line2[] = "Suspended";
const char suspended_line3[] = "";
const char suspended_line4[] = "Press to resume";

const char reboot_line1[] = "Rebooting...";

#endif /*LANGUAGE_ENGLISH*/

#ifdef LANGUAGE_GERMAN

const char start_line1[] = "Bootloader Modus";
const char start_line2[] = "Version ";
const char start_line3[] = "";
const char start_line4[] = "soldernerd.com";

const char search_line1[] = "Bootloader Modus";
const char search_line2[] = "Suche File";
const char search_line3[] = "FIRMWARE.HEX auf";
const char search_line4[] = "USB-Laufwerk...";

const char found_line1[] = "Bootloader Modus";
const char found_line2[] = "File gefunden";
//const char found_line3[] = "Grösse: ";
const char found_line3[] = {'G','r',DISPLAY_CC_oe_ADDRESS,'s','s','e',':',0};
const char found_line3b[] = " Bytes";
//const char found_line4[] = "Drücken zm verwenden";
const char found_line4[] = {'D','r',DISPLAY_CC_ue_ADDRESS,'c','k','e','n',' ','z','m',' ','v','e','r','w','e','n','d','e','n',0};

const char verify_line1[] = "Bootloader Modus";
//const char verify_line2[] = "Prüfe File...";
const char verify_line2[] = {'P','r',DISPLAY_CC_ue_ADDRESS,'f','e',' ','F','i','l','e','.','.','.',0};
const char verify_line3[] = "Eintrag";

const char checked_line1[] = "Bootloader Modus";
//const char checked_line2[] = "File geprüft";
const char checked_line2[] = {'F','i','l','e',' ','g','e','p','r',DISPLAY_CC_ue_ADDRESS,'f','t',0};
//const char checked_line3[] = "Einträge";
const char checked_line3[] = {'E','i','n','t','r',DISPLAY_CC_ae_ADDRESS,'g','e',0};
//const char checked_line4[] = "Drücken zum program.";
const char checked_line4[] = {'D','r',DISPLAY_CC_ue_ADDRESS,'c','k','e','n',' ','z','u','m',' ','p','r','o','g','r','a','m','.',0};

const char failed_line1[] = "Bootloader Modus";
const char failed_line2[] = "File fehlerhaft";
const char failed_line3_startCode[] = "Fehlender Start Code";
//const char failed_line3_noNextRecord[] = "Plötzliches Fileende";
const char failed_line3_noNextRecord[] = {'P','l',DISPLAY_CC_oe_ADDRESS,'t','z','l','i','c','h','e','s',' ','F','i','l','e','e','n','d','e',0};
//const char failed_line3_checksum[] = "Prüfsumme falsch";
const char failed_line3_checksum[] = {'P','r',DISPLAY_CC_ue_ADDRESS,'f','s','u','m','m','e',' ','f','a','l','s','c','h',0};
const char failed_line3_dataTooLong[] = "Daten zu lange";
//const char failed_line3_addressRange[] = "Addresse unzulässig";
const char failed_line3_addressRange[] = {'A','d','d','r','e','s','s','e',' ','u','n','z','u','l',DISPLAY_CC_ae_ADDRESS,'s','s','i','g',0};
const char failed_line4[] = "Eintrag ";

const char programming_line1[] = "Bootloader Modus";
const char programming_line2[] = "Programmiere Flash";
const char programming_line3[] = "Eintrag ";
const char programming_line4[] = "Geschr. Seiten: ";

const char done_line1[] = "Bootloader Modus";
const char done_line2[] = "Fertig!";
const char done_line3[] = "Geschr. Seiten: ";
//const char done_line4[] = "Drücken zum Neustart";
const char done_line4[] = {'D','r',DISPLAY_CC_ue_ADDRESS,'c','k','e','n',' ','z','u','m',' ','N','e','u','s','t','a','r','t',0};

const char suspended_line1[] = "Bootloader Modus";
const char suspended_line2[] = "Suspendiert";
const char suspended_line3[] = "";
//const char suspended_line4[] = "Drücken z Fortfahren";
const char suspended_line4[] = {'D','r',DISPLAY_CC_ue_ADDRESS,'c','k','e','n',' ','z',' ','F','o','r','t','f','a','h','r','e','n',0};

const char reboot_line1[] = "Neustart...";

#endif /*LANGUAGE_GERMAN*/




char display_content[4][20];

static void _display_start(void);
static void _display_search(void);
static void _display_found(void);
static void _display_verify(void);
static void _display_checked(void);
static void _display_failed(void);
static void _display_programming(void);
static void _display_done(void);
static void _display_rebooting(void);
static void _display_suspended(void);

void display_init(void)
{
    i2c_display_init();
    i2c_display_program_custom_character(DISPLAY_CC_VERTICALBAR_ADDRESS, bit_pattern_verticalbar); 
    i2c_display_program_custom_character(DISPLAY_CC_DEGREE_ADDRESS, bit_pattern_degree);
    i2c_display_program_custom_character(DISPLAY_CC_ae_ADDRESS, bit_pattern_ae);
    i2c_display_program_custom_character(DISPLAY_CC_ue_ADDRESS, bit_pattern_ue);
    i2c_display_program_custom_character(DISPLAY_CC_oe_ADDRESS, bit_pattern_oe);
}

uint8_t display_get_character(uint8_t line, uint8_t position)
{
    return display_content[line][position];
}

static void _display_clear(void)
{
    uint8_t row;
    uint8_t col;
    for(row=0;row<4;++row)
    {
        for(col=0;col<20;++col)
        {
            display_content[row][col] = ' ';
        }
    }
}

static uint8_t _display_itoa_u16(uint16_t value,  char *text)
{
    itoa(text, value, 10);
    if(value>9999)
    {
        *(text+5) = ' ';
        return 5;
    }
    else if (value>999)
    {
        *(text+4) = ' ';
        return 4;
    }
    else if (value>99)
    {
        *(text+3) = ' ';
        return 3;
    }
    else if (value>9)
    {
        *(text+2) = ' ';
        return 2;
    }
    else
    {
        *(text+1) = ' ';
        return 1;
    }
}

static uint8_t _get_hex_char(uint8_t value)
{
    if(value&0xF0)
    {
        value >>= 4;
    }
    else
    {
        value &= 0x0F;
    }    
    switch(value)
    {
        case 0x00:
            return '0';
        case 0x01:
            return '1';
        case 0x02:
            return '2';
        case 0x03:
            return '3';
        case 0x04:
            return '4';         
        case 0x05:
            return '5';       
        case 0x06:
            return '6';  
        case 0x07:
            return '7'; 
        case 0x08:
            return '8';      
        case 0x09:
            return '9';         
        case 0x0A:
            return 'A'; 
        case 0x0B:
            return 'B';       
        case 0x0C:
            return 'C';          
        case 0x0D:
            return 'D';       
        case 0x0E:
            return 'E';         
        case 0x0F:
            return 'F'; 
    }
    return 'G';
}

static void _display_itoa_u8(uint8_t value,  char *text)
{
    *(text+0) = _get_hex_char(value & 0xF0);
    *(text+1) = _get_hex_char(value & 0x0F);
}

static uint8_t _display_itoa_u32(uint32_t value,  char *text)
{
    uint8_t number_of_digits;
    int8_t cntr;
    
    //Find number of digits
    if(value<10) {number_of_digits = 1;}
    else if(value<100) {number_of_digits = 2;}
    else if(value<1000) {number_of_digits = 3;}
    else if(value<10000) {number_of_digits = 4;}
    else if(value<100000) {number_of_digits = 5;}
    else if(value<1000000) {number_of_digits = 6;}
    else if(value<10000000) {number_of_digits = 7;}
    else if(value<100000000) {number_of_digits = 8;}
    else if(value<1000000000) {number_of_digits = 9;}
    else {number_of_digits = 10;}
    
    for(cntr=number_of_digits-1; cntr>=0; --cntr)
    {
        text[cntr] = (value%10) + 0x30;
        value /= 10;
    }
 
    return number_of_digits;
}

static void _display_itoa(int16_t value, uint8_t decimals, char *text)
{
    uint8_t pos;
    uint8_t len;
    int8_t missing;
    char tmp[10];
    itoa(tmp, value, 10);
    len = strlen(tmp);
    
    
    if(value<0) //negative values
    {
        missing = decimals + 2 - len;
        if(missing>0) //zero-padding needed
        {
            for(pos=decimals;pos!=0xFF;--pos)
            {
                if(pos>=missing) //there is a character to copy
                {
                    tmp[pos+1] = tmp[pos+1-missing];
                }
                else //there is no character
                {
                    tmp[pos+1] = '0';
                }
            }
            len = decimals + 2;
        }  
    }
    else
    {
        missing = decimals + 1 - len;
        if(missing>0) //zero-padding needed
        {
            for(pos=decimals;pos!=0xFF;--pos)
            {
                if(pos>=missing) //there is a character to copy
                {
                    tmp[pos] = tmp[pos-missing];
                }
                else //there is no character
                {
                    tmp[pos] = '0';
                }
            }
            len = decimals + 1;
        }       
    }
 
    decimals = len - decimals - 1;
    
    for(pos=0;pos<len;++pos)
    {
        text[pos] = tmp[pos];
        if(pos==decimals)
        {
            //Insert decimal point
            ++pos;
            text[pos] = '.';
            break;
        }
    }
    for(;pos<len;++pos)
    {
        text[pos+1] = tmp[pos];
    }
}

void display_prepare(uint8_t mode)
{
    _display_clear();
    
    switch(mode&0xF0)
    {
        case DISPLAY_MODE_BOOTLOADER_START:
            _display_start();
            break;
            
        case DISPLAY_MODE_BOOTLOADER_SEARCH:
            _display_search();
            break;
            
        case DISPLAY_MODE_BOOTLOADER_FILE_FOUND:
            _display_found();
            break;
            
        case DISPLAY_MODE_BOOTLOADER_FILE_VERIFYING:
            _display_verify();
            break;
            
        case DISPLAY_MODE_BOOTLOADER_CHECK_COMPLETE:
            _display_checked();
            break;
            
        case DISPLAY_MODE_BOOTLOADER_CHECK_FAILED:
            _display_failed();
            break;
            
        case DISPLAY_MODE_BOOTLOADER_PROGRAMMING:
            _display_programming();
            break;
            
        case DISPLAY_MODE_BOOTLOADER_DONE:
            _display_done();
            break;
            
        case DISPLAY_MODE_BOOTLOADER_REBOOTING:
            _display_rebooting();
            break;
            
        case DISPLAY_MODE_BOOTLOADER_SUSPENDED:
            _display_suspended();
            break;
    }
    
    //Pulse
    switch((os.timeSlot>>5)&0b11)
    {
        case 3:
            #ifdef LANGUAGE_ENGLISH
            display_content[0][17] = '.';
            #endif /*LANGUAGE_ENGLISH*/
            #ifdef LANGUAGE_GERMAN
            display_content[0][18] = '.';
            #endif /*LANGUAGE_GERMAN*/
            //fall through
        case 2:
            #ifdef LANGUAGE_ENGLISH
            display_content[0][16] = '.';
            #endif /*LANGUAGE_ENGLISH*/
            #ifdef LANGUAGE_GERMAN
            display_content[0][17] = '.';
            #endif /*LANGUAGE_GERMAN*/
            //fall through
        case 1:
            #ifdef LANGUAGE_ENGLISH
            display_content[0][15] = '.';
            #endif /*LANGUAGE_ENGLISH*/
            #ifdef LANGUAGE_GERMAN
            display_content[0][16] = '.';
            #endif /*LANGUAGE_GERMAN*/
            //fall through     
    }
    
    //Some debugging output
    //Warn if interrupts are enabled
    if(INTCONbits.GIE)
    {
        display_content[0][18] = 'I';
    }
}

static void _display_start(void)
{
    uint8_t cntr;
    cntr = 0;
    while(start_line1[cntr])
        display_content[0][cntr] = start_line1[cntr++];
    cntr = 0;
    while(start_line2[cntr])
        display_content[1][cntr] = start_line2[cntr++];
    cntr += _display_itoa_u16(FIRMWARE_VERSION_MAJOR, &display_content[1][cntr]);
    display_content[1][cntr++] = '.';
    cntr += _display_itoa_u16(FIRMWARE_VERSION_MINOR, &display_content[1][cntr]);
    display_content[1][cntr++] = '.';
    cntr += _display_itoa_u16(FIRMWARE_VERSION_FIX, &display_content[1][cntr]);
    cntr = 0;
    while(start_line3[cntr])
        display_content[2][cntr] = start_line3[cntr++];
    cntr = 0;
    while(start_line4[cntr])
        display_content[3][cntr] = start_line4[cntr++];
}

static void _display_search(void)
{
    uint8_t cntr;
    cntr = 0;
    while(search_line1[cntr])
        display_content[0][cntr] = search_line1[cntr++];
    cntr = 0;
    while(search_line2[cntr])
        display_content[1][cntr] = search_line2[cntr++];
    cntr = 0;
    while(search_line3[cntr])
        display_content[2][cntr] = search_line3[cntr++];
    cntr = 0;
    while(search_line4[cntr])
        display_content[3][cntr] = search_line4[cntr++];
}

static void _display_found(void)
{
    uint8_t cntr;
    uint8_t start;
    //First line
    cntr = 0;
    while(found_line1[cntr])
        display_content[0][cntr] = found_line1[cntr++];
    //Second line
    cntr = 0;
    while(found_line2[cntr])
        display_content[1][cntr] = found_line2[cntr++];
    //Third line (file size)
    cntr = 0;
    while(found_line3[cntr])
        display_content[2][cntr] = found_line3[cntr++];
    start = _display_itoa_u32(bootloader_get_file_size(), &display_content[2][cntr]) + 6;
    cntr = 0;
    while(found_line3b[cntr])
        display_content[2][start+cntr] = found_line3b[cntr++];
    //Fourth line
    cntr = 0;
    while(found_line4[cntr])
        display_content[3][cntr] = found_line4[cntr++];
}

static void _display_verify(void)
{
    uint8_t cntr;
    uint8_t start;
    cntr = 0;
    while(verify_line1[cntr])
        display_content[0][cntr] = verify_line1[cntr++];
    cntr = 0;
    while(verify_line2[cntr])
        display_content[1][cntr] = verify_line2[cntr++];
    cntr = 0;
    while(verify_line3[cntr])
        display_content[2][cntr] = verify_line3[cntr++];
    _display_itoa_u16(bootloader_get_entries(), &display_content[2][cntr+1]);
}

static void _display_checked(void)
{
    uint8_t cntr;
    uint8_t start;
    cntr = 0;
    while(checked_line1[cntr])
        display_content[0][cntr] = checked_line1[cntr++];
    cntr = 0;
    while(checked_line2[cntr])
        display_content[1][cntr] = checked_line2[cntr++];
    //Display number of records
    start = _display_itoa_u16(bootloader_get_total_entries(), &display_content[2][0]);
    start++;
    cntr = 0;
    while(checked_line3[cntr])
        display_content[2][start+cntr] = checked_line3[cntr++];
    //Display question
    cntr = 0;
    while(checked_line4[cntr])
        display_content[3][cntr] = checked_line4[cntr++];
}

static void _display_failed(void)
{
    uint8_t cntr;
    uint8_t start;
    cntr = 0;
    while(failed_line1[cntr])
        display_content[0][cntr] = failed_line1[cntr++];
    cntr = 0;
    while(failed_line2[cntr])
        display_content[1][cntr] = failed_line2[cntr++];
    //Display error
    cntr = 0;
    switch(bootloader_get_error())
    {
        case ShortRecordErrorStartCode:
            while(failed_line3_startCode[cntr])
            display_content[2][cntr] = failed_line3_startCode[cntr++];
            break;
            
        case ShortRecordErrorChecksum:
            while(failed_line3_checksum[cntr])
            display_content[2][cntr] = failed_line3_checksum[cntr++];
            //_itoa(&display_content[2][14], bootloader_get_rec_checksum(), 10);
            //itoa(&display_content[2][17], bootloader_get_rec_checksumCheck(), 10);
            break;
            
        case ShortRecordErrorNoNextRecord:
            while(failed_line3_noNextRecord[cntr])
            display_content[2][cntr] = failed_line3_noNextRecord[cntr++];
            break;
            
        case ShortRecordErrorDataTooLong:
            while(failed_line3_dataTooLong[cntr])
            display_content[2][cntr] = failed_line3_dataTooLong[cntr++];
            break;
            
        case ShortRecordErrorAddressRange:
            while(failed_line3_addressRange[cntr])
            display_content[2][cntr] = failed_line3_addressRange[cntr++];
            _display_itoa_u32(bootloader_get_rec_address(), &display_content[3][14]);
            break;
            
    }
    //Display record number
    cntr = 0;
    while(failed_line4[cntr])
        display_content[3][cntr] = failed_line4[cntr++];
    _display_itoa_u16(bootloader_get_entries(), &display_content[3][cntr]);
}

static void _display_programming(void)
{
    uint8_t cntr;
    cntr = 0;
    while(programming_line1[cntr])
        display_content[0][cntr] = programming_line1[cntr++];
    
    cntr = 0;
    while(programming_line2[cntr])
        display_content[1][cntr] = programming_line2[cntr++];
    
    cntr = 0;
    while(programming_line3[cntr])
        display_content[2][cntr] = programming_line3[cntr++];
    cntr += _display_itoa_u16(bootloader_get_entries(), &display_content[2][cntr]);
    display_content[2][cntr++] = '/';
    _display_itoa_u16(bootloader_get_total_entries(), &display_content[2][cntr]);
    
    cntr = 0;
    while(programming_line4[cntr])
        display_content[3][cntr] = programming_line4[cntr++];
    _display_itoa_u16(bootloader_get_flashPagesWritten(), &display_content[3][cntr]);
}

static void _display_done(void)
{
    uint8_t cntr;
    cntr = 0;
    while(done_line1[cntr])
        display_content[0][cntr] = done_line1[cntr++];
    cntr = 0;
    while(done_line2[cntr])
        display_content[1][cntr] = done_line2[cntr++];
    cntr = 0;
    while(done_line3[cntr])
        display_content[2][cntr] = done_line3[cntr++];
    _display_itoa_u16(bootloader_get_flashPagesWritten(), &display_content[2][cntr]);
    cntr = 0;
    while(done_line4[cntr])
        display_content[3][cntr] = done_line4[cntr++];
}

static void _display_rebooting(void)
{
    uint8_t cntr;
    cntr = 0;
    while(reboot_line1[cntr])
        display_content[0][cntr] = reboot_line1[cntr++];
}

static void _display_suspended(void)
{
    uint8_t cntr;

    //Line 1
    cntr = 0;
    while(suspended_line1[cntr])
        display_content[0][cntr] = suspended_line1[cntr++];
    //Line 2
    cntr = 0;
    while(suspended_line2[cntr])
        display_content[1][cntr] = suspended_line2[cntr++]; 
    //Line 3
    cntr = 0;
    while(suspended_line3[cntr])
        display_content[2][cntr] = suspended_line3[cntr++]; 
    //Line 4
    cntr = 0;
    while(suspended_line4[cntr])
        display_content[3][cntr] = suspended_line4[cntr++]; 
}

void display_update(void)
{
    i2c_display_cursor(0, 0);
    i2c_display_write_fixed(&display_content[0][0], 20);
    i2c_display_cursor(1, 0);
    i2c_display_write_fixed(&display_content[1][0], 20);
    i2c_display_cursor(2, 0);
    i2c_display_write_fixed(&display_content[2][0], 20);
    i2c_display_cursor(3, 0);
    i2c_display_write_fixed(&display_content[3][0], 20);
}