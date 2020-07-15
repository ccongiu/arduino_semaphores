
#define pirA 2 // crossroad A
#define redA 7
#define yellowA 8
#define greenA 9

#define pirB 3 // crossroad B
#define redB 13
#define yellowB 12
#define greenB 11

/*
#define pirPed -1 // crossroad pedestrians: high priority
#define redPed -1
#define yellowPed -1
#define greenPed -1
*/

#define on HIGH
#define off LOW

#define WAIT 500
#define THRESHOLD 5000

void setup() {
 pinMode(redA, OUTPUT);
 pinMode(yellowA, OUTPUT);
 pinMode(greenA, OUTPUT);
 pinMode(pirA, INPUT);

 digitalWrite(redA, on);
 digitalWrite(yellowA, off);
 digitalWrite(greenA, off);

 pinMode(redB, OUTPUT);
 pinMode(yellowB, OUTPUT);
 pinMode(greenB, OUTPUT);
 pinMode(pirB, INPUT);

 digitalWrite(redB, on);
 digitalWrite(yellowB, off);
 digitalWrite(greenB, off);

 /* PEDESTRIAN PINS
 pinMode(redPed, OUTPUT);
 pinMode(yellowPed, OUTPUT);
 pinMode(greenPed, OUTPUT);
 pinMode(buttonPed, INPUT);
 digitalWrite(redPed, on);
 digitalWrite(yellowPed, off);
 digitalWrite(greenPed, off);
 */
 
 /*
 pinMode for the other semaphores goes here
 initial red for the other semaphores goes here
 */
 
 Serial.begin(9600); // initialize serial monitor
};

typedef struct {
  int red;
  int yellow;
  int green;
  bool current;
  bool previous;
  int elapsedtime;
} Semaphore;

void setOnSem(Semaphore *sem);
void setOffSem(Semaphore *sem);
void alternateSem(Semaphore *semA, Semaphore *semB); // Semaphore *semC, etc...

/* todo BLYNK
void blynkStats();
bool blynkButton();
*/

Semaphore *initSem(int red, int yellow, int green);

Semaphore *semA = initSem(redA, yellowA, greenA);
Semaphore *semB = initSem(redB, yellowB, greenB);
// Semaphore *semC = initSem(redC, yellowC, greenC); etc...
// Semaphore *semPed = initSem(redPed, yellowPed, greenPed);

unsigned long start_timeA=0;
unsigned long start_timeB=0;

void loop() {
  /* PEDESTRIANS
  // state update for semPed: this one is a little different
  if (digitalRead(buttonPed) == HIGH or blynkButton()) {
    semPed->current = true;
  }
  */
  
  // state update for semA
  if (digitalRead(pirA) == HIGH) {
    semA->current = not(semA->current);
    start_timeA=millis();
  }
  // state update for semB
  if (digitalRead(pirB) == HIGH) {
    semB->current = not(semB->current);
    start_timeB=millis();
  }
  // state update for semC etc...

  alternateSem(semA, semB); // semC etc...
  // blynkStats();
}

void setOnSem(Semaphore *sem){
    delay(WAIT);
    digitalWrite(sem->red, off);
    digitalWrite(sem->yellow, off);
    digitalWrite(sem->green, on);
    sem->previous = true;
};

void setOffSem(Semaphore *sem){
    delay(WAIT);
    digitalWrite(sem->green, off);
    
    if (sem->previous) { // if the semaphore was previously green, I must switch on the yellow light 
      delay(WAIT);
      digitalWrite(sem->yellow, on);
      
          }
    
    digitalWrite(sem->yellow, off);
    digitalWrite(sem->red, on);
    sem->previous = false;
};


void alternateSem(Semaphore *semA, Semaphore *semB){ // Semaphore *semC, etc...
    /* if (semPed->current) {  // this semaphore has priority and will always stay 5 seconds on
      setOffSem(semA); setOffSem(semB); // and all of the others 
      setOnSem(semPed);
      delay(THRESHOLD);
      setOffSem(semPed);
      semPed->current = false;
      return;
    } else */ 
    unsigned long current_time=millis();
    
    if (current_time - start_timeA < THRESHOLD and semA->current){
      setOffSem(semB); // and all of the others
      setOnSem(semA);
      //semA->elapsedtime++;
      return;
    } else if (current_time - start_timeB < THRESHOLD and semB->current) {
      setOffSem(semA); // and all of the others
      setOnSem(semB);
      //semB->elapsedtime++;
      return;
    } /* else if (semC->elapsedtime < THRESHOLD and semC->current) {
      ...
    } etc... */
    else {
      setOffSem(semA); setOffSem(semB); // turn off all semaphores
      //semA->elapsedtime = 0;
      //semB->elapsedtime = 0;
      // semC->elapsedtime = 0; etc...
      return;
    }
};

Semaphore *initSem(int red, int yellow, int green){
    Semaphore *newSem = (Semaphore *) malloc(sizeof(Semaphore));
    
    newSem->red = red;
    newSem->yellow = yellow;
    newSem->green = green;
    newSem->current = false;
    newSem->previous = false;
    // newSem->elapsedtime = 0;
    
    return newSem;
};
