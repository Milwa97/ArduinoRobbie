// pins for motors
const int pin_left_forward = 4;     
const int pin_left_backward = 2; 
const int pin_right_forward = 3;     
const int pin_right_backward = 5;  


// pins for the distance sensor 
const int pin_trig = 7 ;
const int pin_echo = 6 ;


// pins from NodeMCU
const int pin_autonomic = 12;  
const int pin_node_lf = 10;     
const int pin_node_lb = 8;   
const int pin_node_rf = 9; 
const int pin_node_rb = 11;    


// signal for motors
int node_signal_lf;
int node_signal_lb;
int node_signal_rf;
int node_signal_rb;
int control_signal;


// analog readings from photoresistors
int right_photoresistor ; 
int left_photoresistor ; 
const int margin = 150;

// for ultrasonic distance sensor
float duration, distance;


/*
-------------------------------------------------------------------------------------------------------------------
        HELPER FUNCTIONS
-------------------------------------------------------------------------------------------------------------------
*/

void getDistance(){
  
  // Clears the trigPin condition
  digitalWrite(pin_trig, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(pin_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin_trig, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(pin_echo, HIGH);
  distance = duration * 0.034 / 2; 
  
  // Displays the distance on the Serial Monitor
  Serial.print("Distance: "); 
  Serial.print(distance);  
  Serial.print(" cm \n"); 
}

  

void moveForward(int duration){
  
    digitalWrite(pin_left_forward, HIGH);
    digitalWrite(pin_left_backward, LOW);
    digitalWrite(pin_right_forward, HIGH);
    digitalWrite(pin_right_backward, LOW);
    delay(duration);
}

void moveBackward(int duration){
    
    digitalWrite(pin_left_forward, LOW);
    digitalWrite(pin_left_backward, HIGH);
    digitalWrite(pin_right_forward, LOW);
    digitalWrite(pin_right_backward, HIGH);
    delay(duration);
 
}

void turnRight(int duration){

    digitalWrite(pin_left_forward, HIGH);
    digitalWrite(pin_left_backward, LOW);
    digitalWrite(pin_right_forward, LOW);
    digitalWrite(pin_right_backward, HIGH);
    delay(duration);

}

void turnLeft(int duration){

    digitalWrite(pin_left_forward, LOW);
    digitalWrite(pin_left_backward, HIGH);
    digitalWrite(pin_right_forward, HIGH);
    digitalWrite(pin_right_backward, LOW);
    delay(duration);

}


void moveStop(int duration){

  digitalWrite(pin_left_backward, LOW);
  digitalWrite(pin_right_forward, LOW);
  digitalWrite(pin_left_forward, LOW);
  digitalWrite(pin_right_backward, LOW);
  delay(duration);
}


void blynk_control(){

  if (node_signal_lf){digitalWrite(pin_left_forward, HIGH);}
  else {digitalWrite(pin_left_forward, LOW);}

  if (node_signal_lb){digitalWrite(pin_left_backward, HIGH);}
  else {digitalWrite(pin_left_backward, LOW);}

  if (node_signal_rf){digitalWrite(pin_right_forward, HIGH);}
  else {digitalWrite(pin_right_forward, LOW);}

  if (node_signal_rb){digitalWrite(pin_right_backward, HIGH);}
  else {digitalWrite(pin_right_backward, LOW);}
}



void autonomic_car_mode(){ 

    if (right_photoresistor > left_photoresistor + margin){ turnRight(10); }
    else if (left_photoresistor > right_photoresistor + margin){ turnLeft(10); }
    else if (distance > 50){moveForward(10); }
    else {moveStop(10);}  
}




















/*
-------------------------------------------------------------------------------------------------------------------
          SETUP
-------------------------------------------------------------------------------------------------------------------
*/

void setup(){
  
  Serial.begin(115200);

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

  pinMode(pin_trig, OUTPUT); 
  pinMode(pin_echo, INPUT); 
  
  pinMode(pin_left_forward, OUTPUT);
  pinMode(pin_left_backward, OUTPUT);
  pinMode(pin_right_forward, OUTPUT);
  pinMode(pin_right_backward, OUTPUT);

  pinMode(pin_autonomic, INPUT);   
  pinMode(pin_node_lf, INPUT);  
  pinMode(pin_node_lb, INPUT);  
  pinMode(pin_node_rf, INPUT);  
  pinMode(pin_node_rb, INPUT);  

  moveStop(0) ;  // set all motors to LOW - don't move
}



/*
-------------------------------------------------------------------------------------------------------------------
          LOOP
-------------------------------------------------------------------------------------------------------------------
*/

void loop(){ 

  moveStop(0);
  
  control_signal = digitalRead(pin_autonomic);
  node_signal_lf = digitalRead(pin_node_lf);
  node_signal_lb = digitalRead(pin_node_lb);
  node_signal_rf = digitalRead(pin_node_rf);
  node_signal_rb = digitalRead(pin_node_rb);

  right_photoresistor = analogRead(A0);  
  left_photoresistor =  analogRead(A1) ;
  
  getDistance() ;

  Serial.print("\nRIGHT: ");  
  Serial.print(right_photoresistor);  
  Serial.print("\nLEFT:  ");  
  Serial.print(left_photoresistor);  
  
  if (control_signal){ blynk_control(); }  
  else {autonomic_car_mode();}

  delay(100);
  
}
