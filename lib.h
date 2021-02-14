/*
Outline

* books to be stored in alphabetical title, then book ID
* borrowes to be stored in alphabetical lastname, then firstname

INITIAL INPUT
- Linked List of Books
- Linked List of Borrowers
- Linked List of Borrowed Books per Borrower

Books
- bookID
- title 
- author
- category
- status
- Borrower
- date Borrowed

Borrowers
- student number
- name
- course
- number of books Borrowed
- borrowed books List

Borrowed books List
- book ID
- title
- date borrowed
- return date
- lapse fine


PROCESS

[Adding Books]
- always available status per add
- insert sort per book by title then author

[Searching Books]
- pass the og list by value
	by title
	- search title then add to new list
	by author
	- search author then insert sort to new list
- display
- user choice, get bookid, then display full details

[Viewing Books]
- by category
- by all

[Deleting Books]
- view books fxn
- user choice, 
	- if borrowed, display info, go back
	- if available, then confirm
		- modify books list

[Adding Borrower]
- info all user input
- search bookI, if existing invalid
- insert sort per borrower by surname then name

[Deleting Borrower]
- view borrowers
- user choice, 
	- if unreturned books, display info, go back
	- if clear, confirm
		- modify

[Borrow Book]
- input borrower id, search then confirm
- search book fxn, then display chosen
- get book id, get current date
- add borrower to book; add book to borrower

[returning book]
- borrower info, confirm
- enter current return date
- display borrowed books
- choose from list
- generate fines, display				
- modify borrower and book

[view borrowed books]
- if borrowed then display

[Insert Sort Fxn]
- sort Books 							
- sort borrowers

FILE IMPLEMENTATION
- saving to borrowers.txt in alphabetical order
- saving books alphabetically then ascending bookID
- saving borrowed books in another FILE


system("cls");

BOOK
number of books
status
title
bookID
author last, firstname
category
borrower name last, firstnam
student id
course
number of books
date mm-dd-yyyy

BORROWER
number of borrowers
name last, firstnam
student id
course
borrowedCount
[booklist]
bookID
title
date borrowed
returndate

*/

typedef struct{
	char firstName[100];
	char lastName[100];
}aName;

typedef struct{
	int month;
	int day;
	int year;
}aDate;
 
typedef struct{
	int num;
	char value[20];
}bookCategory;

typedef struct{
	int num;
	char value[10];
}bookStatus;

typedef struct borrowedBooks{
	int bookID;
	char title[50];
	aDate dateBorrowed;
	aDate returnDate;
	int fine;
	struct borrowedBooks *next;
}borrowedBooks;

typedef struct borrower_tag{
	char studentNumber[11];
	aName studentName;
	char course[30];
	int borrowedBookCount;
	borrowedBooks *bookList;					//linked list of borrowed books
	struct borrower_tag *next;
}aBorrower;

typedef struct book_tag{
	int bookID;
	char title[50];
	aName author;
	bookCategory category;
	bookStatus status;
	aBorrower borrower;
	aDate dateBorrowed;
	struct book_tag *next;
}aBook;


//menu
int menu(){
	int i;

	do{
	printf("\n\t||======= MAIN MENU ========||\n");
	printf("\t|| [ 1] Add Book            ||\n");
	printf("\t|| [ 2] Search Book         ||\n");
	printf("\t|| [ 3] View Books          ||\n");
	printf("\t|| [ 4] Delete Book         ||\n");
	printf("\t|| [ 5] Add Borrower        ||\n");
	printf("\t|| [ 6] Delete Borrower     ||\n");
	printf("\t|| [ 7] Borrow Book         ||\n");
	printf("\t|| [ 8] Return Book         ||\n");
	printf("\t|| [ 9] View Borrowed Books ||\n");
	printf("\t|| [10] View Borrowers      ||\n");
	printf("\t|| [ 0] Exit                ||\n");
	printf("\t||==========================||\n");


	printf("\n    Please choose one: ");
	scanf("%d", &i);

	if(i<0||i>10) printf("    Invalid choice. Please choose again.\n");			//if invalid
	} while(i<0||i>10);

	return i;
}

																						//validates a date input
int valiDate(aDate date){
	int valid = 1;
	if(1800<=date.year&&date.year<=9999){
		if(1<=date.month&&date.month<=12){
			switch(date.month){
				case 2:
					if(date.year%4==0){
						if(1<=date.day&&date.day<=29) return valid;
					}else{
						if(1<=date.day&&date.day<=28) return valid;
					}
					break;
				case 4: case 6: case 9: case 11:
					if(1<=date.day&&date.day<=30) return valid;
					break;
				default:
					if(1<=date.day&&date.day<=31) return valid;
					break;
			}
		}
	}
	valid = 0;
	printf("\n\t  Invalid input. Please follow the given date format.\n");			//if invalid returns 0
	return valid;
}


aDate returnDateGenerator(aDate dateBorrowed){										//generates return date given the date borrowed
	aDate returnDate;
	
	returnDate.month = dateBorrowed.month+1;
	returnDate.year = dateBorrowed.year;

	if(dateBorrowed.month==2){
		if(dateBorrowed.year%4==0){
			if(dateBorrowed.day>22){
				returnDate.day = dateBorrowed.day+7-29;
				return returnDate;
			}
		}else{
			if(dateBorrowed.day>21){
				returnDate.day = dateBorrowed.day+7-28;
				return returnDate;
			}
		}
	}else if(dateBorrowed.month==4||dateBorrowed.month==6||dateBorrowed.month==9||dateBorrowed.month==11){
		if(dateBorrowed.day>23){
			returnDate.day = dateBorrowed.day+7-30;
			return returnDate;
		}
	}else{
		if(dateBorrowed.day>24){
			if(dateBorrowed.month==12){
				returnDate.day = dateBorrowed.day+7-31;
				returnDate.month = 1;
				returnDate.year = dateBorrowed.year+1;
				return returnDate;
			}else{
				returnDate.day = dateBorrowed.day+7-31;
				return returnDate;
			}
		}
	}

	returnDate.day = dateBorrowed.day+7;
	returnDate.month = dateBorrowed.month;

	return returnDate;															//returns expected return dateBorrowed
}

int viewBorrowers(aBorrower *borrowerHead, int show){							//views borrowers
	aBorrower *tempBorrower = borrowerHead;
	borrowedBooks *tailBookList;
	int count = 1, bookCount = 1;

	printf("\n\t== REGISTERED BORROWERS ========\n");

	while(tempBorrower!=NULL){
		printf("\n\t[%d] %s %s\n", count, tempBorrower->studentName.firstName, tempBorrower->studentName.lastName);
		printf("\t||StudentNumber: %s\n", tempBorrower->studentNumber);
		printf("\t||Course:        %s\n", tempBorrower->course);
		printf("\t||Number of Borrowed: %d\n", tempBorrower->borrowedBookCount);

		if(show==1){
			if(tempBorrower->bookList!=NULL){
				tailBookList = tempBorrower->bookList;
				printf("\t== BORROWED BOOKS ========\n");
				while(tailBookList!=NULL){
					printf("\t  [%d] %s\n", bookCount, tailBookList->title);
					printf("\t  Book ID:       %d\n", tailBookList->bookID);
					printf("\t  Date Borrowed: %d-%d-%d\n", tailBookList->dateBorrowed.month, tailBookList->dateBorrowed.day, tailBookList->dateBorrowed.year);
					printf("\t  Return Date:   %d-%d-%d", tailBookList->returnDate.month, tailBookList->returnDate.day, tailBookList->returnDate.year);
					printf("\n");

					tailBookList = tailBookList->next;
					bookCount++;
				}
			}
		}

		tempBorrower = tempBorrower->next;
		count++;
	}

	if(count==1) printf("\n\tNo Borrowers yet.\n");
	return count-1;
}

int addBorrowerToBook(aBook **bookHead, aBook **updateBook, aBorrower *borrowerHead, int *borrower, aDate *dateBorrowed){
	aBook *tempBook;
	aBorrower *tempBorrower;																//modifies book and adds the borrower info to it
	int choice, borrowerCount, valid;

	borrowerCount = viewBorrowers(borrowerHead, 0);
	printf("\n\t[0] Not found\n");
	
	do{
		printf("\n\tPlease choose one: ");
		scanf("%d", &choice);

		*borrower = choice;

		if((borrowerCount==0 && choice!=0)||(choice<0||choice>borrowerCount)){
			printf("\tInvalid choice. Please choose again.\n");
		}else{
			if(choice==0){
				printf("\tBorrower not found. Please register first.\n");
				return 0;
			}else{
				tempBorrower = borrowerHead;
				for(int i=1; i<choice; i++){
					tempBorrower = tempBorrower->next;
				}

				if(tempBorrower->borrowedBookCount!=5){
				    (*updateBook)->status.num = 2;
					strcpy((*updateBook)->status.value, "Borrowed");
					(*updateBook)->borrower = *tempBorrower;
				}else{
					printf("\n\tBorrower has reached the allowed maximum number of borrowed books.\n");
					return 0;
				}
			}
			do{
				printf("\n\tDate Borrowed (mm-dd-yyyy): ");
				scanf("%d-%d-%d", &(*updateBook)->dateBorrowed.month, &(*updateBook)->dateBorrowed.day, &(*updateBook)->dateBorrowed.year);

				valid = valiDate((*updateBook)->dateBorrowed);
				if(valid==1){
					dateBorrowed->month = (*updateBook)->dateBorrowed.month;
					dateBorrowed->day = (*updateBook)->dateBorrowed.day;
					dateBorrowed->year = (*updateBook)->dateBorrowed.year;
				}
			}while(valid==0);
		}
	}while((borrowerCount==0 && choice!=0)||(choice<0||choice>borrowerCount));

	return 1;
}

void addBookToBorrower(aBook *newBook, aBorrower **borrowerHead, int borrower, aDate dateBorrowed){
	aBorrower *tempBorrower = *borrowerHead;
	borrowedBooks *tailBookList, *newBorrowedBook;										//modifies borrower and adds the book info into it

	for(int i=0; i<borrower-1; i++){
		tempBorrower = tempBorrower->next;
	}

	tempBorrower->borrowedBookCount++;

	newBorrowedBook = (borrowedBooks *) malloc(sizeof(borrowedBooks));
	newBorrowedBook->bookID = newBook->bookID;
	strcpy(newBorrowedBook->title, newBook->title);
	newBorrowedBook->dateBorrowed = dateBorrowed;
	newBorrowedBook->returnDate = returnDateGenerator(newBorrowedBook->dateBorrowed);
	newBorrowedBook->next = NULL;

	tailBookList = tempBorrower->bookList;

	if(tailBookList!=NULL){
		while(tailBookList->next!=NULL){
			tailBookList = tailBookList->next;
		} 
		tailBookList->next = newBorrowedBook;
	}else{
		tempBorrower->bookList = newBorrowedBook;
	}
}

void insertSort(aBook **bookHead, aBook **newBook, aBorrower **borrowerHead, aBorrower **newBorrower, int type){
    aBook *tempBook;
    aBorrower *tempBorrower;																						//insert sort every addition

    if(((type==1 ||type==3 || type==4) && *bookHead!=NULL) || ((type==2 || type==5) && *borrowerHead!=NULL)){                                                                //checking the head
        if(type==1 || type==3 || type==4) tempBook = *bookHead;
        else tempBorrower = *borrowerHead;

        char cmp1NewLevel1[50], cmp2TempLevel1[50], cmp1NewLevel2[100], cmp2TempLevel2[100];

        if(type==1 || type==4){
            strcpy(cmp1NewLevel1, (*newBook)->title);
            strcpy(cmp2TempLevel1, tempBook->title);
            strcpy(cmp1NewLevel2, (*newBook)->author.lastName);
            strcpy(cmp1NewLevel2, strcat(cmp1NewLevel2, (*newBook)->author.firstName));

            strcpy(cmp2TempLevel2, tempBook->author.lastName);
            strcpy(cmp2TempLevel2, strcat(cmp2TempLevel2, tempBook->author.firstName));
        }else if(type==2 || type==5){
            strcpy(cmp1NewLevel1, (*newBorrower)->studentName.lastName);
            strcpy(cmp2TempLevel1, tempBorrower->studentName.lastName);
            strcpy(cmp1NewLevel2, (*newBorrower)->studentName.firstName);
            strcpy(cmp2TempLevel2, tempBorrower->studentName.firstName);
        }else{
        	strcpy(cmp1NewLevel1, (*newBook)->author.lastName);
            strcpy(cmp2TempLevel1, tempBook->author.lastName);
            strcpy(cmp1NewLevel2, (*newBook)->author.firstName);
            strcpy(cmp2TempLevel2, tempBook->author.firstName);
        }

        strupr(cmp1NewLevel1);
        strupr(cmp2TempLevel1);
        strupr(cmp1NewLevel2);
        strupr(cmp2TempLevel2);

        if(strcmp(cmp1NewLevel1, cmp2TempLevel1) < 0){
            if(type==1 || type==3 || type==4){
                (*newBook)->next = tempBook;
                *bookHead = *newBook;                                                
            }else{
                (*newBorrower)->next = tempBorrower;
                *borrowerHead = *newBorrower;
            }                                                    
            
            if(type==1) printf("\n\t  Book successfully added.\n");
            else if(type==2) printf("\n\t  Borrower successfully added.\n");
            return;
        }else{
            if(strcmp(cmp1NewLevel1, cmp2TempLevel1) == 0){
                if(strcmp(cmp1NewLevel2, cmp2TempLevel2) < 0){
                    if(type==1 || type==3 || type==4){
                        (*newBook)->next = tempBook;
                        *bookHead = *newBook;                                                //modifying head
                    }else{
                        (*newBorrower)->next = tempBorrower;
                        *borrowerHead = *newBorrower;
                    }
                    
                    if(type==1) printf("\n\t  Book successfully added.\n");
                    else if(type==2)printf("\n\t  Borrower successfully added.\n");
                    return;
                }else if(strcmp(cmp1NewLevel2, cmp2TempLevel2) == 0){
                    if(type==1) printf("\n\t  Invalid. You are adding a duplicate of an existing book.\n");
                    else if(type==2)printf("\n\t  Invalid. You are adding a duplicate of an existing borrower.\n");
                    return;
                }
            }

            if(((type==1 || type==3 || type==4) && tempBook->next!=NULL) || ((type==2 || type==5) && tempBorrower->next!=NULL)){
                if(type==1 || type==4){
                    strcpy(cmp2TempLevel1, tempBook->next->title);
                    strcpy(cmp2TempLevel2, tempBook->author.lastName);
            		strcpy(cmp2TempLevel2, strcat(cmp2TempLevel2, tempBook->author.firstName));
                }else if(type==2 || type==5){
                    strcpy(cmp2TempLevel1, tempBorrower->next->studentName.lastName);
                    strcpy(cmp2TempLevel2, tempBorrower->next->studentName.firstName);
                }else{
                	strcpy(cmp2TempLevel1, tempBook->next->author.lastName);
                    strcpy(cmp2TempLevel2, tempBook->next->author.firstName);
                }
                
                strupr(cmp2TempLevel1);
                strupr(cmp2TempLevel2);
            }

            while((((type==1 || type==3 || type==4) && tempBook->next!=NULL) || ((type==2 || type==5) && tempBorrower->next!=NULL)) && (strcmp(cmp1NewLevel1, cmp2TempLevel1)>0 || (strcmp(cmp1NewLevel1, cmp2TempLevel1)==0 && strcmp(cmp1NewLevel2, cmp2TempLevel2)>0))){
                if(type==1 || type==3 || type==4) tempBook = tempBook->next;
                else tempBorrower = tempBorrower->next;
                
                if(((type==1 || type==3 || type==4) && tempBook->next!=NULL) || ((type==2 || type==5) && tempBorrower->next!=NULL)){
                    if(type==1 || type==4){
                        strcpy(cmp2TempLevel1, tempBook->next->title);
                        strcpy(cmp2TempLevel2, tempBook->author.lastName);
            			strcpy(cmp2TempLevel2, strcat(cmp2TempLevel2, tempBook->author.firstName));
                    }else if(type==2 || type==5){
	                    strcpy(cmp2TempLevel1, tempBorrower->next->studentName.lastName);
	                    strcpy(cmp2TempLevel2, tempBorrower->next->studentName.firstName);
	                }else{
	                	strcpy(cmp2TempLevel1, tempBook->next->author.lastName);
	                    strcpy(cmp2TempLevel2, tempBook->next->author.firstName);
	                }
                    
                    strupr(cmp2TempLevel1);
                    strupr(cmp2TempLevel2);
                }
            }

            if((((type==1 || type==3 || type==4) && tempBook->next!=NULL) || ((type==2 || type==5) && tempBorrower->next!=NULL)) && strcmp(cmp1NewLevel1, cmp2TempLevel1)==0 && strcmp(cmp1NewLevel2, cmp2TempLevel2)==0){
                if(type==1) printf("\n\t  Invalid. You are adding a duplicate of an existing book.\n");
                else printf("\n\t  Invalid. You are adding a duplicate of an existing borrower.\n");
                return;
            }else{
                if(type==1 || type==3 || type==4){
                    (*newBook)->next = tempBook->next;
                    tempBook->next = *newBook;
                }else{
                    (*newBorrower)->next = tempBorrower->next;
                    tempBorrower->next = *newBorrower;
                }
            }
        }
    }else{
        if(type==1 || type==3 || type==4){
            *bookHead = *newBook;                                                //modifying head
            (*newBook)->next = NULL;
        }else{
            *borrowerHead = *newBorrower;
            (*newBorrower)->next = NULL;
        }
    }

    if(type==1) printf("\n\t  Book successfully added.\n");
    else if(type==2)printf("\n\t  Borrower successfully added.\n");
}

void addBook(aBook **bookHead, int *totalBooks){														//addbook fxn
	aBook *tempBook, *newBook;

	//adding info to new book
	newBook = (aBook *) malloc(sizeof(aBook));

	printf("\n\t== BOOK DETAILS ========\n");
	printf("\t||Book ID: ");
	scanf("%d", &newBook->bookID);

	printf("\t||Title: ");
	getchar();
	scanf("%50[^\n]", newBook->title);

	printf("\t||Author (surname, firstname): ");
	getchar();
	scanf("%30[^,]%*c %30[^\n]", newBook->author.lastName, newBook->author.firstName);

	printf("\n\t== Category ========\n");
	printf("\t[1] General Reference\n\t[2] Journal\n\t[3] Magazine\n\t[4] Newspaper\n\t[5] Literary Works\n");
	
	do{
		printf("\tSelect one: ");
		scanf("%d", &newBook->category.num);

		switch(newBook->category.num){
			case 1:
				strcpy(newBook->category.value, "General Reference");
				break;
			case 2:
				strcpy(newBook->category.value, "Journal");
				break;
			case 3:
				strcpy(newBook->category.value, "Magazine");
				break;
			case 4:
				strcpy(newBook->category.value, "Newspaper");
				break;
			case 5:
				strcpy(newBook->category.value, "Literary Works");
				break;
			default:
				printf("\tInvalid input. Please choose again.\n");
				break;
		}
	}while(newBook->category.num<1||newBook->category.num>5);

	newBook->status.num = 1;
	strcpy(newBook->status.value, "Available");
	//end of info addition to new book node

	//insert sort by title then author
	insertSort(bookHead, &newBook, NULL, NULL, 1);

	*totalBooks = *totalBooks + 1;
}

int viewBooks(aBook *bookHead, int type, int *printed){											//viewbooks in general and in category
	aBook *tempBook = bookHead;
	int choice, viewed = 0;

	printf("\n\t== VIEW BOOKS ========\n");
	printf("\t[1] General References\n\t[2] Journals\n\t[3] Magazines\n\t[4] Newspapers\n\t[5] Literary Works\n");
	printf("\t[6] View all\n");
	
	do{
		printf("\n\tSelect one: ");
		scanf("%d", &choice);

		if(choice<1 || choice>6){ 
			printf("\tInvalid choice. Please choose again.\n");
		}else{
			while(tempBook!=NULL){
				if(tempBook->category.num==choice || choice==6){
					if(type==0)	printf("\n\t||Title:         %s\n", tempBook->title);
					else printf("\n\t[%d] %s\n", viewed+1, tempBook->title);
					
					printf("\t||Author:        %s %s\n", tempBook->author.firstName, tempBook->author.lastName);
					printf("\t||Book ID:       %d\n", tempBook->bookID);
					printf("\t||Category:      %s\n", tempBook->category.value);
					if(type!=2) printf("\t||Status:        %s\n", tempBook->status.value);
					
					if(tempBook->status.num==2 && type!=2){
						printf("\t||Borrower:      %s %s\n", tempBook->borrower.studentName.firstName, tempBook->borrower.studentName.lastName);
						printf("\t||Date Borrowed: %d-%d-%d\n", tempBook->dateBorrowed.month, tempBook->dateBorrowed.day, tempBook->dateBorrowed.year);
					}

					viewed++;
				}
				tempBook = tempBook->next;
			}
		}
	}while(choice<1 || choice>6);

	if(viewed==0) printf("\n\tThere are no books in this category.\n");
	*printed = viewed;

	return choice;
}

int searchBorrower(aBorrower *borrowerHead, char studentID[11], int type){							//search borrower given an id
	aBorrower *tempBorrower = borrowerHead;
	int count = 0;

	while(tempBorrower!=NULL){
		if(strcmp(tempBorrower->studentNumber, studentID)==0){
			count = 1;
			if(type==2){
				printf("\n\t||Name:          %s %s\n", tempBorrower->studentName.firstName, tempBorrower->studentName.lastName);
				printf("\t||StudentNumber: %s\n", tempBorrower->studentNumber);
				printf("\t||Course:        %s\n", tempBorrower->course);
				printf("\t||Number of Borrowed Books: %d\n", tempBorrower->borrowedBookCount);
			}
			break;
		}
		tempBorrower = tempBorrower->next;
	}

	return count;
}

void addBorrower(aBorrower **borrowerHead, int *totalBorrowers){
	aBorrower *tempBorrower, *newBorrower;															//add borrower
	int existing;

	//adding info of borrower
	newBorrower = (aBorrower *) malloc(sizeof(aBorrower));

	printf("\n\t== BORROWER DETAILS ========\n");
	printf("\t||Student Number: ");
	scanf("%s", newBorrower->studentNumber);

	//CHECK ID: if existing, either return or resenter ID
	existing = searchBorrower(*borrowerHead, newBorrower->studentNumber, 1);

	if(existing==1){
		printf("\n\tFound existing student ID. Borrower cannot be added.\n");
		return;
	}else{
		printf("\t||Name (surname, firstname): ");
		scanf("%*c%30[^,]%*c %30[^\n]", newBorrower->studentName.lastName, newBorrower->studentName.firstName);

		printf("\t||Course: ");
		getchar();
		scanf("%30[^\n]", newBorrower->course);

		newBorrower->borrowedBookCount = 0;
		newBorrower->bookList = NULL;
		//end of borrower input

		//insert sort by surname then firstname
		insertSort(NULL, NULL, borrowerHead, &newBorrower, 2);

		*totalBorrowers = *totalBorrowers + 1;
	}
}


void viewBorrowedBooks(aBook *bookHead){													//if borrowed then view
	aBook *tempBook;
	int i = 1;
	aDate returnDate;

	tempBook = bookHead;

	printf("\n\t== BORROWED BOOKS ========\n");
	while(tempBook!=NULL){
		if(tempBook->status.num==2){
			returnDate = returnDateGenerator(tempBook->dateBorrowed);

			printf("\n\t||Title:          %s\n", tempBook->title);
			printf("\t||Author:         %s %s\n", tempBook->author.firstName, tempBook->author.lastName);
			printf("\t||Borrower Name:  %s %s\n", tempBook->borrower.studentName.firstName, tempBook->borrower.studentName.lastName);
			printf("\t||Student Number: %s\n", tempBook->borrower.studentNumber);
			printf("\t||Date Borrowed:  %d-%d-%d\n", tempBook->dateBorrowed.month, tempBook->dateBorrowed.day, tempBook->dateBorrowed.year);
			printf("\t||Return Date:    %d-%d-%d\n", returnDate.month, returnDate.day, returnDate.year);
			i++;
		}
		tempBook = tempBook->next;
	}

	if(i==1) printf("\n\tNo borrowed books yet.\n");
}
	
void deleteBorrower(aBorrower **borrowerHead, int *totalBorrowers){									//deletes borrower
	aBorrower *tempBorrower = *borrowerHead, *delBorrower = *borrowerHead;
	int choice=1, borrowerCount, existing, deleteOption, delChoice;
	char studentID[11];

	printf("\n\t== DELETE BORROWER ========\n");
	printf("\n\t[1] Search Borrower\n");
	printf("\t[2] View All Borrowers\n");
	printf("\t[0] Back\n");
	
	do{
		printf("\n\tPlease choose one: ");
		scanf("%d", &deleteOption);

		if(deleteOption<0 || deleteOption>2) printf("\n\tInvalid choice. Please choose again.\n");
	}while(deleteOption<0 || deleteOption>2);
	
	//option by view or by search
	if(deleteOption==0) return;
	else if(deleteOption==1){
		printf("\n\tEnter Student Number/Borrower ID: ");
		scanf("%s", studentID);

		existing = searchBorrower(*borrowerHead, studentID, 2);
		if(existing==0){
			printf("\n\tBorrower ID not found.\n");
			return;
		}
		while(strcmp(studentID, delBorrower->studentNumber)!=0){
			tempBorrower = delBorrower;
			delBorrower = delBorrower->next;
		}
	}else{
		borrowerCount = viewBorrowers(*borrowerHead, 0);
		
		if(borrowerCount==0){
			return;
		}else{
			printf("\n\t[0] Back\n");
			do{
				printf("\n\t Choose a borrower to delete: ");
				scanf("%d", &choice);

				if(choice<0||choice>borrowerCount){
					printf("\t Invalid choice. Please choose again.\n");
				}else if(choice==0){
					return;
				}else{
					for(int i=0; i<choice-1; i++){
						tempBorrower = delBorrower;
						delBorrower = delBorrower->next;
					}

				}
			}while(choice<0||choice>borrowerCount);
		}
	}

	if(delBorrower->borrowedBookCount==0){
		printf("\n\t Are you sure you want to delete this borrower? \n");
		printf("\t [1] Yes\n");
		printf("\t [2] No\n");
		
		do{
			printf("\t Please choose one: ");
			scanf("%d", &delChoice);

			if(delChoice==1){
				if(borrowerCount!=1 && delBorrower!=*borrowerHead){
					tempBorrower->next = delBorrower->next;
					free(delBorrower);
				}else{
					*borrowerHead = (*borrowerHead)->next;
					free(delBorrower);
				}

				*totalBorrowers = *totalBorrowers -1;
				printf("\n\t Borrower deleted successfully.\n");
			}else if(delChoice==2){
				printf("\n\t Borrower not deleted.\n");
			}else{
				printf("\n\tInvalid choice. Please choose again.\n");
			}
		}while(delChoice!=1 && delChoice!=2);
	}else{
		printf("\n\t Borrower cannot be deleted. %d book/s unreturned.\n", delBorrower->borrowedBookCount);
	}
}

int searchBook(aBook *bookHead, int type){													//search by title or author
	int choice, bookID = 0;
	char searchString[50], upperCompareString[50];
	char *strptr;
	aBook *tempBook = bookHead, *tailBook, *newHead = NULL, *addBookToNew;

	printf("\n\t=== SEARCH BOOK ============== \n");
	
	if(bookHead!=NULL){
		printf("\t[1] By Title  \n");
		printf("\t[2] By Author \n");
		printf("\t[0] Back      \n");

		do{
			printf("\tPlease choose one: ");
			scanf("%d", &choice);

			if(choice==0) break;
			else if(choice<0||choice>2) printf("\n\tInvalid choice. Please choose again.\n");
			else{
		   		if(choice==1){
		   			printf("\n\tEnter the title (or a keyword from) of the book: ");
		   			getchar();
		   			scanf("%50[^\n]", searchString);

		   			while(tempBook!=NULL){
		   				strcpy(upperCompareString, tempBook->title);

		   				strptr = strstr(strupr(upperCompareString), strupr(searchString));
		   				
		   				if(strptr!=NULL){
		   					//add tempbook to newhead linked list
		   					addBookToNew = (aBook *) malloc(sizeof(aBook));
		   					
		   					addBookToNew->bookID = tempBook->bookID;
		   					strcpy(addBookToNew->title, tempBook->title);
		   					strcpy(addBookToNew->author.lastName, tempBook->author.lastName);
		   					strcpy(addBookToNew->author.firstName, tempBook->author.firstName);
		   					addBookToNew->category.num = tempBook->category.num;
		   					strcpy(addBookToNew->category.value, tempBook->category.value);
		   					addBookToNew->status.num = tempBook->status.num;
		   					strcpy(addBookToNew->status.value, tempBook->status.value);
		   					
		   					if(addBookToNew->status.num==2){
		   						strcpy(addBookToNew->borrower.studentName.lastName, tempBook->borrower.studentName.lastName);
		   						strcpy(addBookToNew->borrower.studentName.firstName, tempBook->borrower.studentName.firstName);
		   						addBookToNew->dateBorrowed.month = tempBook->dateBorrowed.month;
		   						addBookToNew->dateBorrowed.day = tempBook->dateBorrowed.day;
		   						addBookToNew->dateBorrowed.year = tempBook->dateBorrowed.year;
		   					}

		  					//insert sort by author
		  					insertSort(&newHead, &addBookToNew, NULL, NULL, 3);
		   				}
						
		   				tempBook = tempBook->next;
		   			}		   			
		   		}else{
		   			printf("\n\tEnter the author's firstname or lastname (can be incomplete): ");
		   			getchar();
		   			scanf("%50[^\n]", searchString);

		   			while(tempBook!=NULL){
		   				strcpy(upperCompareString, tempBook->author.lastName);
		   				strptr = strstr(strupr(upperCompareString), strupr(searchString));
		   				if(strptr==NULL){
		   					strcpy(upperCompareString, tempBook->author.firstName);
		   					strptr = strstr(strupr(upperCompareString), strupr(searchString));
		   				}

		   				if(strptr!=NULL){
		   					//add tempbook to newhead linked list
		   					addBookToNew = (aBook *) malloc(sizeof(aBook));
		   					
		   					addBookToNew->bookID = tempBook->bookID;
		   					strcpy(addBookToNew->title, tempBook->title);
		   					strcpy(addBookToNew->author.lastName, tempBook->author.lastName);
		   					strcpy(addBookToNew->author.firstName, tempBook->author.firstName);
		   					addBookToNew->category.num = tempBook->category.num;
		   					strcpy(addBookToNew->category.value, tempBook->category.value);
		   					addBookToNew->status.num = tempBook->status.num;
		   					strcpy(addBookToNew->status.value, tempBook->status.value);
		   					
		   					if(addBookToNew->status.num==2){
		   						strcpy(addBookToNew->borrower.studentName.lastName, tempBook->borrower.studentName.lastName);
		   						strcpy(addBookToNew->borrower.studentName.firstName, tempBook->borrower.studentName.firstName);
		   						addBookToNew->dateBorrowed.month = tempBook->dateBorrowed.month;
		   						addBookToNew->dateBorrowed.day = tempBook->dateBorrowed.day;
		   						addBookToNew->dateBorrowed.year = tempBook->dateBorrowed.year;
		   					}

		   					//add to new list
		   					if(newHead!=NULL){
		   						tailBook = newHead;
		   						while(tailBook->next!=NULL){
		   							tailBook = tailBook->next;
		   						}

		   						addBookToNew->next = tailBook->next;
		   						tailBook->next = addBookToNew;
		   					}else{
		   						newHead = addBookToNew;
		   						addBookToNew->next = NULL;
		   					}
		   				}
						
		   				tempBook = tempBook->next;
		   			}
		   		}

		   		if(newHead==NULL){
		   			printf("\n\tNo book matches your search keyword.\n");
		   			return bookID;
		   		}

		   		tempBook = newHead;
		   		int i = 0, bookChoice;
		   		while(tempBook!=NULL){
		   			if(choice==2) printf("\n\t[%d] Title: %s | Author: %s %s", i+1, tempBook->title, tempBook->author.firstName, tempBook->author.lastName);
		   			else printf("\n\t[%d] Author: %s %s | Title: %s", i+1, tempBook->author.firstName, tempBook->author.lastName, tempBook->title);
		   			tempBook = tempBook->next;
		   			i++;
		   		}

		   		do{
			   		printf("\n\tSelect one to view full details: ");
			   		scanf("%d", &bookChoice);

			   		if(bookChoice>i||bookChoice<1) printf("\n\tInvalid input. Please choose again.\n");
			   	}while(bookChoice>i||bookChoice<1);


		   		tempBook = newHead;
		   		for(int j=1; j<bookChoice; j++){
		   			tempBook = tempBook->next;
		   		}

		   		//display full details
		   		printf("\n\t||Title:         %s\n", tempBook->title);					
				printf("\t||Author:        %s %s\n", tempBook->author.firstName, tempBook->author.lastName);
				printf("\t||Book ID:       %d\n", tempBook->bookID);
				printf("\t||Category:      %s\n", tempBook->category.value);
				if(type!=1) printf("\t||Status:        %s\n", tempBook->status.value);
				
				if(tempBook->status.num==2 && type!=1){
					printf("\t||Borrower:      %s %s\n", tempBook->borrower.studentName.firstName, tempBook->borrower.studentName.lastName);
					printf("\t||Date Borrowed: %d-%d-%d\n", tempBook->dateBorrowed.month, tempBook->dateBorrowed.day, tempBook->dateBorrowed.year);
				}

		   		bookID = tempBook->bookID;
			}
		}while(choice<0||choice>2);
	}else{
		printf("\n\tEmpty Library. Please add a book first.\n");
	}

	return bookID;
}

void deleteBook(aBook **bookHead, int *totalBooks){														//delete book
	aBook *tempBook = *bookHead, *delBook = *bookHead;
	int bookChoice, delChoice, category, viewed, count = 0, deleteOption, bookID;

	printf("\n\t[1] Search Book\n");
	printf("\t[2] View Books\n");
	printf("\t[0] Back\n");
	
	do{
		printf("\n\tPlease choose one: ");
		scanf("%d", &deleteOption);

		if(deleteOption<0 || deleteOption>2) printf("\tInvalid choice. Please choose again.\n");
	}while(deleteOption<0 || deleteOption>2);

	if(deleteOption==0) return;
	else if(deleteOption==1){
		bookID = searchBook(*bookHead, 0);
		if(bookID==0) return;
		while(delBook->bookID!=bookID){
			tempBook = delBook;
			delBook = delBook->next;
		}
	}else{
		category = viewBooks(*bookHead, 1, &viewed);
		printf("\n\t[0] Back\n");
		
		do{
			printf("\n\tPlease choose one: ");
			scanf("%d", &bookChoice);

			if(bookChoice==0){
				return;
			}else if(bookChoice<0||bookChoice>viewed){
				printf("\n\tInvalid choice. Please choose again.\n");
			}else{
				while(count!=bookChoice){
					if(delBook->category.num==category ||category==6) count++;
					
					if(count!=bookChoice){
						tempBook = delBook;
						delBook = delBook->next;
					}else{
						break;
					}
				}
			}
		}while(bookChoice<0||bookChoice>viewed);
	}
	
	int bookCount = *totalBooks;

	if(delBook->status.num!=2){		
		printf("\n\t Are you sure you want to delete this book? ");
		printf("\n\t [1] Yes\n");
		printf("\t [2] No\n");
		
		do{
			printf("\t Please choose one: ");
			scanf("%d", &delChoice);

			if(delChoice==1){
				if(bookCount!=1 && delBook!=*bookHead){
					tempBook->next = delBook->next;
					free(delBook);
				}else{
					*bookHead = (*bookHead)->next;
					free(delBook);
				}

				*totalBooks = *totalBooks -1;
				printf("\n\t Book deleted successfully.\n");
			}else if(delChoice==2){
				printf("\n\t Book was not deleted.\n");
			}else{
				printf("\n\t Invalid choice. Please choose again.\n");
			}
		}while(delChoice!=1 && delChoice!=2);
	}else{
		printf("\n\t Book still borrowed and cannot be deleted.\n");
	}
}

void borrowBook(aBook **bookHead, aBorrower **borrowerHead){													//borrow book
	aBorrower *tempBorrower = *borrowerHead;
	aBook *tempBook = *bookHead;
	int borrower, bookChoice, borrowStatus = 1, category, count = 0, viewed, borrowOption, bookID;
	aDate dateBorrowed;

	//new option to search
	printf("\n\t[1] Search Book\n");
	printf("\t[2] View Books\n");
	printf("\t[0] Back\n");
	
	do{
		printf("\n\tPlease choose one: ");
		scanf("%d", &borrowOption);

		if(borrowOption<0 || borrowOption>2) printf("\tInvalid choice. Please choose again.\n");
	}while(borrowOption<0 || borrowOption>2);

	if(borrowOption==0) return;
	else if(borrowOption==1){
		bookID = searchBook(*bookHead, 1);
		if(bookID==0) return;
		while(tempBook->bookID!=bookID){
			tempBook = tempBook->next;
		}

		//is dis the book u want to borrow
	}else{
		category = viewBooks(*bookHead, 2, &viewed);
		if(viewed==0) return;

		printf("\n\t[0] Back\n");

		do{
			printf("\n\tPlease choose one: ");
			scanf("%d", &bookChoice);

			if(bookChoice<0||bookChoice>viewed){
				printf("\n\tInvalid choice. Please choose again.\n");
			}else if(bookChoice==0){
				return;
			}else{
				while(count!=bookChoice){
					if(tempBook->category.num==category ||category==6) count++;
					if(count!=bookChoice) tempBook = tempBook->next;
					else break;
				}
			}
		}while(bookChoice<1||bookChoice>viewed);
	}

	if(tempBook->status.num==2){
		printf("\n\tThis book is currently unavailable. See borrower details.\n");

		printf("\n\t== BORROWER DETAILS ========\n");
		printf("\t||Borrower Name:  %s %s\n", tempBook->borrower.studentName.firstName, tempBook->borrower.studentName.lastName);
		printf("\t||Student Number: %s\n", tempBook->borrower.studentNumber);
		printf("\t||Course:         %s\n", tempBook->borrower.course);
		printf("\t||Date Borrowed:  %d-%d-%d\n", tempBook->dateBorrowed.month, tempBook->dateBorrowed.day, tempBook->dateBorrowed.year);
	
		return;
	}

	borrowStatus = addBorrowerToBook(bookHead, &tempBook, *borrowerHead, &borrower, &dateBorrowed);

	if(borrowStatus==1){
		addBookToBorrower(tempBook, borrowerHead, borrower, dateBorrowed);
		
		printf("\n\tProcessing...\n");

		char clear[5];
		printf("\n\tPress any character: ");
		getchar();
		scanf("%20[^\n]", clear);

		printf("\n\tBook was borrowed successfully.\n");
	}
}

void compareDates(aDate dateBorrowed, aDate returnDate, aDate actualReturnDate, int *lapses){
	int daysPerMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int yearGapFromBorrow, yearGapFromReturn, leapYearCount = 0, initialDays = 0, toCompareDays = 0;

	yearGapFromBorrow = actualReturnDate.year - dateBorrowed.year;											//fxn comparing dates
	yearGapFromReturn = actualReturnDate.year - returnDate.year;

	if(yearGapFromBorrow<0){
		printf("Invalid Return Date. You cannot return a book before it was borrowed.\n");
		return; //samting
	}else{
		if(returnDate.year<=actualReturnDate.year){
			for(int i=returnDate.year; i<actualReturnDate.year; i++){
				if(i%4==0) leapYearCount = leapYearCount +1; 
			}

			for(int i=0; i<returnDate.month-1; i++){
				initialDays = initialDays + daysPerMonth[i];
			}
			initialDays = initialDays + returnDate.day;
			if(returnDate.year%4==0 && returnDate.month>2) initialDays = initialDays +1;

			for(int j=0; j<yearGapFromReturn; j++){
				toCompareDays = toCompareDays + 365;
			}
			for(int i=0; i<actualReturnDate.month-1; i++){
				toCompareDays = toCompareDays + daysPerMonth[i];
			}
			toCompareDays = toCompareDays + actualReturnDate.day + leapYearCount;
			if(actualReturnDate.year%4==0 && actualReturnDate.month>2) toCompareDays = toCompareDays +1;
			
			*lapses = toCompareDays - initialDays;
			printf("%d\n", *lapses);

		}else{
			for(int i=0; i<dateBorrowed.month-1; i++){
				initialDays = initialDays + daysPerMonth[i];
			}
			initialDays = initialDays + dateBorrowed.day;
			if(dateBorrowed.year%4==0 && (dateBorrowed.month>2 || (dateBorrowed.month==2 && dateBorrowed.day==29))) initialDays = initialDays +1;

			for(int i=0; i<actualReturnDate.month-1; i++){
				toCompareDays = toCompareDays + daysPerMonth[i];
			}
			toCompareDays = toCompareDays + actualReturnDate.day;
			if(actualReturnDate.year%4==0 && (actualReturnDate.month>2 || (actualReturnDate.month==2 && actualReturnDate.day==29))) toCompareDays = toCompareDays +1;

			if(toCompareDays-initialDays<0){
				printf("Invalid Return Date. You cannot return a book before it was borrowed.\n");
				return; //samting
			}else{
				printf("%d\n", toCompareDays-initialDays);
			}
		}
	}
}

void returnBook(aBook **bookHead, aBorrower **borrowerHead){
	int returnOption, existing, borrowerCount, valid, fines, choice, bookID;						//return book fxn
	char studentID[11];
	aBorrower *tempBorrower = *borrowerHead;
	aDate actualReturnDate;
	borrowedBooks *tailBookList, *tempBorrowedBook, *prevBorrowedBook;
	aBook *tempBook = *bookHead;

	printf("\n\t== RETURN BOOK ========\n");
	printf("\n\t[1] Search Borrower\n\t[2] View Borrowers\n\t[0] Back\n");

	do{
		printf("\n\tPlease choose one: ");
		scanf("%d", &returnOption);

		if(returnOption<0 || returnOption>2) printf("\tInvalid choice. Please choose again.");
	}while(returnOption<0 || returnOption>2);

	//ask for borrower info
	//search borrower, display then confirm
	if(returnOption==0) return;
	else if(returnOption==1){
		//search option
		printf("\n\tEnter Student Number/Borrower ID: ");
		scanf("%s", studentID);

		existing = searchBorrower(*borrowerHead, studentID, 2);
		if(existing==0){
			printf("\n\tBorrower ID not found.\n");
			return;
		}
		while(strcmp(studentID, tempBorrower->studentNumber)!=0){
			tempBorrower = tempBorrower->next;
		}

		if(tempBorrower->borrowedBookCount==0){
			printf("\n\tYou have no borrowed books to return.\n");
			return;
		}

		//is dis you?
	}else{
		//view option
		borrowerCount = viewBorrowers(*borrowerHead, 0);
		
		if(borrowerCount==0){
			return;
		}else{
			printf("\n\t[0] Not found\n");
			do{
				printf("\n\tSelect one: ");
				scanf("%d", &choice);

				if(choice<0||choice>borrowerCount){
					printf("\t Invalid choice. Please choose again.\n");
				}else if(choice==0){
					return;
				}else{
					for(int i=1; i<choice; i++){
						tempBorrower = tempBorrower->next;
					}

					if(tempBorrower->borrowedBookCount==0){
						printf("\n\tYou have no borrowed books to return.\n");
						return;
					}
				}
			}while(choice<0||choice>borrowerCount);
		}
	}
	//tempBorrower now points to the borrower

	//ask for current date
	do{
		printf("\n\tEnter Current Return Date (mm-dd-yyyy): ");
		scanf("%d-%d-%d", &actualReturnDate.month, &actualReturnDate.day, &actualReturnDate.year);

		valid = valiDate(actualReturnDate);
	}while(valid==0);

	//view borrowed books, user choice of book to return
	tailBookList = tempBorrower->bookList;
	int bookCount = 0, returnDisBook;
	if(tailBookList!=NULL){
		printf("\n\t==BORROWED BOOKS========\n");
		while(tailBookList!=NULL){
			printf("\n\t[%d] %s\n", bookCount+1, tailBookList->title);
			printf("\tBook ID:       %d\n", tailBookList->bookID);
			printf("\tDate Borrowed: %d-%d-%d\n", tailBookList->dateBorrowed.month, tailBookList->dateBorrowed.day, tailBookList->dateBorrowed.year);

			tailBookList = tailBookList->next;
			bookCount++;
		}

		printf("\n\t[0] Back\n");

		do{
			printf("\n\tWhich book would you like to return? ");
			scanf("%d", &returnDisBook);

			if(returnDisBook==0){
				printf("\n\tNo Book returned.\n");
				return;
			}else if((bookCount==0 && returnDisBook!=0) || (returnDisBook<0 || returnDisBook>bookCount)){
				printf("\n\tInvalid choice. Please choose again.");
			}
		}while((bookCount==0 && returnDisBook!=0) || (returnDisBook<0 || returnDisBook>bookCount));
	}

	tempBorrowedBook = prevBorrowedBook = tempBorrower->bookList;
	int position = 0;
	for(int i=1; i<returnDisBook; i++){
		prevBorrowedBook = tempBorrowedBook;
		tempBorrowedBook = tempBorrowedBook->next;
		position++;
	}

	//display fines
	compareDates(tempBorrowedBook->dateBorrowed, tempBorrowedBook->returnDate, actualReturnDate, &fines);
	if(fines<0){
		printf("\n\tYou did not exceed the maximum number of days allowed to borrow a book. You have no fines to pay.\n");
	}else{
		printf("\n\tYou have exceeded the maximum number of days allowed to borrow a book.");
		printf("\n\n\t||Expected Return Date (mm-dd-yyyy): %d-%d-%d", tempBorrowedBook->returnDate.month, tempBorrowedBook->returnDate.day, tempBorrowedBook->returnDate.year);
		printf("\n\t||Actual Return Date (mm-dd-yyyy):   %d-%d-%d", actualReturnDate.month, actualReturnDate.day, actualReturnDate.year);
		printf("\n\n\tPenalty Fee: Php %d.00 \n", fines);
	}
	
	bookID = tempBorrowedBook->bookID;

	//modify borrower booklist
	tempBorrower->borrowedBookCount--;
	if(bookCount!=1 && position!=1){
		prevBorrowedBook->next = tempBorrowedBook->next;
		free(tempBorrowedBook);
	}else{	//if only one or one in front
		tempBorrower->bookList = tempBorrowedBook->next;
		free(tempBorrowedBook);
	}

	//modify book
	while(tempBook->bookID!=bookID){
		tempBook = tempBook->next;
	}

	tempBook->status.num = 1;
	strcpy(tempBook->status.value, "Available");
	
	strcpy(tempBook->borrower.studentNumber, "\0"); 
	strcpy(tempBook->borrower.studentName.firstName, "\0"); 
	strcpy(tempBook->borrower.studentName.lastName, "\0"); 
	strcpy(tempBook->borrower.course, "\0"); 
	tempBook->borrower.borrowedBookCount = 0;
	tempBook->borrower.bookList = NULL;
	tempBook->borrower.next = NULL;

	tempBook->dateBorrowed.month = 0;
	tempBook->dateBorrowed.day = 0;
	tempBook->dateBorrowed.year = 0;

	printf("\n\tProcessing...\n");

	char clear[5];
	printf("\n\tPress any character: ");
	getchar();
	scanf("%20[^\n]", clear);
	//book return
	printf("\n\tBook was returned successfully.\n");
}


void saveBooksToFile(aBook *bookHead, int totalBooks){										//write to file
	FILE *writeFile;
	aBook *tempBook = bookHead;

	writeFile = fopen("books.txt", "w");

	fprintf(writeFile, "%d\n", totalBooks);
	while(tempBook!=NULL){
		fprintf(writeFile, "%s\n", tempBook->title);
		fprintf(writeFile, "%d\n", tempBook->bookID);
		fprintf(writeFile, "%s, %s\n", tempBook->author.lastName, tempBook->author.firstName);
		fprintf(writeFile, "%d\n", tempBook->category.num);
		fprintf(writeFile, "%d\n", tempBook->status.num);
		if(tempBook->status.num==2){
			fprintf(writeFile, "%s, %s\n", tempBook->borrower.studentName.lastName, tempBook->borrower.studentName.firstName);
			fprintf(writeFile, "%s\n", tempBook->borrower.studentNumber);
			fprintf(writeFile, "%s\n", tempBook->borrower.course);
			fprintf(writeFile, "%d-%d-%d\n", tempBook->dateBorrowed.month, tempBook->dateBorrowed.day, tempBook->dateBorrowed.year);
		}

		tempBook = tempBook->next;
	}
	fclose(writeFile);
}

void saveBorrowersToFile(aBorrower *borrowerHead, int totalBorrowers){						//write borrowers to file
	FILE *writeFile;
	aBorrower *tempBorrower = borrowerHead;
	borrowedBooks *tempBorrowedBook;

	writeFile = fopen("borrowers.txt", "w");

	fprintf(writeFile, "%d\n", totalBorrowers);
	while(tempBorrower!=NULL){
		fprintf(writeFile, "%s, %s\n", tempBorrower->studentName.lastName, tempBorrower->studentName.firstName);
		fprintf(writeFile, "%s\n", tempBorrower->studentNumber);
		fprintf(writeFile, "%s\n", tempBorrower->course);
		fprintf(writeFile, "%d\n", tempBorrower->borrowedBookCount);
		if(tempBorrower->borrowedBookCount!=0){
			tempBorrowedBook = tempBorrower->bookList;
			while(tempBorrowedBook!=NULL){
				fprintf(writeFile, "%d\n", tempBorrowedBook->bookID);
				fprintf(writeFile, "%s\n", tempBorrowedBook->title);
				fprintf(writeFile, "%d-%d-%d\n", tempBorrowedBook->dateBorrowed.month, tempBorrowedBook->dateBorrowed.day, tempBorrowedBook->dateBorrowed.year);
				fprintf(writeFile, "%d-%d-%d\n", tempBorrowedBook->returnDate.month, tempBorrowedBook->returnDate.day, tempBorrowedBook->returnDate.year);
				
				tempBorrowedBook = tempBorrowedBook->next;
			}	
		}

		tempBorrower = tempBorrower->next;
	}
	fclose(writeFile);
}
	
void loadBooks(aBook **bookHead, int *totalBooks){
	FILE *readFile;
	aBook *tempBook = *bookHead, *newBook;											//loadbooks fxn

	readFile = fopen("books.txt", "r");

	fscanf(readFile, "%d", totalBooks);
	if(*totalBooks!=0){
		for(int i=0; i<*totalBooks; i++){
			newBook = (aBook *) malloc(sizeof(aBook));

			fscanf(readFile, "%*c%50[^\n]%*c", newBook->title);
			fscanf(readFile, "%d", &newBook->bookID);
			fscanf(readFile, "%*c%50[^,]%*c %50[^\n]%*c", newBook->author.lastName, newBook->author.firstName);
			fscanf(readFile, "%d", &newBook->category.num);
			fscanf(readFile, "%d", &newBook->status.num);

			switch(newBook->category.num){
			case 1:
				strcpy(newBook->category.value, "General Reference");
				break;
			case 2:
				strcpy(newBook->category.value, "Journal");
				break;
			case 3:
				strcpy(newBook->category.value, "Magazine");
				break;
			case 4:
				strcpy(newBook->category.value, "Newspaper");
				break;
			default:
				strcpy(newBook->category.value, "Literary Works");
				break;
			}

			if(newBook->status.num==1){
				strcpy(newBook->status.value, "Available");
			}else{
				strcpy(newBook->status.value, "Borrowed");

				fscanf(readFile, "%*c%50[^,]%*c %50[^\n]%*c", newBook->borrower.studentName.lastName, newBook->borrower.studentName.firstName);
				fscanf(readFile, "%11[^\n]%*c", newBook->borrower.studentNumber);
				fscanf(readFile, "%50[^\n]%*c", newBook->borrower.course);
				fscanf(readFile, "%d-%d-%d", &newBook->dateBorrowed.month, &newBook->dateBorrowed.day, &newBook->dateBorrowed.year);
			}
			insertSort(bookHead, &newBook, NULL, NULL, 4);
		}
	}
	fclose(readFile);
}


void loadBorrowers(aBorrower **borrowerHead, int *totalBorrowers){
	FILE *readFile;
	aBorrower *tempBorrower = *borrowerHead, *newBorrower;
	borrowedBooks *newBorrowedBook, *tailBookList;												//loadborrowers

	readFile = fopen("borrowers.txt", "r");

	fscanf(readFile, "%d", totalBorrowers);
	if(*totalBorrowers!=0){
		for(int i=0; i<*totalBorrowers; i++){
			newBorrower = (aBorrower *) malloc(sizeof(aBorrower));

			fscanf(readFile, "%*c%50[^,]%*c %50[^\n]%*c", newBorrower->studentName.lastName, newBorrower->studentName.firstName);
			fscanf(readFile, "%20[^\n]%*c", newBorrower->studentNumber);
			fscanf(readFile, "%50[^\n]%*c", newBorrower->course);
			fscanf(readFile, "%d", &newBorrower->borrowedBookCount);

			if(newBorrower->borrowedBookCount!=0){
				for(int j=0; j<newBorrower->borrowedBookCount; j++){
					newBorrowedBook = (borrowedBooks *) malloc(sizeof(borrowedBooks));

					fscanf(readFile, "%d", &newBorrowedBook->bookID);
					fscanf(readFile, "%*c%50[^\n]%*c", newBorrowedBook->title);
					fscanf(readFile, "%d-%d-%d", &newBorrowedBook->dateBorrowed.month, &newBorrowedBook->dateBorrowed.day, &newBorrowedBook->dateBorrowed.year);
					fscanf(readFile, "%d-%d-%d", &newBorrowedBook->returnDate.month, &newBorrowedBook->returnDate.day, &newBorrowedBook->returnDate.year);
					
					if(j==0){
						newBorrower->bookList = newBorrowedBook;
						newBorrowedBook->next = NULL;
					}else{
						tailBookList = newBorrower->bookList;
						while(tailBookList->next!=NULL){
							tailBookList = tailBookList->next;
						}
						tailBookList->next = newBorrowedBook;
						newBorrowedBook->next = NULL;

					}
				}
			}else{
				newBorrower->bookList = NULL;
			}

			insertSort(NULL, NULL, borrowerHead, &newBorrower, 5);
		}
	}
	fclose(readFile);
}

void deallocate(aBook **bookHead, aBorrower **borrowerHead){
	aBook *tempBook;																//deallocation
	aBorrower *tempBorrower;
	borrowedBooks *tempBorrowedBook;

	while(*bookHead!=NULL){
		tempBook = *bookHead;
		*bookHead = (*bookHead)->next;
		free(tempBook);
	}

	while(*borrowerHead!=NULL){
		while((*borrowerHead)->bookList!=NULL){
			tempBorrowedBook = (*borrowerHead)->bookList;
			(*borrowerHead)->bookList = (*borrowerHead)->bookList->next;
			free(tempBorrowedBook);
		}
		tempBorrower = *borrowerHead;
		*borrowerHead = (*borrowerHead)->next;
		free(tempBorrower);
	}
}