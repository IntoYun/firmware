#include "intorobot_config.h"

#ifndef configNO_NETWORK

#include <stdlib.h>
#include "wiring_ticks.h"
#include "wiring_httpclient.h"
#include "service_debug.h"

/*debug switch*/
//#define WIRING_HTTPCLIENT_DEBUG

#ifdef WIRING_HTTPCLIENT_DEBUG
#define WHTTPCLIENT_DEBUG(...)  do {DEBUG(__VA_ARGS__);}while(0)
#define WHTTPCLIENT_DEBUG_D(...)  do {DEBUG_D(__VA_ARGS__);}while(0)
#else
#define WHTTPCLIENT_DEBUG(...)
#define WHTTPCLIENT_DEBUG_D(...)
#endif

static const uint16_t DEFAULT_TIMEOUT = 5000; // Allow maximum 5s between data packets.

/**
 * Constructor.
 */
HttpClient::HttpClient()
{

}

/**
 * Method to send a header, should only be called from within the class.
 */
void HttpClient::sendHeader(const char* aHeaderName, const char* aHeaderValue)
{
    stringHeader += aHeaderName;
    stringHeader += ": ";
    stringHeader += aHeaderValue;
    stringHeader += "\r\n";

    WHTTPCLIENT_DEBUG("httpClient: sendHeader %s: %s", aHeaderName, aHeaderValue);
}

void HttpClient::sendHeader(const char* aHeaderName, const int aHeaderValue)
{
    stringHeader += aHeaderName;
    stringHeader += ": ";
    stringHeader += aHeaderValue;
    stringHeader += "\r\n";

    WHTTPCLIENT_DEBUG("httpClient: sendHeader %s: %d", aHeaderName, aHeaderValue);
}

void HttpClient::sendHeader(const char* aHeaderName)
{
    stringHeader += aHeaderName;
    stringHeader += "\r\n";

    WHTTPCLIENT_DEBUG("httpClient: sendHeader %s", aHeaderName);
}

/**
 * Method to send an HTTP Request. Allocate variables in your application code
 * in the aResponse struct and set the headers and the options in the aRequest
 * struct.
 */
void HttpClient::request(http_request_t &aRequest, http_response_t &aResponse, http_header_t headers[], const char* aHttpMethod)
{
    // If a proper response code isn't received it will be set to -1.
    aResponse.status = -1;

    // NOTE: The default port tertiary statement is unpredictable if the request structure is not initialised
    // http_request_t request = {0} or memset(&request, 0, sizeof(http_request_t)) should be used
    // to ensure all fields are zero
    bool connected = false;
    if(aRequest.hostname!=NULL) {
        connected = client.connect(aRequest.hostname.c_str(), (aRequest.port) ? aRequest.port : 80 );
        WHTTPCLIENT_DEBUG("httpClient: tcp Connecting to: %s : %d", aRequest.hostname.c_str(), aRequest.port);
    } else {
        connected = client.connect(aRequest.ip, aRequest.port);
        WHTTPCLIENT_DEBUG("httpClient: tcp Connecting to IP: %s : %d", aRequest.hostname.c_str(), aRequest.port);
    }

    if (!connected) {
        WHTTPCLIENT_DEBUG("httpClient: tcp Connection failed.");
        client.stop();
        // If TCP Client can't connect to host, exit here.
        return;
    }

    //
    // Send HTTP Headers
    //

    // Send initial headers (only HTTP 1.0 is supported for now).
    stringHeader= "";

    stringHeader += aHttpMethod;
    stringHeader += " ";
    stringHeader += aRequest.path;
    stringHeader += " HTTP/1.0\r\n";

    WHTTPCLIENT_DEBUG("httpClient: Start of HTTP Request.");
    WHTTPCLIENT_DEBUG("%s %s %s", aHttpMethod, aRequest.path.c_str(), "HTTP/1.0");

    // Send General and Request Headers.
    sendHeader("Connection", "close"); // Not supporting keep-alive for now.
    if(aRequest.hostname!=NULL) {
        sendHeader("HOST", aRequest.hostname.c_str());
    }

    //Send Entity Headers
    // TODO: Check the standard, currently sending Content-Length : 0 for empty
    // POST requests, and no content-length for other types.
    if (aRequest.body != NULL) {
        sendHeader("Content-Length", (aRequest.body).length());
    } else if (strcmp(aHttpMethod, HTTP_METHOD_POST) == 0) { //Check to see if its a Post method.
        sendHeader("Content-Length", 0);
    }

    if (headers != NULL) {
        int i = 0;
        while (headers[i].header != NULL)
        {
            if (headers[i].value != NULL) {
                sendHeader(headers[i].header, headers[i].value);
            } else {
                sendHeader(headers[i].header);
            }
            i++;
        }
    }

    // Empty line to finish headers
    stringHeader += "\r\n";

    //
    // Send HTTP Request Body
    //

    if (aRequest.body != NULL) {
        WHTTPCLIENT_DEBUG("httpClient: body: %s", aRequest.body.c_str());
        stringHeader += aRequest.body;
        stringHeader += "\r\n";
    }

    client.write((uint8_t *)stringHeader.c_str(), stringHeader.length());
    client.flush();
    WHTTPCLIENT_DEBUG("httpClient: End of HTTP Request.");

    // clear response buffer
    memset(&buffer[0], 0, sizeof(buffer));

    //
    // Receive HTTP Response
    //
    // The first value of client.available() might not represent the
    // whole response, so after the first chunk of data is received instead
    // of terminating the connection there is a delay and another attempt
    // to read data.
    // The loop exits when the connection is closed, or if there is a
    // timeout or an error.

    unsigned int bufferPosition = 0;
    unsigned long lastRead = millis();
    unsigned long firstRead = millis();
    bool error = false;
    bool timeout = false;
    uint16_t actualTimeout = aRequest.timeout == 0 ? DEFAULT_TIMEOUT : aRequest.timeout;
    char lastChar = 0;
    bool inHeaders = true;

    do {
        int bytes = client.available();
        if(bytes) {
            WHTTPCLIENT_DEBUG("httpClient: Receiving TCP transaction of %d bytes.", bytes);
        }

        while (client.available()) {
            char c = client.read();
            WHTTPCLIENT_DEBUG_D("%c", c);
            lastRead = millis();

            if (c == -1) {
                error = true;
                WHTTPCLIENT_DEBUG("httpClient: Error: No data available.");
                break;
            }

            if (inHeaders) {
                if ((c == '\n') && (lastChar == '\n')) {
                    // End of headers.  Grab the status code and reset the buffer.
                    aResponse.status = atoi(&buffer[9]);
                    memset(&buffer[0], 0, sizeof(buffer));
                    bufferPosition = 0;
                    inHeaders = false;
                    WHTTPCLIENT_DEBUG("httpClient: End of HTTP Headers ( %d )", aResponse.status);
                    continue;
                } else if (c != '\r') {
                    lastChar = c;
                }
            }

            // Check that received character fits in buffer before storing.
            if (bufferPosition < sizeof(buffer)-1) {
                buffer[bufferPosition] = c;
            } else if ((bufferPosition == sizeof(buffer)-1)) {
                buffer[bufferPosition] = '\0'; // Null-terminate buffer
                client.stop();
                error = true;
                WHTTPCLIENT_DEBUG("httpClient: Error: Response body larger than buffer.");
                break;
            }
            bufferPosition++;
        }
        // We don't need to null terminate the buffer since it was zeroed to start with, or null terminated when it reached capacity.

        if (bytes) {
            break;
            WHTTPCLIENT_DEBUG("httpClient: End of TCP transaction.");
        }

        // Check that there hasn't been more than 5s since last read.
        timeout = millis() - lastRead > actualTimeout;

        // Unless there has been an error or timeout wait 200ms to allow server
        // to respond or close connection.
        if (!error && !timeout) {
            delay(200);
        }
    } while (client.connected() && !timeout && !error);

    if (timeout) {
        WHTTPCLIENT_DEBUG("httpClient: Error: Timeout while reading response.");
    }
    WHTTPCLIENT_DEBUG("httpClient: End of HTTP Response ( %d )ms", millis() - firstRead);

    client.stop();

    WHTTPCLIENT_DEBUG("httpClient: Status Code: %d", aResponse.status);

    if (inHeaders) {
        WHTTPCLIENT_DEBUG("httpClient: Error: Can't find HTTP response body.");
        return;
    }
    // Return the entire message body from bodyPos+4 till end.
    aResponse.body = buffer;
}
#endif
