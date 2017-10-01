# To-Do
What do you want to do next ? Write it down !!

## CsCdanNyx

### RoboticArmClass
- [ ] Equations modify and add Ang2XYZ.
- [X] Add debugging Serial print funtion.
- [ ] Put x, y, z into an array.
- [ ] Add funtion to show absolute angle.
- [X] Pre-defined servos' pin.
- [ ] Add angle limitations' checking in armGoDirect.
- [ ] Add XYZ limitation's checking.
- [ ] Add functions to grab marker pen (J[5] fixing or separation).
- [ ] Establish new coordinate system.
- [ ] Add new class for letters (calculate coordinate path and output).
- [ ] Establish letters' moving path (with parameters to adjust XYZ offsets and the angle of parrallelization).
- [ ] Add write letter funtions.
- [ ] Parallel and 60<sup>。</sup> adjustment.
- [ ] Adjustment with camera.

### Arm working test
- [ ] Arm gravity impact testing, especially for z-axis (armGoLine moving twice).

### Machine vision
- [ ] OpenCV project testing.

### Line tracking
- [ ] Color checking.
- [ ] Two sets of photo-interrupters.


## ktecv2000
### CAR MOVEMENT
- [ ] Move smoothly at uphill.
- [ ] Accurate measurement of wheel moving distance by optical interrupter.
- [X] Detect check point.
- [X] Send the signal of reaching check point back.
### SLIDER MOVEMENT
- [X] Velocity measurement of vertical and horizontal sliders.
- [X] Well APIs and their implementations for sliders (Seperated from original Class).
### COLOR SENSOR
- [ ] Purchase. 
- [ ] Detect color correctly.
- [ ] Install underneath the car.
- [ ] Send signal to other boards.
- [ ] Reset while leaving check point or reset button is pressed.
### *TASK_1* - KNOCK DOWN SIGN STAND
- [X] Detect specific digit.
- [ ] Calculate the distance to the digit for the two sliders.   
    - [ ] 1. Picture as input.   or    
    - [ ] 2. Fixed distances depending on the position digit-sign at.
- [ ] Flat and fixed camera (To fix the inaccurance of y-axis distance).
- [ ] **Completely working**
### *TASK_2* - SHORT PATH
- [X] Cones detections.
- [ ] (Car) Turn to specific angle.
- [ ] Feedback control by camera (cones' positions).
- [ ] Communication between computer(position, vedio) and arduino(movement).
- [ ] **Completely working**
### INTEGRATION
- [X] Serial communication.
- [X] Communication between computer(position, picture) and arduino(movement).
- [ ] Execution can be reset to any task by pressing reset button and providing task number as input.   
    - [ ] send reset signal to PC/board.   
    - [ ] Reset signal could be understand by receivers, stop execution properly and wait for start signal.    
    - [ ] Receivers stop execution properly (including record current task) and wait for restart signal.    
## Chrislu30604

