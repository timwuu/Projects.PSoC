/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>

#define BUFFER_LEN  64u

#define LED_ON  1u
#define LED_OFF 0u

int main()
{
    uint8 LED_STATUS= LED_ON;
    
    uint16 count;
    uint8 buffer[BUFFER_LEN];
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Start USBFS Operation/device 0 and with 5V operation */
    USBUART_1_Start(0u, USBUART_1_DWR_VDDD_OPERATION);
    
    /* Wait for Device to enumerate */
    while(!USBUART_1_GetConfiguration());
    
    USBUART_1_CDC_Init();
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        
    	 if(USBUART_1_DataIsReady() != 0u)               /* Check for input data from PC */
    		{   
    			count = USBUART_1_GetAll(buffer);           /* Read received data and re-enable OUT endpoint */
    			if(count != 0u)
    			{
    				while(USBUART_1_CDCIsReady() == 0u);    /* Wait till component is ready to send more data to the PC */ 
    				USBUART_1_PutData(buffer, count);       /* Send data back to PC */
    				/* If the last sent packet is exactly maximum packet size, 
    				*  it shall be followed by a zero-length packet to assure the
    				*  end of segment is properly identified by the terminal.
    				*/
    				if(count == BUFFER_LEN)
    				{
    					while(USBUART_1_CDCIsReady() == 0u); /* Wait till component is ready to send more data to the PC */ 
    					USBUART_1_PutData(NULL, 0u);         /* Send zero-length packet to PC */
    				}
    			}
        
                LED_STATUS = (LED_STATUS)?LED_OFF:LED_ON;
            
                P2_1_Write( LED_STATUS);
    
            }  
        
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
