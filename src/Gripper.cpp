#include "gripper.h"

Gripper::Gripper()
: state(STT_IDLE), motorDelay(300), calibrateSample(10), collectSample(5) {};
Gripper::~Gripper() {};

void Gripper::init() {
    sensor.init();              // Initialize Hall Effect Sensor
    motor.init();               // Initialize Motor
}

bool Gripper::processCommand(const String& cmd) {

    // Validate command as an option
    if (cmdOptions.count(cmd) < 1) {
        cout << "[ERROR]: Received invalid command: " << cmd << endl;
        return false;
    }
    
    const STT_state command = cmdOptions.at(cmd);

    // Execute state transition
    state = transition(command);
    return true;
};

bool Gripper::checkTransition(const STT_state& command, STT_state& nextState) {
    for (int i = 0; i < transitionTable.size(); i++) {
        if (transitionTable[i][0] == command && transitionTable[i][1] == state) {
            nextState = transitionTable[i][2];
            return true;
        }
    };

    cout << "[WARNING]: Unable to find state transition..." << endl;
    return false;
}


STT_state Gripper::transition(const STT_state& command) {

    // Check the State Transition Table for next state. Return previous state on errors.
    const STT_state previousState = state;
    STT_state nextState;
    if (!checkTransition(command, nextState)) { return previousState; };

    // Create dynamic state object based on command and execute transitions
    SystemState* state_p = createStateObj(command);
    if (!state_p->beforeTransition(this)) { return previousState; };
    if (!state_p->atTransition(this)) { return previousState; };
    if (!state_p->afterTransition(this)) { return previousState; };

    // Return the final transition state if everything is successful
    return nextState;
};

SystemState* Gripper::createStateObj(const STT_state& command) {
    if (command == STT_OPEN) { return &StateOpen::getInstance(); };
    if (command == STT_CLOSE) { return &StateClose::getInstance(); };
    if (command == STT_CONNECT) { return &StateConnect::getInstance(); };
    if (command == STT_CALIBRATE) { return &StateCalibrate::getInstance(); };
    if (command == STT_DISCONNECT) { return &StateIdle::getInstance(); };
    cout << "[WARNING]: Unable to allocate State Object due to unrecognizable command option." << endl;
    return &StateIdle::getInstance();
};

void Gripper::setState(STT_state nextState) {
    state = nextState;
};

bool Gripper::open() {
    motor.open_gripper();
    delay(motorDelay);
    motor.stop_gripper();
    return true;
};

bool Gripper::close() {
	motor.close_gripper();
    delay(motorDelay);
    motor.stop_gripper();
    return true;
}

vector3 Gripper::getTactileData() {
	return sensor.readDataMaxZ();
}

void Gripper::calibrate() {
	sensor.calibrate(calibrateSample);
}

void Gripper::collect() {
    motor.close_gripper();
    delay(200);
    for (int i=0; i<collectSample; i++) {
        sendTactileData(guiClient);
    };
    motor.stop_gripper();
}

void Gripper::sendTactileData(WiFiClient& client) {
    vector3 data = sensor.readDataMaxZ();
    string message = to_string(data.x) + "," + to_string(data.y) + "," + to_string(data.z) + ",";
    const char* result = createBufferMessage(message, 65);
    client.print(result);
};

const char* createBufferMessage(string& message, int size) {
    int remainder = size - message.length() - 1;
    string filler(remainder, '0');
    message.append(filler);
    const char* p = message.c_str();
    return p;
};


StateIdle::StateIdle() : state(STT_IDLE) {};
StateIdle::~StateIdle() {};

SystemState& StateIdle::getInstance() {
    static StateIdle so;
    return so;
};

bool StateIdle::beforeTransition(Gripper* gripper) {
    // clean up GUI client
    // return gripper->destroy
    return true;
};

bool StateIdle::atTransition(Gripper* gripper) {
    gripper->setState(state);
    return true;
};

bool StateIdle::afterTransition(Gripper* gripper) {
    return true;
};


StateOpen::StateOpen() : state(STT_OPEN) {};
StateOpen::~StateOpen() {};

SystemState& StateOpen::getInstance() {
    static StateOpen so;
    return so;
}

bool StateOpen::beforeTransition(Gripper* gripper) {
    // Future state: check motor stop button
    // return gripper->canOpen();
    return true;
};

bool StateOpen::atTransition(Gripper* gripper) {
    gripper->setState(state);
    return gripper->open();
};

bool StateOpen::afterTransition(Gripper* gripper) {
    // Future state: ensure motor is idle
    // return gripper->isMotorStopped();
    return true;
};


StateClose::StateClose() : state(STT_CLOSE) {};
StateClose::~StateClose() {};

SystemState& StateClose::getInstance() {
    static StateClose so;
    return so;
};

bool StateClose::beforeTransition(Gripper* gripper) {
    return true;
};

bool StateClose::atTransition(Gripper* gripper) {
    gripper->setState(state);
    return gripper->close();
};

bool StateClose::afterTransition(Gripper* gripper) {
    return true;
};


StateConnect::StateConnect() : state(STT_CONNECT) {};
StateConnect::~StateConnect() {};

SystemState& StateConnect::getInstance() {
    static StateConnect so;
    return so;
};

bool StateConnect::beforeTransition(Gripper* gripper) {
    return true;
};

bool StateConnect::atTransition(Gripper* gripper) {
    gripper->setState(state);
    return true;
};

bool StateConnect::afterTransition(Gripper* gripper) {
    return true;
};


StateCalibrate::StateCalibrate() : state(STT_CALIBRATE) {};
StateCalibrate::~StateCalibrate() {};

SystemState& StateCalibrate::getInstance() {
    static StateCalibrate so;
    return so;
};

bool StateCalibrate::beforeTransition(Gripper* gripper) {
    return true;
};

bool StateCalibrate::atTransition(Gripper* gripper) {
    gripper->setState(state);
    gripper->calibrate();
    return true;
};

bool StateCalibrate::afterTransition(Gripper* gripper) {
    return true;
};


StateCollect::StateCollect() : state(STT_COLLECT) {};
StateCollect::~StateCollect() {};

SystemState& StateCollect::getInstance() {
    static StateCollect so;
    return so;
};

bool StateCollect::beforeTransition(Gripper* gripper) {
    return true;
};

bool StateCollect::atTransition(Gripper* gripper) {
    gripper->setState(state);
    gripper->collect();
    return true;
};

bool StateCollect::afterTransition(Gripper* gripper) {
    return true;
};
