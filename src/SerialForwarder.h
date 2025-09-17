/**
 * SerialForwarder.h
 * Author: benda95280 (GitHub)
 */

#ifndef SERIAL_FORWARDER_H
#define SERIAL_FORWARDER_H

#include <Arduino.h>
#include <MycilaWebSerial.h> 
#include <stddef.h>          
#include <freertos/FreeRTOS.h> // Required for portMUX_TYPE
#include <freertos/task.h>     // Required for task synchronization primitives

class HardwareSerial; // Forward declaration

class SerialForwarder { 
public:
    // Constructor with optional HardwareSerial
    SerialForwarder(WebSerial* webSerial, HardwareSerial* hwSerial = nullptr);
    ~SerialForwarder();
    
    // --- Convenience Output Methods ---
    size_t print(const char* str);
    size_t println(const char* str);
    size_t println(); 
    size_t printf(const char *format, ...) __attribute__((format(printf, 2, 3)));

    // --- WebSerial Buffering Control ---
    void flushWebSerial(); 

    // --- WebSerial Control ---
    void enableWebSerial(bool enable); 

    HardwareSerial* getHardwareSerial() { return _hwSerial; } // Might be nullptr
    WebSerial* getWebSerial() { return _webSerial; }

private:
    HardwareSerial* _hwSerial; // Can be nullptr
    WebSerial* _webSerial;     // Should not be nullptr after constructor
    bool _isValid;             // Flag to indicate if SerialForwarder is properly initialized
    bool _webSerialEnabled;
    
    // WebSerial buffering members
    char _webSerialBuffer[1024]; 
    size_t _webSerialBufferLen;
    size_t _webSerialWarningMsgLen;
    bool _pendingCriticalWarning; 

    // Spinlock for critical sections
    portMUX_TYPE _bufferMux = portMUX_INITIALIZER_UNLOCKED;

    // Flags and timers for makeBuffer robustness
    bool _webSerialMakeBufferFailedRecently;
    unsigned long _lastMakeBufferFailTime;
    static const unsigned long MAKE_BUFFER_RETRY_DELAY_MS = 5000; 

    // Private helper for buffering WebSerial data
    void appendToWebSerialBuffer(const char* data, size_t len);
    // Removed trySendPendingCriticalWarning as its logic is integrated
};

#endif // SERIAL_FORWARDER_H