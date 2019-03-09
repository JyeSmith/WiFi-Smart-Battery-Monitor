
#include <WiFi.h>
#include <Wire.h>
#include "SBS_Functions.h" // this library should be in Arduino\libraries\SBS
#include "SBS.h" // this library should be in Arduino\libraries\SBS

uint8_t numOfBatteries = 25;
uint8_t sclNumOfWires = 5;
uint8_t sdaArray[] = {21, 32, 33, 25, 26};
uint8_t sclArray[] = {22, 23, 19, 18, 4};

SBS battery = SBS();
            
const char* ssid = "The Battery";

WiFiServer server(80);

// Variable to store the HTTP request
String header;

void setup() {
  Serial.begin(115200);
      
  Wire.begin();
            
  // Connect to Wi-Fi network with SSID and password
  Serial.println("Setting Access Point...");
  WiFi.softAP(ssid);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("IP address: ");
  Serial.println(IP);
  
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character          
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");

              // Body
//              for (uint8_t i = 0; i < numOfBatteries; i++) {
//                client.println("<h1>Battery " + String(i+1) + "/" + String(numOfBatteries) + "</h1>");
//                Wire.begin(sdaArray[i % sclNumOfWires], sclArray[i / sclNumOfWires], 10000);
//                printBatteryRegisters(client, battery);
//                client.println("<p></p>");
//              }
              for (uint8_t i = 0; i < numOfBatteries; i++) {
                Wire.begin(sdaArray[i % sclNumOfWires], sclArray[i / sclNumOfWires], 10000);
                printBatterySummary(client, battery);
                client.println("<br>");
              }

            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
