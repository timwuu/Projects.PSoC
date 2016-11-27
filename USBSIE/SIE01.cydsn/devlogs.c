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

/* 
   ========== USB Analyzer ==============
           - 2016.11.22 timijk

    1. setup SIO pins for 3.3V, it's used for HC-06 bluetooth module, and USB Dp/Dm

    2. Create simple testing signal including [SE0,J],[Idle],[JK],[JJ] states

    3. Create SE0, Reset signal, JK to Data Circuit

    4. Testing the function of the 32bit shift register
       - test the 8 bit version with simple control signal first.
       - using Interrupt to capture the Stored data, but found out there is a resource constraint??
       - there is a problem of using the DMA on the shift register module, to DIY a 32bit shift register
         by using 4 status registers.

    5. Using DMA to transfer the data, because Interrupt has too many overheads
        - It requires 2 stage DMAs to do the data copy.
        - 1. copy the data in the 4 status register to a temporary variable.
        - 2. copy the data to an array.
        
    6. The clock signal to the status register requires syncing.
        
    7. after the testing signal is processed correctly, use pic32mx as the USB host for further testing.
        
    



*/

/* [] END OF FILE */
