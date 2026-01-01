/***************************************************************************//**
* \file ODriveComHAL.h
* \version 1.0.0
*
* \brief
* ODrive communication hardware abstraction layer.
*
* \details
* Transport-agnostic interface supporting CAN, ABZ, USB, SPI, etc.
* Enables ODrive library to work with any communication backend via vtable
* abstraction. Each transport encodes/decodes ODrive protocol independently.
*
*******************************************************************************/

#ifndef ODRIVE_COM_HAL_H
#define ODRIVE_COM_HAL_H

//========================================================
//      Standard Includes
//========================================================

#include <stdint.h>
#include <stdbool.h>

#include "ODriveCore.h"
#include "ODriveResult.h"

//========================================================
//      Communication Types
//========================================================

/**
 * @brief Supported transport types
 *
 * Specifies the physical communication protocol used to interface
 * with ODrive motor controllers.
 */
typedef enum {
    /**< Unknown or uninitialized transport */
    ODRIVE_TRANSPORT_UNKNOWN = 0x00,

    /**< Universal Asynchronous Serial Interface (UART) */
    ODRIVE_TRANSPORT_UART = 0x01,

    /**< Controller Area Network (CAN 2.0) */
    ODRIVE_TRANSPORT_CAN = 0x02,

    /**< Universal Serial Bus (USB) */
    ODRIVE_TRANSPORT_USB = 0x03,

} odrive_transport_t;

typedef enum {
   CAN_FLAG_RTR = ODRIVE_BIT(0u),
   CAN_FLAG_IDE = ODRIVE_BIT(1u),
} odrive_can_flags;

typedef odrive_flags odrive_com_flags;

//========================================================
//      Transport Message
//========================================================

/**
 * @brief Transport-agnostic message frame
 * 
 * High-level message structure used internally by ODrive.
 * Each transport backend handles encoding/decoding to/from wire format.
 * 
 * @note ODrive CAN uses CAN2.0, thus the max payload size is 8 bytes
 * 
 */
typedef struct {
   /**
    * @brief Command message ID
    * 
    * **CAN (11-bit standard):**
    *   - Bits [10:5]: Node ID (0-63)
    *   - Bits [4:0]: Command ID (0-31)
    *   - Range: 0x000 - 0x7FF
    * 
    * **CAN (29-bit extended):**
    *   - Bits [28:23]: Node ID
    *   - Bits [22:18]: Command ID
    *   - Bits [17:0]: Reserved/unused
    */
   uint32_t msg_id;

   /**< Payload data */
   uint8_t  buffer[64];

   /**< Payload length */
   uint16_t dlc;

   odrive_com_flags flags;

   /**< Node ID (if applicable to transport) */
   uint8_t  node_id;

} odrive_message_t;

//========================================================
//      Callback Types
//========================================================

/**
 * @brief RX message callback
 * 
 * Called when a message is received from transport.
 * Runs in ISR context (for interrupt-driven transports) or
 * from polling thread context.
 */
typedef void (*odrive_com_rx_callback)(const odrive_message_t* msg,
                                       void* user_ctx);

/**
 * @brief Status change callback
 * 
 * Called on transport state changes (connected/disconnected/error).
 */
typedef void (*odrive_com_status_callback)(uint8_t status,
                                           void* user_ctx);

//========================================================
//      Communication Function Prototypes
//========================================================

/**
 * @brief Message transmit operation
 *
 * @param[in] msg Message to transmit
 * @return ODRIVE_RESULT_OK on success
 */
typedef odrive_result_t (*odrive_com_transmit_fn_t)(
    const odrive_message_t* msg);

/**
 * @brief Get transport name operation
 *
 * @param[in] hw_ctx Hardware context
 * @return Human-readable transport name (e.g., "CAN 500kbps", "USB CDC")
 */
typedef const char* (*odrive_com_get_name_fn_t)(void* hw_ctx);

typedef odrive_result_t (*odrive_pfn_event_handler) ();

//========================================================
//      Communication Operartions
//========================================================

/**
 * @brief Communication backend operations
 * 
 */
typedef struct {
   /**
    * @brief Message transmit abstraction
    */
   odrive_com_transmit_fn_t transmit;



   /**
    * @brief Get transport name (for logging/debugging)
    * @return Human-readable transport name (e.g., "CAN 500kbps")
    */
   odrive_com_get_name_fn_t get_name;

} odrive_com_ops_t;

//========================================================
//      Communication Context
//========================================================

/**
 * @brief Communication abstraction layer context
 * 
 * Encapsulates transport operations and hardware state.
 * Opaque to user; allocated by backend factory.
 */
typedef struct {
   /**< Transport type */
   odrive_transport_t transport; 

   /**< Operations vtable */
   odrive_com_ops_t ops;              

   /**< Hardware-specific context */
   odrive_ctx_t hw_ctx;                  

} odrive_com_t;

//========================================================
//      Public HAL Interface
//========================================================

/**
 * @brief Get transport type
 * 
 * @param[in] com - Communication context
 * 
 * @return odrive_com_t
 */
odrive_transport_t odrive_com_get_type(const odrive_com_t* com);

/**
 * @brief Get backend name
 * 
 * @param[in] com - Communication context
 * 
 * @return Backend name (const char*)
 */
const char* odrive_get_backend_name(const odrive_com_t* com);

//========================================================
//      End of File
//========================================================

#endif /* !ODRIVE_COM_HAL_H */