#include<stdio.h>  
#include<stdlib.h> 
#include<string.h>
#include"lib.h"

int main(){
	aBook *bookHead = NULL;							//var declarations
	aBorrower *borrowerHead = NULL;
	int choice, totalBooks = 0, totalBorrowers = 0;

	//loading from files
	loadBooks(&bookHead, &totalBooks);
	loadBorrowers(&borrowerHead, &totalBorrowers);

	do{
		choice = menu();
		int viewed = 0; 

		switch(choice){
			case 1:
				//add book
				addBook(&bookHead, &totalBooks);
				break;
			case 2:
				//search book
				searchBook(bookHead, 0);
				break;
			case 3:
				//view books
				viewBooks(bookHead, 0, &viewed);
				break;
			case 4:
				deleteBook(&bookHead, &totalBooks);
				//delete book
				break;
			case 5:
				//add borrower
				addBorrower(&borrowerHead, &totalBorrowers);
				break;
			case 6:
				//delete borrower
				deleteBorrower(&borrowerHead, &totalBorrowers);
				break;
			case 7:
				borrowBook(&bookHead, &borrowerHead);
				break;
				//borrow book
			case 8:
				returnBook(&bookHead, &borrowerHead);
				break;
				//return book
			case 9:
				//view borrowed books
				viewBorrowedBooks(bookHead);
				break;
			case 10:
				//view borrowers and borrowed books
				viewBorrowers(borrowerHead, 1);
				break;
			default:
				//save to files, deallocate then exit
				saveBooksToFile(bookHead, totalBooks);
				saveBorrowersToFile(borrowerHead, totalBorrowers);
				deallocate(&bookHead, &borrowerHead);
				break;
		}

	}while(choice!=0);

	return 0;
}

