#include "vex.h"

using namespace vex;

// Brain and devices
brain Brain;
controller Controller;

// NOTE: replace "true" with "false" if you need to reverse the controls
motor LeftMotor  = motor(PORT7, true);
motor RightMotor = motor(PORT12, false);

motor ClawMotor  = motor(PORT4, false);
motor ArmMotor   = motor(PORT10, true);

bool RemoteControlCodeEnabled = true;

void vexcodeInit() {
  // Leave this if VEXcode expects it
}

// Arm up while LUp is held
void onButtonLUpPress() {
  ArmMotor.spin(forward);

  while (Controller.ButtonLUp.pressing()) {
    wait(20, msec);
  }

  ArmMotor.stop();
}

// Arm down while LDown is held
void onButtonLDownPress() {
  ArmMotor.spin(reverse);

  while (Controller.ButtonLDown.pressing()) {
    wait(20, msec);
  }

  ArmMotor.stop();
}

int main() {
  vexcodeInit();

  // Button callbacks
  Controller.ButtonLUp.pressed(onButtonLUpPress);
  Controller.ButtonLDown.pressed(onButtonLDownPress);

  // Motor behavior
  ArmMotor.setStopping(hold);
  ClawMotor.setStopping(hold);
  LeftMotor.setStopping(brake);
  RightMotor.setStopping(brake);

  ArmMotor.setVelocity(60, percent);
  ClawMotor.setVelocity(30, percent);

  while (true) {
    // Tank drive proportional control
    int leftSpeed  = Controller.AxisA.position(percent);
    int rightSpeed = Controller.AxisD.position(percent);

    LeftMotor.spin(forward, leftSpeed, percent);
    RightMotor.spin(forward, rightSpeed, percent);

    // Claw control
    if (Controller.ButtonRUp.pressing()) {
      ClawMotor.spin(forward);
    }
    else if (Controller.ButtonRDown.pressing()) {
      ClawMotor.spin(reverse);
    }
    else {
      ClawMotor.stop();
    }

    wait(20, msec);
  }
}