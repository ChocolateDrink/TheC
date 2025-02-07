#include <iostream>
#include <string>
#include <Windows.h>
#include <cstdlib>

void setTextColor(int color) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, color);
}

std::string getColor() {
	using namespace std;

	string red;
	string green;
	string blue;

	cout << "enter red value (0-255): ";
	cin >> red;

	cout << "enter green value (0-255): ";
	cin >> green;

	cout << "enter blue value (0-255): ";
	cin >> blue;

	return red + ' ' + green + ' ' + blue;
}

void editRegistry(const char* key, const char* value) {
	HKEY hKey;
	LONG lResult = RegOpenKeyEx(HKEY_CURRENT_USER, "Control Panel\\Colors", NULL, KEY_SET_VALUE, &hKey);
	if (lResult != ERROR_SUCCESS) {
		setTextColor(0x0C);
		std::cout << "Failed to modify";
		setTextColor(0x0F);
	}

	lResult = RegSetValueEx(hKey, key, NULL, REG_SZ, (const byte*)value, strlen(value) + 1);
	RegCloseKey(hKey);

	if (lResult == ERROR_SUCCESS) {
		setTextColor(0x0A);
		std::cout << "Success!";
		setTextColor(0x0F);
	}
}

void process(std::string choice) {
	using namespace std;

	int parsedChoice;
	try {
		parsedChoice = stoi(choice);
	}
	catch (const invalid_argument) { parsedChoice = 6; }

	system("cls");

	switch (parsedChoice) {
		case 1: {
			string color = getColor();
			cout << "changing highlight color to: " << color << '\n';
			editRegistry("HotTrackingColor", color.c_str());

			break;
		}

		case 2: {
			string color = getColor();
			cout << "changing outline color to: " << color << '\n';
			editRegistry("Hilight", color.c_str());

			break;
		}

		case 3: {
			cout << "changing both colors..." << '\n';
			string color = getColor();

			editRegistry("HotTrackingColor", color.c_str());
			editRegistry("Hilight", color.c_str());

			break;
		}

		case 4:
			cout << "reverting highlight colors..." << '\n';
			editRegistry("HotTrackingColor", "0 102 204");

			break;

		case 5:
			cout << "reverting outline colors..." << '\n';
			editRegistry("Hilight", "0 102 215");

			break;

		case 6:
			cout << "reverting highlight colors..." << '\n';
			editRegistry("HotTrackingColor", "0 102 204");

			cout << "reverting outline colors..." << '\n';
			editRegistry("Hilight", "0 102 215");

			break;

		default:
			setTextColor(0x0C);
			cout << "Invalid choice";
			setTextColor(0x0F);
			break;
	}
}

int main() {
	using namespace std;

	cout << "what do you want to do?";
	cout << "\n\n";

	cout << "[1] change highlight color" << '\n';
	cout << "[2] change outline color (this will affect text highlighting)" << '\n';
	cout << "[3] change both colors" << '\n';
	cout << "\n";

	cout << "[4] revert highlight color" << '\n';
	cout << "[5] revert outline color" << '\n';
	cout << "[6] revert both colors" << '\n';
	cout << "\n";

	string choice;
	cout << "choice: ";
	cin >> choice;

	process(choice);

	cout << "\n";

	string temp;
	cin >> temp;

	return EXIT_SUCCESS;
}