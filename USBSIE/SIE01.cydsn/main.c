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
uint32 dat32[BUFFER_SIZE];
uint32 dat32A[BUFFER_SIZE] __attribute__ ((section(".ram")));
uint32 dat32B[BUFFER_SIZE] __attribute__ ((section(".ram2")));
uint16 dat16A[BUFFER_SIZE];
uint16 dat16B[BUFFER_SIZE];
uint16 _dat16;
uint8 _dat_status;
uint8 dat8[BUFFER_SIZE];
uint8 dat8A[BUFFER_SIZE];
uint8 dat8B[BUFFER_SIZE];
uint32 _dat;
uint32 _dat32A __attribute__ ((section(".ram")));
uint32 _dat32B __attribute__ ((section(".ram2")));
uint8 _dat8;

void setup_DMA_SR1A(void);
void setup_DMA_SR1B(void);

void shift_dma_sim3(void)
{
    char str[64];
    uint16 i=0;
    
    for( i=0; i<BUFFER_SIZE; i++) dat32A[i]=i;
    
    UART_1_PutString("\n----- Start ShiftReg Simulation 32bit ------\n");
    CyDelay(500);

    sprintf(str, "reg1/2/3/4:%04x/%04x/%04x/%04x\n",
          LO16((uint32)Status_Reg_1_Status_PTR),LO16((uint32)Status_Reg_2_Status_PTR),
          LO16((uint32)Status_Reg_3_Status_PTR),LO16((uint32)Status_Reg_4_Status_PTR));
    
    UART_1_PutString(str);
    CyDelay(2000);

    DMA_SR1A_DmaRelease();
    DMA_SR1B_DmaRelease();
    
    while(1)
    {
        UART_1_PutString("\n----- dat32A ------\n");
    
        for( i=0; i<BUFFER_SIZE; i++)
        {   
            sprintf( str, "~:dat32[%2d] - %08lx\n", i, dat32A[i]);
            UART_1_PutString(str);
            CyDelay(500);                  
        }
        
        CyDelay(2000);

        UART_1_PutString("\n----- dat32B ------\n");

    
        for( i=0; i<BUFFER_SIZE; i++)
        {   
            sprintf( str, "~:dat32[%2d] - %08lx\n", i, dat32B[i]);
            UART_1_PutString(str);
            CyDelay(500);                  
        }
    }
}

int main()
{
    uint32 _delay=1000u;
    uint32 _tmp;
    uint32 i;
    
    dat_idx=0;
    
    char str[64];
    
    setup_DMA_SR1A();
    setup_DMA_SR1B();
     
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    SyncCounter_Start();
    Vreg3v3_1_Start();
    Opamp_1_Start();
    UART_1_Start();
    
    CyDelay(500u); 
    
    shift_dma_sim3();
    
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

void setup_DMA_SR1A()
{
    /* Defines for DMA_SR1A */
#define DMA_SR1A_BYTES_PER_BURST 1
#define DMA_SR1A_REQUEST_PER_BURST 0
#define DMA_SR1A_SRC_BASE (CYDEV_PERIPH_BASE)
#define DMA_SR1A_DST_BASE (CYREG_SRAM_DATA_MBASE)

    /* Variable declarations for DMA_SR1A */
    /* Move these variable declarations to the top of the function */
    uint8 DMA_SR1A_Chan;
    uint8 DMA_SR1A_TD[1];

    /* Variable declarations for DMA_SR2A */
    /* Move these variable declarations to the top of the function */
    uint8 DMA_SR2A_Chan;
    uint8 DMA_SR2A_TD[1];

    /* DMA Configuration for DMA_SR1A */
    DMA_SR1A_Chan = DMA_SR1A_DmaInitialize(DMA_SR1A_BYTES_PER_BURST, DMA_SR1A_REQUEST_PER_BURST, 
        HI16(DMA_SR1A_SRC_BASE), HI16(DMA_SR1A_DST_BASE));
    DMA_SR1A_TD[0] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(DMA_SR1A_TD[0], 4, DMA_SR1A_TD[0], DMA_SR1A__TD_TERMOUT_EN | TD_INC_SRC_ADR | TD_INC_DST_ADR);
    CyDmaTdSetAddress(DMA_SR1A_TD[0], LO16((uint32)Status_Reg_1_Status_PTR), LO16((uint32)&_dat32A));
    CyDmaChSetInitialTd(DMA_SR1A_Chan, DMA_SR1A_TD[0]);
    CyDmaChEnable(DMA_SR1A_Chan, 1);
    
    /* Defines for DMA_SR2A */
#define DMA_SR2A_BYTES_PER_BURST 4
#define DMA_SR2A_REQUEST_PER_BURST 1
#define DMA_SR2A_SRC_BASE (CYDEV_SRAM_BASE)
#define DMA_SR2A_DST_BASE (CYDEV_SRAM_BASE)

    /* DMA Configuration for DMA_SR2A */
    DMA_SR2A_Chan = DMA_SR2A_DmaInitialize(DMA_SR2A_BYTES_PER_BURST, DMA_SR2A_REQUEST_PER_BURST, 
        HI16(DMA_SR2A_SRC_BASE), HI16(DMA_SR2A_DST_BASE));
    DMA_SR2A_TD[0] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(DMA_SR2A_TD[0], BUFFER_SIZE<<2u, DMA_SR2A_TD[0], DMA_SR2A__TD_TERMOUT_EN |TD_INC_DST_ADR);
    CyDmaTdSetAddress(DMA_SR2A_TD[0], LO16((uint32)&_dat32A), LO16((uint32)dat32A));
    CyDmaChSetInitialTd(DMA_SR2A_Chan, DMA_SR2A_TD[0]);
    CyDmaChEnable(DMA_SR2A_Chan, 1);

}

void setup_DMA_SR1B()
{
    /* Defines for DMA_SR1B */
#define DMA_SR1B_BYTES_PER_BURST 1
#define DMA_SR1B_REQUEST_PER_BURST 0
#define DMA_SR1B_SRC_BASE (CYDEV_PERIPH_BASE)
#define DMA_SR1B_DST_BASE (CYREG_SRAM_DATA_MBASE)

    /* Variable declarations for DMA_SR1B */
    /* Move these variable declarations to the top of the function */
    uint8 DMA_SR1B_Chan;
    uint8 DMA_SR1B_TD[1];

    /* Variable declarations for DMA_SR2B */
    /* Move these variable declarations to the top of the function */
    uint8 DMA_SR2B_Chan;
    uint8 DMA_SR2B_TD[1];

    /* DMA Configuration for DMA_SR1B */
    DMA_SR1B_Chan = DMA_SR1B_DmaInitialize(DMA_SR1B_BYTES_PER_BURST, DMA_SR1B_REQUEST_PER_BURST, 
        HI16(DMA_SR1B_SRC_BASE), HI16(DMA_SR1B_DST_BASE));
    DMA_SR1B_TD[0] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(DMA_SR1B_TD[0], 4, DMA_SR1B_TD[0], DMA_SR1B__TD_TERMOUT_EN | TD_INC_SRC_ADR | TD_INC_DST_ADR);
    CyDmaTdSetAddress(DMA_SR1B_TD[0], LO16((uint32)Status_Reg_1_Status_PTR), LO16((uint32)&_dat32B));
    CyDmaChSetInitialTd(DMA_SR1B_Chan, DMA_SR1B_TD[0]);
    CyDmaChEnable(DMA_SR1B_Chan, 1);
    
    /* Defines for DMA_SR2B */
#define DMA_SR2B_BYTES_PER_BURST 4
#define DMA_SR2B_REQUEST_PER_BURST 1
#define DMA_SR2B_SRC_BASE (CYDEV_SRAM_BASE)
#define DMA_SR2B_DST_BASE (CYDEV_SRAM_BASE)

    /* DMA Configuration for DMA_SR2B */
    DMA_SR2B_Chan = DMA_SR2B_DmaInitialize(DMA_SR2B_BYTES_PER_BURST, DMA_SR2B_REQUEST_PER_BURST, 
        HI16(DMA_SR2B_SRC_BASE), HI16(DMA_SR2B_DST_BASE));
    DMA_SR2B_TD[0] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(DMA_SR2B_TD[0], BUFFER_SIZE<<2u, DMA_SR2B_TD[0], DMA_SR2B__TD_TERMOUT_EN | TD_INC_DST_ADR);
    CyDmaTdSetAddress(DMA_SR2B_TD[0], LO16((uint32)&_dat32B), LO16((uint32)dat32B));
    CyDmaChSetInitialTd(DMA_SR2B_Chan, DMA_SR2B_TD[0]);
    CyDmaChEnable(DMA_SR2B_Chan, 1);

}

/* [] END OF FILE */
