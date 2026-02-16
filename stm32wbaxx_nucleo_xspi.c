/**
  ******************************************************************************
  * @file    stm32wbaxx_nucleo_xspi.c
  * @author  MCD Application Team
  * @brief   This file includes a standard driver for the MX25R3235F
  *          XSPI memory mounted on the STM32WBAXX-NUCLEO board.
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
  @verbatim
  ==============================================================================
                     ##### How to use this driver #####
  ==============================================================================
  [..]
   (#) This driver is used to drive the MX25R3235F Quad NOR external memory
       mounted on STM32WBAXX_NUCLEO board.

   (#) This driver need specific component driver (MX25R3235F) to be included with.

   (#) MX25R3235F Initialization steps:
       (++) Initialize the XSPI external memory using the BSP_XSPI_Init() function. This
            function includes the MSP layer hardware resources initialization and the
            XSPI interface with the external memory.

   (#) MX25R3235F Quad NOR memory operations
       (++) XSPI memory can be accessed with read/write operations once it is
            initialized.
            Read/write operation can be performed with AHB access using the functions
            BSP_XSPI_Read()/BSP_XSPI_Write().
       (++) The function BSP_XSPI_GetInfo() returns the configuration of the XSPI memory.
            (see the XSPI memory data sheet)
       (++) Perform erase block operation using the function BSP_XSPI_Erase_Block() and by
            specifying the block address. You can perform an erase operation of the whole
            chip by calling the function BSP_XSPI_Erase_Chip().
       (++) The function BSP_XSPI_GetStatus() returns the current status of the XSPI memory.
            (see the XSPI memory data sheet)
       (++) The memory access can be configured in memory-mapped mode with the call of
            function BSP_XSPI_EnableMemoryMapped(). To go back in indirect mode, the
            function BSP_XSPI_DisableMemoryMapped() should be used.
       (++) The erase operation can be suspend and resume with using functions
            BSP_XSPI_SuspendErase() and BSP_XSPI_ResumeErase()
       (++) It is possible to put the memory in deep power-down mode to reduce its consumption.
            For this, the function BSP_XSPI_EnterDeepPowerDown() should be called. To leave
            the deep power-down mode, the function BSP_XSPI_LeaveDeepPowerDown() should be called.
       (++) The function BSP_XSPI_ReadID() returns the identifier of the memory
            (see the XSPI memory data sheet)
       (++) The configuration of the interface between peripheral and memory is done by
            the function XSPI_ConfigFlash(), two modes are possible :
            - SPI : instruction, address and data on one line
            - QPI : instruction on one line while address and data on four lines with sampling on one edge of clock

  @endverbatim
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32wbaxx_nucleo_xspi.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32WBAXX_NUCLEO
  * @{
  */

/** @defgroup STM32WBAXX_NUCLEO_XSPI STM32WBAXX_NUCLEO XSPI
  * @{
  */

/* Exported variables --------------------------------------------------------*/
/** @addtogroup STM32WBAXX_NUCLEO_XSPI_Exported_Variables
  * @{
  */
XSPI_HandleTypeDef hxspi[XSPI_INSTANCES_NUMBER] = {0};
XSPI_Ctx_t Xspi_Ctx[XSPI_INSTANCES_NUMBER] =
{
  {
    XSPI_ACCESS_NONE,
    MX25R3235F_SPI_MODE
  }
};
/**
  * @}
  */


/* Private constants --------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/** @defgroup STM32WBAXX_NUCLEO_XSPI_Private_Variables STM32WBAXX_NUCLEO XSPI Private Variables
  * @{
  */
#if (USE_HAL_XSPI_REGISTER_CALLBACKS == 1)
static uint32_t Xspi_IsMspCbValid[XSPI_INSTANCES_NUMBER] = {0};
#endif /* USE_HAL_XSPI_REGISTER_CALLBACKS */
/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/

/** @defgroup STM32WBAXX_NUCLEO_XSPI_Private_Functions STM32WBAXX_NUCLEO XSPI Private Functions
  * @{
  */
static void    XSPI_MspInit(XSPI_HandleTypeDef *hxspi);
static void    XSPI_MspDeInit(XSPI_HandleTypeDef *hxspi);
static int32_t XSPI_ResetMemory(uint32_t Instance);
static int32_t XSPI_EnterQPIMode(uint32_t Instance);
static int32_t XSPI_ExitQPIMode(uint32_t Instance);
static void    XSPI_DLYB_Enable(uint32_t Instance);
static int32_t XSPI_AutoPollingMemReady(XSPI_HandleTypeDef *hxspi);
static int32_t XSPI_ConfigFlash(uint32_t Instance, BSP_XSPI_Interface_t Mode);

/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/

/** @addtogroup STM32WBAXX_NUCLEO_XSPI_Exported_Functions
  * @{
  */

/**
  * @brief  Initializes the XSPI interface.
  * @param  Instance   XSPI Instance
  * @param  Init       XSPI Init structure
  * @retval BSP status
  */
int32_t BSP_XSPI_Init(uint32_t Instance, BSP_XSPI_Init_t *Init)
{
  int32_t ret;
  BSP_XSPI_Info_t pInfo;
  MX_XSPI_InitTypeDef xspi_init;

  /* Check if the instance is supported */
  if (Instance >= XSPI_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Check if the instance is already initialized */
    if (Xspi_Ctx[Instance].IsInitialized == XSPI_ACCESS_NONE)
    {
#if (USE_HAL_XSPI_REGISTER_CALLBACKS == 0)
      /* Msp XSPI initialization */
      XSPI_MspInit(&hxspi[Instance]);
#else
      /* Register the XSPI MSP Callbacks */
      if (Xspi_IsMspCbValid[Instance] == 0UL)
      {
        if (BSP_XSPI_RegisterDefaultMspCallbacks(Instance) != BSP_ERROR_NONE)
        {
          return BSP_ERROR_PERIPH_FAILURE;
        }
      }
#endif /* USE_HAL_XSPI_REGISTER_CALLBACKS */

      /* Get Flash information of one memory */
      (void)MX25R3235F_GetFlashInfo(&pInfo);

      /* Fill config structure */
      xspi_init.ClockPrescaler = 8;
      xspi_init.MemorySize     = (uint32_t)POSITION_VAL((uint32_t)pInfo.FlashSize);
      xspi_init.SampleShifting = HAL_XSPI_SAMPLE_SHIFT_NONE;

      /* STM32 XSPI interface initialization */
      if (MX_XSPI_Init(&hxspi[Instance], &xspi_init) != HAL_OK)
      {
        ret = BSP_ERROR_PERIPH_FAILURE;
      }
      else
      {
        /* XSPI Delay Block enable */
        XSPI_DLYB_Enable(Instance);

        /* XSPI memory reset */
        if (XSPI_ResetMemory(Instance) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }/* Check if memory is ready */
        else if (XSPI_AutoPollingMemReady(&hxspi[Instance]) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }/* Configure the memory */
        else if (XSPI_ConfigFlash(Instance, Init->InterfaceMode) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
    }
    else
    {
      ret = BSP_ERROR_NONE;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  De-Initializes the XSPI interface.
  * @param  Instance   XSPI Instance
  * @retval BSP status
  */
int32_t BSP_XSPI_DeInit(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= XSPI_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Check if the instance is already initialized */
    if (Xspi_Ctx[Instance].IsInitialized != XSPI_ACCESS_NONE)
    {
      /* Disable Memory mapped mode */
      if (Xspi_Ctx[Instance].IsInitialized == XSPI_ACCESS_MMP)
      {
        if (BSP_XSPI_DisableMemoryMappedMode(Instance) != BSP_ERROR_NONE)
        {
          return BSP_ERROR_COMPONENT_FAILURE;
        }
      }

      /* Set default Xspi_Ctx values */
      Xspi_Ctx[Instance].IsInitialized = XSPI_ACCESS_NONE;
      Xspi_Ctx[Instance].InterfaceMode = BSP_XSPI_SPI_MODE;

#if (USE_HAL_XSPI_REGISTER_CALLBACKS == 0)
      XSPI_MspDeInit(&hxspi[Instance]);
#endif /* (USE_HAL_XSPI_REGISTER_CALLBACKS == 0) */

      /* Call the DeInit function to reset the driver */
      if (HAL_XSPI_DeInit(&hxspi[Instance]) != HAL_OK)
      {
        ret = BSP_ERROR_PERIPH_FAILURE;
      }
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Initializes the XSPI interface.
  * @param  hxspi          XSPI handle
  * @param  Init           XSPI config structure
  * @retval BSP status
  */
__weak HAL_StatusTypeDef MX_XSPI_Init(XSPI_HandleTypeDef *hxspi, MX_XSPI_InitTypeDef *Init)
{
  /* XSPI initialization */
  hxspi->Instance = XSPI1;

  hxspi->Init.FifoThresholdByte       = 1U;
  hxspi->Init.MemoryMode              = HAL_XSPI_SINGLE_MEM;
  hxspi->Init.MemorySize              = Init->MemorySize; /* 512 MBits */
  hxspi->Init.ChipSelectHighTimeCycle = 2U;
  hxspi->Init.FreeRunningClock        = HAL_XSPI_FREERUNCLK_DISABLE;
  hxspi->Init.ClockMode               = HAL_XSPI_CLOCK_MODE_0;
  hxspi->Init.ClockPrescaler          = Init->ClockPrescaler;
  hxspi->Init.SampleShifting          = Init->SampleShifting;
  hxspi->Init.MemoryType              = HAL_XSPI_MEMTYPE_MICRON;
  hxspi->Init.DelayHoldQuarterCycle   = HAL_XSPI_DHQC_DISABLE;


  return HAL_XSPI_Init(hxspi);
}

#if (USE_HAL_XSPI_REGISTER_CALLBACKS == 1)
/**
  * @brief Default BSP XSPI Msp Callbacks
  * @param Instance      XSPI Instance
  * @retval BSP status
  */
int32_t BSP_XSPI_RegisterDefaultMspCallbacks(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= XSPI_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Register MspInit/MspDeInit Callbacks */
    if (HAL_XSPI_RegisterCallback(&hxspi[Instance], HAL_XSPI_MSP_INIT_CB_ID, XSPI_MspInit) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_XSPI_RegisterCallback(&hxspi[Instance], HAL_XSPI_MSP_DEINIT_CB_ID, XSPI_MspDeInit) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      Xspi_IsMspCbValid[Instance] = 1U;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief BSP XSPI Msp Callback registering
  * @param Instance     XSPI Instance
  * @param CallBacks    pointer to MspInit/MspDeInit callbacks functions
  * @retval BSP status
  */
int32_t BSP_XSPI_RegisterMspCallbacks(uint32_t Instance, BSP_XSPI_Cb_t *CallBacks)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= XSPI_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Register MspInit/MspDeInit Callbacks */
    if (HAL_XSPI_RegisterCallback(&hxspi[Instance], HAL_XSPI_MSP_INIT_CB_ID, CallBacks->pMspInitCb) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_XSPI_RegisterCallback(&hxspi[Instance],
                                       HAL_XSPI_MSP_DEINIT_CB_ID, CallBacks->pMspDeInitCb) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      Xspi_IsMspCbValid[Instance] = 1U;
    }
  }

  /* Return BSP status */
  return ret;
}
#endif /* (USE_HAL_XSPI_REGISTER_CALLBACKS == 1) */

/**
  * @brief  Reads an amount of data from the XSPI memory.
  * @param  Instance  XSPI instance
  * @param  pData     Pointer to data to be read
  * @param  ReadAddr  Read start address
  * @param  Size      Size of data to read
  * @retval BSP status
  */
int32_t BSP_XSPI_Read(uint32_t Instance, uint8_t *pData, uint32_t ReadAddr, uint32_t Size)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= XSPI_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (MX25R3235F_Read(&hxspi[Instance], Xspi_Ctx[Instance].InterfaceMode, pData, ReadAddr, Size) != MX25R3235F_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_NONE;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Writes an amount of data to the XSPI memory.
  * @param  Instance  XSPI instance
  * @param  pData     Pointer to data to be written
  * @param  WriteAddr Write start address
  * @param  Size      Size of data to write
  * @retval BSP status
  */
int32_t BSP_XSPI_Write(uint32_t Instance, const uint8_t *pData, uint32_t WriteAddr, uint32_t Size)
{
  int32_t ret = BSP_ERROR_NONE;
  uint32_t end_addr;
  uint32_t current_size;
  uint32_t current_addr;
  uint32_t data_addr;

  /* Check if the instance is supported */
  if (Instance >= XSPI_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Calculation of the size between the write address and the end of the page */
    current_size = MX25R3235F_PAGE_SIZE - (WriteAddr % MX25R3235F_PAGE_SIZE);

    /* Check if the size of the data is less than the remaining place in the page */
    if (current_size > Size)
    {
      current_size = Size;
    }

    /* Initialize the address variables */
    current_addr = WriteAddr;
    end_addr = WriteAddr + Size;
    data_addr = (uint32_t)pData;

    /* Perform the write page by page */
    do
    {
      /* Check if Flash busy ? */
      if (XSPI_AutoPollingMemReady(&hxspi[Instance]) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }/* Enable write operations */
      else if (MX25R3235F_WriteEnable(&hxspi[Instance]) != MX25R3235F_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        /* Issue page program command */
        if (MX25R3235F_PageProgram(&hxspi[Instance], Xspi_Ctx[Instance].InterfaceMode, (uint8_t *)data_addr, current_addr,
                                    current_size) != MX25R3235F_OK)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }

        if (ret == BSP_ERROR_NONE)
        {
          /* Configure automatic polling mode to wait for end of program */
          if (XSPI_AutoPollingMemReady(&hxspi[Instance]) != BSP_ERROR_NONE)
          {
            ret = BSP_ERROR_COMPONENT_FAILURE;
          }
          else
          {
            /* Update the address and size variables for next page programming */
            current_addr += current_size;
            data_addr += current_size;
            current_size = ((current_addr + MX25R3235F_PAGE_SIZE) > end_addr)
                           ? (end_addr - current_addr)
                           : MX25R3235F_PAGE_SIZE;
          }
        }
      }
    } while ((current_addr < end_addr) && (ret == BSP_ERROR_NONE));
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Erases the specified block of the XSPI memory.
  * @param  Instance     XSPI instance
  * @param  BlockAddress Block address to erase
  * @param  BlockSize    Erase Block size: MX25R3235F_ERASE_4K, MX25R3235F_ERASE_32K or MX25R3235F_ERASE_64K
  * @retval BSP status
  */
int32_t BSP_XSPI_Erase_Block(uint32_t Instance, uint32_t BlockAddress, BSP_XSPI_Erase_t BlockSize)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= XSPI_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Check Flash busy ? */
    if (XSPI_AutoPollingMemReady(&hxspi[Instance]) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }/* Enable write operations */
    else if (MX25R3235F_WriteEnable(&hxspi[Instance]) != MX25R3235F_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }/* Issue Block Erase command */
    else if (MX25R3235F_BlockErase(&hxspi[Instance],BlockAddress, BlockSize) != MX25R3235F_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_NONE;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Erases the entire XSPI memory.
  * @param  Instance  XSPI instance
  * @retval BSP status
  */
int32_t BSP_XSPI_Erase_Chip(uint32_t Instance)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= XSPI_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Check Flash busy ? */
    if (XSPI_AutoPollingMemReady(&hxspi[Instance]) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }/* Enable write operations */
    else if (MX25R3235F_WriteEnable(&hxspi[Instance]) != MX25R3235F_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }/* Issue Chip erase command */
    else if (MX25R3235F_ChipErase(&hxspi[Instance]) != MX25R3235F_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_NONE;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Reads current status of the XSPI memory.
  * @param  Instance  XSPI instance
  * @retval XSPI memory status: whether busy or not
  */
int32_t BSP_XSPI_GetStatus(uint32_t Instance)
{
  static uint8_t reg[1];
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= XSPI_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (MX25R3235F_ReadSecurityRegister(&hxspi[Instance], reg) != MX25R3235F_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }/* Check the value of the register */
    else if ((reg[0] & (MX25R3235F_SECR_P_FAIL | MX25R3235F_SECR_E_FAIL)) != 0U)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else if ((reg[0] & (MX25R3235F_SECR_PSB | MX25R3235F_SECR_ESB)) != 0U)
    {
      ret = BSP_ERROR_XSPI_SUSPENDED;
    }
    else if (MX25R3235F_ReadStatusRegister(&hxspi[Instance], reg) != MX25R3235F_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }/* Check the value of the register */
    else if ((reg[0] & MX25R3235F_SR_WIP) != 0U)
    {
      ret = BSP_ERROR_BUSY;
    }
    else
    {
      ret = BSP_ERROR_NONE;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Return the configuration of the XSPI memory.
  * @param  Instance  XSPI instance
  * @param  pInfo     pointer on the configuration structure
  * @retval BSP status
  */
int32_t BSP_XSPI_GetInfo(uint32_t Instance, BSP_XSPI_Info_t *pInfo)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= XSPI_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    (void)MX25R3235F_GetFlashInfo(pInfo);
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Configure the XSPI in memory-mapped mode
  * @param  Instance  XSPI instance
  * @retval BSP status
  */
int32_t BSP_XSPI_EnableMemoryMappedMode(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= XSPI_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (MX25R3235F_EnableMemoryMappedMode(&hxspi[Instance], Xspi_Ctx[Instance].InterfaceMode) != MX25R3235F_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else /* Update XSPI context if all operations are well done */
    {
      Xspi_Ctx[Instance].IsInitialized = XSPI_ACCESS_MMP;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Exit form memory-mapped mode
  *         Only 1 Instance can running MMP mode. And it will lock system at this mode.
  * @param  Instance  XSPI instance
  * @retval BSP status
  */
int32_t BSP_XSPI_DisableMemoryMappedMode(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= XSPI_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (Xspi_Ctx[Instance].IsInitialized != XSPI_ACCESS_MMP)
    {
      ret = BSP_ERROR_XSPI_MMP_UNLOCK_FAILURE;
    }/* Abort MMP back to indirect mode */
    else if (HAL_XSPI_Abort(&hxspi[Instance]) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else /* Update XSPI NOR context if all operations are well done */
    {
      Xspi_Ctx[Instance].IsInitialized = XSPI_ACCESS_INDIRECT;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get flash ID 3 Bytes:
  *         Manufacturer ID, Memory type, Memory density
  * @param  Instance  XSPI instance
  * @param  Id Pointer to retrieve ID from memory
  * @retval BSP status
  */
int32_t BSP_XSPI_ReadID(uint32_t Instance, uint8_t *Id)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= XSPI_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (MX25R3235F_ReadID(&hxspi[Instance], Id) != MX25R3235F_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Set Flash to desired Interface mode. And this instance becomes current instance.
  *         If current instance running at MMP mode then this function doesn't work.
  *         Indirect -> Indirect
  * @param  Instance  XSPI instance
  * @param  Mode      XSPI mode
  * @retval BSP status
  */
static int32_t XSPI_ConfigFlash(uint32_t Instance, BSP_XSPI_Interface_t Mode)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= XSPI_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Check if MMP mode locked ************************************************/
    if (Xspi_Ctx[Instance].IsInitialized == XSPI_ACCESS_MMP)
    {
      ret = BSP_ERROR_XSPI_MMP_LOCK_FAILURE;
    }
    else
    {
      /* Setup Flash interface ***************************************************/
      switch (Xspi_Ctx[Instance].InterfaceMode)
      {
        case BSP_XSPI_QPI_MODE :  /* 1-4-4 commands */
          if (Mode != BSP_XSPI_QPI_MODE)
          {
            /* Exit QPI mode */
            ret = XSPI_ExitQPIMode(Instance);
          }
          break;

        case BSP_XSPI_SPI_MODE :  /* 1-1-1 commands, Power on H/W default setting */
        default :
          if (Mode == BSP_XSPI_QPI_MODE)
          {
            /* Enter QPI mode */
            ret = XSPI_EnterQPIMode(Instance);
          }
          break;
      }

      /* Update XSPI context if all operations are well done */
      if (ret == BSP_ERROR_NONE)
      {
        /* Update current status parameter *****************************************/
        Xspi_Ctx[Instance].IsInitialized = XSPI_ACCESS_INDIRECT;
        Xspi_Ctx[Instance].InterfaceMode = Mode;
      }
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  This function suspends an ongoing erase command.
  * @param  Instance  XSPI instance
  * @retval BSP status
  */
int32_t BSP_XSPI_SuspendErase(uint32_t Instance)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= XSPI_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  /* Check whether the device is busy (erase operation is in progress). */
  else if (BSP_XSPI_GetStatus(Instance) != BSP_ERROR_BUSY)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if (MX25R3235F_Suspend(&hxspi[Instance]) != MX25R3235F_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if (BSP_XSPI_GetStatus(Instance) != BSP_ERROR_XSPI_SUSPENDED)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  This function resumes a paused erase command.
  * @param  Instance  XSPI instance
  * @retval BSP status
  */
int32_t BSP_XSPI_ResumeErase(uint32_t Instance)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= XSPI_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  /* Check whether the device is busy (erase operation is in progress). */
  else if (BSP_XSPI_GetStatus(Instance) != BSP_ERROR_XSPI_SUSPENDED)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if (MX25R3235F_Resume(&hxspi[Instance]) != MX25R3235F_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  /*
  When this command is executed, the status register write in progress bit is set to 1, and
  the flag status register program erase controller bit is set to 0. This command is ignored
  if the device is not in a suspended state.
  */
  else if (BSP_XSPI_GetStatus(Instance) != BSP_ERROR_BUSY)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  This function enter the XSPI memory in deep power down mode.
  * @param  Instance  XSPI instance
  * @retval BSP status
  */
int32_t BSP_XSPI_EnterDeepPowerDown(uint32_t Instance)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= XSPI_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (MX25R3235F_EnterPowerDown(&hxspi[Instance]) != MX25R3235F_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  /* ---          Memory takes 10us max to enter deep power down          --- */

  /* Return BSP status */
  return ret;
}

/**
  * @brief  This function leave the XSPI memory from deep power down mode.
  * @param  Instance  XSPI instance
  * @retval BSP status
  */
int32_t BSP_XSPI_LeaveDeepPowerDown(uint32_t Instance)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= XSPI_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (MX25R3235F_NoOperation(&hxspi[Instance]) != MX25R3235F_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  /* --- A NOP command is sent to the memory, as the nCS should be low for at least 20 ns --- */
  /* ---                  Memory takes 30us min to leave deep power down                  --- */

  /* Return BSP status */
  return ret;
}
/**
  * @}
  */

/** @addtogroup STM32WBAXX_NUCLEO_XSPI_Private_Functions
  * @{
  */

/**
  * @brief  Initializes the XSPI MSP.
  * @param  hxspi XSPI handle
  * @retval None
  */
static void XSPI_MspInit(XSPI_HandleTypeDef *hxspi)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* hxspi unused argument(s) compilation warning */
  UNUSED(hxspi);

  /* Enable the XSPI memory interface clock */
  XSPI_CLK_ENABLE();

  /* Reset the XSPI memory interface */
  XSPI_FORCE_RESET();
  XSPI_RELEASE_RESET();

  /* Enable GPIO clocks */
  XSPI_CLK_GPIO_CLK_ENABLE();
  XSPI_CS_GPIO_CLK_ENABLE();
  XSPI_D0_GPIO_CLK_ENABLE();
  XSPI_D1_GPIO_CLK_ENABLE();
  XSPI_D2_GPIO_CLK_ENABLE();
  XSPI_D3_GPIO_CLK_ENABLE();

  /* XSPI CS GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_CS_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = XSPI_CS_PIN_AF;
  HAL_GPIO_Init(XSPI_CS_GPIO_PORT, &GPIO_InitStruct);

  /* XSPI CLK GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_CLK_PIN;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Alternate = XSPI_CLK_PIN_AF;
  HAL_GPIO_Init(XSPI_CLK_GPIO_PORT, &GPIO_InitStruct);

  /* XSPI D0 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_D0_PIN;
  GPIO_InitStruct.Alternate = XSPI_D0_PIN_AF;
  HAL_GPIO_Init(XSPI_D0_GPIO_PORT, &GPIO_InitStruct);

  /* XSPI D1 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_D1_PIN;
  GPIO_InitStruct.Alternate = XSPI_D1_PIN_AF;
  HAL_GPIO_Init(XSPI_D1_GPIO_PORT, &GPIO_InitStruct);

  /* XSPI D2 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_D2_PIN;
  GPIO_InitStruct.Alternate = XSPI_D2_PIN_AF;
  HAL_GPIO_Init(XSPI_D2_GPIO_PORT, &GPIO_InitStruct);

  /* XSPI D3 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_D3_PIN;
  GPIO_InitStruct.Alternate = XSPI_D3_PIN_AF;
  HAL_GPIO_Init(XSPI_D3_GPIO_PORT, &GPIO_InitStruct);
}

/**
  * @brief  De-Initializes the XSPI MSP.
  * @param  hxspi XSPI handle
  * @retval None
  */
static void XSPI_MspDeInit(XSPI_HandleTypeDef *hxspi)
{
  /* hxspi unused argument(s) compilation warning */
  UNUSED(hxspi);

  /* XSPI GPIO pins de-configuration  */
  HAL_GPIO_DeInit(XSPI_CLK_GPIO_PORT, XSPI_CLK_PIN);
  HAL_GPIO_DeInit(XSPI_CS_GPIO_PORT, XSPI_CS_PIN);
  HAL_GPIO_DeInit(XSPI_D0_GPIO_PORT, XSPI_D0_PIN);
  HAL_GPIO_DeInit(XSPI_D1_GPIO_PORT, XSPI_D1_PIN);
  HAL_GPIO_DeInit(XSPI_D2_GPIO_PORT, XSPI_D2_PIN);
  HAL_GPIO_DeInit(XSPI_D3_GPIO_PORT, XSPI_D3_PIN);

  /* Reset the XSPI memory interface */
  XSPI_FORCE_RESET();
  XSPI_RELEASE_RESET();

  /* Disable the XSPI memory interface clock */
  XSPI_CLK_DISABLE();
}

/**
  * @brief  This function reset the XSPI memory.
  * @param  Instance  XSPI instance
  * @retval BSP status
  */
static int32_t XSPI_ResetMemory(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  if (MX25R3235F_ResetEnable(&hxspi[Instance]) != MX25R3235F_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if (MX25R3235F_ResetMemory(&hxspi[Instance]) != MX25R3235F_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    Xspi_Ctx[Instance].IsInitialized = XSPI_ACCESS_INDIRECT;     /* After reset S/W setting to indirect access  */
    Xspi_Ctx[Instance].InterfaceMode = BSP_XSPI_SPI_MODE;    /* After reset H/W back to SPI mode by default */

    /* Wait SWreset CMD is effective and check that memory is ready */
    if (XSPI_AutoPollingMemReady(&hxspi[Instance]) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  This function enables the quad mode of the memory.
  * @param  Instance  XSPI instance
  * @retval BSP status
  */
static int32_t XSPI_EnterQPIMode(uint32_t Instance)
{
  uint8_t reg;

  if (MX25R3235F_ReadStatusRegister(&hxspi[Instance], &reg) != MX25R3235F_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  /* Enable write operations */
  if (MX25R3235F_WriteEnable(&hxspi[Instance]) != MX25R3235F_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  /* Activate/deactivate the Quad mode */
  SET_BIT(reg, MX25R3235F_SR_QE);

  if (MX25R3235F_WriteStatusRegister(&hxspi[Instance], reg) != MX25R3235F_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  /* Wait that memory is ready */
  if (XSPI_AutoPollingMemReady(&hxspi[Instance]) != BSP_ERROR_NONE)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  if (MX25R3235F_ReadStatusRegister(&hxspi[Instance], &reg) != MX25R3235F_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  if ((reg & MX25R3235F_SR_QE) == 0)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  return BSP_ERROR_NONE;
}

/**
  * @brief  This function disables the quad mode of the memory.
  * @param  Instance  XSPI instance
  * @retval BSP status
  */
static int32_t XSPI_ExitQPIMode(uint32_t Instance)
{
  uint8_t reg;

  if (MX25R3235F_ReadStatusRegister(&hxspi[Instance], &reg) != MX25R3235F_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  /* Enable write operations */
  if (MX25R3235F_WriteEnable(&hxspi[Instance]) != MX25R3235F_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  /* Activate/deactivate the Quad mode */
  CLEAR_BIT(reg, MX25R3235F_SR_QE);

  if (MX25R3235F_WriteStatusRegister(&hxspi[Instance], reg) != MX25R3235F_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  /* Wait that memory is ready */
  if (XSPI_AutoPollingMemReady(&hxspi[Instance]) != BSP_ERROR_NONE)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  if (MX25R3235F_ReadStatusRegister(&hxspi[Instance], &reg) != MX25R3235F_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  if ((reg & MX25R3235F_SR_QE) != 0U)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  return BSP_ERROR_NONE;
}

/**
  * @brief  This function enables delay block.
  * @param  Instance  XSPI instance
  * @retval BSP status
  */
static void XSPI_DLYB_Enable(uint32_t Instance)
{
  HAL_XSPI_DLYB_CfgTypeDef  dlyb_cfg;

  (void)HAL_XSPI_DLYB_GetClockPeriod(&hxspi[Instance], &dlyb_cfg);

  /* PhaseSel is divided by 4 (emperic value)*/
  dlyb_cfg.PhaseSel /= 4U;

  /*set delay block configuration*/
  (void)HAL_XSPI_DLYB_SetConfig(&hxspi[Instance], &dlyb_cfg);
}

/**
  * @brief  Polling WIP(Write In Progress) bit become to 0
  *         XSPI;
  * @param  hxspi XSPI handle
  * @retval BSP status
  */
static int32_t XSPI_AutoPollingMemReady(XSPI_HandleTypeDef *hxspi)
{
  uint8_t reg;
  uint32_t timeout = HAL_XSPI_TIMEOUT_DEFAULT_VALUE;

  reg = 0xFFU;

  while(((reg & MX25R3235F_SR_WIP) == 1) && (timeout > 0U))
  {
    if (MX25R3235F_ReadStatusRegister(hxspi, &reg) != MX25R3235F_OK)
    {
      return BSP_ERROR_COMPONENT_FAILURE;
    }
    timeout--;
  }
  if ((reg & MX25R3235F_SR_WIP) == 1)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }
  return BSP_ERROR_NONE;
}

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
