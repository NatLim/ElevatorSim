//Author: Aaron Cheong, 17990152
//Author: Natkamol Limapichat 49025166

//Header Files
#include 	"..\..\rt.h"				// change pathlist to this header file according to where it is stored
#include 	<stdio.h>
#include "..\Monitor.h"
#include <stdlib.h>


//Variables
string RequestElevator = {'\0','\0'};

//create a pipeline to write data
CTypedPipe <char> TypedPipe("ChildPipe", 1024);

//============DATAPOOL DATA===================//
struct ElevatorStatus {
	char direction; //Up or Down or Idle
	int general_status; //1=in service or 0=out of service
	int door_status; //1=open or 0=closed
	int current_floor;	//1st-10th floor 
	int session;
	int Eregister[10];	//store all the stops
};

char E1Direction, E2Direction;
int E1Status, E2Status;
int E1Door, E2Door;
int E1Floor, E2Floor;
int E1Session, E2Session;
int E1Boundary[10], E2Boundary[10];
int E1maxfloor, E2maxfloor;

//create a datapool to read from
CDataPool dp1("__DataPool__Elevator1", sizeof(struct ElevatorStatus));
struct ElevatorStatus* Datapool1 = (struct ElevatorStatus*)dp1.LinkDataPool();

CDataPool dp2("__DataPool__Elevator2", sizeof(struct ElevatorStatus));
struct ElevatorStatus* Datapool2 = (struct ElevatorStatus*)dp2.LinkDataPool();

//Semaphore
CSemaphore C2("IO_E_Elevator1", 1);
CSemaphore P2("E_IO_Elevator1", 0);
CSemaphore C4("IO_E_Elevator2", 1);
CSemaphore P4("E_IO_Elevator2", 0);

//IO Threads
UINT  __stdcall  Get_Elevator1_Status(void *)
{
	CMutex m1("mutexIO");
	while (true) {
		P2.Wait();
		m1.Wait();
		E1Direction = Datapool1->direction;
		E1Status = Datapool1->general_status;
		E1Door = Datapool1->door_status;
		E1Floor = Datapool1->current_floor;
		E1Session = Datapool1->session;

		MOVE_CURSOR(0, 25);
		cout << " E1 Dir: " << E1Direction << " E1 Sts: " << E1Status << " E1 Door: " << E1Door << " E1 Flr: " << E1Floor << " E1 Ses: " << E1Session << endl;
		m1.Signal();
		C2.Signal();
		
		switch (E1Floor) {
		case 0:
			m1.Wait();
			for (int i = 0; i < 9; i++) {
				MOVE_CURSOR(0, i);
				cout << 10 - i - 1 << "|  |		";
			}
			MOVE_CURSOR(0, 9);
			cout << 0 << "|E1|";
			if (E1Direction == 'u') {
				cout<<"/\\	";
			}
			else if(E1Direction == 'd'){
				cout << "\\/	";
			}
			m1.Signal();
			break;

		case 1:
			m1.Wait();
			for (int i = 0; i < 8; i++) {
				MOVE_CURSOR(0, i);
				cout << 10 - i - 1 << "|  |		";
			}
			MOVE_CURSOR(0, 8);
			cout << 1 << "|E1|";
			if (E1Direction == 'u') {
				cout << "/\\	";
			}
			else if (E1Direction == 'd') {
				cout << "\\/	";
			}
			MOVE_CURSOR(0, 9);
			cout << 0 << "|  |		";
			m1.Signal();
			break;

		case 2:
			m1.Wait();
			for (int i = 0; i < 7; i++) {
				MOVE_CURSOR(0, i);
				cout << 10 - i - 1 << "|  |		";
			}
			MOVE_CURSOR(0, 7);
			cout << 2 << "|E1|";
			if (E1Direction == 'u') {
				cout << "/\\	";
			}
			else if (E1Direction == 'd') {
				cout << "\\/	";
			}
			for (int i = 8; i <= 9; i++) {
				MOVE_CURSOR(0, i);
				cout << 10 - i - 1 << "|  |		";
			}
			m1.Signal();
			break;

		case 3:
			m1.Wait();
			for (int i = 0; i < 6; i++) {
				MOVE_CURSOR(0, i);
				cout << 10 - i - 1 << "|  |		";
			}
			MOVE_CURSOR(0, 6);
			cout << 3 << "|E1|";
			if (E1Direction == 'u') {
				cout << "/\\	";
			}
			else if (E1Direction == 'd') {
				cout << "\\/	";
			}
			for (int i = 7; i <= 9; i++) {
				MOVE_CURSOR(0, i);
				cout << 10 - i - 1 << "|  |		";
			}
			m1.Signal();
			break;

		case 4:
			m1.Wait();
			for (int i = 0; i < 5; i++) {
				MOVE_CURSOR(0, i);
				cout << 10 - i - 1 << "|  |		";
			}
			MOVE_CURSOR(0, 5);
			cout << 4 << "|E1|";
			if (E1Direction == 'u') {
				cout << "/\\	";
			}
			else if (E1Direction == 'd') {
				cout << "\\/	";
			}
			for (int i = 6; i <= 9; i++) {
				MOVE_CURSOR(0, i);
				cout << 10 - i - 1 << "|  |		";
			}
			m1.Signal();
			break;

		case 5:
			m1.Wait();
			for (int i = 0; i < 4; i++) {
				MOVE_CURSOR(0, i);
				cout << 10 - i - 1 << "|  |		";
			}
			MOVE_CURSOR(0, 4);
			cout << 5 << "|E1|";
			if (E1Direction == 'u') {
				cout << "/\\	";
			}
			else if (E1Direction == 'd') {
				cout << "\\/	";
			}
			for (int i = 5; i <= 9; i++) {
				MOVE_CURSOR(0, i);
				cout << 10 - i - 1 << "|  |		";
			}
			m1.Signal();
			break;

		case 6:
			m1.Wait();
			for (int i = 0; i < 3; i++) {
				MOVE_CURSOR(0, i);
				cout << 10 - i - 1 << "|  |		";
			}
			MOVE_CURSOR(0, 3);
			cout << 6 << "|E1|";
			if (E1Direction == 'u') {
				cout << "/\\	";
			}
			else if (E1Direction == 'd') {
				cout << "\\/	";
			}
			for (int i = 4; i <= 9; i++) {
				MOVE_CURSOR(0, i);
				cout << 10 - i - 1 << "|  |		";
			}
			m1.Signal();
			break;

		case 7:
			m1.Wait();
			for (int i = 0; i < 2; i++) {
				MOVE_CURSOR(0, i);
				cout << 10 - i - 1 << "|  |		";
			}
			MOVE_CURSOR(0, 2);
			cout << 7 << "|E1|	";
			if (E1Direction == 'u') {
				cout << "/\\	";
			}
			else if (E1Direction == 'd') {
				cout << "\\/	";
			}
			for (int i = 3; i <= 9; i++) {
				MOVE_CURSOR(0, i);
				cout << 10 - i - 1 << "|  |		";
			}
			m1.Signal();
			break;

		case 8:
			m1.Wait();
			for (int i = 0; i < 1; i++) {
				MOVE_CURSOR(0, i);
				cout << 10 - i - 1 << "|  |		";
			}
			MOVE_CURSOR(0, 1);
			cout << 8 << "|E1|	";
			if (E1Direction == 'u') {
				cout << "/\\	";
			}
			else if (E1Direction == 'd') {
				cout << "\\/	";
			}
			for (int i = 2; i <= 9; i++) {
				MOVE_CURSOR(0, i);
				cout << 10 - i - 1 << "|  |		";
			}
			m1.Signal();
			break;


		case 9:
			m1.Wait();
			MOVE_CURSOR(0, 0);
			cout << 9 << "|E1|	";
			if (E1Direction == 'u') {
				cout << "/\\	";
			}
			else if (E1Direction == 'd') {
				cout << "\\/	";
			}

			for (int i = 1; i <= 9; i++) {
				MOVE_CURSOR(0, i);
				cout << 10 - i - 1 << "|  |		";
			}
			m1.Signal();
			break;
		}
		
	}
	return 0;
}

UINT  __stdcall  Get_Elevator2_Status(void*)
{
	CMutex m1("mutexIO");
	while (true) {
		//Semaphore Wait
		P4.Wait();
		m1.Wait();
		//Read Datapool
		E2Direction = Datapool2->direction;
		E2Status = Datapool2->general_status;
		E2Door = Datapool2->door_status;
		E2Floor = Datapool2->current_floor;
		E2Session = Datapool2->session;

		MOVE_CURSOR(0, 26);
		cout << " E2 Dir: " << E2Direction << " E2 Sts: " << E2Status << " E2 Door: " << E2Door << " E2 Flr: " << E2Floor << " E2 Ses: " << E2Session << endl;
		m1.Signal();
		//Semaphore Signal
		C4.Signal();
		
		switch (E2Floor) {
		case 0:
			m1.Wait();
			for (int i = 0; i < 9; i++) {
				MOVE_CURSOR(20, i);
				cout << 10 - i - 1 << "|  |";
			}
			MOVE_CURSOR(20, 9);
			cout << 0 << "|E2|";
			m1.Signal();
			break;

		case 1:
			m1.Wait();
			for (int i = 0; i < 8; i++) {
				MOVE_CURSOR(20, i);
				cout << 10 - i - 1 << "|  |";
			}
			MOVE_CURSOR(20, 8);
			cout << 1 << "|E2|";
			MOVE_CURSOR(10, 9);
			cout << 0 << "|  |";
			m1.Signal();
			break;

		case 2:
			m1.Wait();
			for (int i = 0; i < 7; i++) {
				MOVE_CURSOR(20, i);
				cout << 10 - i - 1 << "|  |";
			}
			MOVE_CURSOR(10, 7);
			cout << 2 << "|E2|";
			for (int i = 8; i <= 9; i++) {
				MOVE_CURSOR(20, i);
				cout << 10 - i - 1 << "|  |";
			}
			m1.Signal();
			break;

		case 3:
			m1.Wait();
			for (int i = 0; i < 6; i++) {
				MOVE_CURSOR(20, i);
				cout << 10 - i - 1 << "|  |";
			}
			MOVE_CURSOR(10, 6);
			cout << 3 << "|E2|";
			for (int i = 7; i <= 9; i++) {
				MOVE_CURSOR(20, i);
				cout << 10 - i - 1 << "|  |";
			}
			m1.Signal();
			break;

		case 4:
			m1.Wait();
			for (int i = 0; i < 5; i++) {
				MOVE_CURSOR(20, i);
				cout << 10 - i - 1 << "|  |";
			}
			MOVE_CURSOR(10, 5);
			cout << 4 << "|E2|";
			for (int i = 6; i <= 9; i++) {
				MOVE_CURSOR(20, i);
				cout << 10 - i - 1 << "|  |";
			}
			m1.Signal();
			break;

		case 5:
			m1.Wait();
			for (int i = 0; i < 4; i++) {
				MOVE_CURSOR(20, i);
				cout << 10 - i - 1 << "|  |";
			}
			MOVE_CURSOR(10, 4);
			cout << 5 << "|E2|";
			for (int i = 5; i <= 9; i++) {
				MOVE_CURSOR(20, i);
				cout << 10 - i - 1 << "|  |";
			}
			m1.Signal();
			break;

		case 6:
			m1.Wait();
			for (int i = 0; i < 3; i++) {
				MOVE_CURSOR(20, i);
				cout << 10 - i - 1 << "|  |";
			}
			MOVE_CURSOR(10, 3);
			cout << 6 << "|E2|";
			for (int i = 4; i <= 9; i++) {
				MOVE_CURSOR(20, i);
				cout << 10 - i - 1 << "|  |";
			}
			m1.Signal();
			break;

		case 7:
			m1.Wait();
			for (int i = 0; i < 2; i++) {
				MOVE_CURSOR(20, i);
				cout << 10 - i - 1 << "|  |";
			}
			MOVE_CURSOR(10, 2);
			cout << 7 << "|E2|";
			for (int i = 3; i <= 9; i++) {
				MOVE_CURSOR(20, i);
				cout << 10 - i - 1 << "|  |";
			}
			m1.Signal();
			break;

		case 8:
			m1.Wait();
			for (int i = 0; i < 1; i++) {
				MOVE_CURSOR(20, i);
				cout << 10 - i - 1 << "|  |";
			}
			MOVE_CURSOR(10, 1);
			cout << 8 << "|E2|";
			for (int i = 2; i <= 9; i++) {
				MOVE_CURSOR(20, i);
				cout << 10 - i - 1 << "|  |";
			}
			m1.Signal();
			break;

		case 9:
			m1.Wait();
			MOVE_CURSOR(20, 0);
			cout << 9 << "|E2|";

			for (int i = 1; i <= 9; i++) {
				MOVE_CURSOR(20, i);
				cout << 10 - i - 1 << "|  |";
			}
			m1.Signal();
			break;
		}
	}

	return 0;
}

UINT  __stdcall  GetInputCommands(void *)
{
	CMutex m1("mutex1");
	while (true) {
		m1.Wait();
		MOVE_CURSOR(0, 30);	
		RequestElevator[0] = _getch();
		RequestElevator[1] = _getch();
		TypedPipe.Write(&RequestElevator[0]);
		TypedPipe.Write(&RequestElevator[1]);
		//cout << "Instruction sent to Dispatcher via Typed Pipeline" << endl;
		//RequestElevator = ""; //reset RequestElevator
		m1.Signal();
	}
	return 0;
}


//----MAIN----//
int main()
{
	CRendezvous     r1("MyRendezvous", 4);
	r1.Wait();
//	cout << "IO created.\n";
//	cout << "Welcome to the elevators!" << endl;
	
	//Assume both elevators are at ground level. We will simulate user input commands as if they are starting from the ground levelv
	//Enter commands from outside
	
	CThread t1(Get_Elevator1_Status, ACTIVE, NULL);
	CThread t2(Get_Elevator2_Status, ACTIVE, NULL);
	CThread t3(GetInputCommands, ACTIVE, NULL);

	t1.WaitForThread();
	t2.WaitForThread();
	t3.WaitForThread();

	//When update has been done
	//IO gets elevator status from monitor once it's been signalled to do so
	//IO then shows the elevator status on screen
	//Get_Elevator1_Status();
	//Elevator1.show_status();


	//Mailbox from Dispatcher
	UINT DMessage;
	CMailbox  MailBox3;
	     	     
	do {
		//cout << "IO Process: Polling Mailbox\n";
		Sleep(2000);			      // sleep for 1/2 second

		if (MailBox3.TestForMessage() == TRUE) {	      // is there a message for this thread?
			DMessage = MailBox3.GetMessage();	      // if so, get the message
			if (DMessage == 500)      // if message intended for thread1
				cout << "Dispatcher has terminated...\n";		      // forward message to DispatcherMail
		}
	} while (1);
	return 0;		// exit child program by returning status value 0
				// Note we could also call exit(0) to achieve the same thing
}
