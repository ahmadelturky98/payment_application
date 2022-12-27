#include <string.h>
#include <stdio.h>
#include "card.h";

const char* cardError[] = { "CARD_OK", "WRONG_NAME", "WRONG_EXP_DATE", "WRONG_PAN" };

EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	int name_length = strlen(cardData->cardHolderName);

	if (name_length >= 20 && name_length <= 24)
		return CARD_OK;
	else
		return WRONG_NAME;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
	int date_length = strlen(cardData->cardExpirationDate);

	if (date_length == 5 && cardData->cardExpirationDate[2] == '/')
		return CARD_OK;
	else
		return WRONG_EXP_DATE;
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
	int pan_length = strlen(cardData->primaryAccountNumber);

	if (pan_length >= 16 && pan_length <= 19)
		return CARD_OK;
	else
		return WRONG_PAN;
}

void getCardHolderNameTest(void) 
{
	int name_length = 0;

	printf("Tester Name : Ahmed Nader\nFunction Name: getCardHolderName\nTest Case 3:\nInput Data: ");

	gets(card_user.cardHolderName);

	puts("Expected Result: WRONG_NAME");
	name_length = strlen(card_user.cardHolderName);
	
	if (name_length >= 20 && name_length <= 24)
	{
		card_status = CARD_OK;
		printf("Actual Result: %s\n", cardError[card_status]);
	}
	else
	{
		card_status = WRONG_NAME;
		printf("Actual Result: %s\n", cardError[card_status]);
	}
} 

void getCardExpiryDateTest(void)
{
	int date_length = 0;

	printf("Tester Name : Ahmed Nader\nFunction Name: getCardExpiryDate\nTest Case 1:\nInput Data: ");

	gets(card_user.cardExpirationDate);
	puts("Expected Result: WRONG_EXP_DATE");

	date_length = strlen(card_user.cardExpirationDate);

	if(date_length == 5 && card_user.cardExpirationDate[2] == '/')
	{ 
		card_status = CARD_OK;
		printf("Actual Result: %s\n", cardError[card_status]);
		
	}
	else
	{
		card_status = WRONG_EXP_DATE;
		printf("Actual Result: %s\n", cardError[card_status]);
	}
}

void getCardPANTest(void)
{
	int pan_length = 0;

	printf("Tester Name : Ahmed Nader\nFunction Name: getCardPAN\nTest Case 4:\nInput Data: ");

	gets(card_user.primaryAccountNumber);
	puts("Expected Result: CARD_OK");

	pan_length = strlen(card_user.primaryAccountNumber);

	if (pan_length >= 16 && pan_length <= 19)
	{
		card_status = CARD_OK;
		printf("Actual Result: %s\n", cardError[card_status]);

	}
	else
	{
		card_status = WRONG_PAN;
		printf("Actual Result: %s\n", cardError[card_status]);
	}
}