
#include "SBS.h" // this library should be in Arduino\libraries\SBS

void printBatteryRegisters(WiFiClient client, SBS battery) {
  // print over serial the data for each register we know about, handeling each register based on type.
  for(int i = 1; i < 38; i++) {
//    client.print("<p>");
    client.print(battery.commands[i].slaveFunction + " "); // print the register name
    if(battery.commands[i].type == "Boolean") { //only used for atRateOk register
      client.println(battery.sbsReadByte(battery.commands[i].code) == 0x00 ? "FALSE" : "TRUE");
    } // Long block of simple 2 digit numbers.
    else if(battery.commands[i].type == "mV") { // divide by 1000 for Volts
      client.print((int)battery.sbsReadInt(battery.commands[i].code));
      client.println(" " + battery.commands[i].type);
    }
    else if(battery.commands[i].type == "mAh") { // divide by 1000 for Amps
      client.print((int)battery.sbsReadInt(battery.commands[i].code));
      client.println(" " + battery.commands[i].type);
    }
    else if(battery.commands[i].type == "minutes") { // divide by 60 for hours
      client.print((int)battery.sbsReadInt(battery.commands[i].code));
      client.println(" " + battery.commands[i].type);
    }
    else if(battery.commands[i].type == "percent") { // divide by 100 for decimal
      client.print((int)battery.sbsReadInt(battery.commands[i].code));
      client.println(" " + battery.commands[i].type);
    }
    else if(battery.commands[i].type == "count") { // only used for CycleCount register
      client.println((int)battery.sbsReadInt(battery.commands[i].code));
    }
    else if(battery.commands[i].type == "number") { // only used for SerialNumber register
      client.println((uint)battery.sbsReadInt(battery.commands[i].code));
    }
    else if(battery.commands[i].type == "0.1K") { // only used for temperature register
      client.print(((int)battery.sbsReadInt(battery.commands[i].code) / 10 - 273.15) * 1.8 + 32); //register is in kelvin, this math changes to C, and converts to F
      client.println("F");
    }
    else if(battery.commands[i].type == "mA") { // divide by 1000 for Amps
      client.print((int)battery.sbsReadInt(battery.commands[i].code));
      client.println(" " + battery.commands[i].type);
    }
    else if(battery.commands[i].slaveFunction == "ManufactureDate") { // only one date register
    int raw = (int)battery.sbsReadInt(battery.commands[i].code);
    client.print((raw >> 5 ) & 0xF); client.print("/"); // month
    client.print(raw & 0xF); client.print("/"); // day
      client.println((raw >> 9 ) + 1980); // year
    }
    else if(battery.commands[i].type == "string") { // char array, library is broken on this, but hack seems to work
      char s[33]; //max string is 32 chars + null terminator
      s[32] = (char)0; //for bof safety
      battery.sbsReadString(s, battery.commands[i].code);
      client.println(s);
    }
    else
    client.println("?");

    
    client.print("</br>");
  }
}
