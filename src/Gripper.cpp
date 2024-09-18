#include "gripper.h"

Gripper::Gripper()
: state(STT_IDLE), motorDelay(400), calibrateSample(10), collectSample(5) {};
Gripper::~Gripper() {};



void Gripper::init() {
    sensor.init();              // Initialize Hall Effect Sensor
    motor.init();               // Initialize Motor
}


bool Gripper::processCommand(const string& cmd, WiFiClient& client) {
/**
 * Performs the gripper's state transition based on an incoming command from the client (GUI).
 * 
 * @param cmd       Reference to an incoming command from a client.
 * @param client    WiFiClient instance to retain if the Gripper is set to the Connected state
 *                  so it can repeatedly send data back to the GUI.
 */
    
    // Validate command as an option
    if (cmdOptions.count(cmd) < 1) {
        CommandPrompt::invalid();
        return false;
    }
    const FSM_state command = cmdOptions.at(cmd);

    // Retain client to send data when connected but only when current state is IDLE
    if (state == STT_IDLE && command == STT_CONNECT) { guiClient = client; };

    // Execute state transition
    state = transition(command);
    CommandPrompt::prompt;
    return true;
};


FSM_state Gripper::transition(const FSM_state& command) {
/**
 * The Gripper's state transition follows a FSM design pattern that passes control to 
 * state objects that manage the transition process associated with an incoming command
 * from the client. For example, a command to collect data will create a StateConnect object
 * and perform the transition steps: before, at, after. After the state transition is complete
 * it returns the next state (or the previous state if there is an error).
 * 
 * @param command       command request from a client (GUI) 
 */

    // Check the State Transition Table for next state. Return previous state on errors.
    FSM_state nextState = getNextState(command);
    if (nextState == state) { return state; };
    
    // Create dynamic state object based on command and execute transitions
    SystemState* state_p = createStateObj(command);
    if (!state_p->beforeTransition(this)) { return state; };
    if (!state_p->atTransition(this)) { return state; };
    if (!state_p->afterTransition(this)) { return state; };

    // Return the final transition state if everything is successful
    return nextState;
};


FSM_state Gripper::getNextState(const FSM_state& command) {
/**
 * Returns the next FSM state associated with the command and current state of the Gripper.
 * If the state transition is not found for the incoming command it will return the current state.
 * 
 * @param command       reference to FSM_state command option for the Gripper
 */

    for (int i = 0; i < transitionTable.size(); i++) {
        if (transitionTable[i][0] == command && transitionTable[i][1] == state) {
            return transitionTable[i][2];
        }
    };

    cout << "[WARNING]: Invalid state transition for " << command << "at state " << state << endl;
    return state;
};


bool Gripper::isCommandValid(const string& cmd) {
    if (cmdOptions.count(cmd) < 1) {
        CommandPrompt::invalid();
        return false;
    };
    return true;
};


SystemState* Gripper::createStateObj(const FSM_state& command) {
/**
 * Returns a pointer to a new state object based on the requested command.
 * This technique only returns the state object instance when its getInstance()
 * method is called, which is useful for minimizing memory when not used.
 * 
 * @param command       reference to FSM_state command option for the Gripper
 */
    if (command == STT_OPEN) { return &StateOpen::getInstance(); };
    if (command == STT_CLOSE) { return &StateClose::getInstance(); };
    if (command == STT_CONNECT) { return &StateConnect::getInstance(); };
    if (command == STT_COLLECT) { return &StateCollect::getInstance(); };
    if (command == STT_CALIBRATE) { return &StateCalibrate::getInstance(); };
    if (command == STT_DISCONNECT) { return &StateIdle::getInstance(); };
    cout << "[WARNING]: Unable to allocate State Object due to unrecognizable command option." << endl;
    return &StateIdle::getInstance();
};

void Gripper::setState(FSM_state nextState) {
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
    delay(300);
    for (int i=0; i<collectSample; i++) {
        sendTactileData();
    };
    motor.stop_gripper();
}

void Gripper::sendTactileData() {
    
    // Disconnect and return to IDLE if client is no longer connected
    if (!guiClient.available()) {
        guiClient.stop();
        transition(STT_DISCONNECT);
        return;
    };

    // Create a fixed buffer message with tactile data and send to client
    vector3 data = sensor.readDataMaxZ();
    string message = to_string(data.x) + "," + to_string(data.y) + "," + to_string(data.z) + ",";
    const char* result = createBufferMessage(message, 65);
    guiClient.print(result);

};

const char* Gripper::createBufferMessage(string& message, int size) {
    int remainder = size - message.length() - 1;
    string filler(remainder, '0');
    message.append(filler);
    const char* p = message.c_str();
    return p;
};

bool Gripper::isConnected() {
    if (state == STT_CONNECT) {
        return true;
    }
    return false;
}


/************************************************************
 **************** State Object Methods **********************
 ************************************************************
 * 
 * State Objects execute the gripper methods during state
 * transitions. They follow the same pattern of performing
 * functionality before a transition, during the transition,
 * and after the transition for an associated command/request.
 */

StateIdle::StateIdle() : state(STT_IDLE) {};
StateIdle::~StateIdle() {};

SystemState& StateIdle::getInstance() {
    static StateIdle so;
    return so;
};

bool StateIdle::beforeTransition(Gripper* gripper) {
    // clean up GUI client
    // return gripper->disconnectClient();
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
    // return gripper->isOpen();
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
    return gripper->isConnected();
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
    return gripper->isConnected();
};

bool StateCollect::atTransition(Gripper* gripper) {
    gripper->setState(state);
    gripper->collect();
    return true;
};

bool StateCollect::afterTransition(Gripper* gripper) {
    return true;
};
