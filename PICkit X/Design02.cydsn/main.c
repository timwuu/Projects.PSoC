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
#include "project.h"
#include "stdio.h"

// PWM1 Period 199
#define VDD_CMP_MAX PWM_1_INIT_PERIOD_VALUE   
// Vpp=13.77v, fb=3.3v, coeff=4.174
#define VPP_CMP_MAX 131 //(168*PWM_1_INIT_PERIOD_VALUE/255)

int main(void)
{
    char s[64];  
    uint8 vdd_tgt_x, vdd_tgt_fb;
    uint8 vpp_tgt_x, vpp_tgt_fb;    
    float tgt_d,tgt_p;
    uint8 cmp1, cmp2;
    uint16 cnt=0;
    
    UART_1_Start();
    
    ADC_SAR_1_Start();
    ADC_SAR_2_Start();
    
    
    PWM_1_Start();
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    UART_1_PutString("---Program Start---\n");
    
    for(;;cnt++)
    {
        /* Place your application code here. */
        
        ADC_SAR_1_StartConvert();
        ADC_SAR_2_StartConvert();
        
        ADC_SAR_1_IsEndConversion(ADC_SAR_1_WAIT_FOR_RESULT);
        ADC_SAR_2_IsEndConversion(ADC_SAR_1_WAIT_FOR_RESULT);
        
        
        vdd_tgt_fb = (uint8)ADC_SAR_1_GetResult8();
        vpp_tgt_fb = (uint8)ADC_SAR_2_GetResult8();

        vdd_tgt_x = 168;  //3.3v
        vpp_tgt_x = 110;  //9v 9/4.174*255/5, coeff: 9.6/2.3=4.174, 110:9v, 168:13.77
        
        if( cnt%1000==0)
        {
            tgt_d = (float)vdd_tgt_fb/51.0;        
            tgt_p = (float)vpp_tgt_fb/12.21875;  //51.0;                  
            sprintf( s,"VDD_TGT_FB: %d.%02d,  VPP_TGT_FB: %d.%02d\n", (int)tgt_d, (int)((tgt_d-(int)tgt_d)*100.0), (int)tgt_p, (int)((tgt_p-(int)tgt_p)*100.0));
            UART_1_PutString(s);
        }
        
        if( vdd_tgt_fb > vdd_tgt_x )
        {   
            cmp1 = PWM_1_ReadCompare1();
            
            if(cmp1>0)
            {            
                cmp1--;                  
                PWM_1_WriteCompare1( cmp1);
            }
            
        }
        else if( vdd_tgt_fb < vdd_tgt_x)
        {
            cmp1 = PWM_1_ReadCompare1();
            
            if(cmp1<VDD_CMP_MAX)
            {
                cmp1++;     
                PWM_1_WriteCompare1( cmp1);
            }
        }
        
        //wait for vdd_tgt stable?
        if( vpp_tgt_fb > vpp_tgt_x )
        {   
            cmp2 = PWM_1_ReadCompare2();
            
            if(cmp2>0)
            {            
                cmp2--;                  
                PWM_1_WriteCompare2( cmp2);
            }
            
        }
        else if( vpp_tgt_fb < vpp_tgt_x)
        {
            cmp2 = PWM_1_ReadCompare2();
            
            if(cmp2<VPP_CMP_MAX)
            {
                cmp2++;     
                PWM_1_WriteCompare2( cmp2);
            }
        }
        
        
        CyDelay(1);
    }
    
    return 1;
}

/* [] END OF FILE */
