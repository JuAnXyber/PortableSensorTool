#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHTesp.h>
#include <ModbusMaster.h>
// #include <SoftwareSerial.h>

DHTesp dhtSensor;
LiquidCrystal_I2C LCD = LiquidCrystal_I2C(0x27, 16, 2);
// SoftwareSerial rs485Serial(2, 15);
HardwareSerial rs485Serial(1);
ModbusMaster node;

#define btn1 15
#define btn2 4
#define btn3 5
#define btn4 18
#define btn5 19
#define relay 2

#define RX_PIN 16
#define TX_PIN 17



int page = 0;
int pilihanSensor = 0;

int tempSlaveId = 0;
int tempDigit1 = 0;
int tempDigit2 = 0;
int tempDigit3 = 0;

int tempContainer = 0;

int humidSlaveId = 0;
int temphumidSlaveId = 0;
int humidDigit1 = 0;
int humidDigit2 = 0;
int humidDigit3 = 0;

int humidContainer = 0;

int tmbol = 0;
int scan;

int tempInputMode = 0;
int humidInputMode = 0;

unsigned long previousMillis = 0;  
const long interval = 300;

//int slaveId = 255;

uint8_t result;


// const int DHT_PIN = 2;

float temp,humid;
uint8_t id_temp,id_humid;
uint8_t readsensor(int slaveId,uint8_t regsensor,uint8_t regid,float *sens,uint8_t *id){
  node.begin(slaveId, rs485Serial);
  result = node.readHoldingRegisters(regsensor, 1);
  if (result == node.ku8MBSuccess) {
    *sens = (float)node.getResponseBuffer(0)/10;  
  } 
  node.clearResponseBuffer();
  // delay(30);
  result = node.readHoldingRegisters(regid, 1);
  if (result == node.ku8MBSuccess) {
    *id = (float)node.getResponseBuffer(0);  
  } 
   return result;
  
}

void page_pilihSensor() {
  LCD.setCursor(0, 1);
  LCD.println("<Back Next>   OK");
  if (pilihanSensor == 1) {
    LCD.setCursor(0, 0);
    LCD.println("<  temperature >");
    Serial.println(pilihanSensor);
  } else if (pilihanSensor == 2) {
    LCD.setCursor(0, 0);
    LCD.println("<   humidity   >");
    Serial.println(pilihanSensor);
  }
  if (digitalRead(btn1) == 0) {
    while (digitalRead(btn1) == 0)
    {
      delay(50);
    }
    pilihanSensor = pilihanSensor + 1;
    Serial.println(pilihanSensor); }
  else if (digitalRead(btn2) == 0) {
    while (digitalRead(btn2) == 0)
    {
      delay(50);
    }
    pilihanSensor = pilihanSensor + 1;
    Serial.println(pilihanSensor);}
  else if (digitalRead(btn5) == 0) {
    while (digitalRead(btn5) == 0)
    {
      delay(50);
    }
    Serial.println("OK");
    Serial.println(pilihanSensor);
    scan = 1;
    Serial.println("SCAN: " + scan);
    if (pilihanSensor == 1) {
      LCD.clear();
      page = 2;
    } else if (pilihanSensor == 2) {
      LCD.clear();
      page = 7;
    }
  }
  if (pilihanSensor == 3) {
    pilihanSensor = 1;
  }
  Serial.println(page);
}


void page_humidConnectMode() {

  LCD.setCursor(0,0);
  LCD.print("CONNECTING MODE:");
  LCD.setCursor(0,1);
  LCD.print("<AUTO MANUAL>   ");
  

  if (digitalRead(btn1)==0)
  {
    while (digitalRead(btn1)==0) {
    delay(50);
    }
    LCD.setCursor(0,0);
    LCD.print("CONNECTING .....");
    LCD.setCursor(0,1);
    LCD.print("                ");
    LCD.clear();
    page = 8;
  }
  else if (digitalRead(btn2)==0)
  {
    while (digitalRead(btn2)==0) {
    delay(50);
    }
    LCD.clear();
    LCD.setCursor(10, 0);
    LCD.print("000");
    page = 9;
  }
}

void page_humidAutoScan() {
  int scanId = 1; 
  LCD.setCursor(0,0);
  LCD.print("CONNECTING");
  LCD.setCursor(0,1);
  LCD.print("<BACK");
  // if (digitalRead(btn1) == 0) {
  //   while (digitalRead(btn1) == 0)
  //   {
  //     delay(50);
  //   }
  //   Serial.print("BACK");
  //   LCD.clear();
  //   page = 0;
  // }
  for (scanId = 1; scanId <=247; scanId++) {
    delay(500);
    node.begin(scanId, rs485Serial);
    if (digitalRead(btn1) == 0) {
    // while (digitalRead(btn1) == 0)
    // {
    //   delay(50);
    // }
    break;
    Serial.print("BACK");
    LCD.clear();
    page = 0;
  }
    if (node.readHoldingRegisters(0x0002, 1) == node.ku8MBSuccess) {
      Serial.print("Slave Id Found: ");
      Serial.println(scanId);
      humidSlaveId = scanId;
      LCD.clear();
      page = 1;
      break;
    } 
    else {
      LCD.setCursor(0, 0);
      LCD.print("CONNECTING ID ");
      LCD.setCursor(13, 0);
      LCD.print(scanId); 
      Serial.print("No Response from Slave Id: ");
      Serial.println(scanId);
      if (scanId == 247)
      {
        if (node.readHoldingRegisters(0x0002, 1) != node.ku8MBSuccess)
        {
          LCD.setCursor(0,0);
          LCD.print("CONNECT FAILED  ");
          LCD.setCursor(0,1);
          LCD.print("TRY MANUAL INPUT");
          delay(3000);
          LCD.clear();
          page = 9;
        }
      }
    };
    Serial.println("----------------");
    Serial.println(scanId);
    Serial.println("SCAN: " + scan);
  }
  if (digitalRead(btn1) == 0) {
    // while (digitalRead(btn1) == 0)
    // {
    //   delay(50);
    // }
    
    Serial.print("BACK");
    LCD.clear();
    page = 0;
  }
  Serial.println("SCAN: " + scan);
  if (node.readHoldingRegisters(humidSlaveId, 1) == node.ku8MBSuccess) {
    // Serial.print("readn humid");
    // LCD.setCursor(0, 0);
    // LCD.print(String(humid) + "%" + " ID:" + String(id_humid));
    // Serial.println("Humidity: " + String(humid) + "%");
    // Serial.println("id: " + String(id_humid) );
  }
}

void page_humidManual() {
  // LCD.setCursor(13,0);
  // LCD.print(" ");
  Serial.print(humidContainer);
  if (humidInputMode == 0) 
  {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      static bool toggle = false;
      LCD.setCursor(10, 0);
      if (toggle) {
        LCD.print(humidDigit1);
        Serial.print("NYALA");
      } else {
        LCD.print(" ");
        Serial.print("MATI");
      }
      toggle = !toggle;
    };
    LCD.setCursor(0, 0);
    LCD.print("INPUT ID: ");
    LCD.setCursor(11, 0);
    LCD.print(humidDigit2);
    LCD.setCursor(12, 0);
    LCD.print(humidDigit3);
    LCD.setCursor(0, 1);
    LCD.println("<Back Sel - + OK");
    if (humidContainer != 0) {
      if (digitalRead(btn3) == 0) {
      while (digitalRead(btn3) == 0)
      {
        delay(50);
      }
      humidContainer = humidContainer - 1;
      }
    // Serial.println(tempDigit1);
    };
    if (digitalRead(btn4) == 0) {
      while (digitalRead(btn4) == 0)
      {
        delay(50);
      }
      humidContainer = humidContainer + 1;
    // Serial.println(tempDigit1);
    };
    humidDigit1 = humidContainer;
    if (humidContainer <= 0) {
      humidContainer = 0;
    } else if (humidContainer >= 3) {
      humidContainer = 0;
    };
  }
  else if (humidInputMode == 1) 
  {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      static bool toggle = false;
      LCD.setCursor(11, 0);
      if (toggle) {
        LCD.print(humidDigit2);
        Serial.print("NYALA");
      } else {
        LCD.print(" ");
        Serial.print("MATI");
      }
      toggle = !toggle;
    };
    LCD.setCursor(0, 0);
    LCD.print("INPUT ID: ");
    LCD.setCursor(10, 0);
    LCD.print(humidDigit1);
    LCD.setCursor(12, 0);
    LCD.print(humidDigit3);
    LCD.setCursor(0, 1);
    LCD.println("<Back Sel - + OK");
    if (humidContainer != 0) {
      if (digitalRead(btn3) == 0) {
      while (digitalRead(btn3) == 0)
      {
        delay(50);
      }
      humidContainer = humidContainer - 1;
      }
    };
    if (digitalRead(btn4) == 0) {
        while (digitalRead(btn4) == 0)
        {
          delay(50);
        }
        humidContainer = humidContainer + 1;
      // Serial.println(tempDigit2);
    };
    humidDigit2 = humidContainer;
    if (humidContainer <= 0) {
      humidContainer = 0;
    } else if (humidContainer >= 10) {
      humidContainer = 0;
    };
  }
  else if (humidInputMode == 2) 
  {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      static bool toggle = false;
      LCD.setCursor(12, 0);
      if (toggle) {
        LCD.print(humidDigit3);
        Serial.print("NYALA");
      } else {
        LCD.print(" ");
        Serial.print("MATI");
      }
      toggle = !toggle;
    };
    LCD.setCursor(0, 0);
    LCD.print("INPUT ID: ");
    LCD.setCursor(10, 0);
    LCD.print(humidDigit1);
    LCD.setCursor(11, 0);
    LCD.print(humidDigit2);
    if (humidContainer != 0) {
      if (digitalRead(btn3) == 0) {
      while (digitalRead(btn3) == 0)
      {
        delay(50);
      }
      humidContainer = humidContainer - 1;
      }
    };
    if (digitalRead(btn4) == 0) {
        while (digitalRead(btn4) == 0)
        {
          delay(50);
        }
        humidContainer = humidContainer + 1;
      // Serial.println(tempDigit3);
    };
    humidDigit3 = humidContainer;
    if (humidContainer <= 0) {
      humidContainer = 0;
    } else if (humidContainer >= 10) {
      humidContainer = 0;
    };
  }
  if (digitalRead(btn1) == 0) {
    while (digitalRead(btn1) == 0)
    {
      delay(50);
    }
    page = 7;
    LCD.clear();
  } 
  else if (digitalRead(btn2) == 0) {
    while (digitalRead(btn2) == 0)
    {
      delay(50);
    }
    humidInputMode++;
    humidContainer = 0;
    if (humidInputMode >=3) 
    {
      humidInputMode = 0;
    }
  }
  else if (digitalRead(btn5) == 0) {

    while (digitalRead(btn5) == 0)
    {
      delay(50);
    }
    
    humidDigit1 = humidDigit1*100;
    humidDigit2 = humidDigit2*10;
    humidDigit3 = humidDigit3*1;
    humidSlaveId = humidDigit1+humidDigit2+humidDigit3;

    if (humidSlaveId >= 247)
    {
      LCD.setCursor(0,0);
      LCD.print("CONNECT ID ERROR");
      LCD.setCursor(0,1);
      LCD.print("MAX SLAVE ID 247");
      delay(3000);
      LCD.clear();
      page = 7;
    }
    else
    {
      node.begin(humidSlaveId, rs485Serial);
      if (node.readHoldingRegisters(humidSlaveId, 1) == node.ku8MBSuccess) {
        LCD.clear();
        page = 1;
      }
      else 
      {
        LCD.setCursor(0, 0);
        LCD.print("CONNECT FAILED  ");
        LCD.setCursor(0, 1);
        LCD.print("TRY ANOTHER ID  ");
        delay(3000);
        page = 7;
      }
    }
  }
}

void page_rwSensorHumidity() {
  
  readsensor(humidSlaveId,0x0001,48,&humid,&id_humid);
  LCD.setCursor(0, 0);
  LCD.print(String(humid) + "%" + " ID:" + String(id_humid));
  Serial.println("Humidity: " + String(humid) + "%");
  Serial.println("id: " + String(id_humid) );
  LCD.setCursor(0, 1);
  LCD.print("<Back Write>    ");

  if (digitalRead(btn1) == 0) {
    LCD.clear();
    page = 0;
  } else if (digitalRead(btn2) == 0) {
    while (digitalRead(btn2) == 0)
    {
      delay(50);
    }    
    LCD.clear();
    page = 3;
  }
  Serial.println("SCAN:" + humidSlaveId);
  Serial.println(page);
}

void page_rwSensorTemperature() {
  readsensor(255,0,2,&temp,&id_temp);
  LCD.setCursor(0, 0);
  LCD.print(String(temp) + "C" + " ID:" + String(id_temp));
  LCD.setCursor(0, 1);
  LCD.print("<Back Write>    ");
  Serial.println("Temperature: " + String(temp) + "°C");
  Serial.println("id: " + String(id_temp) );
  if (digitalRead(btn1) == 0) {
    page = 0;
    pilihanSensor = 1;
    LCD.setCursor(0, 0);
    LCD.println("< PILIH SENSOR >");
  } else if (digitalRead(btn2) == 0) {
    while (digitalRead(btn2) == 0)
    {
      delay(50);
    }
    page = 4;
    Serial.println(page);
    LCD.clear();
    LCD.setCursor(10, 0);
    LCD.print("000");
  }
  Serial.println(page);
}

void page_writeSensorHumidity() {
  LCD.setCursor(13,0);
  LCD.print(" ");
  LCD.setCursor(0, 1);
  LCD.println("<Back Sel - + OK");
  LCD.setCursor(0, 0);
  LCD.print("INPUT ID: ");
  if (humidInputMode == 0) 
  {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      static bool toggle = false;
      LCD.setCursor(10, 0);
      if (toggle) {
        LCD.print(humidDigit1);
        Serial.print("NYALA");
      } else {
        LCD.print(" ");
        Serial.print("MATI");
      }
      toggle = !toggle;
    };
    LCD.setCursor(11, 0);
    LCD.print(humidDigit2);
    LCD.setCursor(12, 0);
    LCD.print(humidDigit3);
    if (digitalRead(btn3) == 0) {
      while (digitalRead(btn3) == 0)
      {
        delay(50);
      }
      humidContainer = humidContainer - 1;
      // Serial.println(tempDigit1);
    } else if (digitalRead(btn4) == 0) {
        while (digitalRead(btn4) == 0)
        {
          delay(50);
        }
        humidContainer = humidContainer + 1;
      // Serial.println(tempDigit1);
    };
    humidDigit1 = humidContainer;
    if (humidContainer <= 0) {
      humidContainer = 0;
    } else if (humidContainer >= 3) {
      humidContainer = 0;
    };
  }
  else if (humidInputMode == 1) 
  {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      static bool toggle = false;
      LCD.setCursor(11, 0);
      if (toggle) {
        LCD.print(humidDigit2);
        Serial.print("NYALA");
      } else {
        LCD.print(" ");
        Serial.print("MATI");
      }
      toggle = !toggle;
    };
    LCD.setCursor(10, 0);
    LCD.print(humidDigit1);
    LCD.setCursor(12, 0);
    LCD.print(humidDigit3);
    if (digitalRead(btn3) == 0) {
      while (digitalRead(btn3) == 0)
      {
        delay(50);
      }
      humidContainer = humidContainer - 1;
      // Serial.println(tempDigit2);
    } else if (digitalRead(btn4) == 0) {
        while (digitalRead(btn4) == 0)
        {
          delay(50);
        }
        humidContainer = humidContainer + 1;
      // Serial.println(tempDigit2);
    };
    humidDigit2 = humidContainer;
    if (humidContainer <= 0) {
      humidContainer = 0;
    } else if (humidContainer >= 10) {
      humidContainer = 0;
    };
  }
  else if (humidInputMode == 2) 
  {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      static bool toggle = false;
      LCD.setCursor(12, 0);
      if (toggle) {
        LCD.print(humidDigit3);
        Serial.print("NYALA");
      } else {
        LCD.print(" ");
        Serial.print("MATI");
      }
      toggle = !toggle;
    };
    LCD.setCursor(10, 0);
    LCD.print(humidDigit1);
    LCD.setCursor(11, 0);
    LCD.print(humidDigit2);
    if (digitalRead(btn3) == 0) {
      while (digitalRead(btn3) == 0)
      {
        delay(50);
      }
      humidContainer = humidContainer - 1;
      // Serial.println(tempDigit3);
    } else if (digitalRead(btn4) == 0) {
        while (digitalRead(btn4) == 0)
        {
          delay(50);
        }
        humidContainer = humidContainer + 1;
      // Serial.println(tempDigit3);
    };
    humidDigit3 = humidContainer;
    if (humidContainer <= 0) {
      humidContainer = 0;
    } else if (humidContainer >= 10) {
      humidContainer = 0;
    };
  }
  if (digitalRead(btn1) == 0) {
    while (digitalRead(btn1) == 0)
    {
      delay(50);
    }
    page = 1;
    LCD.setCursor(0, 0);
    LCD.println("< PILIH SENSOR >");
  } 
  else if (digitalRead(btn2) == 0) {
    while (digitalRead(btn2) == 0)
    {
      delay(50);
    }
    humidInputMode++;
    if (humidInputMode >=3) 
    {
      humidInputMode = 0;
    }
  }
  else if (digitalRead(btn5) == 0) {

    while (digitalRead(btn5) == 0)
    {
      delay(50);
    }
    
    humidDigit1 = humidDigit1*100;
    humidDigit2 = humidDigit2*10;
    humidDigit3 = humidDigit3*1;
    humidSlaveId = humidDigit1+humidDigit2+humidDigit3;

    if (humidSlaveId >= 247)
    {
      LCD.setCursor(0,0);
      LCD.print("WRITING ID ERROR");
      LCD.setCursor(0,1);
      LCD.print("MAX SLAVE ID 247");
      delay(5000);
      humidContainer=0;
      humidDigit1=0;
      humidDigit2=0;
      humidDigit3=0;
      page = 5;
      LCD.clear();
    }
    else
    {
      if (node.writeSingleRegister(0x30, humidSlaveId) == node.ku8MBSuccess)
        {
          LCD.clear();
          LCD.setCursor(0,0);
          LCD.print("WRITING SUCCESS ");
          LCD.setCursor(0,1);
          LCD.print("WRITING ID : ");
          LCD.setCursor(13, 1);
          LCD.print(humidSlaveId);
          delay(2000);
          LCD.clear();
          page = 5;
        }
        else 
        {
          LCD.setCursor(0,0);
          LCD.print("WRITING ID ERROR");
          LCD.setCursor(0,1);
          LCD.print("MAX SLAVE ID 247");
          delay(5000);
          humidContainer=0;
          humidDigit1=0;
          humidDigit2=0;
          humidDigit3=0;
          page = 1;
          LCD.clear();
        }
      }
    }
  }

void page_writeSensorTemperature() {
  LCD.setCursor(0, 1);
  LCD.println("<Back Sel - + OK");
  LCD.setCursor(0, 0);
  LCD.print("INPUT ID: ");
  LCD.setCursor(13,0);
  LCD.print(" ");
  if (tempInputMode == 0) 
  {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      static bool toggle = false;
      LCD.setCursor(10, 0);
      if (toggle) {
        LCD.print(tempDigit1);
        Serial.print("NYALA");
      } else {
        LCD.print(" ");
        Serial.print("MATI");
      }
      toggle = !toggle;
    };
    LCD.setCursor(11, 0);
    LCD.print(tempDigit2);
    LCD.setCursor(12, 0);
    LCD.print(tempDigit3);
    if (digitalRead(btn3) == 0) {
      while (digitalRead(btn3) == 0)
      {
        delay(50);
      }
      tempContainer = tempContainer - 1;
      LCD.setCursor(10, 0);
      LCD.print(tempDigit1);
      // Serial.println(tempDigit1);
    } else if (digitalRead(btn4) == 0) {
        while (digitalRead(btn4) == 0)
        {
          delay(50);
        }
        tempContainer = tempContainer + 1;
        LCD.setCursor(10, 0);
        LCD.print(tempDigit1);
      // Serial.println(tempDigit1);
    };
    tempDigit1 = tempContainer;
    if (tempContainer <= 0) {
      tempContainer = 0;
    } else if (tempContainer >= 3) {
      tempContainer = 0;
    };
  }
  else if (tempInputMode == 1) 
  {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      static bool toggle = false;
      LCD.setCursor(11, 0);
      if (toggle) {
        LCD.print(tempDigit2);
        Serial.print("NYALA");
      } else {
        LCD.print(" ");
        Serial.print("MATI");
      }
      toggle = !toggle;
    };
    LCD.setCursor(10, 0);
    LCD.print(tempDigit1);
    LCD.setCursor(12, 0);
    LCD.print(tempDigit3);
    if (digitalRead(btn3) == 0) {
      while (digitalRead(btn3) == 0)
      {
        delay(50);
      }
      tempContainer = tempContainer - 1;
      // Serial.println(tempDigit2);
    } else if (digitalRead(btn4) == 0) {
        while (digitalRead(btn4) == 0)
        {
          delay(50);
        }
        tempContainer = tempContainer + 1;
      // Serial.println(tempDigit2);
    };
    tempDigit2 = tempContainer;
    if (tempContainer <= 0) {
      tempContainer = 0;
    } else if (tempContainer >= 10) {
      tempContainer = 0;
    };
  }
  else if (tempInputMode == 2) 
  {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      static bool toggle = false;
      LCD.setCursor(12, 0);
      if (toggle) {
        LCD.print(tempDigit3);
        Serial.print("NYALA");
      } else {
        LCD.print(" ");
        Serial.print("MATI");
      }
      toggle = !toggle;
    };
    LCD.setCursor(10, 0);
    LCD.print(tempDigit1);
    LCD.setCursor(11, 0);
    LCD.print(tempDigit2);
    if (digitalRead(btn3) == 0) {
      while (digitalRead(btn3) == 0)
      {
        delay(50);
      }
      tempContainer = tempContainer - 1;
      // Serial.println(tempDigit3);
    } else if (digitalRead(btn4) == 0) {
        while (digitalRead(btn4) == 0)
        {
          delay(50);
        }
        tempContainer = tempContainer + 1;
      // Serial.println(tempDigit3);
    }
    tempDigit3 = tempContainer;
    if (tempContainer <= 0) {
      tempContainer = 0;
    } else if (tempContainer >= 10) {
      tempContainer = 0;
    };
  }
  if (digitalRead(btn1) == 0) {
    while (digitalRead(btn1) == 0)
    {
      delay(50);
    }
    page = 0;
    LCD.setCursor(0, 0);
    LCD.println("< PILIH SENSOR >");
  } 
  else if (digitalRead(btn2) == 0) {
    while (digitalRead(btn2) == 0)
    {
      delay(50);
    }
    tempInputMode++;
    tempContainer=0;
    if (tempInputMode >=3) 
    {
      tempInputMode = 0;
    }
  }
  else if (digitalRead(btn5) == 0) {

    while (digitalRead(btn5) == 0)
    {
      delay(50);
    }
    
    tempDigit1 = tempDigit1*100;
    tempDigit2 = tempDigit2*10;
    tempDigit3 = tempDigit3*1;
    tempSlaveId = tempDigit1+tempDigit2+tempDigit3;

    if (node.writeSingleRegister(0x002, tempSlaveId) == node.ku8MBSuccess)
    {
      LCD.clear();
      LCD.setCursor(0,0);
      LCD.print("WRITING SUCCESS ");
      LCD.setCursor(0,1);
      LCD.print("WRITING ID : ");
      LCD.setCursor(13, 1);
      LCD.print(tempSlaveId);
      delay(2000);
      LCD.clear();
      page = 6;
    }
    else if (node.writeSingleRegister(0x002, tempSlaveId) != node.ku8MBSuccess, tempSlaveId == 0)
    {
      LCD.setCursor(0,0);
      LCD.print("WRITING ID ERROR");
      LCD.setCursor(0,1);
      LCD.print("CANNOT USE ID 0 ");
      delay(5000);
      tempContainer=0;
      tempDigit1=0;
      tempDigit2=0;
      tempDigit3=0;
      page = 2;
      LCD.clear();
    }
    else if (node.writeSingleRegister(0x002, tempSlaveId) != node.ku8MBSuccess, tempSlaveId >= 247)
    {
      LCD.setCursor(0,0);
      LCD.print("WRITING ID ERROR");
      LCD.setCursor(0,1);
      LCD.print("MAX SLAVE ID 247");
      delay(5000);
      tempContainer=0;
      tempDigit1=0;
      tempDigit2=0;
      tempDigit3=0;
      page = 2;
      LCD.clear();
    }
    else
    {
      LCD.setCursor(0,0);
      LCD.print("WRITING ID ERROR");
      LCD.setCursor(0,1);
      LCD.print("UNKOWN ERROR    ");
      delay(5000);
      tempContainer=0;
      tempDigit1=0;
      tempDigit2=0;
      tempDigit3=0;
      page = 2;
      LCD.clear();
    }
    Serial.print("ID : " + tempSlaveId);
  }
}

void page_vadidateWriteHumidity() {
  LCD.setCursor(0, 0);
  LCD.println("RESTARTING .....");
  LCD.setCursor(0, 1);
  LCD.println("PLEASE WAIT ....");
  humidContainer=0;
  humidDigit1=0;
  humidDigit2=0;
  humidDigit3=0;
  digitalWrite(relay, HIGH);
  Serial.println("relay nyala");
  delay(3000);
  Serial.println("relay mati");
  digitalWrite(relay, LOW);
  LCD.setCursor(0, 0);
  LCD.println("SUCCESS RESTART ");
  LCD.setCursor(0, 1);
  LCD.println("WAIT FOR 2 SEC  ");
  delay(2000);
  LCD.clear();
  scan = 1;
  page = 1;
}

void page_vadidateWriteTemperature() {
  LCD.setCursor(0, 0);
  LCD.println("RESTARTING .....");
  LCD.setCursor(0, 1);
  LCD.println("PLEASE WAIT ....");
  tempContainer=0;
  tempDigit1=0;
  tempDigit2=0;
  tempDigit3=0;
  digitalWrite(relay, HIGH);
  delay(5000);
  digitalWrite(relay, LOW);
  LCD.setCursor(0, 0);
  LCD.println("SUCCESS RESTART ");
  LCD.setCursor(0, 1);
  LCD.println("WAIT FOR 2 SEC  ");
  delay(2000);
  LCD.clear();
  page = 2;
}



void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
  pinMode(btn3, INPUT_PULLUP);
  pinMode(btn4, INPUT_PULLUP);
  pinMode(btn5, INPUT_PULLUP);
  pinMode(relay, OUTPUT);
  // pinMode(sensor, INPUT);
  // dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  rs485Serial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  
  LCD.init();
  LCD.backlight();
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.println("< PILIH SENSOR >");
  LCD.setCursor(0, 1);
  LCD.println("<Back Next>   OK");
}



void loop() {
//  page_rwSensorTemperature();
  Serial.println(page);
  if (page == 0) {
    page_pilihSensor();
  }
  if (page == 1) {
    page_rwSensorHumidity();
  }
  if (page == 2) {
    page_rwSensorTemperature();
  }
  if (page == 3) {
    page_writeSensorHumidity();
  }
  if (page == 4) {
    page_writeSensorTemperature();
  }
  if (page == 5) {
    page_vadidateWriteHumidity();
  }
  if (page == 6) {
    page_vadidateWriteTemperature();
  };
  if (page == 7) {
    page_humidConnectMode();
  }
  if (page == 8) {
    page_humidAutoScan();
  }
  if (page == 9) {
    page_humidManual();
  }

  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  
  // delay(50);
  if (Serial.available()) {
    String command = Serial.readString();  // Membaca pesan dari Serial Monitor
    
    if (command == "RESET") {  // Jika pesan berisi "RESET"
      Serial.println("ESP32 akan di-reset.");
      delay(1001);  // Penundaan sejenak sebelum reset
      ESP.restart();  // Reset ESP32
    }
  }
}
