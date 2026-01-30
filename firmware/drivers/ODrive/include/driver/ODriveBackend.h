/***************************************************************************//**
* \file ODriveBackendL.h
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

#ifndef ODRIVE_BACKEND_H
#define ODRIVE_BACKEND_H

//========================================================
//      Standard Includes
//========================================================

#include <stdint.h>
#include <stdbool.h>

#include "ODriveCore.h"
#include "ODriveResult.h"

//========================================================
//      CAN Constants
//========================================================

#define ODRIVE_CAN_DLC_MAX 8u

//========================================================
//      CAN Helper Macro's
//========================================================

#define ODRIVE_CAN_ID(node_id, cmd_id ) \
    ( ((uint16_t)(node_id) << 5) | ((uint16_t)(cmd_id) & 0x1Fu) )

#define ODRIVE_NODE_ID_FROM_CAN_ID(can_id) \
    ( (uint8_t)( ((can_id) >> 5) & 0x3Fu ) )

#define ODRIVE_CMD_ID_FROM_CAN_ID(can_id) \
    ( (uint8_t)( (can_id) & 0x1Fu ) )


//========================================================
//      CAN Typedefs
//========================================================

typedef uint32_t  odrive_can_id_t;
typedef uint8_t   odrive_can_dlc_t;

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

//========================================================
//      CAN Transport Message
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
   odrive_can_id_t   id;                                    /**< CAN message ID */
   odrive_can_dlc_t  dlc;                              /**< Data length (0-8) in bytes */
   bool              rtr;                              /**< Remote transmission request */
   uint8_t           data[ODRIVE_CAN_DLC_MAX];  /**< Message payload */
} odrive_can_message_t;

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
typedef void (*odrive_backend_rx_cb_t)(const odrive_can_message_t* msg,
                                           void* user_ctx);

/**
 * @brief Status change callback
 * 
 * Called on transport state changes (connected/disconnected/error).
 */
typedef void (*odrive_backend_status_cb_t)(uint8_t status,
                                               void* user_ctx);

//========================================================
//      CAN Function Prototypes
//========================================================

/**
 * @brief Message transmit operation
 *
 * @param[in] ctx - Hardware context
 * @param[in] msg - Message to transmit
 * 
 * @return ODRIVE_RESULT_OK on success
 */
typedef odrive_result_t (*odrive_can_transmit_fn_t)(
   odrive_ctx_t ctx, 
   const odrive_can_message_t* msg
);

/**
 * @brief Message receive operation
 *
 * @param[in] ctx - Hardware context
 * @param[out] msg - Received message pointer
 * 
 * @return ODRIVE_RESULT_OK on success
 */
typedef odrive_result_t (*odrive_can_receive_fn_t)(
   odrive_ctx_t ctx, 
   const odrive_can_message_t* msg
);

/**
 * @brief Get RX message count operation
 * 
 * @param[in] ctx - Hardware context
 * 
 * @return Number of messages available in RX buffer
 */
typedef uint8_t (*odrive_can_rx_count_fn_t)(
   odrive_ctx_t ctx
);

//========================================================
//      Communication Function Prototypes
//========================================================

/**
 * @brief Get transport name operation
 *
 * @param[in] hw_ctx Hardware context
 * 
 * @return Human-readable transport name (e.g., "CAN 500kbps", "USB CDC")
 */
typedef const char* (*odrive_backend_get_name_fn_t)(void* hw_ctx);

typedef odrive_result_t (*odrive_pfn_event_handler) ();

//========================================================
//      GPIO Function Prototypes
//========================================================

/**
 * @brief GPIO write operation
 * 
 * @param[in] pin   - GPIO pin number
 * @param[in] value - Pin value (true=high, false=low)
 * 
 */
typedef void (*odrive_write_gpio_fn_t)(uint8_t pin, uint8_t value);

/**
 * @brief GPIO pulse operation
 * 
 * @param[in] pin         - GPIO pin number
 * @param[in] value       - Pin value (true=high, false=low)
 * @param[in] duration_us - Pulse duration in microseconds
 * 
 */
typedef void (*odrive_pulse_gpio_fn_t)(uint8_t pin, uint32_t duration_us);

//========================================================
//      GPIO Driver Definition
//========================================================

/**
 * @brief GPIO driver abstraction
 * 
 */
typedef struct {
   /**< GPIO write function */
   odrive_write_gpio_fn_t write;

   /**< GPIO pulse function */
   odrive_pulse_gpio_fn_t pulse;

   /**< User context pointer */
   odrive_ctx_t user_ctx;    
} odrive_gpio_driver_t;

//========================================================
//      CAN Communication Operartions
//========================================================

/**
 * @brief Communication backend operations
 * 
 */
typedef struct {
   /**
    * @brief Message transmit abstraction
    */
   odrive_can_transmit_fn_t transmit;

   /**
    * @brief Message receive abstraction
    */
   odrive_can_receive_fn_t receive;

   /**
    * @brief Get RX message count
    */
   odrive_can_rx_count_fn_t get_rx_count;
   
} odrive_can_ops_t;

//========================================================
//      Communication Operartions
//========================================================

typedef union {
   odrive_can_ops_t can_ops;
} odrive_backend_ops;

//========================================================
//      Communication Context
//========================================================

/**
 * @brief Communication abstraction layer context
 * 
 * Encapsulates transport operations and hardware state.
 */
typedef struct {
   /**
    * @brief Transport type
    */
   odrive_transport_t transport;

   /** 
    * @brief Communication operations
    */
   odrive_backend_ops ops;              

   /**
    * @brief GPIO driver abstraction
    */
   odrive_gpio_driver_t gpio;          

   /**
    * @brief Get transport name (for logging/debugging)
    * 
    * @return Human-readable transport name (e.g., "CAN 500kbps")
    */
   odrive_backend_get_name_fn_t get_name;

   /**< Hardware-specific context */
   odrive_ctx_t hw_ctx;                  

} odrive_backend_t;

typedef odrive_backend_t* odrive_backend;

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
odrive_transport_t odrive_backend_get_type(odrive_driver driver);

/**
 * @brief Get backend name
 * 
 * @param[in] com - Communication context
 * 
 * @return Backend name (const char*)
 */
const char* odrive_backend_get_name(odrive_driver driver);

//========================================================
//      End of File
//========================================================

#endif // !ODRIVE_BACKEND_H