#pragma once

#include <iostream>
#include <array>
#include <map>
#include <WiFi.h>
#include "CommandPrompt.h"
#include "TactileSensor.h"
#include "L9110HMotor.h"

using namespace std;

enum STT_state {
    STT_IDLE = 0,
    STT_OPEN = 1,
    STT_CLOSE = 2,
    STT_CONNECT = 3,
    STT_COLLECT = 4,
    STT_CALIBRATE = 5,
    STT_DISCONNECT = 6
};

const array<array<STT_state, 3>, 7> transitionTable{
    {
        {STT_OPEN, STT_IDLE, STT_IDLE},
        {STT_OPEN, STT_CONNECT, STT_CONNECT},
        {STT_CLOSE, STT_IDLE, STT_IDLE},
        {STT_CLOSE, STT_CONNECT, STT_CONNECT},
        {STT_CONNECT, STT_IDLE, STT_CONNECT},
        {STT_CALIBRATE, STT_CONNECT, STT_CONNECT},
        {STT_DISCONNECT, STT_CONNECT, STT_IDLE},
    }
};

const std::map<string, STT_state> cmdOptions = {
    {"open", STT_OPEN},
    {"close", STT_CLOSE},
    {"connect", STT_CONNECT},
    {"collect", STT_COLLECT},
    {"calibrate", STT_CALIBRATE},
    {"disconnect", STT_DISCONNECT},
};


// Forward declaration for Gripper
class SystemState;

class Gripper
{
public:
    Gripper();
	~Gripper();
	void init();
    bool processCommand(const String& cmd);
    void setState(STT_state newState);
    STT_state getState() const { return state; };
	void sendTactileData(WiFiClient& client);
	bool open();
	bool close();
	vector3 getTactileData();
	void calibrate();
	void collect();
	const char* createBufferMessage(string& message, int size);

private:
	TactileSensor sensor{1};
	L9110HMotor motor;
    STT_state state;
	WiFiClient guiClient;
	CommandPrompt prompt;
	const int motorDelay;
	const int calibrateSample;
	const int collectSample;
    bool checkTransition(const STT_state& command, STT_state& nextState);
    SystemState* createStateObj(const STT_state& command);
    STT_state transition(const STT_state& command);
};


class SystemState
{
public:
    virtual bool beforeTransition(Gripper*) = 0;
    virtual bool atTransition(Gripper*) = 0;
    virtual bool afterTransition(Gripper*) = 0;
    virtual ~SystemState() {};
};


class StateIdle : public SystemState
{
public:
    bool beforeTransition(Gripper*);
    bool atTransition(Gripper*);
    bool afterTransition(Gripper*);
    static SystemState& getInstance();
    STT_state getState() const { return state; };

private:
    StateIdle();
    StateIdle(const StateIdle& rhs);
    ~StateIdle();
    StateIdle& operator=(const StateIdle& rhs);
    const STT_state state;
};


class StateOpen : public SystemState
{
public:
    bool beforeTransition(Gripper*);
    bool atTransition(Gripper*);
    bool afterTransition(Gripper*);
    static SystemState& getInstance();
    STT_state getState() const { return state; };

private:
    StateOpen();
    StateOpen(const StateOpen& rhs);
    ~StateOpen();
    StateOpen& operator=(const StateOpen& rhs);
    const STT_state state;
};


class StateClose : public SystemState
{
public:
    bool beforeTransition(Gripper*);
    bool atTransition(Gripper*);
    bool afterTransition(Gripper*);
    static SystemState& getInstance();
    STT_state getState() const { return state; };

private:
    StateClose();
    StateClose(const StateClose& other);
    ~StateClose();
    StateClose& operator=(const StateClose& other);
    const STT_state state;
};


class StateConnect : public SystemState
{
public:
    bool beforeTransition(Gripper*);
    bool atTransition(Gripper*);
    bool afterTransition(Gripper*);
    static SystemState& getInstance();
    STT_state getState() const { return state; };

private:
    StateConnect();
    StateConnect(const StateConnect& other);
    ~StateConnect();
    StateConnect& operator=(const StateConnect& other);
    const STT_state state;
};


class StateCalibrate : public SystemState
{
public:
    bool beforeTransition(Gripper*);
    bool atTransition(Gripper*);
    bool afterTransition(Gripper*);
    static SystemState& getInstance();
    STT_state getState() const { return state; };

private:
    StateCalibrate();
    StateCalibrate(const StateCalibrate& other);
    ~StateCalibrate();
    StateCalibrate& operator=(const StateCalibrate& other);
    const STT_state state;
};


class StateCollect : public SystemState
{
public:
    bool beforeTransition(Gripper*);
    bool atTransition(Gripper*);
    bool afterTransition(Gripper*);
    static SystemState& getInstance();
    STT_state getState() const { return state; };

private:
    StateCollect();
    StateCollect(const StateCollect& other);
    ~StateCollect();
    StateCollect& operator=(const StateCollect& other);
    const STT_state state;
};