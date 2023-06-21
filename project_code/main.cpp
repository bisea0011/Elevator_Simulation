#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "Elevator.h"
#include "Passenger.h"

class DataOfCurrentTime{
private:
    int currentTime;
    int busyLevel;

public:DataOfCurrentTime(int time,int cnt){
        currentTime=time;
        busyLevel=cnt;
    }

    int getCurrentTime() const {
        return currentTime;
    }
    int getBusyLevel() const {
        return busyLevel;
    }

    void clearBusyLevel() {
        busyLevel=0;
    }


} ;

void displaySimulationInfo(const std::vector<Elevator> &elevators, int currentTime) {//仿真界面设计：
    std::cout << std::endl;
    std::cout << "Current time: " << currentTime << "seconds" << std::endl;
    for (int i = 0; i < elevators.size(); i++) {
        std::cout << "Elevator " << i << ": " << "State-" << elevators[i].getState() << " Current Floor-" << elevators[i].getCurrentFloor() << " Passenger Num-" << elevators[i].getPassengerCount() <<" if_Booked "<<elevators[i].getBookerID()<< std::endl;
        if (elevators[i].getPassengerCount() > 0) {
            std::cout << "    Target Floor: ";
            std::vector<Passenger> passengers = elevators[i].getPassengers();
            for (auto & passenger : passengers) {
                std::cout <<"ID: "<<passenger.getPassengerID()<<"->"<< passenger.getDestinationFloor() << " ";
            }
            std::cout << std::endl;
        } else{
            std::cout<<"Empty"<<std::endl;
        }
    }
    std::cout << std::endl;
}

bool isElevatorAllowed(int ElevatorIndex, int DestinationFloor) {
    // E0, E1: 可到达每层
    if (ElevatorIndex == 0 || ElevatorIndex == 1) {
        return true;
    }
        // E2, E3: 可到达1, 25~40层
    else if (ElevatorIndex == 2 || ElevatorIndex == 3) {
        if (DestinationFloor == 1 || (DestinationFloor >= 25 && DestinationFloor <= 40)) {
            return true;
        }
    }
        // E4, E5: 可到达1~25层
    else if (ElevatorIndex == 4 || ElevatorIndex == 5) {
        if (DestinationFloor >= 1 && DestinationFloor <= 25) {
            return true;
        }
    }
        // E6, E7: 可到达1, 2~40层中的偶数层
    else if (ElevatorIndex == 6 || ElevatorIndex == 7) {
        if (DestinationFloor == 1 || (DestinationFloor >= 2 && DestinationFloor <= 40 && DestinationFloor % 2 == 0)) {
            return true;
        }
    }
        // E8, E9: 可到达1~39层中的奇数层
    else if (ElevatorIndex == 8 || ElevatorIndex == 9) {
        if (DestinationFloor >= 1 && DestinationFloor <= 39 && DestinationFloor % 2 != 0) {
            return true;
        }
    }

    return false;
}

int main() {
//仿真前的准备：
    //随机数发生器：
    srand(static_cast<unsigned int>(time(0)));
    //楼层、电梯和仿真过程的相关参数的初始化：
    const int numOfElevators = 10;  //10部电梯
    const int numOfFloors = 40; //40层楼
    const int maxCapacity = 5;  // 假设电梯最大载客量为5人
    int numOfPassengers = rand()%101;  // 仿真开始后，有N位乘客（0 < N < 100）
    int nowHasPassengers = 0;   //目前已到达的乘客数量
    int reachTimePeriod=0;  // 记录乘客逐渐到达楼层的时间段
    bool hasRecord= false; //用于到达生成数量后记录时刻之后就不再记录。
    std::vector<DataOfCurrentTime> operationCount;  //用于时序过程中电梯运行的繁忙程度的计数
    int operationCNT=0; //某一时刻的繁忙程度

    std::vector<Elevator> elevators;    //存储仿真中的10部电梯
    elevators.reserve(numOfElevators);  //因为可以确定需要的元素数量，此操作为提前预留内存，从而防止可能出现的内存重新分配和向量数据的复制
    for (int i = 0; i < numOfElevators; i++) {  //生成电梯，用vector组的元素编号表示电梯编号
        elevators.emplace_back(maxCapacity);
    }

    std::vector<Passenger> passengers;  //用于存储此时处于大楼的乘客信息
    static int ID = 0;  //静态量ID用于按照乘客生成的时间先后顺序为每个乘客赋予独一无二的ID

    int currentTime;//记录仿真时刻
    int re_currentTime;//记下的上一次仿真时刻，用于计算电梯闲置时间
    double waitingTime=0;//用于存储乘客的总等待时间
    int ACTimeStatistic=0;//用于存储乘客的总请求次数

    //开始仿真过程，电梯运行时间5秒一层楼
    currentTime = 0;
    while (true) {
        re_currentTime=currentTime;
        for (int i = 0; i < numOfElevators; i++) {
            elevators[i].updateState();
        }
        displaySimulationInfo(elevators, currentTime);
//        // Print debug
//        std::cout << "nowHasPassengers: " << nowHasPassengers << std::endl;
//        std::cout << "numOfPassengers: " << numOfPassengers << std::endl;
//        std::cout << "passengers container is empty? [Y/N][1/0] : " << passengers.empty() << std::endl;
//        std::cout << "passengers count: " << passengers.size()<< std::endl<<std::endl;

        if (nowHasPassengers >= numOfPassengers && passengers.empty()) { break; }
        // 产生随机乘客
        int randomPassengerReach;
        if(nowHasPassengers<=numOfPassengers){//未超过最大可生成乘客数，继续生成乘客
            randomPassengerReach= rand()%10+1; //随机产生1-20位乘客  rand()%10+1
            for (int i=0; i <randomPassengerReach;i++ ) {
                Passenger passenger(numOfFloors,ID);
                passengers.push_back(passenger); //
                nowHasPassengers++;
                ID++; //更新ID
                ACTimeStatistic+=passenger.getTripTimes();
            }
        } else if(!hasRecord){
            reachTimePeriod=currentTime;//乘客均到达大楼，记录此时时间
            hasRecord= true;
        }
        //检索大楼所有乘客,对于有乘梯请求的乘客进行电梯分配：
        for (auto it3 = passengers.begin(); it3 != passengers.end();) {
//            if (passengers.empty()) { break; }
//            std::cout<<std::endl<<"ID "<<it3->ID<<("----------isBeingAlloc-------------------");
//            std::cout<<std::endl<<"ID "<<it3->ID<<(" ,if in Elevator: ")<<it3->inElevator<<std::endl;

            //对于大楼内有乘梯请求的乘客进行电梯预分配：
            //电梯分配规则：没有被预定的运行方向一致且可以接到的电梯；闲置的电梯；被预定的，运行方向一致可以被接到，同时不影响电梯预定者的行程————>这三种情况的电梯可以上
            if((!it3->isAtDestinationFloor())&&(!it3->isInElevator())){ //不在目标楼层，且不在电梯内的乘客
                   for (int j = 0; j < numOfElevators; j++) {
                       if (((elevators[j].getDirection()==it3->getPassengerDirection())&&(elevators[j].getCurrentFloor()*elevators[j].getDirection()<=it3->getCurrentFloor()*it3->getPassengerDirection())&&(elevators[j].getPassengerCount()<maxCapacity)&&(elevators[j].getBookerID()!=-1))
                           ||((elevators[j].getDirection()==it3->getPassengerDirection())&&(elevators[j].getCurrentFloor()*elevators[j].getDirection()<=it3->getCurrentFloor()*it3->getPassengerDirection())&&(elevators[j].getPassengerCount()<maxCapacity-1)&&(elevators[j].getBookerID()!=-1)&&((it3->isAtDestinationFloor()*it3->getPassengerDirection()<=elevators[j].getTargetFloor()*elevators[j].getDirection())||(it3->getPassengerDirection()==elevators[j].getBookDirection())))
                           || (elevators[j].getState() == "Idle"&&(elevators[j].getPassengerCount()<maxCapacity)&&(elevators[j].getBookerID()==-1)&&(!it3->isHasBooked()))  //||elevators[j]
                               ) { //符合条件的电梯
                           if(isElevatorAllowed(j,it3->getDestinationFloor())){
                               it3->setElevatorIndex(j); //预分配电梯（为什么是预分配：因为每一轮电梯的运行和载客状态都有可能改变，所以要实时预分配）
//                               std::cout<<std::endl<<"ID "<<it3->ID<<" is allocated to elevator "<<it3->getElevatorIndex(<<("----------------\n");
                               if(elevators[j].getState()=="Idle"){
                                   it3->setBookingState(true);
                                   elevators[j].startFromIdle(it3->getCurrentFloor(),it3->getPassengerDirection(),it3->getPassengerID());
                               }
                               break;
                           }
                       }
                   }
               } else if(it3->isAtDestinationFloor()&&it3->hasRemainingTrips()){ //到达了目标楼层，并且停留了足够长时间的乘客
                    if(it3->getNextTakeTime()<currentTime){
                        it3->updateDestination(numOfFloors);
//                        printf("\n\n----------it->updateDestination(numOfFloors);-------------------\n\n");
                        for (int j = 0; j < numOfElevators; j++) {
                            if (((elevators[j].getDirection()==it3->getPassengerDirection())&&(elevators[j].getCurrentFloor()*elevators[j].getDirection()<=it3->getCurrentFloor()*it3->getPassengerDirection())&&(elevators[j].getPassengerCount()<maxCapacity)&&(elevators[j].getBookerID()!=-1))
                                ||((elevators[j].getDirection()==it3->getPassengerDirection())&&(elevators[j].getCurrentFloor()*elevators[j].getDirection()<=it3->getCurrentFloor()*it3->getPassengerDirection())&&(elevators[j].getPassengerCount()<maxCapacity-1)&&(elevators[j].getBookerID()!=-1)&&((it3->isAtDestinationFloor()*it3->getPassengerDirection()<=elevators[j].getTargetFloor()*elevators[j].getDirection())||(it3->getPassengerDirection()==elevators[j].getBookDirection())))
                                ||(elevators[j].getState() == "Idle"&&(elevators[j].getPassengerCount()<maxCapacity)&&(elevators[j].getBookerID()==-1)&&(!it3->isHasBooked()))  //||elevators[j]
                                    ) { //符合条件的电梯
                                if(isElevatorAllowed(j,it3->getDestinationFloor())){
                                    it3->setElevatorIndex(j); //预分配电梯（为什么是预分配：因为每一轮电梯的运行和载客状态都有可能改变，所以要实时预分配）
//                               std::cout<<std::endl<<"ID "<<it3->ID<<" is allocated to elevator "<<it3->getElevatorIndex(<<("----------------\n");
                                    if(elevators[j].getState()=="Idle"){
                                        it3->setBookingState(true);
                                        elevators[j].startFromIdle(it3->getCurrentFloor(),it3->getPassengerDirection(),it3->getPassengerID());
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
//            std::cout<<std::endl<<"ID "<<it3->ID<<" 's current Floor:"<<it3->getCurrentFloor()<<"---------------\n";
//            std::cout<<std::endl<<"ID "<<it3->ID<<" 's Destination Floor:"<<it3->getDestinationFloor()<<"---------------\n";
            // 每部电梯在当前所在楼层检索电梯内乘客信息和大楼内乘客信息，电梯上客和下客
            for (int i = 0; i < numOfElevators; i++) {
                elevators[i].updateState(); //根据当前所在楼层，判断和更新当前运行状态（继续/已到达目的地）
                elevators[i].checkPassengerExit(passengers,currentTime); //检索在此楼层要下电梯的乘客，下客
                elevators[i].checkPassengerIn(passengers,i,currentTime); //检索在此楼层要上电梯的乘客，上客
                elevators[i].updateDestinationFloors(); //更新电梯的目标楼层（最终楼层）
                elevators[i].updateState(); //根据当前所在楼层，判断和更新当前运行状态（继续/已到达目的地）
            }
            if(!it3->isInElevator()&&it3->getNextTakeTime()<currentTime) {
                waitingTime+=5;
            }
            ++it3;
        }
        for (auto it2 = passengers.begin(); it2 != passengers.end();) {
            if(!it2->hasRemainingTrips()){
                std::cout<<"Passenger ID "<<it2->getPassengerID()<<" finished "<<it2->getPassengerGender()<<" trips!"<<std::endl;
                it2 = passengers.erase(it2);
            }else{
                ++it2;
            }
        }
        //电梯更新运行状态
        for (int i = 0; i < numOfElevators; i++) {
                elevators[i].updateState();
        }
        // 运行电梯
        for (int i = 0; i < numOfElevators; i++) {
            elevators[i].run(currentTime-re_currentTime); //继续运行电梯，使其到下一层或者继续空闲
            if(elevators[i].getState()!="Idle"){ //如果电梯不是空闲的，则当前时段内电梯的运行情况被统计
                operationCNT++;
            }
        }
        currentTime+=5;
        DataOfCurrentTime DataNow = DataOfCurrentTime(currentTime,operationCNT);
        operationCount.push_back(DataNow);
        operationCNT=0;
        // 显示仿真信息
    }
    std::cout <<"________________________________________________________________________________________"<<std::endl;
    std::cout << "This elevator simulation is over!\n"
                 "This time, "<<nowHasPassengers<<" passengers randomly arrived at the building within the first "<<reachTimePeriod/60+1<<" minutes.\n"
                 "At "<<currentTime<<" second, they all finished their journey.\n"
                 "The relevant statistics are as follows:" << std::endl;
    std::cout << std::endl;
    //运行后的统计分析环节：
    //统计和显示各电梯的总运行时间与空闲时间：
    for (int i = 0; i < numOfElevators; i++) {
        std::cout<<"Elevator "<<i<<" Total Operating Time: "<<elevators[i].getTotalOperationTime()<<", Total Idle Time: "<<elevators[i].getTotalIdleTime()<<std::endl;
    }
    std::cout << std::endl;
    //统计和显示平均一位乘客一次请求的等待时间：
    double averageWaitingTime = waitingTime/ACTimeStatistic; //计算平均一位乘客一次请求的等待时间
    //显示等待时间的统计信息：
    std::cout << "Passenger waiting time statistics:" << std::endl;
    std::cout << " averageWaitingTime: " << averageWaitingTime << "seconds" << std::endl;
    std::cout << std::endl;
    std::cout <<"________________________________________________________________________________________"<<std::endl;
    //计算电梯运行的峰值区的开始与结束时刻,这里我们遍历出二十个峰值时段：
    int maxOperations = 0;
    int peakPeriodStart = 0;
    for (int j=0;j<20;j++) {
        for (auto it =operationCount.begin(); it!=operationCount.end();) {
            if (it->getBusyLevel() > maxOperations) {
                maxOperations = it->getBusyLevel();
                peakPeriodStart = it->getCurrentTime();
                it->clearBusyLevel();
            }
            ++it;
        }
        int peakPeriodEnd = peakPeriodStart + 5;
        std::cout << "Elevator Peak Time Period " <<j<<" :"<< std::endl;
        std::cout << " Busy level:" << maxOperations<< std::endl;
        std::cout << " Start time:" << peakPeriodStart<< "seconds" << std::endl;
        std::cout << " End time: " << peakPeriodEnd << "seconds" << std::endl;
        maxOperations=0;
        peakPeriodStart=0;
    }

    return 0;
}