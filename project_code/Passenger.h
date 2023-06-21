//
// Created by surface on 2023-06-01.
//

#ifndef ELEVATOR_SIMULATION_2_PASSENGER_H
#define ELEVATOR_SIMULATION_2_PASSENGER_H

#include <string>

class Passenger {
private:
    int ID;                 //乘客ID
    int gender;             //乘客性别
    int currentFloor;       //乘客现在所在楼层
    int destinationFloor;   //乘客目标楼层
    int passengerDirection; //乘客的乘梯方向

    bool inElevator;        //乘客是否在电梯内
    bool atDestinationFloor;//乘客是否在目标楼层

    bool hasBooked;         //如果乘客已预定一空闲电梯，不能再调用其他空闲电梯；
    int elevatorIndex;      //乘客发出乘梯请求后的被分配的电梯编号

    int TripTimes;          //乘客的总共乘梯次数
    int currentTripCount;   //乘客现在已乘坐的次数
    int minStopTime;        //乘客最短停留时间
    int maxStopTime;        //乘客最长停留时间
    int nextTakeTime;       //乘客下一次发出乘梯请求的时刻

public:
    explicit Passenger(int maxFloor,int allocatedID); //乘客对象构造函数

    int getPassengerID() const;//获取乘客ID
    std::string getPassengerGender() const;//获取乘客性别
    int getCurrentFloor() const;//获取乘客当前楼层
    int getDestinationFloor() const;//获取乘客目标楼层
    int getPassengerDirection() const;//获取乘客乘坐电梯方向

    bool isInElevator() const;//判断乘客是否在电梯内
    void setInElevator(bool set);//设置乘客是否在电梯内的状态
    bool isAtDestinationFloor() const;//判断乘客是否到达目标楼层
    void setAtDestinationFloor(bool set);//设置乘客是否到达目标楼层

    bool isHasBooked() const;//判断乘客是否已经预定了闲置电梯
    void setBookingState(bool set);//设置乘客预定状态
    int getElevatorIndex() const;//获取乘客已分配的电梯编号
    void setElevatorIndex(int set);//为乘客分配电梯

    bool hasRemainingTrips() const ;//判断乘客是否还有剩余乘梯次数
    int getTripTimes() const;//获取乘客的计划乘梯次数
    void takeTrip() ;//更新乘客的乘梯记录
    int getNextTakeTime() const;//获取乘客的下次乘梯时刻
    void setNextTakeTime(int set);//设置乘客的下次乘梯时刻
    void updateCurrent();//更新乘客现在所在楼层
    void updateDestination(int maxFloor);//更新乘客的目标楼层

};




#endif //ELEVATOR_SIMULATION_2_PASSENGER_H
