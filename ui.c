#include <xc.h>
#include <stdint.h>

#include "hardware_config.h"
#include "os.h"
#include "i2c.h"
#include "display.h"
#include "ui.h"
#include "internal_flash.h"
#include "application_config.h"

/* ****************************************************************************
 * Variables definitions
 * ****************************************************************************/

userInterfaceStatus_t userInterfaceStatus;
uint16_t system_ui_inactive_count;
uint16_t startup_timer;

/* ****************************************************************************
 * Static functions
 * ****************************************************************************/

//React if the push button has been pressed
static void _ui_encoder(void)
{
    switch(os.display_mode)
    {
        case DISPLAY_MODE_BOOTLOADER_START:
            if(os.button1>0 || os.button2>0)
            {
                os.button1 = 0;
                os.button2 = 0;
            }
            break;
            
        case DISPLAY_MODE_BOOTLOADER_SEARCH:
            if(os.button1>0 || os.button2>0)
            {
                os.button1 = 0;
                os.button2 = 0;;
            }
            break;
            
        case DISPLAY_MODE_BOOTLOADER_FILE_FOUND:
            if(os.button1>0 || os.button2>0)
            {
                os.bootloader_mode = BOOTLOADER_MODE_FILE_VERIFYING;
                os.display_mode = DISPLAY_MODE_BOOTLOADER_FILE_VERIFYING;
                os.button1 = 0;
                os.button2 = 0;
            }
            break;
            
        case DISPLAY_MODE_BOOTLOADER_FILE_VERIFYING:
            if(os.button1>0 || os.button2>0)
            {
                os.button1 = 0;
                os.button2 = 0;
            }
            break;
            
        case DISPLAY_MODE_BOOTLOADER_CHECK_FAILED:
            if(os.button1>0 || os.button2>0)
            {
                os.button1 = 0;
                os.button2 = 0;
            }
            break;
            
        case DISPLAY_MODE_BOOTLOADER_CHECK_COMPLETE:
            if(os.button1>0 || os.button2>0)
            {
                os.bootloader_mode = BOOTLOADER_MODE_PROGRAMMING;
                os.display_mode = DISPLAY_MODE_BOOTLOADER_PROGRAMMING;
                os.button1 = 0;
                os.button2 = 0;
            }
            break;

        case BOOTLOADER_MODE_PROGRAMMING:
            if(os.button1>0 || os.button2>0)
            {
                os.button1 = 0;
                os.button2 = 0;
            }
            break; 
            
        case DISPLAY_MODE_BOOTLOADER_DONE:
            if(os.button1>0 || os.button2>0)
            {
                i2c_eeprom_writeByte(EEPROM_BOOTLOADER_BYTE_ADDRESS, BOOTLOADER_BYTE_FORCE_NORMAL_MODE);
                system_delay_ms(10); //ensure data has been written before rebooting
                reboot();
            }
            break;
            
        case DISPLAY_MODE_BOOTLOADER_SUSPENDED:
            if(os.button1>0 || os.button2>0)
            {
                os.bootloader_mode = BOOTLOADER_MODE_SEARCH;
                os.display_mode = DISPLAY_MODE_BOOTLOADER_SEARCH;
                os.button1 = 0;
                os.button2 = 0;
            }
            break; 
    }    
}

/* ****************************************************************************
 * Public functions
 * ****************************************************************************/

//Return current UI status
userInterfaceStatus_t ui_get_status(void)
{
    return userInterfaceStatus;
}

//Start initialization of the display
void ui_init(void)
{
    system_ui_inactive_count = 0;
    userInterfaceStatus = USER_INTERFACE_STATUS_STARTUP_1;
}

//This function is periodically called to keep the user interface alive
void ui_run(void)
{
	switch(userInterfaceStatus)
	{
		case USER_INTERFACE_STATUS_OFF:
            //Wake up if button has been pressed
			if (os.button1!=0 || os.button2!=0)
			{
                ui_init();
                os.button1 = 0;
                os.button2 = 0;
			}
			break;

		case USER_INTERFACE_STATUS_STARTUP_1:
            //Display is always powered, nothing to do here
            //Proceed to next state
			system_ui_inactive_count = 0;
			userInterfaceStatus = USER_INTERFACE_STATUS_STARTUP_2;
			break;
            
        case USER_INTERFACE_STATUS_STARTUP_2:
            //Put display into reset
            DISP_RESET_PIN = 0;
            //Proceed to next state
			system_ui_inactive_count = 0;
			userInterfaceStatus = USER_INTERFACE_STATUS_STARTUP_3;
			break;

		case USER_INTERFACE_STATUS_STARTUP_3:
			++system_ui_inactive_count;
            //Turn off reset after 32ms
			if (system_ui_inactive_count>3)
			{
				DISP_RESET_PIN = 1;
				system_ui_inactive_count = 0;
				userInterfaceStatus = USER_INTERFACE_STATUS_STARTUP_4;
			}
			break;

		case USER_INTERFACE_STATUS_STARTUP_4:
            //Send init sequence
            //i2c_display_init();
            display_init();
            //i2c_display_send_init_sequence();
            //Turn backlight on
            DISP_BACKLIGHT_PIN = 1;
            //Enable rotary encoder inputs
            system_encoder_enable();
            //User interface is now up and running
            system_ui_inactive_count = 0;
            startup_timer = 0;
            userInterfaceStatus = USER_INTERFACE_STATUS_ON;
			break;

		case USER_INTERFACE_STATUS_ON:
            if(os.display_mode==DISPLAY_MODE_BOOTLOADER_START)
            {
                ++startup_timer;
                if(startup_timer==250)
                {
                    os.display_mode = DISPLAY_MODE_BOOTLOADER_SEARCH;
                }
            }
			if (os.encoder1Count==0 && os.button1==0 && os.encoder2Count==0 && os.button2==0)
			{
                if(system_ui_inactive_count<0xFFFF)
                    ++system_ui_inactive_count;
			}
			else
			{
				system_ui_inactive_count = 0;
				_ui_encoder();
			}
			break;
	}
}