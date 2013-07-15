#include <Wire.h>
#include <PCA.h>

PCA pca; //create instance of PCA

void setup()
{
  pca.start(); // Start PCA
  //pca.start(0b00111000); //this also works to start a second instance with a second PCA chip
  
}

void loop()
{

  delay(5000);
  //loop through all possible integers
   for(int i = -999; i < 10000; i += 1) {
      pca.print(i);
      pca.updateDisplay();
   delay(50);
  }
  
  pca.print(123); //print int
  pca.updateDisplay();
  delay(5000);
  
  pca.println(321); // println displays int right justified with
                    // with leading zeros...
  pca.updateDisplay();
  delay(5000);
}

