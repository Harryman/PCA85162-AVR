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

  //delay(5000);
  //loop through all possible char 
   for(char a = 32; a < 125; a++) {
     char b = a + 1;
     char c = a + 2;
     char d = a + 3;
      pca.print(a, 0);
      pca.print(b, 1);
      pca.print(c, 2);
      pca.print(d, 3);
      pca.updateDisplay();
   delay(500);
  }
  
  pca.print("done"); //print int
  pca.updateDisplay();
  delay(5000);

}

