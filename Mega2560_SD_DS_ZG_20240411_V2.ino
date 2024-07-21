#include <ModbusMaster.h>
#include <RTClib.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>

RTC_DS3231 rtc;
char t[32];
File dataFile;
unsigned long fileSize = 0;
#define FILE_SIZE_LIMIT 1000000  // 文件大小限制为1 Mbyte

ModbusMaster node;
const int slaveID = 1;
const uint16_t regAddressV1 = 0x0000;  // 寄存器起始地址 V1电压有效值
const uint16_t regAddressV2 = 0x0001;  // 寄存器起始地址 V1电压有效值
const uint16_t regAddressV3 = 0x0002;  // 寄存器起始地址 V1电压有效值
const uint16_t regLength = 1;          // 寄存器长度
char filename[] = "data00.txt";

void setup() {
  Serial.begin(9600);  // 初始化串口通信
  rtc.begin();
  //rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
 //rtc.adjust(DateTime(2024, 4, 8, 21, 15, 0));

  if (SD.begin()) {
    Serial.println("SD card is ready to use.");
    createNewFile();
  } else {
    Serial.println("SD card initialization failed");
    return;
  }
 if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  // 创建一个新文件
  //createNewFile();

  delay(1000);                   // 延迟一段时间后再次读取
  Serial3.begin(9600);           // 初始化Modbus通信的串口
  node.begin(slaveID, Serial3);  // 设置Modbus通信的串口
}

void loop() {
  Serial.println(' ');
  showSer();
  //SReadHold(regAddressV1,regLength);
  // SReadHold(regAddressV2,regLength);
  // SReadHold(regAddressV3,regLength);

  //-----------------------------------------------------
  if (Serial.available()) {
    String receivedTime = Serial.readStringUntil('\n'); // 讀取從 PC 發送的時間
    
     if (receivedTime == "LIST_FILES") {
      listFiles();
    } else {
      setTime(receivedTime);
    }

    Serial.println("Received Time: ");
    Serial.println(receivedTime);

    //rtc.adjust(currentTime); // 設定 DS3231 的時間為從 PC 接收的時間
    Serial.println("Time updated!");
  }
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  delay(1000);  // 延迟一段时间后再次读取
  dataFile = SD.open(filename, FILE_WRITE);
  DateTime now = rtc.now();
  if (dataFile) {
    dataFile.print(now.year(), DEC);
    dataFile.print("-");
    dataFile.print(now.month(), DEC);
    dataFile.print("-");
    dataFile.print(now.day(), DEC);
    dataFile.print(" ");
    dataFile.print(now.hour(), DEC);
    dataFile.print(':');
    dataFile.print(now.minute(), DEC);
    dataFile.print(':');
    dataFile.print(now.second(), DEC);
    uint16_t result1;
    uint16_t data[6];  // 存储读取到的寄存器数据
                       // 读取寄存器数据
    result1 = node.readHoldingRegisters(regAddressV1, regLength);
    // 检查读取是否成功
    if (result1 == node.ku8MBSuccess) {
      // 打印读取到的寄存器数据
      data[0] = node.getResponseBuffer(0);
      dataFile.print(" V1:");
      dataFile.print(data[0] * 0.01, 2);
      dataFile.print(' ');
    } else {
      // 打印错误信息
      dataFile.print("Modbus read failed: ");
      dataFile.println(result1, DEC);
    }
    uint16_t result2;
    result2 = node.readHoldingRegisters(regAddressV2, regLength);
    // 检查读取是否成功
    if (result2 == node.ku8MBSuccess) {
      // 打印读取到的寄存器数据
      data[1] = node.getResponseBuffer(0);
      dataFile.print(" V2:");
      dataFile.print(data[1] * 0.01, 2);
      dataFile.print(' ');
    } else {
      // 打印错误信息
      dataFile.print("Modbus read failed: ");
      dataFile.println(result2, DEC);
    }
    uint16_t result3;
    result3 = node.readHoldingRegisters(regAddressV3, regLength);
    // 检查读取是否成功
    if (result3 == node.ku8MBSuccess) {
      // 打印读取到的寄存器数据
      data[2] = node.getResponseBuffer(0);
      dataFile.print(" V3:");
      dataFile.print(data[2] * 0.01, 2);
      dataFile.print(' ');
    } else {
      // 打印错误信息
      dataFile.print("Modbus read failed: ");
      dataFile.println(result3, DEC);
    }
    dataFile.print('\n');
    dataFile.close();  // close the file
  }
  // if the file didn't open, print an error:
  else {
    Serial.println("error opening test.txt");
  }

  // delay(1000);  // 延迟一段时间后再次读取
}

int32_t SReadHold(uint16_t reg, uint16_t le) {
  uint16_t result;
  uint16_t data[regLength];  // 存储读取到的寄存器数据
  uint16_t FValue = 0;

  // 读取寄存器数据
  result = node.readHoldingRegisters(reg, le);
  // 检查读取是否成功
  if (result == node.ku8MBSuccess) {
    // 打印读取到的寄存器数据
    data[0] = node.getResponseBuffer(0);
    Serial.print(" V:");
    Serial.print(data[0] * 0.01, 2);
    Serial.print(' ');

  } else {
    // 打印错误信息
    Serial.print("Modbus read failed: ");
    Serial.println(result, DEC);
  }
}


void WriData() {
  uint16_t result;
  uint16_t data[6];  // 存储读取到的寄存器数据
  uint16_t FValue = 0;
  dataFile = SD.open(filename, FILE_WRITE);
  DateTime now = rtc.now();
  if (dataFile) {
    dataFile.print(now.year(), DEC);
    dataFile.print("-");
    dataFile.print(now.month(), DEC);
    dataFile.print("-");
    dataFile.print(now.day(), DEC);
    dataFile.print(" ");
    dataFile.print(now.hour(), DEC);
    dataFile.print(':');
    dataFile.print(now.minute(), DEC);
    dataFile.print(':');
    dataFile.print(now.second(), DEC);
    dataFile.print('\n');
    //-------------------------------------------------------------------------
    // WReadHold(regAddressV1,regLength);
    // WReadHold(regAddressV2,regLength);
    // WReadHold(regAddressV3,regLength);
    dataFile.close();  // close the file
  }
  // if the file didn't open, print an error:
  else {
    Serial.println("error opening data.txt");
  }
}

int32_t WReadHold(uint16_t reg, uint16_t le) {
  uint16_t result;
  uint16_t data[regLength];  // 存储读取到的寄存器数据
  uint16_t FValue = 0;

  // 读取寄存器数据
  result = node.readHoldingRegisters(reg, le);
  // 检查读取是否成功
  if (result == node.ku8MBSuccess) {
    // 打印读取到的寄存器数据
    data[0] = node.getResponseBuffer(0);
    dataFile.print(" V:");
    dataFile.print(data[0] * 0.01, 2);
    dataFile.print(' ');
    dataFile.flush();  // 刷新缓冲区

    // 检查文件大小是否超过限制
    if (dataFile.size() > FILE_SIZE_LIMIT) {
      dataFile.close();
      createNewFile();  // 创建一个新文件
    }
  } else {
    // 打印错误信息
    dataFile.print("Modbus read failed: ");
    dataFile.println(result, DEC);
  }
}


void showSer() {
  //-------------------------------------------------------------
  DateTime now = rtc.now();
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  // Serial.println();
  SReadHold(regAddressV1, regLength);
  SReadHold(regAddressV2, regLength);
  SReadHold(regAddressV3, regLength);
}

void createNewFile() {

  for (uint8_t i = 0; i < 100; i++) {
    filename[4] = i / 10 + '0';
    filename[5] = i % 10 + '0';
    if (!SD.exists(filename)) {
      break;
    }
  }

  dataFile = SD.open(filename, FILE_WRITE);
  if (dataFile) {
    fileSize = 0;
    Serial.print("Crate file: ");
    Serial.println(filename);
  } else {
    Serial.println("Crate file fail !");
  }
}
void setTime(String timeStr) {
  int year = timeStr.substring(0, 4).toInt();
  int month = timeStr.substring(5, 7).toInt();
  int day = timeStr.substring(8, 10).toInt();
  int hour = timeStr.substring(11, 13).toInt();
  int minute = timeStr.substring(14, 16).toInt();
  int second = timeStr.substring(17, 19).toInt();

  rtc.adjust(DateTime(year, month, day, hour, minute, second));
 }


void listFiles() {
  File root = SD.open("/");
  while (true) {
    File entry = root.openNextFile();
    if (!entry) {
      break;
    }
    Serial.println(entry.name());
    entry.close();
  }
}

