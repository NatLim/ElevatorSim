//Author: Aaron Cheong
#pragma once
#ifndef __Monitor1__
#define __Monitor1__

#include "..\..\rt.h"

class 	ElevatorMonitor {
private:
	struct ElevatorStatus {
		string direction; //up or down or idle
		BOOL general_status; //1=in service or 0=out of service
		BOOL door_status; //1=open or 0=closed
		UINT current_floor;	//1st-10th floor 
	};

	//Named Monitor Variables
	CDataPool* ElevatorDatapool;  // a datapool containing the data to be protected: ElevatorStatus; 
	CMutex* ElevatorMutex;	       // a pointer to a hidden mutex protecting the struct data above
	ElevatorStatus* elevatorstatus_ptr;			// pointer to the data
	
	//Semaphores
	CSemaphore* Dispatcher_Consume;
	CSemaphore* IO_Consume;
	CSemaphore* P1;
	CSemaphore* C1;
	CSemaphore* P2;
	CSemaphore* C2;
	CSemaphore* P3;
	CSemaphore* C3;
	CSemaphore* P4;
	CSemaphore* C4;

public:
	void show_status() {
		cout << "Current direction: " << elevatorstatus_ptr->direction << endl;
	    cout << "Current general status: " << elevatorstatus_ptr->general_status << endl;
		cout << "Current door status: " << elevatorstatus_ptr->door_status << endl;
		cout << "Current current floor: " << elevatorstatus_ptr->current_floor << endl;
	}
	
	void open_door() {
		ElevatorMutex->Wait();
		cout << "Door opening\n";
		Sleep(1000);
		elevatorstatus_ptr->door_status = 1;
		cout << "Door is opened\n";
		ElevatorMutex->Signal();
	}

	void close_door() {
		ElevatorMutex->Wait();
		cout << "Door closing\n";
		Sleep(1000);
		elevatorstatus_ptr->door_status = 0;
		cout << "Door is closed\n";
		ElevatorMutex->Signal();
	}

	void in_service() {
		ElevatorMutex->Wait();
		elevatorstatus_ptr->general_status = 1;
		ElevatorMutex->Signal();
	}

	void out_service() {
		ElevatorMutex->Wait();
		elevatorstatus_ptr->general_status = 0;
		ElevatorMutex->Signal();
	}

	void Update_Status(string direction, BOOL general_status, BOOL door_status, UINT current_floor) {
		//wait for both Dispatcher and IO to consume last update
		Dispatcher_Consume->Wait();
		IO_Consume->Wait();
		//write new data to datapool
		elevatorstatus_ptr->direction = direction;
		elevatorstatus_ptr->general_status = general_status;
		elevatorstatus_ptr->door_status = door_status;
		elevatorstatus_ptr->current_floor = current_floor;
		//signal both Dispatcher and IO new data is available
		Dispatcher_Consume->Signal();
		IO_Consume->Signal();
	}
	
	void DGet_Elevator1_Status()
		//Two of the threads respond to changes in elevator statusand copy that status to local variables
		//within the process so that when a new command arrives, it can be dealt with immediately
	{
		P2->Wait();
		//Read from datapool of updated variables (consume)
		elevatorstatus_ptr->direction = elevatorstatus_ptr->direction;
		elevatorstatus_ptr->general_status = elevatorstatus_ptr->general_status;
		elevatorstatus_ptr->door_status = elevatorstatus_ptr->door_status;
		elevatorstatus_ptr->current_floor = elevatorstatus_ptr->current_floor;
	    C2->Signal();
	}

	void DGet_Elevator2_Status()
		//Two of the threads respond to changes in elevator statusand copy that status to local variables
		//within the process so that when a new command arrives, it can be dealt with immediately
	{
		P4->Wait();
		//Read from datapool of updated variables (consume)
		elevatorstatus_ptr->direction = elevatorstatus_ptr->direction;
		elevatorstatus_ptr->general_status = elevatorstatus_ptr->general_status;
		elevatorstatus_ptr->door_status = elevatorstatus_ptr->door_status;
		elevatorstatus_ptr->current_floor = elevatorstatus_ptr->current_floor;
		C4->Signal();
	}

	void DGet_Elevator1_Status()
		//Two of the threads respond to changes in elevator statusand copy that status to local variables
		//within the process so that when a new command arrives, it can be dealt with immediately
	{
		P1->Wait();
		//Read from datapool of updated variables (consume)
		elevatorstatus_ptr->direction = elevatorstatus_ptr->direction;
		elevatorstatus_ptr->general_status = elevatorstatus_ptr->general_status;
		elevatorstatus_ptr->door_status = elevatorstatus_ptr->door_status;
		elevatorstatus_ptr->current_floor = elevatorstatus_ptr->current_floor;
		C1->Signal();
	}

	void IGet_Elevator2_Status()
		//Two of the threads respond to changes in elevator statusand copy that status to local variables
		//within the process so that when a new command arrives, it can be dealt with immediately
	{
		P3->Wait();
		//Read from datapool of updated variables (consume)
		elevatorstatus_ptr->direction = elevatorstatus_ptr->direction;
		elevatorstatus_ptr->general_status = elevatorstatus_ptr->general_status;
		elevatorstatus_ptr->door_status = elevatorstatus_ptr->door_status;
		elevatorstatus_ptr->current_floor = elevatorstatus_ptr->current_floor;
		C3->Signal();
	}

	
	void GoFloorX(int x, int y) {
		ElevatorMutex->Wait();
		//x = destination, y = current floor
		if (elevatorstatus_ptr->direction == "up") { //if going up, destination-current floor
			int count = x - y;
			for (int z = 0; z < count; z++) {
				elevatorstatus_ptr->current_floor++;
				Sleep(2000);
				cout << "Currently at the: " << elevatorstatus_ptr->current_floor << "th floor." << endl;
			}
			elevatorstatus_ptr->direction = "idle";
			Sleep(1000);
			elevatorstatus_ptr->door_status = 1;
		}

		if (elevatorstatus_ptr->direction == "down") { //if going down, current floor-destination
			int count = y - x;
			for (int z = 0; z < count; z++) {
				elevatorstatus_ptr->current_floor--;
				Sleep(2000);
				cout << "Currently at the: " << elevatorstatus_ptr->current_floor << "th floor." << endl;
			}
			elevatorstatus_ptr->direction = "idle";
			Sleep(1000);
			elevatorstatus_ptr->door_status = 1;
		}
	
		ElevatorMutex->Signal();
	}

	// constructor and destructor
	ElevatorMonitor(string Name) {
		ElevatorMutex = new CMutex(string("__Mutex__") + string(Name));
		ElevatorDataPool = new CDataPool(string("__DataPool__") + string(Name), sizeof(struct ElevatorStatus));
		elevatorstatus_ptr = (struct ElevatorStatus*)(ElevatorDataPool->LinkDataPool());

		string direction = "idle"; //up or down or not moving
		BOOL general_status = 1;//in or out of service
		BOOL door_status = 0;//open or closed
		UINT current_floor = 1;	//1st-10th floor 

		//1 = free, 0 = busy
		Dispatcher_Consume = new CSemaphore(string("__Dispatcher_Consume__"), 1);
		IO_Consume = new CSemaphore(string("__IO_Consume__"), 1);
		
	}
	~ElevatorMonitor() { /* delete mutex and datapool; */ }
};
#endif
#pragma once
#pragma once
#pragma once
#pragma once
