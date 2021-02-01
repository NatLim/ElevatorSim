//Author: Aaron Cheong, 17990152
//Author: Natkamol Limapichat 49025166
#include 	"..\..\rt.h"				// change pathlist to this header file according to where it is stored
#include 	<stdio.h>	
#include "..\Monitor.h"

//============DATAPOOL DATA===================//

	int max_floor = 0, next_max = 0;
	int Eregister[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int new_request=0;

	int state, nextstate; //0=buffer(in session), 1=going up, 2=going down, 3 = idle, 8=open door, 9=close door
	int fault=0;
	int Message;

//===============MAIN FUNCTION====================//

UINT   __stdcall statemachine(void* args) {
	
	ElevatorMonitor E1("Elevator1");
	E1.Update_Status();
	state = 3;

	while (true) {

		if (E1.get_direction() == 'u' && (new_request > max_floor) && (new_request != 0)) {
			cout << "first" << endl;
			max_floor = new_request;
			new_request = 0;
		}
		else if (E1.get_direction() == 'u' && (new_request < max_floor) && (new_request != 0)) {
			cout << "second" << endl;
			next_max = new_request;
			new_request = 0;
		}
		else if (E1.get_direction() == 'd' && (new_request < max_floor) && (new_request != 0)) {
			cout << "third" << endl;
			max_floor = new_request;
			new_request = 0;
		}
		else if (E1.get_direction() == 'd' && (new_request > max_floor) && (new_request != 0)) {
			cout << "fourth" << endl;
			next_max = new_request;
			new_request = 0;
		}
		else if (E1.get_direction() == 'i' && (new_request != 0)) {
			cout << "fifth" << endl;
			max_floor = new_request;
			new_request = 0;
		}
		/*	else if (E1.get_direction() == 'u' && (new_request < next_max)) {             //case for ppl pushing down button on up elevator
				next_max = new_request;
			}
			else if (E1.get_direction() == 'd' && (new_request < next_max)) {
				next_max = new_request;
			}
		*/
		switch (state) {
		case 0:
			cout << "state 0 reached" << endl;
			E1.close_door();
			E1.session = 1;
			E1.Update_Status();
			if (((E1.currentfloor()+1) < max_floor) && (max_floor != 0)) {
				nextstate = 1;
				cout << "going to state1" << nextstate << endl;
			}
			else if (((E1.currentfloor() +1) > max_floor) && (max_floor != 0)) {
				nextstate = 2;
				cout << "going to state2" << nextstate << endl;
			}
			else if ((E1.currentfloor() + 1) == max_floor) {
				if (next_max != 0) {
					max_floor = next_max;
					next_max = 0;
					if (max_floor < (E1.currentfloor() + 1)) {
						nextstate = 2;
						cout << "going to state3" << nextstate << endl;
					}
					else if (max_floor > (E1.currentfloor() + 1)) {
						nextstate = 1;
						cout << "going to state4" << nextstate << endl;
					}
				}
				else {
					max_floor = 0;
					nextstate = 3;	//idle
					cout << "going to state5" << nextstate << endl;
				}
			}
			cout << max_floor << endl;
			SLEEP(1000);
			break;
		case 1:
			cout << "state 1 reached" << endl;
			//E1.print();
			cout << max_floor << endl;
			for (int x = 0; x < 10; x++) {
				cout << Eregister[x];
			}
			cout << endl;
			E1.go_up();
			E1.session = 1;
			
			if ((E1.currentfloor() + 1) == max_floor || Eregister[E1.currentfloor()] == 1) {
				nextstate = 8;			//open door
				cout << "going to state" << nextstate << endl;
			}
			else if ((E1.currentfloor() + 1) < max_floor) {
				nextstate = 1;
				cout << "going to state" << nextstate << endl;
			}
			E1.Update_Status();
			SLEEP(1000);
			break;
		case 2:
			cout << "state 2 reached" << endl;
			E1.go_down();
			E1.session = 1;

			if ((E1.currentfloor() + 1) == max_floor || Eregister[E1.currentfloor()] == 1) {
				nextstate = 8;			//open door
				cout << "going to state" << nextstate << endl;
			}
			else if (((E1.currentfloor() + 1)) > max_floor) {
				nextstate = 2;
				cout << "going to state" << nextstate << endl;
			}

			E1.Update_Status();
			SLEEP(1000);
			break;
		case 3:
			E1.go_idle();
			//cout << "state 3 reached" << endl;
			if (Eregister[E1.currentfloor()] == 1) {		//idle
				nextstate = 8;
			//	cout << "going to state1" << nextstate << endl;
			}
			else if (((E1.currentfloor() + 1) < max_floor) && (max_floor != 0)) {
				nextstate = 1;
			//	cout << "going to state2" << nextstate << endl;
			}
			else if (((E1.currentfloor() + 1) > max_floor) && (max_floor != 0)) {
				nextstate = 2;
			//	cout << "going to state3" << nextstate << endl;
			}
			else {
				nextstate = 3;
			//	cout << "going to state4" << nextstate << endl;
			}
			E1.Update_Status();
			//cout << "going to state5" << nextstate << endl;
			SLEEP(1000);
			break;
		case 7:										//fault state
			if (fault == 1) {
				cout << "fault" << endl;
				E1.fault();
				nextstate = 7;
			}
			else if (fault == 0) {
				E1.unfault();
				nextstate = 3;
			}
			E1.Update_Status();
			break;
		case 8:
			cout << "state 8 reached" << endl;
			E1.open_door();
			E1.session = 1;
			Eregister[E1.currentfloor()] = 0;
			E1.Eregister[E1.currentfloor()] = 0;
			E1.Update_Status();
			nextstate = 0;
			cout << "going to state" << nextstate << endl;
			SLEEP(1000);
			break;
		}
		state = nextstate;
		cout << state;
	}
}

int main() {

	CThread t1(statemachine, ACTIVE, NULL);

	//UINT	Message = 0;
	CMailbox MyMailBox;

	CRendezvous     r1("MyRendezvous", 4);
	r1.Wait();
	CTypedPipe <int> Sending("SpecialPipe", 1024);

	
	do {
//		if (MyMailBox.TestForMessage() == TRUE) {
//			Message = MyMailBox.GetMessage();
		
		Sending.Read(&Message);

			if (Message == 1) {
				Eregister[0] = 1;
				new_request = 1;
				cout << Message << " received " << endl;
			}
			else if (Message == 2) {
				Eregister[1] = 1;
				new_request = 2;
				cout << Message << " received " << endl;
			}
			else if (Message == 3) {
				Eregister[2] = 1;
				new_request = 3;
				cout << Message << " received " << endl;
			}
			else if (Message == 4) {
				Eregister[3] = 1;
				new_request = 4;
				cout << Message << " received " << endl;
			}
			else if (Message == 5) {
				Eregister[4] = 1;
				new_request = 5;
				cout << Message << " received " << endl;
			}
			else if (Message == 6) {
				Eregister[5] = 1;
				new_request = 6;
				cout << Message << " received " << endl;
			}
			else if (Message == 7) {
				Eregister[6] = 1;
				new_request = 7;
				cout << Message << " received " << endl;
			}
			else if (Message == 8) {
				Eregister[7] = 1;
				new_request = 8;
				cout << Message << " received " << endl;
			}
			else if (Message == 9) {
				Eregister[8] = 1;
				new_request = 9;
				cout << Message << " received " << endl;
			}
			else if (Message == 10) {
				Eregister[9] = 1;
				new_request = 10;
				cout << Message << " received " << endl;
			}
			else if (Message == 98) {
				max_floor = 0;
				next_max = 0;
				fault = 1;
				state = 7;
				nextstate = 7;
				for (int f = 0; f < 10; f++) {
					Eregister[f] = 0;
				}
				new_request = 0;
				cout << Message << " received " << endl;
			}
			else if (Message == 99) {
				fault = 0;
				state = 7;
				cout << Message << " received " << endl;
			}

//		}

	} while (true);
	

	return 0;
}
