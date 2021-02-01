//Author: Aaron Cheong, 17990152
//Author: Natkamol Limapichat 49025166

#include 	"..\..\rt.h"
#include 	<stdio.h>
#include "..\Monitor.h"
#include <string>

//============DATAPOOL DATA===================//
struct ElevatorStatus {
	char direction; //Up or Down or Idle
	int general_status; //1=in service or 0=out of service
	int door_status; //1=open or 0=closed
	int current_floor;	//1st-10th floor 
	int session;
	int Eregister[10];	//store all the stops
};

//=============ELEVATOR STATUS===============//
char E1Direction, E2Direction;
int E1Status, E2Status;
int E1Door, E2Door;
int E1Floor, E2Floor;
int E1Session, E2Session;
int E1Boundary[10], E2Boundary[10];
int E1maxfloor, E2maxfloor;

//=========request var=============//
int upcall[10], E1call[10], E2call[10];
int downcall[10];
int maxfloor;
int minfloor;
int requested_floor, out_requestF;
char intrequest[10], out_requestD;
int in_requestF, in_requestE;
int new_requestout, new_requestin;
int E1Register[10], E2Register[10];
int specialrequest = 0;

//Create a string to hold commands
string RequestElevator = { '\0','\0' };

//create typed pipeline
CTypedPipe <char> TypedPipe("ChildPipe", 1024);

//Semaphores
CSemaphore C1("D_E_Elevator1", 1); 
CSemaphore P1("E_D_Elevator1", 0); 
CSemaphore C3("D_E_Elevator2", 1); 
CSemaphore P3("E_D_Elevator2", 0); 


//Elevators
//ElevatorMonitor Elevator1("Elevator1");
//ElevatorMonitor Elevator2("Elevator1");

//Datapools
CDataPool dp1("__DataPool__Elevator1", sizeof(struct ElevatorStatus));
struct ElevatorStatus* Datapool1 = (struct ElevatorStatus*)dp1.LinkDataPool();

CDataPool dp2("__DataPool__Elevator2", sizeof(struct ElevatorStatus));
struct ElevatorStatus* Datapool2 = (struct ElevatorStatus*)dp2.LinkDataPool();

//Threads
UINT  __stdcall  Get_Elevator1_Status(void* args)
{
	//Read Datapool
	while (true) {
		P1.Wait();
		E1Direction = Datapool1->direction;
		E1Status = Datapool1->general_status;
		E1Door = Datapool1->door_status;
		E1Floor = Datapool1->current_floor;
		E1Session = Datapool1->session;
		C1.Signal();
	}
	return 0;
}

UINT  __stdcall  Get_Elevator2_Status(void* args)
{
	while (true) {
		//Semaphore Wait
		P3.Wait();
		//Read Datapool
		E2Direction = Datapool2->direction;
		E2Status = Datapool2->general_status;
		E2Door = Datapool2->door_status;
		E2Floor = Datapool2->current_floor;
		E2Session = Datapool2->session;
		//Semaphore Signal
		C3.Signal();
	}
	return 0;
}


UINT  __stdcall  GetIOCommands(void* args)
{
	//PIPELINED from IO 
	while (1) {
		
		//put information into global variables

		if ((TypedPipe.TestForData()) >= 1) {
			TypedPipe.Read(&RequestElevator[0]);
			TypedPipe.Read(&RequestElevator[1]);
			cout << "Elevator Command from Outside read from IO: " << RequestElevator << endl;

			//if outside update global variables
			if (RequestElevator[0] == 'u' || RequestElevator[0] == 'd') {
				new_requestin = 0;
				new_requestout = 1;
				//outside commands
				out_requestD = RequestElevator[0]; //request direction
				out_requestF = ((int)RequestElevator[1] - '0') + 1; //request floor
			}

			//if inside update global variables
			if (RequestElevator[0] == '1' || RequestElevator[0] == '2') {
				new_requestin = 1;
				new_requestout = 0;
				//inside commands
				in_requestF = ((int)RequestElevator[1] - '0' ) + 1;
				in_requestE = (int)RequestElevator[0] - '0';
			}

			if (RequestElevator[1] == 'e' && RequestElevator[0] == 'e') {
				cout << "THREE" << endl;
				//bring both elevators to the ground
				//open doors
				//do not take any further commands
				//stop simulation
			}

			if (RequestElevator[0] == '+' && RequestElevator[1] == '1') {
				in_requestE = 1;
				specialrequest = 99;
			}

			if (RequestElevator[0] == '-' && RequestElevator[1] == '1') {
				in_requestE = 1;
				specialrequest = 98;
			}

			if (RequestElevator[0] == '+' && RequestElevator[1] == '2') {
				in_requestE = 2;
				specialrequest = 99;
			}

			if (RequestElevator[0] == '-' && RequestElevator[1] == '2') {
				in_requestE = 2;
				specialrequest = 98;
			}
			//if down is selected possible values are d1-d9

			//if going up block floors lower or same floor than you
			if (RequestElevator == "u9") {
				cout << "You are already at the tenth floor!" << endl;
			}

			else if (RequestElevator == "d0") {
				cout << "You are already at the first floor!" << endl;
			}
		}
	}
	return 0;
}


//----MAIN----//
int main()
{
	cout << "Creating both elevators and IO.....\n";

	CProcess Elevator1("E:\\UBC\\CPEN333\\Assignment\\ElevatorSim\\Debug\\Elevator1.exe",	// pathlist to child program executable				
		NORMAL_PRIORITY_CLASS,			// priority
		OWN_WINDOW,						// process has its own window					
		ACTIVE							// process is active immediately
	);

	CProcess Elevator2("E:\\UBC\\CPEN333\\Assignment\\ElevatorSim\\Debug\\Elevator2.exe",	// pathlist to child program executable				
		NORMAL_PRIORITY_CLASS,			// priority
		OWN_WINDOW,						// process has its own window					
		ACTIVE							// process is active immediately
	);

	CProcess IO("E:\\UBC\\CPEN333\\Assignment\\ElevatorSim\\Debug\\IO.exe",	// pathlist to child program executable	plus some arguments		
		NORMAL_PRIORITY_CLASS,			// priority
		OWN_WINDOW,						// process has its own window					
		ACTIVE
	);

	CRendezvous     r1("MyRendezvous", 4);
	r1.Wait();

	cout << "All 4 processes are synchronized by Rendezvous\n";
	cout << "Establishing typed pipeline to IO" << endl;

	//semaphore should be threads

	CSemaphore		P2("P2", 0, 1);    // 2nd semaphore with initial value 0 and max value 1
	CSemaphore		C2("C2", 1, 1);    // 2nd semaphore with initial value 1 and max value 1

	//-----------Algorithm---------------// 

	CThread t1(Get_Elevator1_Status, ACTIVE, NULL);
	CThread t2(Get_Elevator2_Status, ACTIVE, NULL);
	CThread t3(GetIOCommands, ACTIVE, NULL); 

	CTypedPipe <int> Sending("SpecialPipe", 1024);
	CTypedPipe <int> Sending2("SpecialPipe2", 1024);

	while (true) {


		if (specialrequest != 0) {
			cout << "fault!" << endl;
			if (in_requestE == 1) {
				Sending.Write(&specialrequest);
				cout << specialrequest << endl;
				specialrequest = 0;
			}
			else if (in_requestE == 2) {
				Sending2.Write(&specialrequest);
				cout << specialrequest << endl;
				specialrequest = 0;
			}
		}

		//outside request

		if (new_requestout == 1) {
			if (E1Session == 0 && E2Session == 0) {
				if (abs(E1Floor - out_requestF) < abs(E2Floor - out_requestF)) {
					//##send mailbox out_requestF to E1
					//Elevator1.Post(out_requestF + 999);
					Sending.Write(&out_requestF);
					new_requestout = 0;
					out_requestF = 0;
					cout << "1" << endl;
				}
				else {				//E2 closr
					//##send mailbox out_requestF to E2
					//Elevator2.Post(out_requestF + 999);
					Sending2.Write(&out_requestF);
					new_requestout = 0;
					out_requestF = 0;
					cout << "2" << endl;
				}
			}
			else if ((E1Direction == 'u') && (E1Floor < out_requestF) && (E1Direction == out_requestD)) {	//on the way of E1 going up
				//##send mailbox out_requestF to E1
				//Elevator1.Post(out_requestF + 999);
				Sending.Write(&out_requestF);
				new_requestout = 0;
				out_requestD = NULL;
				out_requestF = 0;
				cout << "3" << endl;
			}
			else if ((E1Direction == 'd') && (E1Floor > out_requestF) && (E1Direction == out_requestD)) {	//on the way of E1 going down
				//##send mailbox out_requestF to E1
				//Elevator1.Post(out_requestF + 999);
				Sending.Write(&out_requestF);
				new_requestout = 0;
				out_requestD = NULL;
				out_requestF = 0;
				cout << "4" << endl;
			}
			else if ((E2Direction == 'u') && (E2Floor < out_requestF) && (E2Direction == out_requestD)) {	//on the way of E1 going up
				//##send mailbox out_requestF to E2
				//Elevator2.Post(out_requestF + 999);
				Sending2.Write(&out_requestF);
				new_requestout = 0;
				out_requestD = NULL;
				out_requestF = 0;
				cout << "5" << endl;
			}
			else if ((E2Direction == 'd') && (E2Floor > out_requestF) && (E2Direction == out_requestD)) {	//on the way of E1 going up
				//##send mailbox out_requestF to E2
				//Elevator2.Post(out_requestF + 999);
				Sending2.Write(&out_requestF);
				new_requestout = 0;
				out_requestD = NULL;
				out_requestF = 0;
				cout << "6" << endl;
			}
			else if (E1Session == 0) {
				//##send mailbox out_requestF to E1
				//Elevator1.Post(out_requestF + 999);
				Sending.Write(&out_requestF);
				new_requestout = 0;
				out_requestD = NULL;
				out_requestF = 0;
				cout << "7" << endl;
			}
			else if (E2Session == 0) {
				//##send mailbox out_requestF to E2
				//Elevator2.Post(out_requestF + 999);
				Sending2.Write(&out_requestF);
				new_requestout = 0;
				out_requestD = NULL;
				out_requestF = 0;
				cout << "8" << endl;
			}
			else {										//add to request log
				intrequest[out_requestF] = out_requestF;
				new_requestout = 0;
			}
		}

		//inside request
		if (new_requestin == 1) {
			if (in_requestE == 1) {			//request E1 from inside
				//##send mailbox in_requestE to E1
				//Elevator1.Post(in_requestF + 999);
				Sending.Write(&in_requestF);
				new_requestin = 0;
				cout << "E1" << endl;
				cout << in_requestF << endl;
				in_requestF = 0;
			}
			else if (in_requestE == 2) {	//request E2 from inside
				//##send mailbox in_requestE to E2
				//Elevator2.Post(in_requestF + 999);
				Sending2.Write(&in_requestF);
				new_requestin = 0;
				cout << "E2" << endl;
				cout << in_requestF << endl;
				in_requestF = 0;
			}
		}
	} 
	Elevator1.WaitForProcess();
	Elevator2.WaitForProcess();
	IO.WaitForProcess();


	return 0;
}
