#ifndef STT_H
#define STT_H

#include <array>
#include <map>
#include <iostream>
#include "gripper.h"

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

const array<array<STT_state, 3>, 7> exampleTable {
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

const std::map<string, array<STT_state, 3>> STT_transitions = {
    {"open", { STT_IDLE, STT_OPEN, STT_IDLE }},
    {"open", { STT_CONNECT, STT_OPEN, STT_CONNECT }},
    {"close", { STT_IDLE, STT_CLOSE, STT_IDLE }},
    {"close", { STT_CONNECT, STT_CLOSE, STT_CONNECT }},
    {"connect", { STT_IDLE, STT_CONNECT, STT_CONNECT }},
    {"collect", { STT_CONNECT, STT_COLLECT, STT_CONNECT}},
    {"calibrate", { STT_CONNECT, STT_CALIBRATE, STT_CONNECT}},
    {"disconnect", { STT_CONNECT, STT_IDLE, STT_IDLE }},
};


class SystemState {
public:
    virtual ~SystemState();

    virtual bool beforeTransition(const STT_state&) = 0;
    virtual bool atTransition(Gripper&) = 0;
    virtual bool afterTransition(Gripper&) = 0;
    STT_state transition(Gripper&, const STT_state&);
    bool checkTransition(const STT_state& command, const STT_state& state);
private:
    STT_state command;
    STT_state cmdRequest;
    STT_state currentState;
    STT_state nextState;
};

class StateIdle : public SystemState {
public:
    StateIdle();
    ~StateIdle();
private:
    const STT_state command;
    bool beforeTransition(const STT_state& state);
    bool atTransition(Gripper&);
    bool afterTransition(Gripper&);
};

class StateOpen: public SystemState {
public:
    StateOpen();
    ~StateOpen();
private:
    const STT_state command;
    bool beforeTransition(const STT_state& state);
    bool atTransition(Gripper&);
    bool afterTransition(Gripper&);
};

class StateClose : public SystemState {
public:
    StateClose();
    ~StateClose();
private:
    const STT_state command;
    bool beforeTransition(const STT_state& state);
    bool atTransition(Gripper&);
    bool afterTransition(Gripper&);
};

class StateConnect : public SystemState {
public:
    StateConnect();
    ~StateConnect();
private:
    const STT_state command;
    bool beforeTransition(const STT_state& state);
    bool atTransition(Gripper&);
    bool afterTransition(Gripper&);
};

class StateCollect : public SystemState {
public:
    StateCollect();
    ~StateCollect();
private:
    const STT_state command;
    bool beforeTransition(const STT_state& command, const STT_state& state);
    bool atTransition();
    bool afterTransition();
};

class StateCalibrate : public SystemState {
public:
    StateCalibrate();
    ~StateCalibrate();
private:
    const STT_state command;
    bool beforeTransition(const STT_state& state);
    bool atTransition(Gripper&);
    bool afterTransition(Gripper&);
};


class STT {
public:
    STT();
    ~STT();
    STT_state getState() const { return state; };
    bool processCommand(Gripper&, string& cmd);
    bool createStateObject(const STT_state& command);
private:
    STT_state state;
    SystemState* st_p;
    //SystemState stateOpen;
    //SystemState stateClose;
    //map<STT_state, SystemState*> states{
    //    {STT_OPEN, &stateOpen},
    //    {STT_OPEN, &stateClose},
    //};
};


#endif