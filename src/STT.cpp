#include "stt.h"

// STT Methods
STT::STT() : state(STT_IDLE), st_p(nullptr) {
    cout << "Constructor of STT..." << endl;
};
STT::~STT() {
    cout << "Destructor of STT main class..." << endl;
    delete st_p;
};

bool STT::processCommand(Gripper& gripper, string& cmd) {

    // Validate command as an option
    if (cmdOptions.count(cmd) < 1) return false;
    cout << "Found command." << endl;
    const STT_state command = cmdOptions.at(cmd);

    // Validate state object mapping contains command
    //if (states.count(command) < 1) return false;
    const STT_state currentState = state;

    // Pointer to System State - execute transitions of STT command
    //SystemState* st_p = states[command];
    if (createStateObject(command)) {
        this->state = st_p->transition(gripper, currentState);
    };

    delete st_p;

    return true;
};

bool STT::createStateObject(const STT_state& command) {
    if (command == STT_OPEN) {
        st_p = new StateOpen;
        return true;
    }
    if (command == STT_CLOSE) {
        st_p = new StateClose;
        return true;
    }
    if (command == STT_CONNECT) {
        st_p = new StateConnect;
        return true;
    }
    if (command == STT_CALIBRATE) {
        st_p = new StateCalibrate;
        return true;
    }
    if (command == STT_DISCONNECT) {
        st_p = new StateIdle;
        return true;
    }
    else {
        cout << "Unable to allocate State Object" << endl;
        return false;
    }
};

// SystemState Methods
SystemState::~SystemState() {
    cout << "Destructor of SystemState..." << endl;
};

STT_state SystemState::transition(Gripper& gripper, const STT_state& currentState) {

    // Any errors will return FSM to previous state
    if (!beforeTransition(currentState)) { return currentState; };
    if (!atTransition(gripper)) { return currentState; };
    if (!afterTransition(gripper)) { return currentState; };
    cout << "Successfully executed all state transitions.." << endl;
    return nextState;

};

bool SystemState::checkTransition(const STT_state& command, const STT_state& currentState) {

    for (int i = 0; i < exampleTable.size(); i++) {
        if (exampleTable[i][0] == command && exampleTable[i][1] == currentState) {
            cout << "Found a matching state combination" << endl;
            this->nextState = exampleTable[i][2];
            return true;
        }
    };

    cout << "Unable to find appropriate transition..." << endl;
    return false;
}

// State Classes
// State Open
StateOpen::StateOpen() : command(STT_OPEN) {
    cout << "StateOpen constructor" << endl;
};
StateOpen::~StateOpen() {
    cout << "StateOpen destructor" << endl;
};

bool StateOpen::beforeTransition(const STT_state& currentState) {
    // Check for Correct State Transition
    cout << "Executing StateOpen before transition..." << endl;
    return checkTransition(this->command, currentState);
}

bool StateOpen::atTransition(Gripper& gripper) {
    return gripper.open();
};

bool StateOpen::afterTransition(Gripper& gripper) {
    // Maybe also send the HTTPS requests back to GUI?
    cout << "Executing StateOpen after transition..." << endl;
    
    return true;
};

// State Close
StateClose::StateClose() : command(STT_CLOSE) {
    cout << "StateClose constructor" << endl;
};
StateClose::~StateClose() {
    cout << "StateClose destructor" << endl;
};

bool StateClose::beforeTransition(const STT_state& currentState) {
    cout << "Executing StateClose before transition..." << endl;
    return checkTransition(this->command, currentState);
}

bool StateClose::atTransition(Gripper& gripper) {
    return gripper.close();
};

bool StateClose::afterTransition(Gripper& gripper) {
    // Maybe also send the HTTPS requests back to GUI?
    cout << "Executing StateClose after transition..." << endl;
    return true;
};


// Idle State
StateIdle::StateIdle() : command(STT_DISCONNECT) {
    cout << "StateIdle constructor..." << endl;
};
StateIdle::~StateIdle() {
    cout << "StateIdle destructor..." << endl;
};
bool StateIdle::beforeTransition(const STT_state& currentState) {
    cout << "Executing StateIdle before transition..." << endl;
    return checkTransition(this->command, currentState);
}

bool StateIdle::atTransition(Gripper& gripper) {
    // close client
    return true;
};

bool StateIdle::afterTransition(Gripper& gripper) {
    // Maybe also send the HTTPS requests back to GUI?
    cout << "Executing StateIdle after transition..." << endl;
    return true;
};


// Connected State
StateConnect::StateConnect() : command(STT_CONNECT) {
    cout << "StateConnect constructor..." << endl;
};
StateConnect::~StateConnect() {
    cout << "StateConnect destructor..." << endl;
};
bool StateConnect::beforeTransition(const STT_state& currentState) {
    cout << "Executing StateConnect before transition..." << endl;
    return checkTransition(this->command, currentState);
}

bool StateConnect::atTransition(Gripper& gripper) {
    vector3 data = gripper.getTactileData();
    cout << "Sensor data is: " << data.x << " | " << data.y << " | " << data.z << endl;
    return true;
};

bool StateConnect::afterTransition(Gripper& gripper) {
    // Maybe also send the HTTPS requests back to GUI?
    cout << "Executing StateConnect after transition..." << endl;
    return true;
};


// Calibrate State
StateCalibrate::StateCalibrate() : command(STT_CALIBRATE) {
    cout << "StateCalibrate constructor..." << endl;
};
StateCalibrate::~StateCalibrate() {
    cout << "StateCalibrate destructor..." << endl;
};
bool StateCalibrate::beforeTransition(const STT_state& currentState) {
    cout << "Executing StateCalibrate before transition..." << endl;
    return checkTransition(this->command, currentState);
}

bool StateCalibrate::atTransition(Gripper& gripper) {
    gripper.calibrate();
    return true;
};

bool StateCalibrate::afterTransition(Gripper& gripper) {
    // Maybe also send the HTTPS requests back to GUI?
    cout << "Executing StateCalibrate after transition..." << endl;
    vector3 data = gripper.getTactileData();
    cout << "Sensor data is: " << data.x << " | " << data.y << " | " << data.z << endl;
    return true;
};