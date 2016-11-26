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
#include <stdio.h>

#define BUFFER_SIZE 32

volatile uint32 dat_idx;
uint32 dat[BUFFER_SIZE];
uint32 dat2[BUFFER_SIZE];
uint32 dat3[BUFFER_SIZE];
uint8 _dat_status;
uint8 dat8[BUFFER_SIZE];
uint32 _dat;
uint8 _dat8;

void setup_DMA1(void);
void setup_DMA2(void);

void _isr_1_Interrupt_InterruptCallback(void)
{
    
    //_dat=CY_GET_REG32(ShiftReg_1_OUT_FIFO_VAL_LSB_PTR); //ShiftReg_1_ReadData();
    
    _dat = ShiftReg_1_ReadRegValue();
    
    if( dat_idx < BUFFER_SIZE) dat[dat_idx++]= _dat;
    
    // clear the interrupt
    //ShiftReg_1_GetIntStatus();
    
    //Counter_1_ReadStatusRegister();

    return;
}


void isr_1_Interrupt_InterruptCallback(void)
{
    
    //_dat=CY_GET_REG32(ShiftReg_1_OUT_FIFO_VAL_LSB_PTR); //ShiftReg_1_ReadData();
    
    _dat8 = ShiftReg_3_ReadData();
    
    // clear the interrupt
    ShiftReg_3_GetIntStatus();
    
    return;
}

void isr_2_Interrupt_InterruptCallback(void)
{

    if( dat_idx < BUFFER_SIZE) 
    {
        dat[dat_idx]= ShiftReg_1_ReadData();
        dat2[dat_idx]= ShiftReg_2_ReadData();
        dat3[dat_idx++]= ShiftReg_4_ReadData();
    }
    else 
    {
        //isr_2_Disable();
        ShiftReg_1_DisableInt();
    }
    // clear the interrupt
    ShiftReg_1_GetIntStatus();
    
    return;
}

void shift_sim(void)
{
    char str[64];
    int32 _tmp;
    //uint32 _tmp1, _tmp2;
    
    UART_1_PutString("\n----- Start ShiftReg Simulation ------\n");

    //sync mode CE95372
    Clock_5_Write(1u);
    Clock_5_Write(0u);

    while(1)
    {
    
    ShiftIn_1_Write(1u);
    Clock_5_Write(1u);
    Clock_5_Write(0u);
    
    sprintf( str, "-- SHIFT LEFT 1\n"); //, ShiftReg_3_ReadRegValue());
    UART_1_PutString(str);
    CyDelay(2000u); 
    
    ShiftIn_1_Write(0u);
    Clock_5_Write(1u);
    Clock_5_Write(0u);
    
    sprintf( str, "SL0~reg:%8x\n", ShiftReg_3_ReadRegValue());
    UART_1_PutString(str);
    CyDelay(500u); 
    
    ShiftIn_1_Write(1u);
    Clock_5_Write(1u);
    Clock_5_Write(0u);
    
    sprintf( str, "SL1~reg:%8x\n", ShiftReg_3_ReadRegValue());
    UART_1_PutString(str);
    CyDelay(500u); 
    
    Store_1_Write(1u);
    Clock_5_Write(1u);
    Store_1_Write(0u);
    Clock_5_Write(0u);

    CyDelay(1u); 

    sprintf( str, "----dat:%8x\n", ShiftReg_3_ReadData());
    UART_1_PutString(str);
    CyDelay(2000u);
    
    UART_1_PutString("-- get INT status --\n");

    ShiftReg_3_GetIntStatus();

    CyDelay(2000u);
    
    sprintf( str, "REG~reg:%8x\n", ShiftReg_3_ReadRegValue());
    UART_1_PutString(str);
    _tmp = dat_idx;
    CyDelay(2000u); 
    _tmp = dat_idx - _tmp;
    //sprintf( str, "----tmp:%12ld %12ld\n", _tmp1, _tmp2);
    sprintf( str, "----tmp:%8ld\n", _tmp);
    UART_1_PutString(str);
    CyDelay(500u); 


    ShiftIn_1_Write(0u);
    Clock_5_Write(1u);
    Clock_5_Write(0u);
    
    sprintf( str, "SL0~reg:%8x\n", ShiftReg_3_ReadRegValue());
    UART_1_PutString(str);
    CyDelay(2000u); 

    UART_1_PutString("-- STORE --\n");

    Store_1_Write(1u);
    Clock_5_Write(1u);
    Clock_5_Write(0u);
    Store_1_Write(0u);

    sprintf( str, "----dat:%8x\n", ShiftReg_3_ReadData());
    UART_1_PutString(str);

    CyDelay(2000u); 
    
    
//    
//    sprintf( str, "----dat:%8x\n", ShiftReg_3_ReadData());
//    UART_1_PutString(str);
//    CyDelay(500u); 
//    
//    sprintf( str, "REG~reg:%8x\n", ShiftReg_3_ReadRegValue());
//    UART_1_PutString(str);
//    CyDelay(500u); 

    }
}
    
    
void shift_dma_sim(void)
{
    char str[64];
    //int32 _tmp;
    //uint32 _tmp1, _tmp2;
    uint8 i=0;
    
    UART_1_PutString("\n----- Start ShiftReg Simulation ------\n");

    //sync mode CE95372
    Clock_5_Write(1u);
    Clock_5_Write(0u);

    while(1)
    {
    
    ShiftIn_1_Write(1u);
    Clock_5_Write(1u);
    Clock_5_Write(0u);
    
    sprintf( str, "-- SHIFT LEFT 1\n"); //, ShiftReg_3_ReadRegValue());
    UART_1_PutString(str);
    CyDelay(2000u); 
    
    ShiftIn_1_Write(0u);
    Clock_5_Write(1u);
    Clock_5_Write(0u);
    
    sprintf( str, "SL0~reg:%8x\n", ShiftReg_3_ReadRegValue());
    UART_1_PutString(str);
    CyDelay(500u); 
    
    ShiftIn_1_Write(1u);
    Clock_5_Write(1u);
    Clock_5_Write(0u);
    
    sprintf( str, "SL1~reg:%8x\n", ShiftReg_3_ReadRegValue());
    UART_1_PutString(str);
    CyDelay(500u); 
    
    Store_1_Write(1u);
    Clock_5_Write(1u);
    Store_1_Write(0u);
    Clock_5_Write(0u);

    CyDelay(1u); 

    sprintf( str, "--- STO::dat8[%d]:%8x\n", i, dat8[i]);
    i= (i+1)%16;
    UART_1_PutString(str);
    CyDelay(1000u);

    }
}
 

int main()
{
    uint32 _delay=1000u;
    uint32 _tmp;
    uint32 i;
    
    dat_idx=0;
    
    char str[64];
    
    setup_DMA1();
    setup_DMA2();
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

//    isr_1_Start();
    Vreg3v3_1_Start();
    Opamp_1_Start();
    UART_1_Start();
    ShiftReg_1_Start();
    //Counter_1_Start();
    ShiftReg_2_Start();
    ShiftReg_3_Start();
    ShiftReg_4_Start();
    
    CyDelay(500u); 
    
    ResetIn_Write(1u);
    ResetIn_Write(0u);
    ResetIn_Write(1u);
//    isr_2_Start();
    ResetIn_Write(0u);
   
    shift_dma_sim();
    
    shift_sim();
    
    UART_1_PutString("\n----- Start USBIE Simulation ------\n");
    
    while(dat_idx<BUFFER_SIZE)
    {  sprintf( str, "~idx:%8lx\n", dat_idx);
        UART_1_PutString(str);
        CyDelay(500u); 
    }
    
    for( i=0; i<BUFFER_SIZE; i++)
    {          
        _tmp= dat[i];
    
        sprintf( str, "~:%8lx - %8lx - %8lx\n", _tmp, dat2[i], dat3[i]);
        UART_1_PutString(str);
        CyDelay(_delay/2);                  
    }
    
    for(;;)
    {
        LED_Write(1);
        //UART_1_PutChar('1');
        /* Place your application code here. */
        //_tmp= ShiftReg_1_ReadData();
        sprintf( str, "DAT:%8lx\n", _dat);
        UART_1_PutString(str);
        CyDelay(_delay);
        
        LED_Write(0);
        //UART_1_PutChar('0');
        //_tmp= ShiftReg_1_ReadRegValue();
        //sprintf( str, "REG:%8lx\n", _tmp);
        //UART_1_PutString(str);
        CyDelay(_delay);
       
    }
}

void setup_DMA1()
{
    /* Defines for DMA_1 */
#define DMA_1_BYTES_PER_BURST 1
#define DMA_1_REQUEST_PER_BURST 1
#define DMA_1_SRC_BASE (CYDEV_PERIPH_BASE)
#define DMA_1_DST_BASE (CYREG_SRAM_DATA_MBASE)

    /* Variable declarations for DMA_1 */
    /* Move these variable declarations to the top of the function */
    uint8 DMA_1_Chan;
    uint8 DMA_1_TD[1];

    /* DMA Configuration for DMA_1 */
    DMA_1_Chan = DMA_1_DmaInitialize(DMA_1_BYTES_PER_BURST, DMA_1_REQUEST_PER_BURST, 
        HI16(DMA_1_SRC_BASE), HI16(DMA_1_DST_BASE));
    DMA_1_TD[0] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(DMA_1_TD[0], 16, DMA_1_TD[0], TD_INC_DST_ADR);
    CyDmaTdSetAddress(DMA_1_TD[0], LO16((uint32)ShiftReg_3_OUT_FIFO_VAL_LSB_PTR), LO16((uint32)dat8));
    CyDmaChSetInitialTd(DMA_1_Chan, DMA_1_TD[0]);
    CyDmaChEnable(DMA_1_Chan, 1);

}

void setup_DMA2()
{

/* Defines for DMA_2 */
#define DMA_2_BYTES_PER_BURST 1
#define DMA_2_REQUEST_PER_BURST 1
#define DMA_2_SRC_BASE (CYDEV_PERIPH_BASE)
#define DMA_2_DST_BASE (CYREG_SRAM_DATA_MBASE)

    /* Variable declarations for DMA_2 */
    /* Move these variable declarations to the top of the function */
    uint8 DMA_2_Chan;
    uint8 DMA_2_TD[1];

    /* DMA Configuration for DMA_2 */
    DMA_2_Chan = DMA_2_DmaInitialize(DMA_2_BYTES_PER_BURST, DMA_2_REQUEST_PER_BURST, 
        HI16(DMA_2_SRC_BASE), HI16(DMA_2_DST_BASE));
    DMA_2_TD[0] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(DMA_2_TD[0], 1, DMA_2_TD[0], DMA_2__TD_TERMOUT_EN);
    CyDmaTdSetAddress(DMA_2_TD[0], LO16((uint32)ShiftReg_3_SR_STATUS_PTR), LO16((uint32)(&_dat_status)));
    CyDmaChSetInitialTd(DMA_2_Chan, DMA_2_TD[0]);
    CyDmaChEnable(DMA_2_Chan, 1);  
    
}


/* [] END OF FILE */
