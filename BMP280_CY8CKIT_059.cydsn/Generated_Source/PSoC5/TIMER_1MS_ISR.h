/*******************************************************************************
* File Name: TIMER_1MS_ISR.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_TIMER_1MS_ISR_H)
#define CY_ISR_TIMER_1MS_ISR_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void TIMER_1MS_ISR_Start(void);
void TIMER_1MS_ISR_StartEx(cyisraddress address);
void TIMER_1MS_ISR_Stop(void);

CY_ISR_PROTO(TIMER_1MS_ISR_Interrupt);

void TIMER_1MS_ISR_SetVector(cyisraddress address);
cyisraddress TIMER_1MS_ISR_GetVector(void);

void TIMER_1MS_ISR_SetPriority(uint8 priority);
uint8 TIMER_1MS_ISR_GetPriority(void);

void TIMER_1MS_ISR_Enable(void);
uint8 TIMER_1MS_ISR_GetState(void);
void TIMER_1MS_ISR_Disable(void);

void TIMER_1MS_ISR_SetPending(void);
void TIMER_1MS_ISR_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the TIMER_1MS_ISR ISR. */
#define TIMER_1MS_ISR_INTC_VECTOR            ((reg32 *) TIMER_1MS_ISR__INTC_VECT)

/* Address of the TIMER_1MS_ISR ISR priority. */
#define TIMER_1MS_ISR_INTC_PRIOR             ((reg8 *) TIMER_1MS_ISR__INTC_PRIOR_REG)

/* Priority of the TIMER_1MS_ISR interrupt. */
#define TIMER_1MS_ISR_INTC_PRIOR_NUMBER      TIMER_1MS_ISR__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable TIMER_1MS_ISR interrupt. */
#define TIMER_1MS_ISR_INTC_SET_EN            ((reg32 *) TIMER_1MS_ISR__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the TIMER_1MS_ISR interrupt. */
#define TIMER_1MS_ISR_INTC_CLR_EN            ((reg32 *) TIMER_1MS_ISR__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the TIMER_1MS_ISR interrupt state to pending. */
#define TIMER_1MS_ISR_INTC_SET_PD            ((reg32 *) TIMER_1MS_ISR__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the TIMER_1MS_ISR interrupt. */
#define TIMER_1MS_ISR_INTC_CLR_PD            ((reg32 *) TIMER_1MS_ISR__INTC_CLR_PD_REG)


#endif /* CY_ISR_TIMER_1MS_ISR_H */


/* [] END OF FILE */
