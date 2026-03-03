/*
 * L76LM33.h
 *
 *  Created on: May 12, 2024
 *      Author: AudaceLol12
 *
 *  Edited on: Jul 04, 2024
 *      Autor: mathouqc
 *
 *  Edited on: Mar 02, 2026
 *      Autor: AudaceLol12
 */

#include <GAUL_Drivers/l76lm33.h>


/**
 * Send array of character to L76LM33 using UART HAL functions.
 *
 * @param L76_data: pointer to a L76LM33 structure.
 * @param command[]: array of character to send.
 * @param size: size of the data to send.
 *
 * @retval L76LM33_OK
 * @retval L76LM33_ERROR
 */
static int8_t L76LM33_Send_Command(l76lm33_t *dev, char command[], uint8_t size) {
    if(command == NULL) {
    	dev->state = L76LM33_ERROR;
        return L76LM33_ERROR; // Error
    }

    if(HAL_UART_Transmit(dev->huart, (uint8_t *)command, size, L76LM33_UART_TIMEOUT) != HAL_OK) {
    	dev->state = L76LM33_ERROR;
        return L76LM33_ERROR; // Error with UART
    }

    dev->state = L76LM33_OK;
    return L76LM33_OK; // OK
}


/*
 * Source:
 * LG76 Series GNSS Protocol Specification - Section 2.3. PMTK Messages
 *
 * Only output RMC (Recommended Minimum Specific GNSS Sentence) once every one position fix
 * NMEA_RMC[] = "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*35<CR><LF>"
 *
 * Set the navigation mode to "Aviation Mode" (for large acceleration movement, altitude of 10'000m max)
 * NMEA_NAVMODE = "PMTK886,2*2A<CR><LF>"
 */

/**
 * Initialize L76LM33 sensor.
 *
 * @param L76_data: pointer to a L76LM33 structure.
 * @param huart: pointer to the GPS HAL UART handler.
 *
 * @retval L76LM33_OK
 * @retval L76LM33_ERROR
 */
l76lm33_state_t L76LM33_Init(l76lm33_t *dev, UART_HandleTypeDef *huart) {
    // Set UART handler
    dev->huart = huart;

    // Initialize circular buffer
    ring_buffer_init(&(dev->UART_Buffer), dev->UART_Buffer_arr, L76LM33_BUFFER_SIZES);

    // Receive UART data with interrupts
    if(HAL_UART_Receive_IT(dev->huart, &(dev->received_byte), 1) != HAL_OK) {
        dev->state = L76LM33_ERROR; // Bad state
        return L76LM33_ERROR; // Error with UART
    }

    // Only output GPRMC sentence
    char NMEA_RMC[] = "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*35\r\n";
    if(L76LM33_Send_Command(dev, NMEA_RMC, sizeof(NMEA_RMC)) != L76LM33_OK) {
        dev->state = L76LM33_ERROR; // Bad state
        return L76LM33_ERROR; // Error with UART
    }
    // Set navigation mode
    char NMEA_NAVMODE[] = "PMTK886,2*2A\r\n";
    if(L76LM33_Send_Command(dev, NMEA_NAVMODE, sizeof(NMEA_NAVMODE)) != L76LM33_OK) {
        dev->state = L76LM33_ERROR; // Bad state
        return L76LM33_ERROR; // Error with UART
    }

    dev->state = L76LM33_OK; // Good state
    return L76LM33_OK; // OK
}

/**
 * Read and parse a NMEA GPRMC sentence into data structure. Call this function
 * frequently to have the latest GPS data available.
 *
 * Takes 0.3ms to complete when buffer is full
 *
 * @param L76_data: pointer to a L76LM33 structure to update.
 *
 * @retval L76LM33_OK
 * @retval L76LM33_ERROR
 * @retval L76LM33_EMPTY_BUFF Empty UART buffer, struct unchanged
 *
 */
l76lm33_state_t L76LM33_Read(l76lm33_t *dev) {
    // Read sentence
    int8_t valid = L76LM33_Read_Sentence(dev);
    if(valid == L76LM33_EMPTY_BUFF) {
        // Empty buffer, don't change structure
    	dev->state = L76LM33_EMPTY_BUFF; // Bad state
        return L76LM33_EMPTY_BUFF;
    } else if(valid != L76LM33_OK) {
        dev->state = L76LM33_ERROR; // Bad state
        return L76LM33_ERROR; // Error
    }

    // Validate sentence ID is RMC
    if(NMEA_ValidateRMC(dev->NMEA_Buffer) != NMEA_OK) {
        dev->state = L76LM33_ERROR; // Bad state
        return L76LM33_ERROR; // Error, sentence ID is not RMC
    }

    // Parse NMEA RMC sentence to local structure
    if(NMEA_ParseRMC(&(dev->gps_data), dev->NMEA_Buffer) != NMEA_OK) {
        dev->state = L76LM33_ERROR; // Bad state
        return L76LM33_ERROR;
    }

    dev->state = L76LM33_OK; // Good status (no error)
    return L76LM33_OK; // OK
}

/**
 * Read NMEA sentence from UART circular buffer into a NMEA buffer.
 *
 * Takes 0.24ms to complete when buffer is full
 *
 * @param L76_data: pointer to a L76LM33 structure.
 *
 * @retval L76LM33_OK
 * @retval L76LM33_ERROR cannot find starting or ending character.
 * @retval L76LM33_EMPTY_BUFF empty UART buffer
 *
 */
int8_t L76LM33_Read_Sentence(l76lm33_t *dev) {
    if(dev->new_line_flag == 0) {
    	dev->state = L76LM33_EMPTY_BUFF;
        return L76LM33_EMPTY_BUFF; // Error, empty UART circular buffer
    }

    // Reset flag
    dev->new_line_flag = 0;

    // Clear NMEA buffer
    for(int16_t i = 0; i < sizeof(dev->NMEA_Buffer); i++) {
    	dev->NMEA_Buffer[i] = 0;
    }

    // Variable to store character from UART buffer
    char c;

    // Try to find '$' in 100 iterations
    for(uint16_t i = 0; i < 100; i++) {
        // Read character from UART buffer
        if(ring_buffer_dequeue(&(dev->UART_Buffer), &c) == 0) {
            return L76LM33_EMPTY_BUFF; // Error, empty buffer
        }

        if(c == '$') {
			// Set starting character in NMEA buffer
			dev->NMEA_Buffer[0] = '$';
			break; // Found starting characters
        }
    }

    if(c != '$') {
    	dev->state = L76LM33_ERROR;
        return L76LM33_ERROR; // Error, cannot find starting character in 100 iterations
    }


    // Read into NMEA buffer until ending character is found
    for (uint16_t i = 1; i < sizeof(dev->NMEA_Buffer); i++) {
        // Read character from UART buffer
        if(ring_buffer_dequeue(&(dev->UART_Buffer), &c) == 0) {
        	dev->state = L76LM33_EMPTY_BUFF;
            return L76LM33_EMPTY_BUFF; // Error, empty buffer
        }

        // Add character to NMEA buffer
        dev->NMEA_Buffer[i] = c;

        if(c == '\n') {
            break; // Found ending character
        }
    }

    if(c != '\n') {
    	dev->state = L76LM33_ERROR;
        return L76LM33_ERROR; // Error, cannot find '\n'
    }

    dev->state = L76LM33_ERROR;
    return L76LM33_OK;
}

/**
 * Callback called on incoming UART data. It is called when HAL_UART_RxCpltCallback is called in main.c.
 * Add received byte to UART circular buffer.
 *
 * @param L76_data: pointer to a L76LM33 structure.
 * @param huart: pointer to a HAL UART handler triggering the callback.
 */
void L76LM33_RxCallback(l76lm33_t *dev, UART_HandleTypeDef *huart) {
    if(huart->Instance == dev->huart->Instance) {
        // Add data to circular buffer
        ring_buffer_queue(&(dev->UART_Buffer), dev->received_byte);
        if(dev->received_byte == '\n') {
        	dev->new_line_flag = 1;
        }
        // Receive UART data with interrupts
        HAL_UART_Receive_IT(dev->huart, &(dev->received_byte), 1);
    }
}
