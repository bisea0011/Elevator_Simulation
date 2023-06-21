//
// Created by surface on 2023-06-01.
//

#include "Elevator.h"

Elevator::Elevator(int capacity)  : maxCapacity(capacity), currentFloor(1), direction(0) ,targetFloor(1),bookerFloor(-1),bookDirection(0),totalOperationTime(0),totalIdleTime(0),bookerID(-1){}
// "-1" 表示 "无"



std::vector<Passenger> Elevator::getPassengers() const {
    return passengers;
}

int Elevator::getPassengerCount() const {
    return passengers.size();
}




int Elevator::getCurrentFloor() const {
    return currentFloor;
}

int Elevator::getTargetFloor() const{
    return targetFloor;
}

int Elevator::getDirection() const {
    return direction;
}

std::string Elevator::getState() const {
    if (direction == 1) {
        return "Up  ";
    }
    else if (direction == -1) {
        return "Down";
    }
    else {
        return "Idle";
    }
}




bool Elevator::addPassenger(Passenger passenger) {
    if (passengers.size() < maxCapacity) {
        passenger.setInElevator(true);
        passengers.push_back(passenger);
        return true;
    }
    else {
        passenger.setInElevator(false);
        std::cout << "The elevator is overcrowded!" << std::endl;
        return false;
    }
}

void Elevator::checkPassengerExit(std::vector<Passenger>& passengersInBuilding, int& currentTime) {
    for (auto it = passengers.begin(); it != passengers.end();) {
        if (it->getDestinationFloor() == currentFloor) {//检索电梯内的目标楼层和当前楼层相同的乘客
            for (auto it2 = passengersInBuilding.begin(); it2 != passengersInBuilding.end();){
                if(it->getPassengerID()==it2->getPassengerID()){//在大楼乘客信息中找到该乘客
                    it2->takeTrip();//乘客乘电梯次数记录+1
                    it2->setInElevator(false);//乘客出电梯
                    it2->setAtDestinationFloor(true);//乘客到达目标楼层
                    it2->setElevatorIndex(-1);//乘客电梯分配情况清除
                    it2->updateCurrent();//更新乘客当前楼层
                    it2->setBookingState(false);//乘客预定电梯信息清除
                    if (it2->hasRemainingTrips()){//判断乘客此时是否还有乘电梯次数，如果还有的话进行下一次乘电梯时间的设置
                        it2->setNextTakeTime(currentTime);
                    }
                    break;
                }
                ++it2;
            }
            currentTime+=3;//该乘客下电梯的花费时间记录下来
            totalOperationTime+=3;
            it = passengers.erase(it);//电梯乘客组删除该乘客信息
        }
        else {
            ++it;
        }
    }
}

void Elevator::checkPassengerIn(std::vector<Passenger> &passengersInBuilding,int elevatorID,int& currentTime) {
    for (auto it = passengersInBuilding.begin(); it != passengersInBuilding.end();){
        if((it->getElevatorIndex()==elevatorID)&&(!it->isInElevator())&&(it->getCurrentFloor()==currentFloor)){//检索大楼中所在楼层与该电梯当前楼层相同且分配的电梯编号与该电梯相同的乘客
            if(addPassenger(*it)) {//如果加入乘客成功（即电梯此时未满）
                currentTime+=3;//记录乘客上电梯花费时间
                totalOperationTime+=3;
                it->setInElevator(true);//乘客上了电梯
                if (it->getPassengerID() == bookerID) {//如果该电梯是该乘客预定的，则已接到预定者，电梯的预定者ID消除
                    bookerID = -1;
                }
            }
//            std::cout<<"\n-------------add ID "<<it->ID<<" to elevator "<<elevatorID<<"------------\n";
        }
        ++it;
    }
    if(bookerID!=-1&&currentFloor==bookerFloor){//如果到了预定者的楼层但是预定者没有上该电梯，则电梯的预定者ID消除（电梯错付了）
        bookerID=-1;
        bookerFloor=-1;
    }
}

void  Elevator::updateDestinationFloors() {//更新电梯此时的目标楼层
    int highestFloor = 1;
    int lowestFloor = 40;
    if(bookerID==-1){//非被预定的情况，遍历乘客组目标楼层，按照方向取最高或最低楼层
        if(!passengers.empty()){
            for (auto & passenger : passengers) {
                targetFloor = passenger.getDestinationFloor();
                if (targetFloor > highestFloor) {
                    highestFloor = targetFloor;
                }
                if (targetFloor < lowestFloor) {
                    lowestFloor = targetFloor;
                }
            }
            if (direction == 1) {
                targetFloor = highestFloor;
            }
            else if (direction == -1) {
                targetFloor = lowestFloor;
            }
        }
    }//预定的情况，此时还没接到预定者，故运行方向不变
}

void Elevator::updateState() {
    if (currentFloor < targetFloor) {
        direction = 1;
    }
    else if (currentFloor > targetFloor) {
        direction = -1;
    }
    else {
        direction = 0;
    }
}




void Elevator::run(int stopTimePeriod) {
    if (direction == 1) {
        currentFloor++;
        totalOperationTime+=5;
    }
    else if (direction == -1) {
        currentFloor--;
        totalOperationTime+=5;
    } else if (direction==0){
        totalIdleTime+=stopTimePeriod+5;

    }
}

void Elevator::startFromIdle(int passengerCurrentFloor , int passengerDirection,int ID) {
//    std::cout<<"\nMy BookID == "<<bookerID<<" !---------------\n";
    if(bookerID == -1){//如果电梯此时没有被预定，则可以被预定
        targetFloor = passengerCurrentFloor;
        bookerFloor=passengerCurrentFloor;
        bookDirection = passengerDirection;
        bookerID = ID;
//        std::cout<<"\nElevator Get the order from ID "<<ID<<"!---------------\n";
    }
}




int Elevator::getBookerID() const {
    return bookerID;
}

int Elevator::getBookDirection() const {
    return bookDirection;
}




int Elevator::getTotalOperationTime() const {
    return totalOperationTime;
}

int Elevator::getTotalIdleTime() const {
    return totalIdleTime;
}