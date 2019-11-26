//************************************************************************
//
//   Project (Final)
//   Alexandra Le Neve (300056146) 
//   Syed Haider Rizvi (6842303)
//   Judah Olotu (8448799)
//   CSI2372 Section A - Fall 2019
//
//   Mohamed TALEB
//
// This program reads information about clients and transactions in the following 2 files:
//          - clients.txt
//          - transact.txt
//
// It also allows certain operations on the read data:
//          - Display client bank accounts
//          - Updating client accounts
//
//************************************************************************
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <iomanip>
#include <locale>
#include <vector>
#include <algorithm>

#include "BankABC.h"

using namespace std;

double DepositAccount::rate = 6.50;
double totalAccounts = 0.0;

//******************************************************************
// Basic functions of the BankAccount class
//******************************************************************
inline BankAccount::BankAccount()
{
	accountId = type = updateDate = 0;
	clientName = new char[1];
	balance = 0.0;
}

inline BankAccount::BankAccount(long id, int newType,
	char * name, long newDate, double newBalance) :
	accountId(id), type(newType),
	updateDate(newDate), balance(newBalance)
{
	clientName = new char[strlen(name) + 1];
	strcpy(clientName, name);
}

inline BankAccount::~BankAccount()
{
	if (clientName)
	{
		delete[] clientName;
	}
}

inline void BankAccount::setAccountId(long account)
{
	accountId = account;
}

inline void BankAccount::setType(int newType)
{
	type = newType;
}

inline void BankAccount::setClientName(char * name)
{
	if (clientName)
	{
		delete[] clientName;
	}
	clientName = new char[strlen(name) + 1];
	strcpy(clientName, name);
}

inline void BankAccount::setUpdateDate(long newDate)
{
	updateDate = newDate;
}

inline void BankAccount::setBalance(double newBalance)
{
	balance = newBalance;
}

void BankAccount::print()
{
	cout.setf(ios::fixed);
	cout.precision(2);
	cout << accountId << "\t\t\t" << type << "\t" << updateDate << "\t\t" << balance;
}

//******************************************************************
// Basic functions of the DepositAccount class
//******************************************************************
inline DepositAccount::DepositAccount(long id, int newType, char * name,
	long newDate, double balanceArg, int nbyear) :
	BankAccount(id, newType, name,
		newDate, balanceArg), nbyears(nbyear)
{}

inline void DepositAccount::setNbYears(int nbyear)
{
	nbyears = nbyear;
}

void DepositAccount::print()
{
	Bonus();
	BankAccount::print();
	cout.setf(ios::fixed);
	cout.precision(2);
	cout << "\t" << nbyears << "\t\t" << rate << endl;
}

//******************************************************************
// Functions of the class DepositAccount. It allows to calculate
// the annual bonus on deposit accounts.
// Inputs: Nothing
// Outputs: Nothing
//******************************************************************
void DepositAccount::Bonus()
{
	setBalance(getBalance() + (getBalance()* getRate() * (getNbYears()) / 36000.00));
}



//******************************************************************
// Basic functions of the LoanAccount class
//******************************************************************
inline LoanAccount::LoanAccount(long id, int newType, char * name,
	long newDate, double newBalance, int nbyear,
	double newRate) : BankAccount(id, newType,
		name, newDate, newBalance), nbyears(nbyear), rate(newRate)
{ }

inline void LoanAccount::setNbYears(int nbyear)
{
	nbyears = nbyear;
}

inline void LoanAccount::setRate(double newRate)
{
	rate = newRate;
}

void LoanAccount::print()
{
	BankAccount::print();
	cout.setf(ios::fixed);
	cout.precision(2);
	cout << "\t" << nbyears << "\t\t" << rate << endl;
}

//******************************************************************
// Basic functions of the Transaction class
//******************************************************************
inline Transaction::Transaction(long idTr, int typeTr, long dateTr,
	int codeTr = 01, double amountTr) :
	accountId(idTr), type(typeTr),
	date(dateTr), code(codeTr),
	amount(amountTr)
{ }

inline void Transaction::setAccountId(long compteTr)
{
	accountId = compteTr;
}

inline void Transaction::setType(int typeTr)
{
	type = typeTr;
}

inline void Transaction::setDate(long dateTr)
{
	date = dateTr;
}

inline void Transaction::setCode(int codeTr)
{
	code = codeTr;
}

inline void Transaction::setAmount(double amountTr)
{
	amount = amountTr;
}




//****************************************************************************
// Purpose: Sort a list of bank accounts in ascending order of ids and types.
//
// WARNING: This sort leaves the last account (of id 0) of the list which has 
// its position to assure the tests of end of list later !!!
//
// Inputs: listAccount(BankAccount *), a list of bank accounts.
// Outputs: listAccount(BankAccount *), sorted list of bank accounts.
//****************************************************************************
void sortAccounts(BankAccount ** list)
{
	BankAccount **ptr;
	BankAccount **min;

	int size = 0;
	while ((*list)->getAccountId() != 0){
		size++;
		list++;
	}
	// //bring iterator back
	for (int i = 0; i <size; i++){
		list--;
	}

	for (int i=0;i<size-1;i++){
		min = list;
		ptr = list;
		for (int j=i;j<size;j++){
			if ((**ptr).getAccountId() < (**min).getAccountId()){
				min = ptr;
			}
			else if ((**ptr).getAccountId() == (**min).getAccountId()){
				if ((**ptr).getType() < (**min).getType()){
					min = ptr;
				}
			}
			ptr++;
		}
		//swapping
		BankAccount *tmp = *list;
		*list = *min;
		*min = tmp;

		list++;
	}
}

//******************************************************************
// Purpose: This function reads the file 'clients.txt' and builds 
// an array containing the different bank accounts of customers.
//
// Inputs: Nothing
// Output: listAccount(type: BankAccount *), the list of bank 
//         accounts read from the file 'clients.txt'.
//******************************************************************
BankAccount ** readAccounts()
{
	ifstream inputFile("clients.txt");	// Opening the input file
	if (!inputFile)            		// If the file is not found...
	{
		cout << "File not found !!!" << endl;
		exit(0);
	}

	BankAccount ** listAccounts = new BankAccount*[K_SizeMax];
	if (!listAccounts) {
		cout << "Full memory. !!!" << endl;
		return listAccounts;
	}

	BankAccount ** pAccount = listAccounts;

	long accountRead, dateRead;
	int TypeRead, nbyearRead, counter = 0;
	double balanceRead, RateRead;
	char nameRead[60];

	inputFile >> accountRead >> TypeRead >> dateRead >> balanceRead >> nbyearRead >> RateRead;
	inputFile.getline(nameRead, 60);

	while (inputFile && (counter < K_SizeMax - 1)) {
		if (TypeRead == 03)
			*pAccount = new DepositAccount(accountRead, TypeRead, nameRead, dateRead, balanceRead, nbyearRead);
		else if (TypeRead == 04)
			*pAccount = new LoanAccount(accountRead, TypeRead, nameRead, dateRead, balanceRead, nbyearRead, RateRead);
		else
			*pAccount = new BankAccount(accountRead, TypeRead, nameRead, dateRead, balanceRead);
		inputFile >> accountRead >> TypeRead >> dateRead >> balanceRead >> nbyearRead >> RateRead;
		inputFile.getline(nameRead, 60);
		pAccount++;
		counter++;
	}
	*pAccount = new BankAccount();
	return listAccounts;
}





//*****************************************************************************************
// Purpose: This function validates whether the transaction code 
//          corresponds to the correct account:
//              - 01 ==> account (01: Check, 02: Savings, 03: Deposit and 04: Loan)
//              - 02 ==> account (01: Check, 02: Savings)
//              - 03 ==> account (01: Check).
//
// Inputs: trans (Type: Transaction) an instance of the Transaction class.
// Outputs: true (Type bool) if the transaction matches one of the accounts listed above.
// false (Type bool) otherwise.
//*****************************************************************************************
Bool BankAccount::validateTransaction(const Transaction trans) const
{
	if (((trans.getCode() == 02) && (isDepositAccount() || isLoanAccount())) ||
		((trans.getCode() == 03) && (isDepositAccount() || isLoanAccount() || isSavingsAccount())))
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}

}





//******************************************************************************
// Purpose: This function is used to execute the transaction already performed 
// (update the balance of an account).
//
// Inputs: trans (Transaction Type), instance of Transaction class
// Outputs: Nothing
//*******************************************************************************
void BankAccount::executeTransaction(const Transaction trans)
{
	if (validateTransaction(trans))
	{
		if (trans.getCode() == 01)    // Deposit
		{
			setBalance(getBalance() + trans.getAmount());
		}
		else
		{
			if (trans.getCode() == 02)    // Withdrawal
			{
				if (getBalance() >= trans.getAmount())
				{
					setBalance(getBalance() - (trans.getAmount() + 0.50));
				}
				else { cout << " insufficient balance!! " << endl; }
			}
			else 			// Check
			{
				if (getBalance() >= trans.getAmount())
				{
					setBalance(getBalance() - trans.getAmount());
				}
				else { cout << " insufficient balance!! " << endl; }
			}
		}

	}

}

//***********************************************************************
// Purpose: This function is used to execute a read transaction
// (updating the balance of the loan account).
//
// Inputs: trans (Transaction Type), instance of the Transaction class
// Outputs: Nothing
//***********************************************************************
void LoanAccount::executeTransaction(const Transaction trans)
{
	if (validateTransaction(trans))
	{
		if (trans.getCode() == 01)    // Deposit
		{
			setBalance(getBalance() - trans.getAmount());

		}
	}
}





//*************************************************************************
// Purpose: This function allows to read the file 'transact.txt' 
//          and to update the accounts concerned by the transactions read.
//
// Inputs: listAccount (type BankAccount *), the list of bank accounts.
// Output: Nothing.
//*************************************************************************
void updateAccounts(BankAccount ** listAccounts) {
	ifstream inputFile("transact.txt");	// Opening the input file
	if (!inputFile){
		cout << "File not found !!!" << endl;
		exit(0);
	}

	long accountRead, trDateRead;
	int typeRead, trCode, counter = 0;
	double trAmount;

	inputFile >> accountRead >> typeRead >> trDateRead >> trCode >> trAmount;
	Transaction ** ntrans = new Transaction*[K_SizeMax];
	Transaction ** mtrans = ntrans;
	BankAccount ** pAccount;

	while (inputFile && (counter < K_SizeMax-1)){
		*mtrans = new Transaction(accountRead, typeRead, trDateRead, trCode, trAmount);
		pAccount = listAccounts;
		while ( (**pAccount).getType() != 0) {
			if((**pAccount).getType() == 0){
				cout << "Account Does Not Exist" << endl;
			}else if((**pAccount).getAccountId() == accountRead && (**pAccount).getType() == typeRead){
				(**pAccount).executeTransaction(**mtrans);
			}
			pAccount++;

		}
		inputFile >> accountRead >> typeRead >> trDateRead >> trCode >> trAmount;
		counter++;
		mtrans++;
	}
}

//******************************************************************************
// Purpose: This function displays the list of bank accounts for all customers.
//
// Inputs: listAccount (type: BankAccount *), the list of bank accounts.
// Outputs: Nothing
//******************************************************************************
void displayAccounts(BankAccount ** listAccounts)
{
	cout << endl << endl << endl;

	Bool find[K_SizeMax];
	for (int k = 0; k < K_SizeMax; k++) { find[k] = FALSE; }

	cout << "                       THE REPORT OF THE BANK ACCOUNTS OF CLIENTS" << endl;
	cout << "                       ------------------------------------------" << endl << endl;

	int i = 0;
	vector<int> ids;
	BankAccount ** pAccount = listAccounts;
	while ( (**pAccount).getType() != 0) {
		
		if (std::find(ids.begin(), ids.end(), (**pAccount).getAccountId()) == ids.end() ) {
			int newid = (**pAccount).getAccountId();
		    ids.push_back(newid);
		    cout.setf(ios::fixed);
			cout.precision(2);

		    cout << "   Client Name: ";
		    cout << (**pAccount).getClientName() << endl;
		    cout << "Bank Account"<< "\t\t" <<"Type" << "\t" <<"Update Date"<< "\t" << "Balance" << "\t\t" <<"Nb. years" << "\t" <<"Rate" << endl;
		    cout << "------------"<< "\t\t" <<"----" << "\t" <<"-----------"<< "\t" << "-------" << "\t\t" <<"---------" << "\t" <<"----" << endl;


		    int j = 0;
		    while ( (**(listAccounts+j)).getType() != 0) {
		    	if((**(listAccounts+j)).getAccountId() == newid){
		    		(**(listAccounts+j)).print();
		    		if((**(listAccounts+j)).getType() <= 2){
		    			cout << endl;
		    		}
		    	}
		    	j++;
		    }

		    cout << endl << endl;

		}
		i++;
		pAccount = listAccounts+i;

	}










}




int main()
{
	std::setlocale(LC_ALL, "en-US.ISO-8859-1");
	BankAccount ** list = readAccounts();
	// sortAccounts(list);
	displayAccounts(list);
	updateAccounts(list);
	cout << endl << endl;
	cout << "               ************************************************" << endl;
	cout << "               *        RE-DISPLAYING DATA AFTER UPDATE       *" << endl;
	cout << "               ************************************************" << endl;
	displayAccounts(list);
	cout << endl;

	system("PAUSE");
	return 0;
}

