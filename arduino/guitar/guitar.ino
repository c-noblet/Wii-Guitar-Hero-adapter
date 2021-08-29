#include <NintendoExtensionCtrl.h>

GuitarController guitar;

void setup() {
	Serial.begin(115200);
	guitar.begin();

  while (!guitar.connect()) {
    Serial.println("Not Connected");
    delay(1000);
  }
}

void loop() {
  boolean success = guitar.update();  // Get new data from the controller

	if (success) {
    // Get frets
		boolean green = guitar.fretGreen();
    boolean red = guitar.fretRed();
    boolean yellow = guitar.fretYellow();
    boolean blue = guitar.fretBlue();
    boolean orange = guitar.fretOrange();

    // Send an 6 array length with fret color and strum position

    if (green) {
      Serial.print("G");
    } else {
      Serial.print("0");
    }
    if (red) {
      Serial.print("R");
    } else {
      Serial.print("0");
    }
    if (yellow) {
      Serial.print("Y");
    } else {
      Serial.print("0");
    }
    if (blue) {
      Serial.print("B");
    } else {
      Serial.print("0");
    }
    if (orange) {
      Serial.print("O");
    } else {
      Serial.print("0");
    }
  
    if (guitar.strumUp()) {
      Serial.println("U");
    } else if (guitar.strumDown()) {
      Serial.println("D");
    } else {
      Serial.println("0");
    }

    
  } else { // If connection is loosed
    Serial.println("Nothing");
    guitar.connect();
  }
}
