#include "SerialForwarder.h"
#include <HardwareSerial.h> 
#include <stdarg.h>
#include <stdio.h>   
#include <string.h>  

// Fixed warning message and buffer capacity
static const char* WEB_SERIAL_WARNING_MSG = "WebSerial buffer full, messages might be lost!\n";
static const size_t WEB_SERIAL_BUFFER_CAPACITY = 1024; // Matches _webSerialBuffer array size

// Note: MAKE_BUFFER_RETRY_DELAY_MS is in the .h file as static const

SerialForwarder::SerialForwarder(WebSerial* webSerial, HardwareSerial* hwSerial /*= nullptr*/)
    : _hwSerial(hwSerial), 
      _webSerial(webSerial), 
      _isValid(true), // Assume valid initially
      _webSerialEnabled(true),
      _webSerialBufferLen(0),
      _pendingCriticalWarning(false),
      _webSerialMakeBufferFailedRecently(false),
      _lastMakeBufferFailTime(0) {
    
    // portMUX_INITIALIZER_UNLOCKED handles _bufferMux initialization

    if (!_webSerial) {
        if (_hwSerial) {
            _hwSerial->println("CRITICAL SerialForwarder_Config: WebSerial instance cannot be null!");
        }
        _isValid = false;
        _webSerialEnabled = false; // Disable WebSerial operations if instance is null
        return;
    }
    
    _webSerialWarningMsgLen = strlen(WEB_SERIAL_WARNING_MSG);

    if (_webSerialWarningMsgLen > WEB_SERIAL_BUFFER_CAPACITY) {
        if (_hwSerial) {
            _hwSerial->println("CRITICAL SerialForwarder_Config: WebSerial warning message is larger than its buffer capacity!");
        }
        // Warning message won't fit, but system will continue. Sending this warning will effectively fail.
    }
}

SerialForwarder::~SerialForwarder() {
    if (!_isValid || !_webSerial) return;

    if (_webSerialEnabled) {
        // Try to flush any remaining data, including a pending warning if possible
        portENTER_CRITICAL(&_bufferMux);
        if (_pendingCriticalWarning) {
             if (_webSerialBufferLen + _webSerialWarningMsgLen <= WEB_SERIAL_BUFFER_CAPACITY) {
                memcpy(_webSerialBuffer + _webSerialBufferLen, WEB_SERIAL_WARNING_MSG, _webSerialWarningMsgLen);
                _webSerialBufferLen += _webSerialWarningMsgLen;
                _pendingCriticalWarning = false; 
            }
            // If it doesn't fit, it's lost as this is the destructor
        }
        bool needsFlush = _webSerialBufferLen > 0;
        portEXIT_CRITICAL(&_bufferMux);

        if (needsFlush) {
            flushWebSerial(); // This will handle the actual sending
        }
    }
}

void SerialForwarder::appendToWebSerialBuffer(const char* data, size_t len) {
    if (!_isValid || !_webSerial || !_webSerialEnabled || len == 0) {
        return;
    }

    portENTER_CRITICAL(&_bufferMux);

    bool initialPendingWarning = _pendingCriticalWarning;
    size_t spaceNeededForWarning = initialPendingWarning ? _webSerialWarningMsgLen : 0;
    bool canBufferWarningNow = (_webSerialBufferLen + spaceNeededForWarning <= WEB_SERIAL_BUFFER_CAPACITY);

    if (initialPendingWarning && canBufferWarningNow) {
        memcpy(_webSerialBuffer + _webSerialBufferLen, WEB_SERIAL_WARNING_MSG, _webSerialWarningMsgLen);
        _webSerialBufferLen += _webSerialWarningMsgLen;
        _pendingCriticalWarning = false; // Warning is now in buffer
    }

    bool dataFits = (_webSerialBufferLen + len <= WEB_SERIAL_BUFFER_CAPACITY);

    if (dataFits) {
        memcpy(_webSerialBuffer + _webSerialBufferLen, data, len);
        _webSerialBufferLen += len;
        portEXIT_CRITICAL(&_bufferMux);
        return;
    }

    // Data does not fit, or initial warning didn't fit and now data doesn't fit
    // Need to flush existing buffer content first
    char localSendBuffer[WEB_SERIAL_BUFFER_CAPACITY];
    size_t localSendLen = 0;

    // Copy out what's currently in the buffer (which might include a just-added warning)
    if (_webSerialBufferLen > 0) {
        localSendLen = _webSerialBufferLen;
        memcpy(localSendBuffer, _webSerialBuffer, _webSerialBufferLen);
    }
    _webSerialBufferLen = 0; // Reset internal buffer under lock

    // Now, attempt to buffer the new data and any still-pending warning
    bool newWarningBuffered = false;
    if (initialPendingWarning && !_pendingCriticalWarning) {
        // Warning was already buffered before this current 'data' was considered
    } else if (initialPendingWarning) { // Warning was pending but couldn't be buffered with existing content
        if (_webSerialWarningMsgLen <= WEB_SERIAL_BUFFER_CAPACITY) { // Can warning fit in an empty buffer?
            memcpy(_webSerialBuffer, WEB_SERIAL_WARNING_MSG, _webSerialWarningMsgLen);
            _webSerialBufferLen += _webSerialWarningMsgLen;
            _pendingCriticalWarning = false; // Warning now buffered
            newWarningBuffered = true;
        } else {
            // Warning is too big even for an empty buffer, remains pending
        }
    }

    // Try to add the new data
    if (_webSerialBufferLen + len <= WEB_SERIAL_BUFFER_CAPACITY) {
        memcpy(_webSerialBuffer + _webSerialBufferLen, data, len);
        _webSerialBufferLen += len;
    } else {
        // New data doesn't fit even after flushing and potentially adding warning.
        // This means new data is lost.
        if (_hwSerial && len > 0) {
            char temp_log_msg[128];
            snprintf(temp_log_msg, sizeof(temp_log_msg),
                     "SerialForwarder: WebSerial discarding message (size %u). Buffer full or message too large for empty buffer (capacity %u).",
                     (unsigned int)len, (unsigned int)WEB_SERIAL_BUFFER_CAPACITY);
            _hwSerial->println(temp_log_msg);
        }
        if (!newWarningBuffered) { // If we didn't just buffer a warning for a *previous* loss
            _pendingCriticalWarning = true; // Signal that the current data was lost
        }
    }

    portEXIT_CRITICAL(&_bufferMux);

    // Send the flushed content (outside critical section)
    if (localSendLen > 0) {
        AsyncWebSocketMessageBuffer* wsBuffer = nullptr;
        bool triedMakeBuffer = false;
        if (!_webSerialMakeBufferFailedRecently) {
            wsBuffer = _webSerial->makeBuffer(localSendLen);
            triedMakeBuffer = true;
        }

        if (wsBuffer) {
            if (triedMakeBuffer) _webSerialMakeBufferFailedRecently = false;
            memcpy(wsBuffer->get(), localSendBuffer, localSendLen);
            _webSerial->send(wsBuffer);
        } else {
            if (triedMakeBuffer) {
                if (_hwSerial) _hwSerial->println("SerialForwarder: WebSerial makeBuffer failed during append, falling back to direct write.");
                _webSerialMakeBufferFailedRecently = true;
                _lastMakeBufferFailTime = millis();
            }
            _webSerial->write((const uint8_t*)localSendBuffer, localSendLen);
        }
    }
}


void SerialForwarder::flushWebSerial() {
    if (!_isValid || !_webSerial || !_webSerialEnabled) { 
        return;
    }

    portENTER_CRITICAL(&_bufferMux);

    if (_pendingCriticalWarning) {
        if (_webSerialBufferLen + _webSerialWarningMsgLen <= WEB_SERIAL_BUFFER_CAPACITY) {
            memcpy(_webSerialBuffer + _webSerialBufferLen, WEB_SERIAL_WARNING_MSG, _webSerialWarningMsgLen);
            _webSerialBufferLen += _webSerialWarningMsgLen;
            _pendingCriticalWarning = false;
            if (_hwSerial) _hwSerial->println("SerialForwarder: Queued pending critical warning during flush.");
        }
        // If warning doesn't fit, it remains pending.
    }

    if (_webSerialBufferLen == 0) {
        portEXIT_CRITICAL(&_bufferMux);
        return; 
    }

    // Prepare data for sending:
    char tempSendBuffer[WEB_SERIAL_BUFFER_CAPACITY]; 
    size_t tempSendBufferLen = _webSerialBufferLen;
    memcpy(tempSendBuffer, _webSerialBuffer, _webSerialBufferLen);
    _webSerialBufferLen = 0; 
    
    portEXIT_CRITICAL(&_bufferMux); // Release lock BEFORE sending

    // Actual sending logic (outside critical section)
    AsyncWebSocketMessageBuffer* wsBuffer = nullptr;
    bool triedMakeBuffer = false; 

    if (_webSerialMakeBufferFailedRecently) {
        if (millis() - _lastMakeBufferFailTime >= MAKE_BUFFER_RETRY_DELAY_MS) {
            _webSerialMakeBufferFailedRecently = false; 
        }
    }
    
    if (!_webSerialMakeBufferFailedRecently) {
        wsBuffer = _webSerial->makeBuffer(tempSendBufferLen);
        triedMakeBuffer = true; 
    }

    if (wsBuffer) {
        if (triedMakeBuffer) _webSerialMakeBufferFailedRecently = false; 
        memcpy(wsBuffer->get(), tempSendBuffer, tempSendBufferLen);
        _webSerial->send(wsBuffer); 
    } else {
        if (triedMakeBuffer) { 
            if (_hwSerial) {
                _hwSerial->println("SerialForwarder: WebSerial makeBuffer failed, falling back to direct write.");
            }
            _webSerialMakeBufferFailedRecently = true;
            _lastMakeBufferFailTime = millis();
        } else if (_hwSerial && _webSerialMakeBufferFailedRecently) {
            // Intentionally skipped makeBuffer due to cooldown
        }
        _webSerial->write((const uint8_t*)tempSendBuffer, tempSendBufferLen); 
    }
}

// --- Convenience Output Methods ---
size_t SerialForwarder::print(const char* str) {
    if (!_isValid) return 0;
    size_t count = 0;
    if (_hwSerial) {
        count = _hwSerial->print(str); // Changed from println to print for consistency
    }

    if (_webSerial && _webSerialEnabled) {
        size_t len = strlen(str);
        if (len > 0) { 
            appendToWebSerialBuffer(str, len);
        }
        if (!_hwSerial) count = len;
    }
    return count;
}

size_t SerialForwarder::println(const char* str) {
    if (!_isValid) return 0;
    size_t count = 0;
    if (_hwSerial) {
        count = _hwSerial->println(str); 
    }

    if (_webSerial && _webSerialEnabled) {
        size_t len = strlen(str);
        if (len > 0) {
            appendToWebSerialBuffer(str, len);
        }
        appendToWebSerialBuffer("\n", 1); 
        if (!_hwSerial) count = len + 1; 
    }
    return count;
}

size_t SerialForwarder::println() {
    if (!_isValid) return 0;
    size_t count = 0;
    if (_hwSerial) {
        count = _hwSerial->println();
    }

    if (_webSerial && _webSerialEnabled) {
        appendToWebSerialBuffer("\n", 1);
        if (!_hwSerial) count = 1;
    }
    return count;
}

size_t SerialForwarder::printf(const char *format, ...) {
    if (!_isValid) return 0;

    char temp_printf_buffer[256]; // Stack-allocated buffer
    va_list arg;
    va_start(arg, format);
    int len_printf = vsnprintf(temp_printf_buffer, sizeof(temp_printf_buffer), format, arg);
    va_end(arg);

    size_t written_count = 0;
    if (len_printf > 0) {
        size_t actual_len_in_buffer = (size_t)len_printf;
        if (actual_len_in_buffer >= sizeof(temp_printf_buffer)) { 
            actual_len_in_buffer = sizeof(temp_printf_buffer) - 1; 
        }
        
        if (_hwSerial) {
            written_count = _hwSerial->write((const uint8_t*)temp_printf_buffer, actual_len_in_buffer);
        }

        if (_webSerial && _webSerialEnabled) {
            appendToWebSerialBuffer(temp_printf_buffer, actual_len_in_buffer);
            if (!_hwSerial) written_count = actual_len_in_buffer; 
        }
    }
    return written_count;
}

// --- WebSerial Control ---
void SerialForwarder::enableWebSerial(bool enable) {
    if (!_isValid) return;
    portENTER_CRITICAL(&_bufferMux);
    _webSerialEnabled = enable;
    portEXIT_CRITICAL(&_bufferMux);
}