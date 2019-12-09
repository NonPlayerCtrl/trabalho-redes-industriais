//-----------------------------------------------------------------------------
// Copyright 2018 Thiago Alves
// This file is part of the OpenPLC Software Stack.
//
// OpenPLC is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// OpenPLC is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OpenPLC.  If not, see <http://www.gnu.org/licenses/>.
//------
//
// This is the main file for the OpenPLC Arduino firmware. It contains the a
// Modbus RTU library to communciate with OpenPLC host as a slave device.
//
// Thiago Alves, Aug 2018
//-----------------------------------------------------------------------------

#include "analogWrite.h"
#include <WiFi.h>


/*********NETWORK CONFIGURATION*********/

const char *ssid = "GAIn Rounter";
const char *password = "gaintech";


/***************************************/

#define NODE_PIN_DO0   16   //Dig Out     | QXn.0
#define NODE_PIN_DO1   4    //Dig Out     | QXn.1
#define NODE_PIN_DO2   2    //Dig Out     | QXn.2
#define NODE_PIN_DO3   15   //Dig Out     | QXn.3

#define NODE_PIN_DI4   23   //Dig Input   | IXn.0
#define NODE_PIN_DI5   22   //Dig Input   | IXn.1
#define NODE_PIN_DI6   1    //Dig Input   | IXn.2
#define NODE_PIN_DI7   3    //Dig Input   | IXn.3

#define NODE_PIN_AO0   17    //Anal. Out  | QWn
#define NODE_PIN_AI0   A0    //Anal. In   | IWn

uint8_t pinMask_DIN[] = { NODE_PIN_DI4, NODE_PIN_DI5, NODE_PIN_DI6, NODE_PIN_DI7 };
uint8_t pinMask_DOUT[] = { NODE_PIN_DO0, NODE_PIN_DO1, NODE_PIN_DO2, NODE_PIN_DO3 };
uint8_t pinMask_AIN[] = { NODE_PIN_AI0 }; 
uint8_t pinMask_AOUT[] = { NODE_PIN_AO0 };

unsigned char modbus_buffer[100];

int processModbusMessage(unsigned char *buffer, int bufferSize);

#include "modbus.h"

extern bool mb_discrete_input[MAX_DISCRETE_INPUT];
extern bool mb_coils[MAX_COILS];
extern uint16_t mb_input_regs[MAX_INP_REGS];
extern uint16_t mb_holding_regs[MAX_HOLD_REGS];

//Create the modbus server instance
WiFiServer server(502);

void pinConfig()
{
    pinMode(NODE_PIN_DO0, OUTPUT);
    pinMode(NODE_PIN_DO1, OUTPUT);
    pinMode(NODE_PIN_DO2, OUTPUT);
    pinMode(NODE_PIN_DO3, OUTPUT);
    
    pinMode(NODE_PIN_DI4, INPUT);
    pinMode(NODE_PIN_DI5, INPUT);
    pinMode(NODE_PIN_DI6, INPUT);
    pinMode(NODE_PIN_DI7, INPUT);

    pinMode(NODE_PIN_AO0, OUTPUT);
    
    analogReadResolution(10);
    analogSetAttenuation(ADC_6db);
    analogWriteResolution(16);
    analogWriteResolution(NODE_PIN_AO0, 16);
    
}

void setup()
{
    Serial.begin(115200);
    delay(10);
    analogWriteFrequency(5000);
    
    pinConfig();
    
    // Connect to WiFi network
    
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("");
    Serial.println("WiFi connected");

    // Start the server
    server.begin();
    Serial.println("Server started");

    // Print the IP address
    Serial.print("My IP: ");
    Serial.println(WiFi.localIP());

    updateIO();
}

void PrintHex(uint8_t *data, uint8_t length) // prints 8-bit data in hex with leading zeroes
{
    for (int i=0; i<length; i++) 
    {
        if (data[i] < 0x10)
            Serial.print("0");
        Serial.print(data[i], HEX); Serial.print(" ");
    }
    Serial.println();
}

void updateIO()
{
    for (int i = 0; i < sizeof(pinMask_DIN); i++)
    {
        mb_discrete_input[i] = digitalRead(pinMask_DIN[i]);
    }
    
    for (int i = 0; i < sizeof(pinMask_DOUT); i++)
    {
        digitalWrite(pinMask_DOUT[i], mb_coils[i]);
    }
    
    for (int i = 0; i < sizeof(pinMask_AIN); i++)
    {
        mb_input_regs[i] = (analogRead(pinMask_AIN[i]) * 64);
    }

    for (int i = 0; i < sizeof(pinMask_AOUT); i++)
    {
        analogWrite(pinMask_AOUT[i], mb_holding_regs[i]/64);
    }
}

void loop()
{
    WiFiClient client = server.available();
    if (!client)
        return;
    
    Serial.println("new client!");

    while(client.connected())
    {
        // Wait until the client sends some data
        while(!client.available())
        {
            delay(1);
            if (!client.connected())
                return;
        }
        
        int i = 0;
        while(client.available())
        {
            modbus_buffer[i] = client.read();
            i++;
            if (i == 100)
                break;
        }

        //DEBUG
        /*
        Serial.print("Received MB frame: ");
        PrintHex(modbus_buffer, i);
        */
        updateIO();
        unsigned int return_length = processModbusMessage(modbus_buffer, i);
        client.write((const uint8_t *)modbus_buffer, return_length);
        updateIO();
        delay(1);
    }
    
    Serial.println("Client disonnected");
    delay(1);
}
