#define SHIFT_REG_LEFT 10
#define OUT_ENABLE 13
#define LATCH_CLK 12
#define CLK 11

#define WE A1
#define OE A0

String strIn;


void setup() {
  Serial.begin(9600);
  //Shift register outputs.
  pinMode(OUT_ENABLE, OUTPUT);
  pinMode(SHIFT_REG_LEFT, OUTPUT);
  pinMode(LATCH_CLK, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A2, OUTPUT);
  
 // pinMode(CE, OUTPUT);
  pinMode(WE, OUTPUT);
  pinMode(OE, OUTPUT);


  Serial.println("~~ E/EPROM Programmer ~~");
  Serial.println("~~~~ Matthew Gilpin ~~~~");
  Serial.println("~~~~ Usage: r, w, e ~~~~");
 }

void setAddress(uint16_t address){
  digitalWrite(OUT_ENABLE, HIGH); //Turn off output of shift registers

  //Send the address to the registers
  shiftOut(SHIFT_REG_LEFT, CLK, LSBFIRST, address & 0x00FF);
  delayMicroseconds(100);
  shiftOut(SHIFT_REG_LEFT, CLK, LSBFIRST, (address & 0xFF00 >> 8));
  delayMicroseconds(100);

  // Enable the output
  digitalWrite(LATCH_CLK, HIGH);
  digitalWrite(OUT_ENABLE, LOW);
  delayMicroseconds(100);
  digitalWrite(LATCH_CLK, LOW);
}


byte readEPROM(uint16_t address){
  digitalWrite(A3, HIGH);
  for (int pin = 2; pin <= 9; pin++){
    pinMode(pin, INPUT);
    }

  delay(1);
  // digitalWrite(CE, LOW);
  digitalWrite(OE, LOW);
  digitalWrite(WE, HIGH);

  setAddress(address);
  delay(1);

  byte data = 0;
  for (int pin = 9; pin >= 2; pin--){
    data = (data << 1) + digitalRead(pin);
    }

  delay(2);

  //digitalWrite(CE, HIGH);
  digitalWrite(OE, HIGH);

  digitalWrite(A3, LOW);
  return data;
  }

void writeEPROM(uint16_t address, uint8_t dataToWrite){
  digitalWrite(A2, HIGH);
  for (int pin = 2; pin <= 9; pin++){
    pinMode(pin, OUTPUT);
    }

  digitalWrite(WE, HIGH);
  //digitalWrite(CE, HIGH);
  digitalWrite(OE, HIGH);
  delayMicroseconds(10);


  setAddress(address);
  delay(5);
  //digitalWrite(CE, LOW);
  //delay(1);
 
  
  for (int pin = 9; pin >= 2; pin--){
    digitalWrite(pin, dataToWrite & (1 << (pin - 2)));
    }

  delay(2);

  
  digitalWrite(WE, LOW);
  delayMicroseconds(1);
  digitalWrite(WE, HIGH);
  //digitalWrite(CE, HIGH);
  digitalWrite(A2, LOW);
 }

void writeData(){

  Serial.println("~~ E/EPROM Writer ~~");
  Serial.println("~~ Matthew Gilpin ~~");
  
  writeEPROM(0, 0x00);
  writeEPROM(1, 0x01);
  writeEPROM(2, 0x02);
  writeEPROM(3, 0x03);
  Serial.println("~~ Finished ~~");
  }

void readData() {
  // put your setup code here, to run once:

  Serial.println("~~ E/EPROM Reader ~~");
  Serial.println("~~ Matthew Gilpin ~~");

  delay(100);
  Serial.println("~~ Reading EEPROM ~~");
  Serial.println("Addr | 00|01|02|03|04|05|06|07");
  Serial.println("------------------------------");
  


  
  byte value[8];
  for(int i=0; i < 32; i++){
    for(int j=0; j < 8; j++){
      value[j] = readEPROM(i+j);
      }
    char buff[80];
    sprintf(buff, "%03d  | %02x %02x %02x %02x %02x %02x %02x %02x", i*8, value[0], value[1], value[2], value[3], value[4], value[5], value[6], value[7]);
    Serial.println(buff);
    
    }

}

void printContents() {
  for (int base = 0; base <= 255; base += 16) {
    byte data[16];
    for (int offset = 0; offset <= 15; offset += 1) {
      data[offset] = readEPROM(base + offset);
    }

    char buf[80];
    sprintf(buf, "%03x:  %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x",
            base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

    Serial.println(buf);
  }
}

void eraseEEPROM() {
   Serial.print("Erasing EEPROM");
  for (int address = 0; address <= 2047; address += 1) {
    writeEPROM(address, 0xff);

    if (address % 64 == 0) {
      Serial.print(".");
    }
  }
  }

void askToWriteData(){
  String dIn;
  Serial.println("Enter number followed by space");
  Serial.println("Please enter the data first: ");
  while (1) {
    if (Serial.available() > 0) {
      char incomingData = Serial.read();
      dIn += incomingData;
      if (incomingData == ' '){
        break;
        }
      }
    }
  int dataToSave = dIn.toInt();
  dIn = "";
  Serial.println("Please enter the address to store the data: ");
  while (1) {
    if (Serial.available() > 0) {
      char incomingAddr = Serial.read();
      dIn += incomingAddr;
      if (incomingAddr == ' '){
        break;
        }
      }
    }
  uint16_t address = (uint16_t)dIn.toInt();
  writeEPROM(address, dataToSave);

  Serial.println("Write Complete");
  }

void loop() {
  // put your main code here, to run repeatedly:
  
  if (Serial.available() > 0) {
    // read the incoming byte:
    char incomingByte = Serial.read();
    strIn += incomingByte; 
    if (incomingByte == 'e'){
      Serial.println("Erasing Contents");
      eraseEEPROM();
      Serial.println("~~~~ Usage: r, w, e ~~~~");
    } else if (incomingByte == 'r') {
      Serial.println("Reading Contents");
      printContents();
      Serial.println("~~~~ Usage: r, w, e ~~~~");
    } else if (incomingByte == 'w') {
      Serial.println("Entering Write mode");
      askToWriteData();
      Serial.println("~~~~ Usage: r, w, e ~~~~");
    }
       
    strIn = ""; // Clear recieved buffer
   
  }
}
