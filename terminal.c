#include "terminal.h"
#include <stdio.h>
#include <string.h>

const char* terminalError[] = { "TERMINAL_OK", "WRONG_DATE", "EXPIRED_CARD", "INVALID_CARD", "INVALID_AMOUNT", "EXCEED_MAX_AMOUNT", "INVALID_MAX_AMOUNT" };

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	int transactionDate_length = strlen(termData->transactionDate);

	if (transactionDate_length == 10 && termData->transactionDate[2] == '/' && termData->transactionDate[5] == '/')
		return TERMINAL_OK;
	else
		return  WRONG_DATE;
}

EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData)
{
	int i, count = 0, expiry_year, expiry_month, transaction_year, transaction_month;

	for (i = 0; i < 10; i++)
	{
		if (i > 2 && i < 6)
		{
			termData->transactionDate[count] = termData->transactionDate[i];
			count++;
		}
		else if (i > 7 && i < 10)
		{
			termData->transactionDate[count] = termData->transactionDate[i];
			count++;
		}
		if (i == 9)
			termData->transactionDate[5] = '\0';
	}

	expiry_year = (cardData->cardExpirationDate[3] - '0') * 10 + (cardData->cardExpirationDate[4] - '0');
	transaction_year = (termData->transactionDate[3] - '0') * 10 + (termData->transactionDate[4] - '0');

	expiry_month = (cardData->cardExpirationDate[0] - '0') * 10 + (cardData->cardExpirationDate[1] - '0');
	transaction_month = (termData->transactionDate[0] - '0') * 10 + (termData->transactionDate[1] - '0');

	if (expiry_year - transaction_year < 0)
		return EXPIRED_CARD;
	else if (expiry_month - transaction_month < 0 && expiry_year - transaction_year == 0)
		return EXPIRED_CARD;
	else
		return TERMINAL_OK;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	if (termData->transAmount > 0.0)
		return TERMINAL_OK;
	else
		return INVALID_AMOUNT;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	termData->maxTransAmount = 12000.0;

	if (termData->transAmount <= termData->maxTransAmount)

		return TERMINAL_OK;
	else
		return EXCEED_MAX_AMOUNT;
}

EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData)
{
	int i, pan_length = 0, total = 0, digit, checksum, sum = 0, actual_checksum;

	pan_length = strlen(cardData->primaryAccountNumber);

	for (i = pan_length - 2; i >= 0; i--)
	{
		if (i % 2 == 0)
		{
			digit = (cardData->primaryAccountNumber[i] - '0') * 2;
			if (digit >= 10)
			{
				sum = 0;
				while (digit > 0)
				{
					sum += (digit % 10);
					digit /= 10;
				}
				digit = sum;
			}
			total += digit;
		}
		else
			total += (cardData->primaryAccountNumber[i] - '0');
	}

	checksum = (10 - (total % 10) % 10);
	actual_checksum = (cardData->primaryAccountNumber[pan_length - 1] - '0');

	if (checksum == actual_checksum)
		return TERMINAL_OK;
	else
		return INVALID_CARD;
}

void getTransactionDateTest(void)
{
	
	int transactionDate_length = 0;

	printf("Tester Name : Ahmed Nader\nFunction Name: getTransactionDate\nTest Case 1:\nInput Data: ");

	gets(terminal_user.transactionDate);
	puts("Expected Result: WRONG_DATE");

	transactionDate_length = strlen(terminal_user.transactionDate);

	if (transactionDate_length == 10 && terminal_user.transactionDate[2] == '/' && terminal_user.transactionDate[5] == '/')
	{
		terminal_status = TERMINAL_OK;
		printf("Actual Result: %s\n", terminalError[terminal_status]);

	}
	else
	{
		terminal_status = WRONG_DATE;
		printf("Actual Result: %s\n", terminalError[terminal_status]);
	}
}

void isCardExpriedTest(void)
{
	int i, count = 0, expiry_year, expiry_month, transaction_year, transaction_month;

	printf("Tester Name : Ahmed Nader\nFunction Name: isCardExpried\nTest Case 1:\nInput Data: ");

	gets(terminal_user.transactionDate);
	strcpy_s(card_user.cardExpirationDate, 6, "11/25");

	puts("Expected Result: EXPIRED_CARD");

	for (i = 0; i < 10; i++)
	{
		if (i > 2 && i < 6)
		{
			terminal_user.transactionDate[count] = terminal_user.transactionDate[i];
			count++;
		}
		else if (i > 7 && i < 10)
		{
			terminal_user.transactionDate[count] = terminal_user.transactionDate[i];
			count++;
		}
		if (i == 9)
			terminal_user.transactionDate[5] = '\0';
	}

	expiry_year = (card_user.cardExpirationDate[3] - '0') * 10 + (card_user.cardExpirationDate[4] - '0');
	transaction_year = (terminal_user.transactionDate[3] - '0') * 10 + (terminal_user.transactionDate[4] - '0');

	expiry_month = (card_user.cardExpirationDate[0] - '0') * 10 + (card_user.cardExpirationDate[1] - '0');
	transaction_month = (terminal_user.transactionDate[0] - '0') * 10 + (terminal_user.transactionDate[1] - '0');

	if (expiry_year - transaction_year < 0)
	{
		terminal_status = EXPIRED_CARD;
		printf("Actual Result: %s\n", terminalError[terminal_status]);
		
	}
	else if(expiry_month - transaction_month < 0 && expiry_year - transaction_year == 0)
	{
		terminal_status = EXPIRED_CARD;
		printf("Actual Result: %s\n", terminalError[terminal_status]);
	}
	else
	{
		terminal_status = TERMINAL_OK;
		printf("Actual Result: %s\n", terminalError[terminal_status]);
	}
}

void getTransactionAmountTest(void)
{
	printf("Tester Name : Ahmed Nader\nFunction Name: getTransactionAmount\nTest Case 1:\nInput Data: ");
	scanf_s("%f", &terminal_user.transAmount);

	puts("Expected Result: INVALID_AMOUNT");

	if (terminal_user.transAmount > 0.0)
	{
		terminal_status = TERMINAL_OK;
		printf("Actual Result: %s\n", terminalError[terminal_status]);
	}
	else
	{
		terminal_status = INVALID_AMOUNT;
		printf("Actual Result: %s\n", terminalError[terminal_status]);
	}
}

void isBelowMaxAmountTest(void)
{
	terminal_user.maxTransAmount = 12000.0;

	printf("Tester Name : Ahmed Nader\nFunction Name: isBelowMaxAmount\nTest Case 1:\nInput Data: ");
	scanf_s("%f", &terminal_user.transAmount);

	puts("Expected Result: EXCEED_MAX_AMOUNT");

	if (terminal_user.transAmount <= terminal_user.maxTransAmount)
	{
		terminal_status = TERMINAL_OK;
		printf("Actual Result: %s\n", terminalError[terminal_status]);
	}
	else
	{
		terminal_status = EXCEED_MAX_AMOUNT;
		printf("Actual Result: %s\n", terminalError[terminal_status]);
	}
}

void setMaxAmountTest(void)
{
	printf("Tester Name : Ahmed Nader\nFunction Name: setMaxAmount\nTest Case 1:\nInput Data: ");
	scanf_s("%f", &terminal_user.maxTransAmount);

	puts("Expected Result: INVALID_MAX_AMOUNT");

	if (terminal_user.maxTransAmount > 0)
	{
		terminal_status = TERMINAL_OK;
		printf("Actual Result: %s\n", terminalError[terminal_status]);
	}
	else
	{
		terminal_status = INVALID_MAX_AMOUNT;
		printf("Actual Result: %s\n", terminalError[terminal_status]);
	}
}

void isValidCardPANTest(void)
{
	int i, pan_length = 0, total = 0, digit, checksum, sum = 0, actual_checksum;

	printf("Tester Name : Ahmed Nader\nFunction Name: isValidCardPAN\nTest Case 1:\nInput Data: ");
	gets(card_user.primaryAccountNumber);

	//puts("Expected Result: INVALID_CARD");

	pan_length = strlen(card_user.primaryAccountNumber);

	for (i = pan_length - 2; i >= 0; i--)
	{
		if (i % 2 == 0)
		{
			digit = (card_user.primaryAccountNumber[i] - '0') * 2;
			if (digit >= 10)
			{
				sum = 0;
				while (digit > 0)
				{
					sum += (digit % 10);
					digit /= 10;
				}
				digit = sum;
			}
			total += digit;
		}
		else
			total += (card_user.primaryAccountNumber[i] - '0');
	}

	checksum = (10 - (total % 10) % 10);
	actual_checksum = (card_user.primaryAccountNumber[pan_length - 1] - '0');

	if (checksum == actual_checksum)
	{
		terminal_status = TERMINAL_OK;
		printf("Actual Result: %s\n", terminalError[terminal_status]);
	}
	else
	{
		terminal_status = INVALID_CARD;
		printf("Actual Result: %s\n", terminalError[terminal_status]);
	}
}
