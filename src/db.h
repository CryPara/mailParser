/*
 authors:
 -Cristina Parasiliti Parracello
 -Dario Safarally
 -Roberta Maccarrone
 */

int create_db();  /*create the database */
int select_db();  /*select the database */
void create_table(); /*create the database table */
void delete_address(char *address); /*delete an address within the database */
int search_mail(char *address); /*search an address within the database */
void add_address(char *addr); /*add address within the database */
void check_selection(char *choice,char n); /* check the user's selection */
int validate(char *address); /* check the validation of an address */
int match(const char *s,char * regex); /*check the match between the regular expression and address format */
void str_to_lower(char* str); /*convert characters from uppercase to lowercase */
void parse_file(char *f_path);
char *get_ext(char *filename);
void read_directory(char *name_directory); /*read directory to file*/
void delete_addresses_from_file(char *file_path); /*read a file and delete addresses from database */ 
int export_db(); /*save address within the database*/
int save(char ch); /*save addresses in alphabetical order*/
int get_nr_addresses_start_with(char ch);/*checks if there is at least an email that starts with "ch" */
int db_is_empty();/*check if database is empty */
int ping(); /*pings of an address */
void save_all(); /*saves all the addresses in the database in a single all_email.txt */
void print_error(char *error);
void count_ping_no_respons();/*counts the number of servers */

/*read the file contains on a directory*/
void read_directory(char *name_directory)
{
    DIR *d;
    struct dirent *dir;
    d = opendir(name_directory);
    char path[MALLOC],filename[MALLOC],fileformat[MALLOC];/*variables used for parsing*/
    if (d != NULL)
    {
        while ((dir = readdir(d)) != NULL)
        {
            strcpy(filename,dir->d_name);
            if(filename[0] == '.') continue;/* "." is directory father, when filename contains "." continue the execution*/
            strcpy(fileformat,get_ext(filename));
            if(strcmp(fileformat,"rtf") == 0 || strcmp(fileformat,"doc") ==0 ||strcmp(fileformat,"txt") ==0 ||strcmp(fileformat,"csv")== 0)/*control extension of fileformat*/
            {

                strcpy(path,name_directory);
                strcat(path,"/");
                strcat(path,filename);
                parse_file(path);

            }
        }

    }
    else
    {
        printf("\n errore lettura cartella \n");
    }
}

/*parses the file */
void parse_file(char *f_path)
{

    char *tmp;
    fd=fopen(f_path, "r");

    printf("\n\n Processing file: %s\n", f_path);

    if(fd==NULL) 
    {
        printf("\n error when opening a file \n");
    }
    else
    {
        while(!feof(fd)) 
        {
            fscanf(fd,"%s\n", buf); 
            for(tmp=strtok(buf,delimiters); tmp!=NULL; tmp=strtok(NULL,delimiters)) /*selects each string separated by delimiters*/
	      
            { /*each string is passed to the function validate */
                if(validate(tmp)==1) /*check if the string is a valid address */
                    add_address(tmp); /* add the address to the database */
            }
        }
        fflush(fd);
        fclose(fd);
    }
}


/*add an address */
void add_address(char *addr) 
{

    str_to_lower(addr); 
    char tmp_addr[strlen(addr)];
    strcpy(tmp_addr,addr);
    char *local_part = strtok(tmp_addr,"@");
    char *domain_part= strtok(NULL,"@");
    
    sprintf(query_insert_address,"INSERT INTO Address (address_id,local_part,domain_part) VALUES ('%s','%s','%s');",addr,local_part,domain_part);
    sprintf(query_insert_domain_part,"INSERT INTO Domain_part (domain_part) VALUES ('%s');",domain_part);
    
    /*inserts an address in the Address table */
    if(mysql_query(&mysql,query_insert_address)==0 ) 
    {
        //printf("\n Added : %s",addr);
	
	/*insert address's domain_part in the Domain_part table */ 
	mysql_query(&mysql,query_insert_domain_part);

    }
    //else
    //{
        //printf("\n Failed to insert: %s",addr);
    //}

}
/*delete an address */

void delete_address(char *address) 
{
    sprintf(query_delete_address, "DELETE FROM Address WHERE address_id='%s';",address);
    if(search_mail(address)==1) /*if the address is present in the database */
    {
        if(mysql_query(&mysql,query_delete_address)==0)
        {
            printf("\n%s : deleted\n",address);
        }
        else
        {
            
            printf("\n Delete Error \n");
        }
    }
}

/*read a file and delete addresses from database */

void delete_addresses_from_file(char *file_path) 
{
    char *tmp;
    fd=fopen(file_path, "r"); 

    if(fd==NULL)

    {
        print_error("\n error when opening file \n"); 
    }
    while(!feof(fd))
    {
        fscanf(fd,"%s\n", buf);
        for(tmp=strtok(buf,delimiters); tmp!=NULL; tmp=strtok(NULL,delimiters)) /*select each address separated by delimiters */
        {
            delete_address(buf);/*delete addresses from database */
        }
    }
    fflush(fd);
    fclose(fd);

}



/*search an address */
int search_mail(char* address)
{
    int returnValue;
    MYSQL_RES *result;
    sprintf(query_search_address, "SELECT count(address_id) FROM Address WHERE address_id='%s';", address);
    if(mysql_query(&mysql,query_search_address)==0)
    {
        result = mysql_store_result(&mysql);
        if (result == NULL)
        {
            printf("\n Error: result = null\n");
            returnValue=-1;
        }
        MYSQL_ROW row = mysql_fetch_row(result);
        if(strcmp(row[0],"1")==0)
        {
            printf("\n It is present!");
            returnValue=1;
        }
        else
        {
            printf("\n Does not exist!");
            returnValue=0;
        }
    }
    else
    {
        printf("\n search query error \n");
        // printf("\n search query error: %s\n",mysql_error(&mysql));
        returnValue=-1;
    }
    mysql_free_result(result);
    return returnValue;
}


/* Save all emails in a folder called "db_name_valid_addresses" */
int export_db()
{
    int returnValue;
    if(db_is_empty()==0)
    {
        int val_09,val_az;
        char path[PATH_MAX_LENGTH];
        printf("\n Type path: ");
        scanf("%s",path);
        strcat(path,"/");
        strcat(path,db_name);
        strcat(path,"_valid_addresses");

        if(mkdir(path,S_IRWXU)==0 && chdir(path)==0)
        {   gettimeofday(&start_time,NULL);
            int i,count_09=0,count_az=0;

            for(i='0'; i<='9'; i++)
            {
                val_09=save((char)i);
                if(val_09!=1)
                    count_09++;
            }
            for(i='a'; i<='z'; i++)
            {
                val_az=save((char)i);
                if(val_az!=1)
                    count_az++;
            }
            if(count_09<10 || count_az<26)
                returnValue=1; /*there is at least an email that starts with 0-9 or az */
            else
                returnValue=-1; /*there is "mysql error" */
                  time(start_time);
        }
        else
        {
            returnValue=0;
            printf("\n directory creation error \n");
        }

    }
    else
    {
        returnValue=-1;
    }
    return returnValue;
}


/*save addresses in alphabetical order into file of 500 rows*/
int save(char ch)
{
    int returnValue;
    int nr_addresses_start_with=get_nr_addresses_start_with(ch);
    if(nr_addresses_start_with!=0 && nr_addresses_start_with!=(-1))
    {
        FILE *valid_addresses;
        char filename[FILENAME_LENGTH];
        char tmp_path[PATH_MAX_LENGTH];
        MYSQL_RES *result;
        sprintf(tmp_path,"./%c/",ch);
        sprintf(query_save_address,"SELECT address_id FROM Address WHERE local_part like '%c%';",ch);

        if(mkdir(tmp_path,S_IRWXU)==0)
        {
            if(mysql_query(&mysql,query_save_address)==0)
            {
                result = mysql_store_result(&mysql);
                if(result==NULL)
                {
                    printf("\n Error: result = null \n");
                    returnValue=-1;
                }
                else
                {
                    MYSQL_ROW row;
                    int count=0,n=0;
                    while (row = mysql_fetch_row(result))
                    {
                        if((count%500)==0)
                        {
                            if(count!=0)
                            {
                                fflush(valid_addresses);
                                fclose(valid_addresses);
                            }
                            sprintf(filename,"%c/file_%c_%d.txt",ch,ch,n);
                            valid_addresses=fopen(filename,"w");
                            n++;
                        }
                        if(row[0]!=NULL)
                        {
                            fprintf(valid_addresses, "%s\n", row[0]);
                        }
                        count++;
                    }
                    fflush(valid_addresses);
                    fclose(valid_addresses);
                    returnValue=1;
                }
                mysql_free_result(result);
            }
            else
            {
                printf("\n query error: %s\n",mysql_error(&mysql));
                returnValue=-1;
            }
        }
        else
        {
            returnValue=-1;
            printf("\n directory \"%c\" creation error \n",ch);
        }
    }
    else
    {
        returnValue=-1;
    }
    return returnValue;
}




/*export all emails in a all_email.txt */
void save_all()
{
    MYSQL_RES *result;
    FILE *all_valid;
    char query_save[QUERY_MAX_LENGTH]="SELECT address_id FROM Address;";
    if(mysql_query(&mysql,query_save)==0)
    {
        result = mysql_store_result(&mysql);
        if(result==NULL)
        {
            printf("\n Error: result = null \n");
        }
        else
        {
            MYSQL_ROW row;
            all_valid=fopen("./all_email.txt","w");
            while ((row = mysql_fetch_row(result)))
            {
                if(row[0]!=NULL)
                {
                    fprintf(all_valid, "%s\n", row[0]); /*write all address in a file.txt */
                }
            }
            fflush(all_valid);
            fclose(all_valid);
        }
        mysql_free_result(result);
    }
    else
    {
        printf("\n query error: %s\n",mysql_error(&mysql));
    }
}
