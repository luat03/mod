#pragma once

#include"Planes.h"
#include"Flights.h"
#include"Passengers.h"
#include"FunctionTab.h"
#include"Data.h"
#include"ManageFlightsTab.h"
class ManagePassengersTab :public ManageFlightsTab {
private:

	Data* d = NULL;
	PTR tempFlight = NULL;


public:
	ManagePassengersTab() {
		initSearchEdittext();
	}
	//Khoi tao cac tham so
	ManagePassengersTab(Data* d) {
		this->d = d;
		initSearchEdittext();

	}
	~ManagePassengersTab() {
		printf("Destructor Pass\n");
	}
	void reset() {
		ManageFlightsTab::reset();
		currentMenu = MAIN_MENU;
		currentPage = 1;

	}

	void drawUI() {
		FunctionTab::drawBackground();
		typing = false;
		switch (currentMenu) {
		case MAIN_MENU: {
			drawMainMenu();
			break;
		}

		case SHOW_MENU: {
			drawShowMenu();
			break;
		}
		default:
			break;
		}
	}

	void drawMainMenu() {

		edittext[ID_FLIGHT].onAction(edittextPointer);
		edittext[DAY].onAction(edittextPointer);
		edittext[MONTH].onAction(edittextPointer);
		edittext[YEAR].onAction(edittextPointer);
		edittext[ARRIVE].onAction(edittextPointer);

		//-----------------VE HUONG DAN TEXT
		char a[40] = "*Left click to see passenger list";
		drawInstruction(LEFT_BORDER - 10, BOTTOM_BORDER + 20, a);

		inputMainMenuHandel();

		int s;
		if (checkAllEdittextIsEmpty()) {
			s = drawFlightData(tempFlight, d->flightList, d->planeList);


		}

		else {
			currentPage = 1;
			clearSearchEdittextCursor();
			s = drawFilterData(tempFlight, d->flightList);

		}
		if (s == 1)
			currentMenu = SHOW_MENU;

	}


	void drawShowMenu() {

		int y = SUBWINDOW_TOP + 5;

		char s[100] = "LIST OF PASSENGERS ON FLIGHT ";
		strcat_s(s, tempFlight->info.idFlight);
		int x = (SUBWINDOW_LEFT + SUBWINDOW_RIGHT - textwidth(s)) / 2;

		drawTitle(x, y, s);

		strcpy_s(s, "Date time: ");
		strcat_s(s, getDateString(tempFlight->info.date));
		y += textheight(s) + 10;
		drawTitle(x - 10, y, s);
		x += textwidth(s) + 10;
		strcpy_s(s, "Arrive: ");
		strcat_s(s, tempFlight->info.arrive);

		drawTitle(x, y, s);
		drawPassengerData(tempFlight);

		button[BACK].onAction();
		if (button[BACK].isClicked()) {

			currentMenu = MAIN_MENU;
		}
	}


	//----------------DRAW DATA
	void drawOnePassenger(int cnt, int preY, int i, AVLTree& p) {
		int spaceX = (RIGHT_BORDER + LEFT_BORDER) / 7;
		int preX = LEFT_BORDER;

		//VE STT
		char temp[40];
		sprintf_s(temp, "%d", cnt + 1);
		int x = preX + 100;
		drawText(preX, preY, x, temp);
		preX = x;

		//VE SO VE


		x = preX + spaceX;
		sprintf_s(temp, "%d", i + 1);
		drawText(preX, preY, x, temp);
		preX = x;



		//VE First Name
		x = preX + spaceX;
		drawText(preX, preY, x, p->data.firstName);
		preX = x;


		//VE Last Name
		x = preX + spaceX;
		drawText(preX, preY, x, p->data.lastName);
		preX = x;



		//VE CMND
		x = preX + spaceX;
		drawText(preX, preY, x, p->data.idPass);
		preX = x;


		//VE GIOI TINH
		x = RIGHT_BORDER;
		if (p->data.gender == 0) {
			strcpy_s(temp, "MALE");
		}
		else
			strcpy_s(temp, "FEMALE");

		drawText(preX, preY, x, temp);


	}
	void drawPassengerData(PTR& flightList) {



		int size = 0;
		int cnt = 1;
		int seats[31];
		int pageSize = 0;


		for (int i = 0; i < flightList->info.totalTicket; i++) {
			if (strcmp(flightList->info.ticketList[i], "0") != 0) {

				if (cnt % 10 == 1) {
					seats[pageSize] = i;
					pageSize++;
					cnt = 1;
				}
				cnt++;
				size++;



			}
		}
		
		onButtonPage(size, this->currentPage);
		showPage(currentPage);


		int spaceY = (TOP_BORDER + BOTTOM_BORDER) / 20;
		int preY = TOP_BORDER + 60;

		drawBorder(6, 2, size == 0);

		setbkcolor(SUBWINDOW_BACKGROUND);


		int t = startPage;
		int i = seats[this->currentPage-1 ];
	
		if (i < 0)
			i = flightList->info.totalTicket;

		
		while (t < (startPage + 10) && i < flightList->info.totalTicket) {
			if (strcmp(flightList->info.ticketList[i], "0") != 0) {
				AVLTree p = findPassenger(d->passengerList, flightList->info.ticketList[i]);
				setcolor(BLACK);
				drawOnePassenger(t, preY, i, p);
				preY += spaceY;
				t++;
			}
			i++;
		}


	}



};
