//Author: Aaron Cheong, 17990152
//Author: Natkamol Limapichat 49025166

#ifndef __Monitor__
#define __Monitor__

#include "..\rt.h"

class 	ElevatorMonitor {
private:
struct ElevatorStatus {
	char direction; //Up or Down or Idle
	int general_status; //1=in service or 0=out of service
	int door_status; //1=open or 0=closed
	int current_floor;	//1st-10th floor 
	int session;
	int Eregister[10];	//store all the stops
};

	char direction; //Up or Down or Idle
	int general_status; //1=in service or 0=out of service
	int door_status; //1=open or 0=closed
	int current_floor;	//1st-10th floor 
	string name;

	//Named Monitor Variables
	CDataPool* ElevatorDatapool;  // a datapool containing the data to be protected: ElevatorStatus; 
	CMutex* ElevatorMutex;	       // a pointer to a hidden mutex protecting the struct data above
	struct ElevatorStatus* elevatorstatus_ptr;			// pointer to the data
	
	//Semaphores
	CSemaphore* Dispatcher_Consume;
	CSemaphore* IO_Consume;
	CSemaphore* C1x; //also c3
	CSemaphore* P1x; //also p3
	CSemaphore* C2x; //also c4
	CSemaphore* P2x; //also p4

public:

	int Eregister[10];
	int session;
	// constructor and destructor
	ElevatorMonitor(string Name) {
		ElevatorMutex = new CMutex(string("__Mutex__") + string(Name));
		ElevatorDatapool = new CDataPool(string("__DataPool__") + string(Name), sizeof(struct ElevatorStatus));
		elevatorstatus_ptr = (struct ElevatorStatus*)(ElevatorDatapool->LinkDataPool());

		direction = 'i'; //up or down or not moving
		general_status = 1;//in or out of service
		door_status = 0;//open or closed
		current_floor = 0;	//1st-10th floor 
		session = 0;
		name = Name;

		//1 = free, 0 = busy
//		Dispatcher_Consume = new CSemaphore((string("__Dispatcher_Consume__") + string(Name)), 1);
//		IO_Consume = new CSemaphore((string("__IO_Consume__") +string(Name)), 1);
		C1x = new CSemaphore(string("D_E_") + string(Name), 1); //also c3
		P1x = new CSemaphore(string("E_D_") + string(Name), 0); //also p3
		C2x = new CSemaphore(string("IO_E_") + string(Name), 1); //also c4
		P2x = new CSemaphore(string("E_IO_") + string(Name), 0); //also p4
	}

	~ElevatorMonitor() { /* delete mutex and datapool; */ }

	void print() {
		cout << "Current direction: " << elevatorstatus_ptr->direction << endl;
		cout << "Current general status: " << elevatorstatus_ptr->general_status << endl;
		cout << "Current door status: " << elevatorstatus_ptr->door_status << endl;
		cout << "Current current floor: " << elevatorstatus_ptr->current_floor << endl;
	}

	void open_door() {
		ElevatorMutex->Wait();
		cout << "Door opening\n";
		Sleep(1000);
		door_status = 1;
		cout << "Door is opened\n";
		ElevatorMutex->Signal();
	}

	void close_door() {
		ElevatorMutex->Wait();
		cout << "Door closing\n";
		Sleep(1000);
		door_status = 0;
		cout << "Door is closed\n";
		ElevatorMutex->Signal();
	}

	void in_service() {
		ElevatorMutex->Wait();
		general_status = 1;
		ElevatorMutex->Signal();
	}

	void out_service() {
		ElevatorMutex->Wait();
		general_status = 0;
		ElevatorMutex->Signal();
	}

	void Update_Status() {
		//wait for both Dispatcher and IO to consume last update
		ElevatorMutex->Wait();
		C1x->Wait();
		C2x->Wait();
		//write new data to datapool
		elevatorstatus_ptr->direction = direction;
		elevatorstatus_ptr->general_status = general_status;
		elevatorstatus_ptr->door_status = door_status;
		elevatorstatus_ptr->current_floor = current_floor;
		elevatorstatus_ptr->session = session;
		//signal both Dispatcher and IO new data is available
		P1x->Signal();
		P2x->Signal();
		ElevatorMutex->Signal();
	}
/*
	void Get_Elevator_Status()
		//Two of the threads respond to changes in elevator status and copy that status to local variables
		//within the process so that when a new command arrives, it can be dealt with immediately
	{
		//semaphores
		C1x->Wait();
		C2x->Wait();
		//Read from datapool of updated variables (consume)
		direction = elevatorstatus_ptr->direction;
		general_status = elevatorstatus_ptr->general_status;
		door_status = elevatorstatus_ptr->door_status;
		current_floor = elevatorstatus_ptr->current_floor;
		P1x->Signal();
		P2x->Signal();
	}
	*/
	void go_up(){						
		ElevatorMutex->Wait();
		if(current_floor == 9){
			direction = 'i';
		}
		else{
			direction = 'u';
			current_floor = current_floor + 1;
		}
		ElevatorMutex->Signal();
	}

	void go_down(){
		ElevatorMutex->Wait();
		if(current_floor == 0){
			direction = 'i';
		}
		else{
			direction = 'd';
			current_floor = current_floor - 1;
		}
		ElevatorMutex->Signal();
	}

	void go_idle() {
		ElevatorMutex->Wait();
		direction = 'i';
		session = 0;
		ElevatorMutex->Signal();
	}

	int currentfloor() {
		ElevatorMutex->Wait();
		return current_floor;
		ElevatorMutex->Signal();
	}

	int return_session() {
		ElevatorMutex->Wait();
		return session;
		ElevatorMutex->Signal();
	}

	char get_direction() {
		ElevatorMutex->Wait();
		return direction;
		ElevatorMutex->Signal();
	}

	void fault() {
		ElevatorMutex->Wait();
		general_status = 0;
		ElevatorMutex->Signal();
	}

	void unfault() {
		ElevatorMutex->Wait();
		general_status = 1;
		ElevatorMutex->Signal();
	}

	int faultcheck() {
		ElevatorMutex->Wait();
		return general_status;
		ElevatorMutex->Signal();
	}

};
#endif
