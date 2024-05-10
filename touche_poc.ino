
#define startGen(x,y) (x |=(1<<y))
#define stopGen(x,y) (x &= (~(1<<y)))
#define togglePIN(x,y) (x ^= (1<<y))

#define numberFrequencies 160

float results[numberFrequencies]; //fitered result buffer
float freq[numberFrequencies];
int sizeofArrays = numberFrequencies;


float alpha;
void average(){
  float moving_average[numberFrequencies];
  for (int i = 0; i < numberFrequencies; i++){
    if (i < 5){
      moving_average[i] = results[i];
    }
    else{
      float total = 0;
      for (int j = 2; j > 0; j--){
        total = total + results[i-j];
      }
      moving_average[i] = total/3;
    }
    Serial.print(i);
    Serial.print(":");
    Serial.println(moving_average[i]);
  }
}
void setup(){
  //Set up frequancy generator
  TCCR1A = 0b10000010;
  //using the registers in arduino chip
  TCCR1B = 0b00011001;
  ICR1 = 110;
  OCR1A = 55;

  //TCCR1A = 0;
  //TCCR1B = 0;

  //TCCR1A |= (1 << COM1A0);
  //TCCR1B |= (1 << WGM12);

  pinMode(9,OUTPUT); //Signal generator pin

  for (int i=0;i<sizeofArrays;i++){ //preset results
    results[i]=0;
  }

  Serial.begin(115200);
}


void loop(){
//  if (Serial.available()){
//    alpha = (float)Serial.read() / 255.0f;
//  }
//  int maxPos, maxVal;
//  maxPos = 0;
//  maxVal = 0;
  for (unsigned int d =0; d < numberFrequencies; d++){
    

    //Stop the generation
    stopGen(TCCR1B, 0);
    //load new frequency
    TCNT1 = 0;
    ICR1 = d;
    OCR1A = d/2;
    //start the generation
    startGen(TCCR1B, 0);
    int v = analogRead(0);
    results[d] = v;
//    results[d] = results[d] * alpha + v * (1-alpha);
//    if (results[d] > maxVal){
//      maxVal = results[d];
//   c:\DeFontysMap\2324\Semester 2\Delta\TouchéBastiaanClone\delta-bastiaan\semester_3\Ellie\touche_poc\Touche_Test2.0.ino   maxPos = d;
//    }

    //filter results
    results[d] = results[d]*0.5+(float)(v)*0.5;
    
//    Serial.print(d);
//    Serial.print(":");
//    Serial.println(results[d]);

//	  Serial.print(d);
//    Serial.print (":");
//    Serial.println(analogRead(0), DEC);
    
    //freq[d] = d;
  }
  average();
}
