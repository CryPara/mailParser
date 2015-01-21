/* 
authors 
-Maccarrone Roberta
-Dario Safarally
-Parasiliti Parracello Cristina
*/


#include "config.h"
#include "db.h"
#include "init_db.h"
#include "validate.h"
#include "ping.h"
#include "service.h"


void connection();
void menu();


int main()
{
    connection();
    menu();
    exit(0);
}

void connection()
{

    printf("\n___________________________________________"
           "\n*******************************************"
           "\n 1. Create Mail Database"
           "\n 2. Connect to the existing Mail Database"
           "\n\n Enter 1 or 2 ==> ");

    scanf("%s",first_choice); 
    check_selection(first_choice,'2'); 

    printf("___________________________________________"
           "\n*******************************************");
    printf("\n Enter hostname: ");
    scanf("%s", host);
    printf("\n Enter username: ");
    scanf("%s", user);
    printf("\n Enter password: ");
    scanf("%s", passw);

    /*initiates the connection */
    if(mysql_init(&mysql)==NULL)
    {
        print_error("Failed to initiate MySQL connection");
    }
    if (mysql_real_connect (&mysql, host, user, passw, "",0, NULL, 0)==NULL)
    {
        print_error("Bad Connection");
    }
    printf("\n Succeeded to initiate MySQL Connection"
           "\n___________________________________________"
           "\n*******************************************"
           "\n Enter db_name: ");

    scanf("%s",db_name);

    if(first_choice[0]=='1')   /*creates a new database and new table */
    {
        if(create_db()==1)
        {
            if(select_db()==1)
            {
                create_table();
            }
            else
            {
                print_error("Error. Exit...");
            }
        }
        else
        {
            print_error("Error. Exit...");
        }
    }
    else if(first_choice[0]=='2')       /*selects an existing database */
    {
        if(select_db()==1)
        {
            printf ("\n Connected to %s!",db_name);
        }
        else
        {
            print_error("Error. Exit...");
        }
    }
}

void menu()
{

    int exitValue=0;
    char address[EMAIL_MAX_LENGTH];


    do
    {
        printf("\n___________________________________________"
               "\n*******************************************"
               "\n 1. Import email addresses from files"
               "\n 2. Insert an email address"
               "\n 3. Delete an email address"
               "\n 4. Delete email addresses from file"
               "\n 5. Search an email"
               "\n 6. Export the database to .txt files"
               "\n 7. PING: check which mail servers respond"
               "\n 8. Exit"
               "\n\n Enter 1, 2, 3, 4 , 5 , 6 , 7 or 8 ==> ");

        scanf("%s",second_choice);
        check_selection(second_choice,'8');

        printf("\n___________________________________________"
               "\n*******************************************");

        if(second_choice[0]=='1') /*import email addresses from files */
        {
            printf("\n 1.Select file path: ");
            scanf("%s",file_path);
            strcpy(fileformat,get_ext(file_path));
            if(strcmp(fileformat,"rtf") == 0 || strcmp(fileformat,"doc") ==0 ||strcmp(fileformat,"txt") ==0 ||strcmp(fileformat,"csv")== 0)
                parse_file(file_path);
            else
                read_directory(file_path);

        }
        else if(second_choice[0]=='2') /*insert an email address from keyboard*/
        {
            printf("\n Enter an e-mail: ");
            scanf("%s",address);
            if(validate(address)==1)
                add_address(address);
            else
                printf("\n Failed: format is not valid!");

        }
        else if(second_choice[0]=='3') /*delete an address from keyboard*/
        {
            printf("\n Enter email address to delete: ");
            scanf("%s",address);
            delete_address(address);

        }
        else if(second_choice[0]=='4') /*delete all addresses from file */
        {
            printf("\n 1.Select file path: ");
            scanf("%s",file_path);
            strcpy(fileformat,get_ext(file_path));
            if(strcmp(fileformat,"rtf") == 0 || strcmp(fileformat,"doc") ==0 ||strcmp(fileformat,"txt") ==0 ||strcmp(fileformat,"csv")== 0)
               delete_addresses_from_file(file_path);

            else
                printf("\n file format not valid");

        }
        else if(second_choice[0]=='5') /*search an address */
        {
            printf("\n Enter email address to search: ");
            scanf("%s",address);
            search_mail(address);

        }
        else if (second_choice[0]=='6') /*save all address in the directory */
        {
            if(export_db()==1){
		save_all();
                printf("\n Done! \n");
	    }
		
            else printf("\n Failed! \n");
        }
        else if (second_choice[0]=='7') /*PING: check which mail servers respond */
        {
            if(ping()!=1)
                printf("\n ping error \n");
        }

        else if(second_choice[0]=='8') /*disconnection of the database */
        {
            
            exitValue=8;
	    mysql_close(&mysql);
            printf("\n ############# Disconnected #############"
                   "\n___________________________________________"
                   "\n*******************************************\n");
	    
        }

    }
    while(exitValue!=8);

}

