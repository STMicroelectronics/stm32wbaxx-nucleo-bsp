/**
  ******************************************************************************
  * @file    stm32wbaxx_nucleo_xspi.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm32wbaxx_nucleo_xspi.c driver.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32WBAXX_NUCLEO_XSPI_H
#define STM32WBAXX_NUCLEO_XSPI_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32wbaxx_nucleo_conf.h"
#include "stm32wbaxx_nucleo_errno.h"
#include "../Components/mx25r3235f/mx25r3235f.h"


/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32WBAXX_NUCLEO
  * @{
  */

/** @addtogroup STM32WBAXX_NUCLEO_XSPI
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup STM32WBAXX_NUCLEO_XSPI_Exported_Types STM32WBAXX_NUCLEO XSPI Exported Types
  * @{
  */
typedef enum
{
  XSPI_ACCESS_NONE = 0U,         /*!<  Instance not initialized,              */
  XSPI_ACCESS_INDIRECT,          /*!<  Instance use indirect mode access      */
  XSPI_ACCESS_MMP                /*!<  Instance use Memory Mapped Mode read   */
} XSPI_Access_t;

#if (USE_HAL_XSPI_REGISTER_CALLBACKS == 1)
typedef struct
{
  pXSPI_CallbackTypeDef  pMspInitCb;
  pXSPI_CallbackTypeDef  pMspDeInitCb;
} BSP_XSPI_Cb_t;
#endif /* (USE_HAL_XSPI_REGISTER_CALLBACKS == 1) */

typedef struct
{
  uint32_t MemorySize;
  uint32_t ClockPrescaler;
  uint32_t SampleShifting;
} MX_XSPI_InitTypeDef;
/**
  * @}
  */

/** @defgroup STM32WBAXX_NUCLEO_XSPI_Exported_Types STM32WBAXX_NUCLEO XSPI Exported Types
  * @{
  */
#define BSP_XSPI_Info_t                MX25R3235F_Info_t
#define BSP_XSPI_Interface_t           MX25R3235F_Interface_t
#define BSP_XSPI_Transfer_t            MX25R3235F_Transfer_t
#define BSP_XSPI_Erase_t               MX25R3235F_Erase_t

typedef struct
{
  XSPI_Access_t          IsInitialized;  /*!<  Instance access Flash method     */
  BSP_XSPI_Interface_t   InterfaceMode;  /*!<  Flash Interface mode of Instance */
} XSPI_Ctx_t;

typedef struct
{
  BSP_XSPI_Interface_t   InterfaceMode;  /*!<  Current Flash Interface mode */
} BSP_XSPI_Init_t;
/**
  * @}
  */


/* Exported constants --------------------------------------------------------*/
/** @defgroup STM32WBAXX_NUCLEO_XSPI_Exported_Constants STM32WBAXX_NUCLEO XSPI Exported Constants
  * @{
  */

/* Definition for XSPI clock resources */
#define XSPI_CLK_ENABLE()                 __HAL_RCC_XSPI1_CLK_ENABLE()
#define XSPI_CLK_DISABLE()                __HAL_RCC_XSPI1_CLK_DISABLE()
#define XSPI_FORCE_RESET()                __HAL_RCC_XSPI1_FORCE_RESET()
#define XSPI_RELEASE_RESET()              __HAL_RCC_XSPI1_RELEASE_RESET()

/* Definition for XSPI Pins */
/* XSPI_CS */
#define XSPI_CS_PIN                       GPIO_PIN_2
#define XSPI_CS_GPIO_PORT                 GPIOA
#define XSPI_CS_PIN_AF                    GPIO_AF11_XSPI1
#define XSPI_CS_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()

/* XSPI_CLK */
#define XSPI_CLK_PIN                      GPIO_PIN_15
#define XSPI_CLK_GPIO_PORT                GPIOA
#define XSPI_CLK_PIN_AF                   GPIO_AF13_XSPI1
#define XSPI_CLK_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()

/* XSPI_D0 */
#define XSPI_D0_PIN                       GPIO_PIN_3
#define XSPI_D0_GPIO_PORT                 GPIOB
#define XSPI_D0_PIN_AF                    GPIO_AF13_XSPI1
#define XSPI_D0_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()

/* XSPI_D1 */
#define XSPI_D1_PIN                       GPIO_PIN_4
#define XSPI_D1_GPIO_PORT                 GPIOB
#define XSPI_D1_PIN_AF                    GPIO_AF11_XSPI1
#define XSPI_D1_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()

/* XSPI_D2 */
#define XSPI_D2_PIN                       GPIO_PIN_5
#define XSPI_D2_GPIO_PORT                 GPIOB
#define XSPI_D2_PIN_AF                    GPIO_AF10_XSPI1
#define XSPI_D2_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()

/* XSPI_D3 */
#define XSPI_D3_PIN                       GPIO_PIN_6
#define XSPI_D3_GPIO_PORT                 GPIOB
#define XSPI_D3_PIN_AF                    GPIO_AF10_XSPI1
#define XSPI_D3_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()

/**
  * @}
  */

/** @defgroup STM32WBAXX_NUCLEO_XSPI_Exported_Constants STM32WBAXX_NUCLEO XSPI Exported Constants
  * @{
  */
#define XSPI_INSTANCES_NUMBER         1U

/* Definition for XSPI modes */
#define BSP_XSPI_SPI_MODE (BSP_XSPI_Interface_t)MX25R3235F_SPI_MODE      /* 1 Cmd, 1 Address and 1 Data Lines */
#define BSP_XSPI_QPI_MODE (BSP_XSPI_Interface_t)MX25R3235F_QUAD_IO_MODE  /* 1 Cmd, 4 Address and 4 Data Lines */

/* XSPI erase types */
#define BSP_XSPI_ERASE_4K             MX25R3235F_ERASE_4K
#define BSP_XSPI_ERASE_64K            MX25R3235F_ERASE_64K

/* XSPI block sizes */
#define BSP_XSPI_BLOCK_4K             MX25R3235F_SUBSECTOR_4K
#define BSP_XSPI_BLOCK_64K            MX25R3235F_SECTOR_64K
/**
  * @}
  */


/* Exported variables --------------------------------------------------------*/
/** @defgroup STM32WBAXX_NUCLEO_XSPI_Exported_Variables STM32WBAXX_NUCLEO XSPI Exported Variables
  * @{
  */
extern XSPI_HandleTypeDef hxspi[XSPI_INSTANCES_NUMBER];
extern XSPI_Ctx_t Xspi_Ctx[XSPI_INSTANCES_NUMBER];
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup STM32WBAXX_NUCLEO_XSPI_Exported_Functions STM32WBAXX_NUCLEO XSPI Exported Functions
  * @{
  */
int32_t BSP_XSPI_Init(uint32_t Instance, BSP_XSPI_Init_t *Init);
int32_t BSP_XSPI_DeInit(uint32_t Instance);
#if (USE_HAL_XSPI_REGISTER_CALLBACKS == 1)
int32_t BSP_XSPI_RegisterMspCallbacks(uint32_t Instance, BSP_XSPI_Cb_t *CallBacks);
int32_t BSP_XSPI_RegisterDefaultMspCallbacks(uint32_t Instance);
#endif /* (USE_HAL_XSPI_REGISTER_CALLBACKS == 1) */
int32_t BSP_XSPI_Read(uint32_t Instance, uint8_t *pData, uint32_t ReadAddr, uint32_t Size);
int32_t BSP_XSPI_Write(uint32_t Instance, const uint8_t *pData, uint32_t WriteAddr, uint32_t Size);
int32_t BSP_XSPI_Erase_Block(uint32_t Instance, uint32_t BlockAddress, BSP_XSPI_Erase_t BlockSize);
int32_t BSP_XSPI_Erase_Chip(uint32_t Instance);
int32_t BSP_XSPI_GetStatus(uint32_t Instance);
int32_t BSP_XSPI_GetInfo(uint32_t Instance, BSP_XSPI_Info_t *pInfo);
int32_t BSP_XSPI_EnableMemoryMappedMode(uint32_t Instance);
int32_t BSP_XSPI_DisableMemoryMappedMode(uint32_t Instance);
int32_t BSP_XSPI_ReadID(uint32_t Instance, uint8_t *Id);
int32_t BSP_XSPI_SuspendErase(uint32_t Instance);
int32_t BSP_XSPI_ResumeErase(uint32_t Instance);
int32_t BSP_XSPI_EnterDeepPowerDown(uint32_t Instance);
int32_t BSP_XSPI_LeaveDeepPowerDown(uint32_t Instance);

/* These functions can be modified in case the current settings
   need to be changed for specific application needs */
HAL_StatusTypeDef MX_XSPI_Init(XSPI_HandleTypeDef *hxspi, MX_XSPI_InitTypeDef *Init);
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32WBAXX_NUCLEO_XSPI_H */
