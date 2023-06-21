//
// Created by surface on 2023-06-01.
//

#ifndef ELEVATOR_SIMULATION_2_ELEVATOR_H
#define ELEVATOR_SIMULATION_2_ELEVATOR_H


#include <vector>
#include <string>
#include <iostream>
#include "Passenger.h"

class Elevator {
private:
    int maxCapacity;                    //电梯最大载客数量
    std::vector<Passenger> passengers;  //在电梯内的乘客组

    int currentFloor;                   //电梯当前所在楼层
    int targetFloor;                    //电梯本次运行的目标楼层
    int direction;                      //电梯当前运行方向（也就是运行状态）

    int bookerID;                       //电梯预定者ID
    int bookerFloor;                    //电梯预定者所在楼层（电梯要去接人）
    int bookDirection;                  //电梯预定者的乘梯方向

    int totalOperationTime;             //电梯总共运行时间
    int totalIdleTime;                  //电梯总共闲置时间

public:
    explicit Elevator(int capacity) ;//电梯对象构造函数

    std::vector<Passenger> getPassengers() const;//获取此时电梯内乘客组的信息
    int getPassengerCount() const ;//获取电梯已有乘客数量

    int getCurrentFloor() const ;//获取电梯当前所在楼层
    int getTargetFloor() const;//获取电梯目标楼层
    int getDirection() const;//获取电梯运行方向
    std::string getState() const ;//获取电梯当前运行状态

    bool addPassenger(Passenger passenger) ;//尝试加入乘客
    void checkPassengerExit(std::vector<Passenger>& passengersInBuilding,int& currentTime);//检查是否有到达了目标楼层的乘客并下客
    void checkPassengerIn(std::vector<Passenger>& passengersInBuilding,int elevatorID,int& currentTime);//检查是否有在这次上该电梯的乘客并上客
    void updateDestinationFloors();//遍历乘客组的目标楼层或者预定者所在楼层，更新电梯目标楼层
    void updateState();//通过目标楼层和当前楼层的比较，更新电梯运行状态

    void run(int stopTimePeriod);//运行电梯
    void startFromIdle(int passengerCurrentFloor,int passengerDirection,int ID);//从闲置启动电梯

    int getBookerID() const;//获取预定者ID
    int getBookDirection() const;//获取预定的电梯运行方向

    int getTotalOperationTime() const;//获取电梯当前总共运行时间
    int getTotalIdleTime() const;//获取电梯当前总共闲置时间
};



#endif //ELEVATOR_SIMULATION_2_ELEVATOR_H
