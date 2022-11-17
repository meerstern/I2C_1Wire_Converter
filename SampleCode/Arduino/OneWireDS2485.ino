#include <Arduino.h>
#include <Wire.h>

#define DS2485_ADDRESS  0x40
#define ONE_WIRE_MAX_SEARCH_NUM 10
#define ONE_WIRE_ID_LENGTH      8

struct OneWireDevice {
  uint8_t id[ONE_WIRE_ID_LENGTH];
  float temp;
};

uint8_t       foundDeviceNum=0;
OneWireDevice OneWireDev[ONE_WIRE_MAX_SEARCH_NUM];

void DS2485_WriteData(uint8_t *data, uint8_t dsize)
{
  Wire.beginTransmission(DS2485_ADDRESS);
  Wire.write(data,dsize);
  Wire.endTransmission(1);
  delay(1);
}

void DS2485_ReadData(uint8_t *data, uint8_t dsize)
{
  uint8_t index;
  Wire.requestFrom((uint8_t)(DS2485_ADDRESS), (uint8_t)dsize);
  while (Wire.available()&& index<dsize) 
  {
    *(data+index)=Wire.read();
    index++;
  }

}

void DS2485_Reset()
{
  uint8_t wdata[1]={0x62};
  DS2485_WriteData(wdata,1);
}

void DS2485_SetRPUP()
{
  uint8_t wdata[5]={0x99,0x03,0x11,0x06,0x00};
  DS2485_WriteData(wdata,5);
}

bool DS2485_CheckResult()
{
  uint8_t rdata[2];
  DS2485_ReadData(rdata,2);

  if(rdata[1]==0xAA)
    return true;
  else
    return false;
}

bool DS2485_SearchOneDevice()
{
  uint8_t wdata[4]={0x11,0x02,0b00000101,0xF0};
  uint8_t rdata[11]={0x00};
  bool hasNextDevice=false;
  String id="";

  if(foundDeviceNum>=ONE_WIRE_MAX_SEARCH_NUM)
    return false;
  
  DS2485_WriteData(wdata,4);
  delay(500);  
  DS2485_ReadData(rdata,11);

  if(rdata[1]==0xAA)
  {
    id= String(rdata[2], 16) + ":"
        + String(rdata[3], 16) + ":"
        + String(rdata[4], 16) + ":"
        + String(rdata[5], 16) + ":"
        + String(rdata[6], 16) + ":"
        + String(rdata[7], 16) + ":"
        + String(rdata[8], 16) + ":"
        + String(rdata[9], 16) ;
    Serial.print("Found Device:");
    Serial.println(id);
    if(rdata[10]==0)
      hasNextDevice=true;

    OneWireDev[foundDeviceNum].id[0]=rdata[2];
    OneWireDev[foundDeviceNum].id[1]=rdata[3];
    OneWireDev[foundDeviceNum].id[2]=rdata[4];
    OneWireDev[foundDeviceNum].id[3]=rdata[5];
    OneWireDev[foundDeviceNum].id[4]=rdata[6];
    OneWireDev[foundDeviceNum].id[5]=rdata[7];
    OneWireDev[foundDeviceNum].id[6]=rdata[8];
    OneWireDev[foundDeviceNum].id[7]=rdata[9];
    foundDeviceNum++;
  }
  return hasNextDevice;
}

bool DS2485_SearchNextDevice()
{
  uint8_t wdata[4]={0x11,0x02,0b00000001,0xF0};
  uint8_t rdata[11]={0x00};
  bool hasNextDevice=false;
  String id="";

  if(foundDeviceNum>=ONE_WIRE_MAX_SEARCH_NUM)
    return false;
  
  DS2485_WriteData(wdata,4);
  delay(500);   
  DS2485_ReadData(rdata,11);

  if(rdata[1]==0xAA)
  {
    id= String(rdata[2], 16) + ":"
        + String(rdata[3], 16) + ":"
        + String(rdata[4], 16) + ":"
        + String(rdata[5], 16) + ":"
        + String(rdata[6], 16) + ":"
        + String(rdata[7], 16) + ":"
        + String(rdata[8], 16) + ":"
        + String(rdata[9], 16) ;
    Serial.print("Found Device:");
    Serial.println(id);
    if(rdata[10]==0)
      hasNextDevice=true;

    OneWireDev[foundDeviceNum].id[0]=rdata[2];
    OneWireDev[foundDeviceNum].id[1]=rdata[3];
    OneWireDev[foundDeviceNum].id[2]=rdata[4];
    OneWireDev[foundDeviceNum].id[3]=rdata[5];
    OneWireDev[foundDeviceNum].id[4]=rdata[6];
    OneWireDev[foundDeviceNum].id[5]=rdata[7];
    OneWireDev[foundDeviceNum].id[6]=rdata[8];
    OneWireDev[foundDeviceNum].id[7]=rdata[9];
    foundDeviceNum++;
  }
  return hasNextDevice;
}

void DS2485_GetTempFromDS18B20(uint8_t index)
{
  uint8_t reset[4]={0x88,0x02,0x00,0b10000010};  
  uint8_t match[4]={0x88,0x02,0x03,0x55};
  uint8_t romid[12]={0x88,0x0A,0x09,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t convt[4]={0x88,0x02,0x03,0x44};
  uint8_t rscrt[4]={0x88,0x02,0x03,0xBE};//Read scratchpad
  uint8_t rbyte[4]={0x88,0x02,0x08,0x09};//Read byte
  uint8_t rdata[12]={0x00};
  float tempf=0.0f;
  romid[4]=OneWireDev[index].id[0];
  romid[5]=OneWireDev[index].id[1];
  romid[6]=OneWireDev[index].id[2];
  romid[7]=OneWireDev[index].id[3];
  romid[8]=OneWireDev[index].id[4];
  romid[9]=OneWireDev[index].id[5];
  romid[10]=OneWireDev[index].id[6];
  romid[11]=OneWireDev[index].id[7];
  
  DS2485_WriteData(reset,4);
  delay(5); 
  DS2485_WriteData(match,4);
  delay(5);  
  DS2485_WriteData(romid,12); 
  delay(5); 
  DS2485_WriteData(convt,4);
  delay(750);
  DS2485_WriteData(reset,4); 
  delay(5); 
  DS2485_WriteData(match,4); 
  delay(5); 
  DS2485_WriteData(romid,12);  
  delay(5); 
  DS2485_WriteData(rscrt,4); 
  delay(5);  
  DS2485_WriteData(rbyte,4); 
  delay(5);  
  DS2485_ReadData(rdata,12);
  delay(5); 
  uint16_t tmpByte=(uint16_t)rdata[5]*256+(uint16_t)rdata[4];
  if(tmpByte&0xF800)
    tempf=-1.0*((~tmpByte+1)*0.0625);
  else
    tempf=tmpByte*0.0625;
  OneWireDev[index].temp=tempf;

  Serial.print("Index:");
  Serial.print(index);
  Serial.print(", Temp:");
  Serial.println(tempf);
}

void setup() 
{
  Wire.begin();
  Wire.setClock(100000);
  Serial.begin(9600);
  Serial.println("Start DS2485 & 1-Wire Temp Sensor Read");
  DS2485_Reset();
  if(DS2485_CheckResult()==false)
  {
    Serial.println("Err Not Found DS2485. Check Connections!");
    while(1);
  }
  DS2485_SetRPUP();
  if(DS2485_CheckResult()==false)
  {
    Serial.println("Err Failed Setting of DS2485.");
    while(1);
  }
  
  foundDeviceNum=0;  
  Serial.println("***1-Wire Device Search Start***");
  if(DS2485_SearchOneDevice()==true)
  {
    while(DS2485_SearchNextDevice());
  }
  Serial.println("***1-Wire Device Search End***");

};

void loop() 
{

    for(uint8_t i=0; i<foundDeviceNum; i++)
    {
      DS2485_GetTempFromDS18B20(i);
      delay(10);
    }
    delay(1000);  

};
