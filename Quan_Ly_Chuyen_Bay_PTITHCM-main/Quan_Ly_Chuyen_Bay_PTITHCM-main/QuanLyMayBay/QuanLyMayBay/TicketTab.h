#pragma once

#include"Planes.h"
#include"Flights.h"
#include"Passengers.h"
#include"Data.h"
#include"ManageFlightsTab.h"

class TicketTab :public ManageFlightsTab {
private:
	PTR flightTemp;
	Button* buttonTicket = NULL, * closeButton = NULL, * genderButton= NULL, * ticketPointer = NULL;
	EditText* addEdittextPointer = NULL;
	PTR* beginPage;
	Data* d;

	bool save = false;
public:

	~TicketTab() {
		printf("Destructor Ticket\n");

		delete [] buttonTicket;
		delete[] beginPage;
		//delete closeButton; //do thang nay tro thoi vung nho stack nen ko soa duoc
		
	}
	TicketTab(Data* d) {
		this->d = d;

		initEdittext();

		buttonTicket = new Button[MAX_SEAT + 1]; //Khoi tao so ghe
		closeButton = &button[CLOSE];

		initSearchEdittext();
	}

	//-----------------KHOI TAO
	//Khoi tao edittext
	void initEdittext() {

		//---------EDITTEXT ID
		char hint[30] = "Data must be entered!";
		char title[30] = "ID PASS";
		char content[30] = "";

		int left = (SUBWINDOW_LEFT + SUBWINDOW_RIGHT - EDITTEXT_WIDTH + 90) / 2;
		int top = SUBWINDOW_TOP + 100;
		int right = left + EDITTEXT_WIDTH;
		int bottom = top + EDITTEXT_HEIGHT;


		//------------EDITTEXT ID PASS

		strcpy_s(title, "ID PASS");
		edittext[ID_PASS] = EditText(hint, title, content, left, top, right, bottom, MAX_ID_PASS, 130);



		//------------EDITTEXT SURNAME
		strcpy_s(title, "FIRST NAME");
		top = bottom + EDITTEXT_SPACE;
		bottom = top + EDITTEXT_HEIGHT;
		edittext[FIRST_NAME] = EditText(hint, title, content, left, top, right, bottom, MAX_FIRST_NAME, 130);

		//-------------EDITTEXT NAME
		strcpy_s(title, "LAST NAME");
		top = bottom + EDITTEXT_SPACE;
		bottom = top + EDITTEXT_HEIGHT;
		edittext[LAST_NAME] = EditText(hint, title, content, left, top, right, bottom, MAX_LAST_NAME, 130);




		top = bottom + 65;
		strcpy_s(title, "FE");
		button[FEMALE] = Button(left, top, left + 30, top + 30, EDITEXT_BACKGROUND_COLOR, COLOR(214, 131, 191), title, BLACK);


		left += 50;
		strcpy_s(title, "M");
		button[MALE] = Button(left, top, left + 30, top + 30, EDITEXT_BACKGROUND_COLOR, COLOR(135, 206, 222), title, BLACK);





	}
	//Khoi tao menu add
	void initAddMenu(bool s, bool id = false) {
		if (id)
			edittext[ID_PASS].setActive(s);
		else
			edittext[ID_PASS].setActive(true);

		button[FEMALE].setActive(s);
		button[MALE].setActive(s);

		edittext[FIRST_NAME].setActive(s);
		edittext[LAST_NAME].setActive(s);
		edittext[FIRST_NAME].setActive(s);



	}
	//Khoi tao ds ve
	void initTicketList(PTR& flight) {



		int y = TOP_BORDER + 10;
		int x = (LEFT_BORDER + RIGHT_BORDER - 1000) / 2;
		char value[4];
		char a[2] = "";
		for (int i = 1; i <= flight->info.totalTicket; i++) {
			sprintf_s(value, "%d", i);

			buttonTicket[i] = Button(x, y, x + 30, y + 30, TICKET_AVAI, TICKET_NOT_AVAI, value, BLACK);
			if (strcmp(flight->info.ticketList[i - 1], "0") != 0) {
				buttonTicket[i].setChoosen(true);
			}
			else
				buttonTicket[i].setChoosen(false);

			x += 40;
			if (i % 25 == 0) {
				x = (LEFT_BORDER + RIGHT_BORDER - 1000) / 2;
				y += 40;

			}


		}
	}

	
	void customEdittext(AVLTree& pass, bool id = false) {
		if (id) {
			edittext[ID_PASS].customInitChar(pass->data.idPass);
		}
		edittext[FIRST_NAME].customInitChar(pass->data.firstName);
		edittext[LAST_NAME].customInitChar(pass->data.lastName);
		if (!id) {
			if (pass->data.gender == 0)
				genderButton = &button[MALE];
			else genderButton = &button[FEMALE];
		}
		else {
			if (pass->data.gender == 0) {
				button[MALE].setActive(true);
				genderButton = &button[MALE];

			}
			else {
				button[FEMALE].setActive(true);
				genderButton = &button[FEMALE];
			}
		}

	}
	//Lay du lieu passenger
	Passenger getPassenger() {
		Passenger p;

		strcpy_s(p.idPass, edittext[ID_PASS].getCharData());
		strcpy_s(p.firstName, edittext[FIRST_NAME].getCharData());
		strcpy_s(p.lastName, edittext[LAST_NAME].getCharData());

		if (genderButton == &button[FEMALE])
			p.gender = true;
		else if (genderButton == &button[MALE])
			p.gender = false;
		return p;

	}

	//Xoa noi dung edittext
	void clearEditext() {
		edittext[ID_PASS].clearText();
		edittext[LAST_NAME].clearText();
		edittext[FIRST_NAME].clearText();
		addEdittextPointer = NULL;
		genderButton = NULL;
		

	}
	//reset tab ticket
	void reset() {
		ManageFlightsTab::reset();
		ticketPointer = NULL;
		clearEditext();
	}
	void moveEdittextUp() {
		if (addEdittextPointer == &edittext[LAST_NAME]) {
			addEdittextPointer = &edittext[FIRST_NAME];
		}
		else if (addEdittextPointer == &edittext[FIRST_NAME]) {
			addEdittextPointer = &edittext[ID_PASS];

		}
	}
	void moveEdittextDown() {
		if (addEdittextPointer == &edittext[ID_PASS]) {
			AVLTree passenger = findPassenger(d->passengerList, edittext[ID_PASS].getCharData());
			if (passenger != NULL) {
				customEdittext(passenger);

			}
			addEdittextPointer = &edittext[FIRST_NAME];
		}
		else if (addEdittextPointer == &edittext[FIRST_NAME]) {
			addEdittextPointer = &edittext[LAST_NAME];
		}

	}

	//Kiem tra loi tung edittext
	bool checkEdittextError() {
		addEdittextPointer->clearCursor();

		if (addEdittextPointer == &edittext[ID_PASS]) {
			if (addEdittextPointer->isEmpty()) {
				drawAnounce(EMPTY);
				return false;
			}
			if (!checkIDPassenger(edittext[ID_PASS].getCharData())) {
				drawAnounce(ID_PASS_ERROR);
				return false;
			}
			int s = checkDupIDOnFlight(flightTemp, edittext[ID_PASS].getCharData());
			if (s != -1) {
				char mess[200] = "Duplicate ID in seat number ";
				char a[5];
				sprintf_s(a, "%d", s);
				strcat_s(mess, a);
				MessageBox(GetForegroundWindow(), (LPCWSTR)convertCharArrayToLPCWSTR(mess), (LPCWSTR)convertCharArrayToLPCWSTR("WARNING"), MB_ICONWARNING | MB_OK);
				edittext[ID_PASS].clearText();
				return false;
			}

			PTR  k = checkPassOnOtherFlightIn12Hours(d->flightList, flightTemp, edittext[ID_PASS].getCharData());
			if (k != NULL) {
				char mess[300] = "Can't not book ticket\n";

				strcat_s(mess, "This passenger has booked ticket on flight ");
				strcat_s(mess, k->info.idFlight);
				strcat_s(mess, "\n Date:");
				strcat_s(mess, getDateString(k->info.date));
				strcat_s(mess, "\n Time >= 12 hours from that flight");
				MessageBox(GetForegroundWindow(), (LPCWSTR)convertCharArrayToLPCWSTR(mess), (LPCWSTR)convertCharArrayToLPCWSTR("WARNING"), MB_ICONWARNING | MB_OK);
				edittext[ID_PASS].clearText();
				return false;
			}
			initAddMenu(true);
		}



		if (addEdittextPointer == &edittext[FIRST_NAME]) {
			if (addEdittextPointer->isEmpty()) {
				drawAnounce(EMPTY);
				return false;
			}
		}

		edittext[FIRST_NAME].standarContent();

		if (addEdittextPointer == &edittext[LAST_NAME]) {
			if (addEdittextPointer->isEmpty()) {
				drawAnounce(EMPTY);
				return false;
			}

		}
		edittext[LAST_NAME].standarContent();
		return true;


	}
	//Kiem tra torng quan de luu toan bo du lieu
	bool checkSaveData() {

		edittext[ID_PASS].clearCursor();
		edittext[FIRST_NAME].clearCursor();
		edittext[LAST_NAME].clearCursor();


		if (edittext[ID_PASS].isEmpty()) {
			drawAnounce(EMPTY);
			addEdittextPointer = &edittext[ID_PASS];
			return false;
		}

		if (!checkIDPassenger(edittext[ID_PASS].getCharData())) {
			drawAnounce(ID_PASS_ERROR);
			addEdittextPointer = &edittext[ID_PASS];
			return false;
		}

		int s = checkDupIDOnFlight(flightTemp, edittext[ID_PASS].getCharData());
		if (s != -1) {
			char mess[200] = "Duplicate ID in seat number ";
			char a[3];
			sprintf_s(a, "%d", s);
			strcat_s(mess, a);
			MessageBox(GetForegroundWindow(), (LPCWSTR)convertCharArrayToLPCWSTR(mess), (LPCWSTR)convertCharArrayToLPCWSTR("WARNING"), MB_ICONWARNING | MB_OK);
			addEdittextPointer = &edittext[ID_PASS];
			return false;
		}

		PTR k = checkPassOnOtherFlightIn12Hours(d->flightList, flightTemp, edittext[ID_PASS].getCharData());
		if (k != NULL) {
			char mess[300] = "Can't not book ticket\n";

			strcat_s(mess, "This passenger has booked ticket on flight ");
			strcat_s(mess, k->info.idFlight);
			strcat_s(mess, "\n Date:");
			strcat_s(mess, getDateString(k->info.date));
			strcat_s(mess, "\n Time >= 12 hours from that flight");
			MessageBox(GetForegroundWindow(), (LPCWSTR)convertCharArrayToLPCWSTR(mess), (LPCWSTR)convertCharArrayToLPCWSTR("WARNING"), MB_ICONWARNING | MB_OK);
			addEdittextPointer = &edittext[ID_PASS];
			return false;
		}


		initAddMenu(true);


		if (edittext[FIRST_NAME].isEmpty()) {
			drawAnounce(EMPTY);
			addEdittextPointer = &edittext[FIRST_NAME];
			return false;
		}

		edittext[FIRST_NAME].standarContent();


		if (edittext[LAST_NAME].isEmpty()) {
			drawAnounce(EMPTY);
			addEdittextPointer = &edittext[LAST_NAME];
			return false;
		}
		edittext[LAST_NAME].standarContent();

		if (genderButton == NULL) {
			drawAnounce(GENDER_ERROR);
			return false;
		}


		return true;
	}

	//Xu li su kien ban phim
	void inputHandel() {
		int c = FunctionTab::getInput();

		if (addEdittextPointer != NULL) {
			switch (c) {
			case -1: {
				addEdittextPointer->deleteChar();
				break;
			}
			case 1: {
				if (!checkEdittextError()) {

				}
				else
					moveEdittextUp();


				break;
			}
			case 3: case 2: {

				if (!checkEdittextError()) {

				}
				else
					moveEdittextDown();
				break;
			}
			default: {


				if (addEdittextPointer == &edittext[ID_PASS]) {
					if (c <= 57 && c >= 48)
						addEdittextPointer->addChar((char)c);

				}
				else if (addEdittextPointer == &edittext[FIRST_NAME] || addEdittextPointer == &edittext[LAST_NAME]) {
					if (c <= 90 && c >= 65 || c == ' ' || c <= 122 && c >= 97)
						addEdittextPointer->addCharName((char)c);

				}




				break;


			}
			}
		}

	}

	//-------------------------UI--------------------------
	void drawUI() {
		typing = false;
		FunctionTab::drawBackground();
		switch (currentMenu) {
		case MAIN_MENU: {
			drawMainMenu();
			break;
		}
		case TICKET_MENU: {
			drawTicketMenu();
			break;
		}

		case ADD_MENU: {
			drawAddMenu();
			break;
		}
		case CANCLE_TICKET_MENU: {
			drawCancelTicketMenu();
		}

		default:
			break;

		}
	}
	void drawMainMenu() {
		typing = false;
		edittext[ID_FLIGHT].onAction(edittextPointer);
		edittext[DAY].onAction(edittextPointer);
		edittext[MONTH].onAction(edittextPointer);
		edittext[YEAR].onAction(edittextPointer);
		edittext[ARRIVE].onAction(edittextPointer);

		inputMainMenuHandel();
		//-----------------VE HUONG DAN TEXT
		char a[40] = "*Left click to book/cancle ticket";
		drawInstruction(LEFT_BORDER - 10, BOTTOM_BORDER + 20, a);

		int s;
		if (checkAllEdittextIsEmpty()) {
			s = drawTicketData(flightTemp, this->d->flightList);

		}
		else {
			clearSearchEdittextCursor();
			s = drawFilterData(flightTemp, this->d->flightList, true);

		}

		if (s == 1 && flightTemp != NULL) {
			Date d = flightTemp->info.date;

			//Khong cho dat ve 30 phut cuoi
			if (!checkTimeBeforeMinute(d, 30)) {
				drawAnounce(THIRTY_MINUTE);
				return;
			}
			initTicketList(flightTemp);
			delay(500);
			currentMenu = TICKET_MENU;
		}

	}
	void drawCancelTicketMenu() {

		typing = true;

		char s[50] = "CHECK INFO PASSENGER";
		drawTitle(s);

		button[BACK].onAction();
		button[CANCLE].onAction();
		button[FEMALE].onAction(genderButton);
		button[MALE].onAction(genderButton);

		drawGenderText();

		edittext[ID_PASS].onAction(addEdittextPointer);
		edittext[FIRST_NAME].onAction(addEdittextPointer);
		edittext[LAST_NAME].onAction(addEdittextPointer);




		if (button[BACK].isClicked()) {
			clearEditext();
			ticketPointer = NULL;
			int s = drawAnounce(PREVIOUS);
			if(s == IDOK){
				reset();
				currentMenu = TICKET_MENU;
			}
			delay(50);

			
			return;
		}
		if (button[CANCLE].isClicked()) {
			//Khong cho chinh sua neu  hoan tat
			if (flightTemp->info.status == 3) {
				currentMenu = MAIN_MENU;
				drawAnounce(ADJUST_ERROR);
				return;
			}
			//Neu luc save da lo qua 30 phut tu dong out ma ko cho luu
			if (!checkTimeBeforeMinute(flightTemp->info.date, 30)) {
				currentMenu = MAIN_MENU;
				drawAnounce(THIRTY_MINUTE);
				return;

			}
			int s = drawAnounce(CANCEL_CONFIRM);
			if (s == IDOK) {
				ticketPointer->setChoosen(false);
				ticketPointer->drawSeatUI();
				char a[MAX_ID_PASS + 1] = "0";
				bookTicket(flightTemp, ticketPointer->getIntData() - 1, a);

			}
			clearEditext();
			ticketPointer = NULL;
			delay(50);
			currentMenu = TICKET_MENU;
			return;
		}

	}
	void drawTicketMenu() {


		//---------HUONG DAN

		char s[200] = "*Left click to book ticket";
		drawInstruction(SUBWINDOW_LEFT + 10, BOTTOM_BORDER + 75, s);
		strcpy_s(s, " Right click to cancle ticket");
		drawInstruction(SUBWINDOW_LEFT + 10, BOTTOM_BORDER + 95, s);

		drawInstructionInline();


		//---------TITLE
		int y = SUBWINDOW_TOP + 5;

		strcpy_s(s, "TICKET LIST ON FLIGHT ");
		strcat_s(s, flightTemp->info.idFlight);
		int x = (SUBWINDOW_LEFT + SUBWINDOW_RIGHT - textwidth(s)) / 2 - 20;

		drawTitle(x, y, s);

		strcpy_s(s, "Date time: ");
		strcat_s(s, getDateString(flightTemp->info.date));
		y += textheight(s) + 10;
		x -= 30;
		drawTitle(x, y, s);
		x += textwidth(s) + 10;
		strcpy_s(s, "Arrive: ");
		strcat_s(s, flightTemp->info.arrive);

		drawTitle(x, y, s);

		//Khong cho chinh sua neu  hoan tat
		if (flightTemp->info.status == 3) {
			drawAnounce(ADJUST_ERROR);
			currentMenu = MAIN_MENU;
			return;
		}
		//Neu luc save da lo qua 30 phut tu dong out ma ko cho luu
		if (!checkTimeBeforeMinute(flightTemp->info.date, 30)) {
			currentMenu = MAIN_MENU;
			drawAnounce(THIRTY_MINUTE);
			return;

		}


		for (int i = 1; i <= flightTemp->info.totalTicket; i++) {
			buttonTicket[i].onActionSeatButton(ticketPointer);
		}


		if (ticketPointer != NULL) {

			if (!ticketPointer->getIsChoosen() && ticketPointer->getIsLeftClick()) {
				addEdittextPointer = &edittext[ID_PASS];
				initAddMenu(false);
				delay(500);
				currentMenu = ADD_MENU;

			}
			else
				if (ticketPointer->getIsChoosen() && ticketPointer->getIsRightClick()) {
					AVLTree passenger = findPassenger(d->passengerList, flightTemp->info.ticketList[ticketPointer->getIntData() - 1]);
					initAddMenu(false, true);
					customEdittext(passenger, true);
					delay(500);
					currentMenu = CANCLE_TICKET_MENU;


				}
		}


		button[BACK].onAction();

		if (button[BACK].isClicked()) {
			int s = drawAnounce(PREVIOUS);
			if (s == IDOK) {
				reset();
				ticketPointer = NULL;
				delay(50);
				currentMenu = MAIN_MENU;
			}
			
			return;
		}




	}
	void drawGenderText() {
		setbkcolor(SUBWINDOW_BACKGROUND);
		setcolor(EDITTEXT_TITLE_COLOR);
		int left = (SUBWINDOW_LEFT + SUBWINDOW_RIGHT - EDITTEXT_WIDTH + 90) / 2 - 130;
		int top = SUBWINDOW_TOP + 450;
		char a[30] = "GENDER";
		drawText(left, top, left + textwidth(a), a);
	}
	void drawAddMenu() {
		typing = true;
		char s[50] = "ADD PASSENGER";
		drawTitle(s);

		button[BACK].onAction();
		button[SAVE].onAction();
		button[FEMALE].onAction(genderButton);
		button[MALE].onAction(genderButton);

		drawGenderText();

		edittext[ID_PASS].onAction(addEdittextPointer);
		edittext[FIRST_NAME].onAction(addEdittextPointer);
		edittext[LAST_NAME].onAction(addEdittextPointer);


		inputHandel();

		if (button[BACK].isClicked()) {
			ticketPointer = NULL;
			clearEditext();
			drawAnounce(PREVIOUS);
			delay(50);
			currentMenu = TICKET_MENU;

		}
		if (button[SAVE].isClicked()) {
			//Khong cho chinh sua neu  hoan tat
			if (flightTemp->info.status == 3) {
				drawAnounce(ADJUST_ERROR);
				return;
			}
			//Neu luc save da lo qua 30 phut tu dong out ma ko cho luu
			if (!checkTimeBeforeMinute(flightTemp->info.date, 30)) {
				currentMenu = MAIN_MENU;
				drawAnounce(THIRTY_MINUTE);
				return;

			}
			if (!checkSaveData()) {

				if (!edittext[ID_PASS].isEmpty() && checkDupIDOnFlight(flightTemp, edittext[ID_PASS].getCharData()) == -1 &&
					checkPassOnOtherFlightIn12Hours(d->flightList, flightTemp, edittext[ID_PASS].getCharData()) == NULL) {
					AVLTree a = findPassenger(d->passengerList, edittext[ID_PASS].getCharData());
					if (a != NULL) {
						customEdittext(a);

					}
				}

			}
			else {

				Passenger p = getPassenger();
				//Neu da co 
				AVLTree a = findPassenger(d->passengerList, p.idPass);

				if (a != NULL) {
					adjustPassenger(p, a);

				}
				else
					d->passengerList = addPassenger(d->passengerList, p);


				ticketPointer->setChoosen(true);
				drawAnounce(SUCCESS);
				clearEditext();
				bookTicket(flightTemp, ticketPointer->getIntData() - 1, p.idPass);
				currentMenu = TICKET_MENU;
				delay(50);
				ticketPointer = NULL;

			}
		}

	}
	//VE HUONG DAN
	void drawInstructionInline() {

		char s[30];
		char a[2] = "";
		int left = SUBWINDOW_RIGHT - 30;
		int top = SUBWINDOW_BOTTOM - 60;

		strcpy_s(s, "Avai");
		drawSmallBox(left, top, 20, 20, TICKET_AVAI, s);


		top += 30;
		strcpy_s(s, "Not avai");
		drawSmallBox(left, top, 20, 20, TICKET_NOT_AVAI, s);


	}
	void drawSmallBox(int x, int y, int width, int height, int color, char title[20]) {

		setbkcolor(SUBWINDOW_BACKGROUND);
		setcolor(BLACK);
		outtextxy(x - textwidth(title) - 10, (2 * y + height - textheight(title)) / 2, title);


		setcolor(color);

		setfillstyle(1, color);
		bar(x, y, x + width, y + height);


	}

	
	//---------------DATA---------------
	int drawTicketData(PTR& tempFlight, PTR& flightList) {

		int spaceY = (TOP_BORDER + BOTTOM_BORDER) / 20;
		int preY = TOP_BORDER + 60;

		int beginPageSize = 0;

		clearSearchEdittextCursor();
		Date date = getDateFromSearch();
		beginPage = new PTR[size(flightList)];
		beginPage[currentPage] = NULL;
		int size = 0;
		int cnt = 1;
		int t = 1;

		for (PTR k = flightList; k != NULL; k = k->next) {
			if (k->info.status == 1 || k->info.status == 2)
			{

				if (cnt % 10 == 1) {
					beginPageSize++;

					beginPage[beginPageSize] = new FlightNode;
					beginPage[beginPageSize] = k;

					cnt = 1;
				}
				cnt++;
				size++;

			}
		}

	
	

		drawBorder(7, 1, size == 0); //Draw border va title

		onButtonPage(size, currentPage); //Su kien nut left / right
		showPage(currentPage); //Hien thi trang


		PTR k = beginPage[currentPage];

		size = 0;
		int i = startPage;

		while (k != NULL && (i < startPage + 10)) {
			tempFlight = NULL;
			if (k->info.status == 1 || k->info.status == 2)
			{
				tempFlight = k;
				int preX = LEFT_BORDER;
				if (k->info.status == 1 || k->info.status == 2)
					checkFull(k);
				setcolor(BLACK);


				if (isPointed(LEFT_BORDER, preY, RIGHT_BORDER, preY + spaceY)) {
					setcolor(ON_SELECTED_DATA_COLOR);
				}


				drawOneFlight(preY, i, k);


				if (isLeftMouseClicked(LEFT_BORDER, preY, RIGHT_BORDER, preY + spaceY)) {

					return 1;

				}

				else if (isRightMouseClicked(LEFT_BORDER, preY, RIGHT_BORDER, preY + spaceY))
				{

					return 2;

				}
				preY += spaceY;
				i++;

			}
			k = k->next;
		}

		return -1;


	}
};

