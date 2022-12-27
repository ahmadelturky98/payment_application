#include <stdio.h>
#include <string.h>
#include "server.h"

const char* transState[] = {"APPROVED", "DECLINED_INSUFFECIENT_FUND", "DECLINED_STOLEN_CARD", "FRAUD_CARD", "INTERNAL_SERVER_ERROR"};
const char* serverError[] = { "SERVER_OK", "SAVING_FAILED", "TRANSACTION_NOT_FOUND", "ACCOUNT_NOT_FOUND", "LOW_BALANCE", "BLOCKED_ACCOUNT" };

int i;
static int t_index = 0;

EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountRefrence)
{
	accountRefrence[4].balance = 500.0;
	strcpy_s(accountRefrence[4].primaryAccountNumber, 20, "123456712345671225");
	accountRefrence[4].state = BLOCKED;

	accountRefrence[5].balance = 20000.0;
	strcpy_s(accountRefrence[5].primaryAccountNumber, 20, "123456712345671233");
	accountRefrence[5].state = RUNNING;

	for (i = 0; i < 255; i++)
	{
		if (strcmp(cardData->primaryAccountNumber, accountRefrence[i].primaryAccountNumber) == 0)
		{
			t_index = i;
			if (accountRefrence[i].state == RUNNING)
				return SERVER_OK;
			else
				return BLOCKED_ACCOUNT;
		}

		if (i == 254)
			return ACCOUNT_NOT_FOUND;
	}
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence)
{
	//printf("INDEX = %d\n", t_index);
	if (termData->transAmount <= accountRefrence[t_index].balance)
	{
		accountRefrence[t_index].balance -= termData->transAmount;
		//printf("AR & TT = %f - %f\n", accountRefrence[t_index].balance, termData->transAmount);
		return SERVER_OK;
	}
	else
		return LOW_BALANCE;
}

EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	transaction_data[t_index].transactionSequenceNumber += 1;
	strcpy_s(transaction_data[t_index].terminalData.transactionDate, 11, terminal_user.transactionDate);
	transaction_data[t_index].terminalData.transAmount = terminal_user.transAmount;
	transaction_data[t_index].transState = APPROVED;
	transaction_data[t_index].terminalData.maxTransAmount = terminal_user.maxTransAmount;
	strcpy_s(transaction_data[t_index].cardHolderData.cardHolderName, 25, card_user.cardHolderName);
	strcpy_s(transaction_data[t_index].cardHolderData.primaryAccountNumber, 20, card_user.primaryAccountNumber);
	strcpy_s(transaction_data[t_index].cardHolderData.cardExpirationDate, 6, card_user.cardExpirationDate);
}

void recieveTransactionDataTest(void)
{
	int i = 0, user;

	printf("Tester Name : Ahmed Nader\nFunction Name: recieveTransactionData\nTest Case 1:\nInput Data: ");
	gets(card_user.primaryAccountNumber);

	puts("Expected Result: INTERNAL_SERVER_ERROR");

	strcpy_s(transaction_data[3].cardHolderData.primaryAccountNumber, 20, "0523011181190401018");
	transaction_data[3].terminalData.transAmount = 1000.0;
	transaction_data[3].terminalData.maxTransAmount = 12000.0;
	transaction_data[3].transState = INTERNAL_SERVER_ERROR;

	for (user = 0; user < 255; user++)
	{
		if (strcmp(card_user.primaryAccountNumber, transaction_data[user].cardHolderData.primaryAccountNumber) == 0)
		{
			break;
		}

	}

	accountsDB[5].balance = 20000.0;
	strcpy_s(accountsDB[5].primaryAccountNumber, 20, "0523011181190401018");
	accountsDB[5].state = RUNNING;

	for (i = 0; i < 255; i++)
	{
		if (strcmp(transaction_data[user].cardHolderData.primaryAccountNumber, accountsDB[i].primaryAccountNumber) == 0)
		{
			if (transaction_data[user].terminalData.transAmount > accountsDB[i].balance)
			{
				transation_status = DECLINED_INSUFFECIENT_FUND;
				printf("Actual Result: %s\n", transState[transation_status]);
				break;
			}
			else if(accountsDB[i].state == BLOCKED)
			{
				transation_status = DECLINED_STOLEN_CARD;
				printf("Actual Result: %s\n", transState[transation_status]);
				break;
			}
			else if (transaction_data[user].transState == INTERNAL_SERVER_ERROR)
			{
				transation_status = INTERNAL_SERVER_ERROR;
				printf("Actual Result: %s\n", transState[transation_status]);
				break;
			}
			else
			{
				transation_status = APPROVED;
				printf("Actual Result: %s\n", transState[transation_status]);
				break;
			}
		}
		if (i == 254)
		{
			transation_status = FRAUD_CARD;
			printf("Actual Result: %s\n", transState[transation_status]);
		}
	}
}

void isValidAccountTest(void)
{
	printf("Tester Name : Ahmed Nader\nFunction Name: isValidAccount\nTest Case 2:\nInput Data: ");
	gets(card_user.primaryAccountNumber);

	strcpy_s(accountsDB[5].primaryAccountNumber, 20, "0523011181190401018");

	puts("Expected Result: ACCOUNT_NOT_FOUND");

	for (i = 0; i < 255; i++)
	{
		if (strcmp(card_user.primaryAccountNumber, accountsDB[i].primaryAccountNumber) == 0)
		{
			server_status = SERVER_OK;
			printf("Actual Result: %s\n", serverError[server_status]);
			break;
		}

		if (i == 254)
		{
			server_status = ACCOUNT_NOT_FOUND;
			printf("Actual Result: %s\n", serverError[server_status]);
		}
	}
}

void isBlockedAccountTest(void)
{
	printf("Tester Name : Ahmed Nader\nFunction Name: isBlockedAccount\nTest Case 2:\nInput Data: ");

	gets(card_user.primaryAccountNumber);

	strcpy_s(accountsDB[5].primaryAccountNumber, 20, "0523011181190401018");
	accountsDB[5].state = BLOCKED;

	puts("Expected Result: SERVER_OK");

	for (i = 0; i < 255; i++)
	{
		if (strcmp(card_user.primaryAccountNumber, accountsDB[i].primaryAccountNumber) == 0)
		{
			if (accountsDB[i].state == RUNNING)
			{
				server_status = SERVER_OK;
				printf("Actual Result: %s\n", serverError[server_status]);
			}
			else
			{
				server_status = BLOCKED_ACCOUNT;
				printf("Actual Result: %s\n", serverError[server_status]);
			}
		}
	}
}

void isAmountAvailableTest(void)
{
	printf("Tester Name : Ahmed Nader\nFunction Name: recieveTransactionData\nTest Case 2:\nInput Data: ");
	scanf_s("%f", &terminal_user.transAmount);

	accountsDB[0].balance = 1000.00;

	puts("Expected Result: LOW_BALANCE");

	if (terminal_user.transAmount <= accountsDB[0].balance)
	{
		server_status = SERVER_OK;
		printf("Actual Result: %s\n", serverError[server_status]);
	}
	else
	{
		server_status = LOW_BALANCE;
		printf("Actual Result: %s\n", serverError[server_status]);
	}
}

void saveTransactionTest(void)
{
	printf("Tester Name : Ahmed Nader\nFunction Name: recieveTransactionData\nTest Case 1:\nInput Data:\n ");
	
	transaction_data[t_index].transactionSequenceNumber += 1;
	strcpy_s(transaction_data[t_index].terminalData.transactionDate, 11, "21/12/2022");
	transaction_data[t_index].terminalData.transAmount = 900.0;
	transaction_data[t_index].transState = APPROVED;
	transaction_data[t_index].terminalData.maxTransAmount = 8000.0;
	strcpy_s(transaction_data[t_index].cardHolderData.cardHolderName, 25, "AHMED NADER ABDULKADER");
	strcpy_s(transaction_data[t_index].cardHolderData.primaryAccountNumber, 20, "0523011181190401018");
	strcpy_s(transaction_data[t_index].cardHolderData.cardExpirationDate, 6, "11/25");
	//listSavedTransactions();
}

void listSavedTransactions(void)
{
	puts("#########################");
	printf("Transaction Sequence Number: %d\n", transaction_data[t_index].transactionSequenceNumber);
	printf("Transaction Date: %s\n", transaction_data[t_index].terminalData.transactionDate);
	printf("Transaction Amount: %f\n", transaction_data[t_index].terminalData.transAmount);
	printf("Transaction State: %d\n", transaction_data[t_index].transState);
	printf("Terminal Max Amount: %f\n", transaction_data[t_index].terminalData.maxTransAmount);
	printf("Cardholder Name: %s\n", transaction_data[t_index].cardHolderData.cardHolderName);
	printf("PAN: %s\n", transaction_data[t_index].cardHolderData.primaryAccountNumber);
	printf("Card Expiration Date: %s\n", transaction_data[t_index].cardHolderData.cardExpirationDate);
	printf("Remaining Balance: %f\n", accountsDB[t_index].balance);
	puts("#########################");
}
