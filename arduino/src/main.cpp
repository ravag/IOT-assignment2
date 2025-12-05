#include <Arduino.h>
#include "config.h"
#include "kernel/Scheduler.h"
#include "kernel/Logger.h"
#include "kernel/MsgService.h"
#include "task/TempTask.h"
#include "task/TakeOffTask.h"
#include "task/LandingTask.h"
#include "task/BlinkingTask.h"
#include "devices/ServoMotorImpl.h"
#include "devices/Sonar.h"
#include "devices/Led.h"
#include "devices/PresenceSensor.h"
#include "LiquidCrystal_I2C.h"

Scheduler sched;

Context* pContext;

void setup() {
  MsgService.init();
  sched.init(500);
  Logger.log("::::: Drone System :::::");

  pContext = new Context();

  ServoMotorImpl* pMotor = new ServoMotorImpl(MOTOR_PIN);
  Sonar* pSonar = new Sonar(ECHO_PIN, TRIG_PIN, 200);
  LiquidCrystal_I2C* lcd = new LiquidCrystal_I2C(0x27,20,4);
  TempSensor* pTempSensor = new TempSensorTMP36(TEMP_PIN);
  Led* pLed = new Led(ON_LED);
  Pir* pir = new Pir(PROX_PIN);

  lcd->init();
  pMotor->on();
  pMotor->setPosition(0);
  Serial.print("lo");
  Serial.println(pMotor->getPosition());

  Task* pTempTask = new TempTask(new Led(ALARM_LED), pTempSensor, new ButtonImpl(BUTTON_PIN), lcd, pContext);
  pTempTask->init(500);
  Serial.println("loTempTask");

  Task* pTakeOffTask = new TakeOffTask(pContext, pSonar, pMotor, lcd, pTempSensor);
  pTakeOffTask->init(500);
  Serial.println("loTEST");

  Task* pLandingTask = new LandingTask(pMotor, pir, pSonar, lcd, pContext);
  pLandingTask->init(500);
  Serial.println("lo landingTask");

  Task* pBlinkingTask = new BlinkingTask(new Led(IN_ACTION_LED), pContext);
  pBlinkingTask->init(500);
  Serial.println("lo blinkingTask");

  sched.addTask(pTempTask);
  sched.addTask(pTakeOffTask);
  sched.addTask(pLandingTask);
  sched.addTask(pBlinkingTask);
  
  Serial.println("lo scheduled");

  pLed->switchOn();
  Serial.println("calibrating");
  pir->calibrate();
  Serial.println("calibrated");
  lcd->backlight();
  Serial.println("lo lcd");
}

void loop() {
  sched.schedule();
}