//
// Created by surface on 2023-06-01.
//

#include <cstdlib>
#include "Passenger.h"

Passenger::Passenger(int maxFloor,int allocatedID) {
    ID=allocatedID;                                 //ID录入
    gender=rand()%2;                                //随机性别，0（女士）或1（男士）
    currentFloor = 1;                               //初始在一楼（由一楼进入电梯）
    do {
        destinationFloor = rand() % maxFloor + 1;
    } while (destinationFloor==currentFloor);      //1-40层随机选一层作为目标楼层
    passengerDirection=1;                           //初次乘梯方向一定是往上（我未设置地下楼层）

    inElevator = false;                             //初始不在电梯内
    atDestinationFloor = false;                     //初始不在目的地

    hasBooked= false;                               //初始未预定空闲电梯
    elevatorIndex = -1;                             //未分配电梯

    TripTimes = rand() % 10+1;                      //随机生成乘梯次数 1~10；
    currentTripCount = 0;                           //初始已乘电梯次数0
    minStopTime = 10;                               //在一个楼层的最小停留时间
    maxStopTime = 120;                              //在一个楼层的最大停留时间
    nextTakeTime=0;                                 //初始第一次乘电梯无需设置下一次乘梯时间
}




int Passenger::getPassengerID() const{
    return ID;
}

std::string Passenger::getPassengerGender() const{
    return gender==1?"his":"her";
}

int Passenger::getCurrentFloor() const{
    return currentFloor;
}

int Passenger::getDestinationFloor() const {
    return destinationFloor;
}

int Passenger::getPassengerDirection() const {
    return passengerDirection;
}




bool Passenger::isInElevator() const {
    return inElevator;
}

void Passenger::setInElevator(bool set) {
    inElevator=set;
}

bool Passenger::isAtDestinationFloor() const {
    return atDestinationFloor;
}

void Passenger::setAtDestinationFloor(bool set) {
    atDestinationFloor=set;
}




bool Passenger::isHasBooked() const {
    return hasBooked;
}

void Passenger::setBookingState(bool set) {
    hasBooked=set;
}

int Passenger::getElevatorIndex() const {
    return elevatorIndex;
}

void Passenger::setElevatorIndex(int set){
    elevatorIndex=set;
}




bool Passenger::hasRemainingTrips() const {
    return currentTripCount <= TripTimes;
}

int Passenger::getTripTimes() const {
    return TripTimes;
}

void Passenger::takeTrip() {
    currentTripCount++;
}

int Passenger::getNextTakeTime() const {
    return nextTakeTime;
}

void Passenger::setNextTakeTime(int set) {
    nextTakeTime=set+rand()% (maxStopTime - minStopTime + 1) + minStopTime;
}

void Passenger::updateCurrent() {
    currentFloor=destinationFloor;
}

void Passenger::updateDestination(int maxFloor) {
    if(currentTripCount<TripTimes){
        do {
            destinationFloor = rand() % maxFloor + 1;
        } while (destinationFloor==currentFloor);
    }else{
        destinationFloor = 1;
//        printf("\n\n-----------------destinationFloor = 1;-------------\n\n");
    }
    atDestinationFloor= false;
    if(destinationFloor > currentFloor)
        passengerDirection=1;
    else if(destinationFloor==currentFloor)
        passengerDirection=0;
    else
        passengerDirection=-1;
}
