#include <stdio.h>
#include "app.h"

void appStart(void)
{
	int condition  = 0 , second_condition = 0;
	
	do
	{
		printf("Cardholder Name: ");
		gets(card_user.cardHolderName);
		condition = getCardHolderName(&card_user);

		if (condition == 1)
			puts("WRONG_NAME");

	} while (condition == 1);
	
	do
	{
		printf("Card Expiration Date: ");
		gets(card_user.cardExpirationDate);
		condition = getCardExpiryDate(&card_user);

		if (condition == 2)
			puts("WRONG_EXP_DATE");
	} while (condition == 2);
	
	do
	{
		printf("PAN: ");
		gets(card_user.primaryAccountNumber);
		condition = getCardPAN(&card_user);

		if (condition == 3)
			puts("WRONG_PAN");
		else
			second_condition = isValidCardPAN(&card_user);

		if (second_condition == 3)
			puts("INVALID_CARD");
	} while (condition == 3 || second_condition == 3);
	

	do
	{
		printf("Transaction Date: ");
		gets(terminal_user.transactionDate);
		condition = getTransactionDate(&terminal_user);

		if (condition == 1)
			puts("WRONG_DATE");
		else
			condition = isCardExpired(&card_user, &terminal_user);

		if (condition == 2)
			puts("EXPIRED_CARD");
	} while (condition != 0);

	do
	{
		printf("Transaction Amount: ");
		scanf_s("%f", &terminal_user.transAmount);
		condition = getTransactionAmount(&terminal_user);

		if (condition == 4)
			puts("INVALID_AMOUNT");
		else
			condition = isBelowMaxAmount(&terminal_user);

		if (condition == 5)
			puts("EXCEED_MAX_AMOUNT");
	} while (condition != 0);
	

	printf("\n\CONNECTING TO SERVER...\n");
	condition = isValidAccount(&card_user, &accountsDB);

	if (condition == 3)
	{
		puts("Declined");
		puts("Invalid Account");
	}
	else if(condition == 5)
	{
		puts("Declined");
		puts("Blocked Account");
	}


	if (condition == 0)
	{
		second_condition = isAmountAvailable(&terminal_user, &accountsDB);

		if (second_condition == 4)
		{
			puts("Declined");
			puts("Insuffecient Funds");
		}
		else if (second_condition == 0)
		{
			saveTransaction(&transaction_data);
			listSavedTransactions();
			puts("THANK YOU FOR YOUR SERVICE!");
		}
	}

}
