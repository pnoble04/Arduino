#include <SPI.h>
#include <SPISRAM.h>

SPISRAM ram(8);

void setup() {
  char test[] =  "This royal throne of kings, this sceptre'd isle, \n"
    "This earth of majesty, this seat of Mars, \n"
    "This other Eden, demi-paradise; \n"
    "This fortress, built by nature for herself, \n"
    "Against infection, and the hand of war; \n"
    "This happy breed of men, this little world, \n"
    "This precious stone set in the silver sea, \n"
    "Which serves it in the office of a wall \n"
    "Or as a moat defensive to a house, \n"
    "Against the envy of less happier lands,-- \n"
    "This blessed plot, this earth, this realm, this England.";

  int pullups[] = { 
    4, 7, 8, 10    };
  for(int i = 0; i < sizeof(pullups); i++) {
    digitalWrite(i, HIGH);
  }

  Serial.begin(9600);
  Serial.println("Hi.");
  Serial.print("string length = ");
  Serial.println(strlen(test));

  Serial.println();

  SPI.begin();
  ram.begin();

  randomSeed(analogRead(0) ^ millis());
  long addr = random(0, 0x1ffff);
  Serial.print("Byte write/read start address: ");
  Serial.println(addr, HEX);
  for (int i = 0; i < strlen(test); i++) {
    ram.write(addr+i, test[i]);
  }
  for (int i = 0; i < strlen(test); i++) {
    Serial.print((char) ram.read(addr+i));
  }
  Serial.println();

  char buf[256];
  addr = random(0, 0x1ffff);
  Serial.println();
  Serial.print("Continuous write/read start address: ");
  Serial.println(addr, HEX);
  ram.writeBytes(addr, (uint8_t*)test, 256);
  ram.readBytes(addr, (uint8_t*)buf, 256);
  buf[256] = 0;
  Serial.println(buf);
  long sum = 0;
  long swatch = millis();
  for(long i = 0; i < 0x20000/256; i++) {
    ram.writeBytes(addr+i*256, (uint8_t*)test, 256);
    ram.readBytes(addr+i*256, (uint8_t*)buf, 256);
    for(int i = 0; i < 256; i++) {
      if ( test[i] != buf[i] )
        sum++;
    }
    if ( (i & 0x7f) == 0 ) {
     Serial.print(i*256);
     Serial.print(" bytes, ");
    }
  }
  Serial.println();
  Serial.print(millis() - swatch);
  Serial.print(" msec., errors = ");
  Serial.println(sum);
}

void loop() {
}



