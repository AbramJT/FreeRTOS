#include <freertos/FreeRTOS.h> //Header files for freertos
#include <freertos/queue.h> //Header files for freertos
#include <freertos/task.h> //Header files for freertos
#include <freertos/semphr.h> //Header files for freertos

int led_1 = 25; //Output port for LED of task 1 
int freq_1 = 12; //Input port to measure task 2 frequency
int freq_2 = 14; //Input port to measure task 3 frequency
int poten = 26; //Input port to show analog frequency
int led_2_error = 27; //Output port for LED of task 4
int led_3 = 4; //Output port for LED of task 7 
int button = 2; //Input port for button of task 6 

typedef struct {
  int eventType; //Used to identify the type of event
  int eventData; //Used to store additional data associated with the event
} event_t; //The structure name

void Task_1(void *param); //Function definition for the task 1
void Task_2(void *param); //Function definition for the task 2
void Task_3(void *param); //Function definition for the task 3
void Task_4(void *param); //Function definition for the task 4
void Task_5(void *param); //Function definition for the task 5
void Task_6(void *param); //Function definition for the task 6
void Task_7(void *param); //Function definition for the task 7

TaskHandle_t Task_Handle1; //Point at task 1 to handle
TaskHandle_t Task_Handle2; //Point at task 2 to handle
TaskHandle_t Task_Handle3; //Point at task 3 to handle
TaskHandle_t Task_Handle4; //Point at task 4 to handle
TaskHandle_t Task_Handle5; //Point at task 5 to handle
TaskHandle_t Task_Handle6; //Point at task 6 to handle
TaskHandle_t Task_Handle7; //Point at task 7 to handle

QueueHandle_t Task_Queue = xQueueCreate(10,sizeof(event_t)); //Create a queue

void setup() {
  
  Serial.begin(9600); //Serial communication with a specific baud rate

  xTaskCreate(Task_1,"TASK 1",1024,NULL,1,&Task_Handle1); //Creates task 1 
  xTaskCreate(Task_2,"TASK 2",1024,NULL,1,&Task_Handle2); //Creates task 2 
  xTaskCreate(Task_3,"TASK 3",1024,NULL,1,&Task_Handle3); //Creates task 3 
  xTaskCreate(Task_4,"TASK 4",1024,NULL,1,&Task_Handle4); //Creates task 4 
  xTaskCreate(Task_5,"TASK 5",1024,NULL,1,&Task_Handle5); //Creates task 5 
  xTaskCreate(Task_6,"TASK 6",1024,NULL,1,&Task_Handle6); //Creates task 6 
  xTaskCreate(Task_7,"TASK 7",1024,NULL,1,&Task_Handle7); //Creates task 7 

  pinMode(led_1, OUTPUT); //Task 1, LED 1 output
  pinMode(freq_1, INPUT); //Task 2, Frequency input
  pinMode(freq_2, INPUT); //Task 3, Frequency input 
  pinMode(poten, INPUT); //Task 4, Potentiometer input
  pinMode(led_2_error, OUTPUT); //Task 4, LED 2 output
  pinMode(led_3, OUTPUT); //Task 7, LED 3 output
  pinMode(button, INPUT); //Task 6, Button input
  
}

void loop() {
  // No code is put here
}


void Task_1(void * param) //Function definition for task 1
{
  (void) param;

  while(true){
  digitalWrite(led_1, HIGH); //High for 200us
  delayMicroseconds(200);
  digitalWrite(led_1, LOW); //Low for 50us
  delayMicroseconds(50);
  digitalWrite(led_1, HIGH); //High for 30us
  delayMicroseconds(30);
  digitalWrite(led_1, LOW); //Low for remaining time period

  vTaskDelay(4/portTICK_PERIOD_MS); //Runs a task for a specified amount of time
  Serial.println("Task 1 Completed"); //End of Task 1   
  } 

}

void Task_2(void * param) //Function definition for task 2
{
  (void) param;

  while(true){
  int count_1 = 0;

  count_1 = pulseIn(freq_1, HIGH); //Measure the pulse of the input

  count_1 = count_1 *  2;
  float frequency_1 = 1000000.0 / (count_1); //Calculate the frequency 
  frequency_1 = constrain(frequency_1, 333, 1000); //Limit frequency from 333 to 1000

  vTaskDelay(20/portTICK_PERIOD_MS); //Runs a task for a specified amount of time
  Serial.println("Task 2 Completed"); //End of Task 2   
  }
  
}

void Task_3(void * param) //Function definition for task 3
{
  (void) param;

  while(true){    
  int count_2 = 0;
  
  count_2 = pulseIn(freq_2, HIGH); //Measure the pulse of the input

  count_2 = count_2 * 2;
  float frequency_2 = 1000000.0 / (count_2); //Calculate the frequency
  frequency_2 = constrain(frequency_2, 500, 1000); //Limit frequency from 500 to 1000
  
  vTaskDelay(8/portTICK_PERIOD_MS); //Runs a task for a specified amount of time
  Serial.println("Task 3 Completed"); //End of Task 3    
  }

}

void Task_4(void * param) //Function definition for task 4
{
  (void) param;

  while(true){
  const int max_analog_input = 1023; // 1024 reflect on 10 bits of memory
  const int readings_num = 4; 
  int readings[readings_num]; 
  int array = 0;  // Index of current readings
  int sum = 0;  // The total run
  int filtered_value = 0; // Average of the filtered
  for (int i = 0; i < readings_num; i++) 
  {
  readings[i] = 0;
  }
  
  int analogValue = analogRead(poten);  //Read analog input
  sum = readings[array];  //Minus the old reading from sum
  sum = analogValue; //New reading is now added in the sum
  readings[array] = analogValue; //New reading is now in the array
  array++; //Increment 

  if (array >= readings_num)
  {
    array = 0;
  }
  filtered_value = sum / readings_num;  //Filtered values average
  if (filtered_value > max_analog_input / 2) {  //Filtered value will be greater than half of max range, and thus LED 2 will be HIGH or LOW
    digitalWrite(led_2_error, HIGH); //Led is HIGH
 
  } else {
    digitalWrite(led_2_error, LOW); //Led is LOW
  }
  
    
  vTaskDelay(20/portTICK_PERIOD_MS); //Runs a task for a specified amount of time
  Serial.println("Task 4 Completed"); //End of Task 4    
  }
   
}

void Task_5(void * param) //Function definition for task 5
{
  (void) param;

  while(true){
  int freq_1 = 0;
  int freq_2 = 0;
  
  freq_1 = pulseIn(freq_1, HIGH) == 0 ? 0 : 1000000 / pulseIn(freq_1, HIGH); //Frequency of Task 2
  freq_1 = map(freq_1, 333, 1000, 0, 99); //Limit and scale Task 2
  freq_2 = pulseIn(freq_2, HIGH) == 0 ? 0 : 1000000 / pulseIn(freq_1, HIGH);  //Frequency of Task 3
  freq_2 = map(freq_2, 500, 1000, 0, 99); //Limit and scale Task 3
  
  Serial.println(freq_1);
  Serial.println(freq_2);
  
  vTaskDelay(100/portTICK_PERIOD_MS); //Runs a task for a specified amount of time
  Serial.println("Task 5 Completed"); //End of Task 5   
  }
 
}

void Task_6(void * param) //Function definition for task 6
{
  (void) param;

  while(true){
    while(digitalRead(button)==HIGH){ //Wait for button to be pressed
      vTaskDelay(10); //Debounce the button
    } 
    // It will queue an event onto the event queue
    event_t event = {1, button};
    xQueueSendToBack(Task_Queue, &event, 0);
  }
  Serial.println("Task 6 Completed"); //End of Task 6   

}

void Task_7(void * param) //Function definition for task 7
{
  (void) param;

  while(true){
    event_t event; //Waiting for an event to be queued
    xQueueReceive(Task_Queue, &event, portMAX_DELAY); 

            
    if(event.eventType == 1){ //Button is pressed
      digitalWrite(led_3, HIGH); //Led is HIGH     
    } else {
      digitalWrite(led_3, LOW); //Led is LOW    
    }
      Serial.println("Task 7 Completed"); //End of Task 7   

}  

    
}