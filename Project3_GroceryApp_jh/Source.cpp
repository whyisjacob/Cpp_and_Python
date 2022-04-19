#include <Python.h>
#include <iostream>
#include <Windows.h>
#include <cmath>
#include <string>
#include <fstream>

using namespace std;

/*
Description:
	To call this function, simply pass the function name in Python that you wish to call.
Example:
	callProcedure("printsomething");
Output:
	Python will print on the screen: Hello from python!
Return:
	None
*/
void CallProcedure(string pName)
{
	char* procname = new char[pName.length() + 1];
	std::strcpy(procname, pName.c_str());

	Py_Initialize();
	PyObject* my_module = PyImport_ImportModule("PythonCode");
	PyErr_Print();
	PyObject* my_function = PyObject_GetAttrString(my_module, procname);
	PyObject* my_result = PyObject_CallObject(my_function, NULL);
	Py_Finalize();

	delete[] procname;
}


/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("PrintMe","Test");
Output:
	Python will print on the screen:
		You sent me: Test
Return:
	100 is returned to the C++
*/
int callIntFunc(string proc, string param)
{
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	char* paramval = new char[param.length() + 1];
	std::strcpy(paramval, param.c_str());


	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(z)", paramval);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;
	delete[] paramval;


	return _PyLong_AsInt(presult);
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("doublevalue",5);
Return:
	25 is returned to the C++
*/
int callIntFunc(string proc, int param)
{
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(i)", param);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;

	return _PyLong_AsInt(presult);
}


int display_text_histogram() {
	ifstream histogram_file;
	string item_name;
	string space = " ";
	int item_qty;
	string stars = "";
	histogram_file.open("../Release/frequency.dat", ios::in);

	//validate temp file
	if (!histogram_file.is_open()) {
		cout << "Could not open the output file" << endl;
		return 1;
	}


	while (!histogram_file.fail()) {
		stars = "";
		//read values from input file
		histogram_file >> item_name;
		histogram_file >> item_qty;

		//cout << "item_name = " << item_name << endl;
		for (int i = 0; i < item_qty; i++) {
			stars += "*";
		}
		//cout << "item_qty = " << item_qty << endl;
		cout << item_name << space << stars<< endl;

	}

	histogram_file.close();


	return 0;
}


void main()
{
	string displayMenu = "yes";
	int menuVal;
	//used for second case
	string specificItem;

	while (displayMenu == "yes")
	{
		specificItem = "";
		cout << "\n1: Determine the number of times each individual item appears \n";
		cout << "2: Determine the frequency of a specific item\n";
		cout << "3: Graphically display a data file as a text-based histogram\n";
		cout << "4: exit \n";
		cin >> menuVal;
		switch (menuVal)
		{
		case 1:

			cout << "Counting how many times each item appears.\n\nPlease wait...\n\n ";
			CallProcedure("count_grocery_items");

			break;


		case 2:
			cout << "\nWhat item are you looking for?: ";
			cin >> specificItem;

			cout << "Got it!\nI'll start looking for how many times " << specificItem << " appears in our list\n";
			cout << "\nTotal num of " << specificItem << " sold is: " << callIntFunc("get_specific_item", specificItem) << endl;
			break;


		case 3:
			cout << "Graphically displaying how many each items appear.\n\nPlease wait...\n";
			CallProcedure("save_sold_items_to_file");
			display_text_histogram();
			break;


		case 4:
			cout << "Closing the dadgum program";
			displayMenu = "no";
			break;


		default:
			cout << "Please enter 1, 2, 3, or 4" << endl;
			break;
		}
	}

}