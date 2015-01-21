
/*check if user's choice is right */
void check_selection(char *choice, char n)
{
    int wrong=0;
    do
    {
        if(strlen(choice)==1 && choice[0]>='1' && choice[0]<=n)
        {
            wrong=1;
        }
        else
        {
            printf("\n Wrong Selection! Range 1 to %c ==> ",n);
            scanf("%s",choice);
        }
    }
    while(wrong!=1);
}

/*checks if there is at least an email that starts with "ch" */
int get_nr_addresses_start_with(char ch)
{
    int returnValue;
    MYSQL_RES *result;
    sprintf(query_nr_addresses_start_with,"SELECT count(address_id) FROM Address WHERE local_part like '%c%';",ch);
    if(mysql_query(&mysql,query_nr_addresses_start_with)==0) 
    {
        result = mysql_store_result(&mysql);
        if (result == NULL)
        {
            printf("\n Error: result = null\n");
            returnValue=-1;
        }
        
        /*row [0]=email's number that start with "ch"*/
        MYSQL_ROW row = mysql_fetch_row(result);

        if(strcmp(row[0],"0")==0) 
        {
            printf("\n No address starts with %c!\n",ch);
            returnValue=0;
        }
        else
        {
            returnValue=atoi(row[0]);
        }
    }
    else
    {
        printf("\n query error: %s\n",mysql_error(&mysql));
        returnValue=-1;
    }
    mysql_free_result(result);
    return returnValue;
}

/*converts characters from uppercase to lowercase */
void str_to_lower(char* str)
{
    int i;
    for(i=0; i<strlen(str); i++)
    {
        str[i]=tolower(str[i]);
    }
}

/*extracts the extension*/
char *get_ext(char *filename)
{
    char *dot = strrchr(filename, '.'); /*copy the last occurrence of '.' in dot*/
    if(!dot || dot == filename) return ""; /*if dot contains filename return empty*/
    return dot + 1; /*else return file format*/
}

//checks if database is empty
int db_is_empty()
{
    int returnValue;
    MYSQL_RES *result;
    if(mysql_query(&mysql,query_db_is_empty)==0)
    {
        result = mysql_store_result(&mysql);
        if (result == NULL)
        {
            printf("\n Error: result = null\n");
            returnValue=-1;
        }
        /*row[0]= email's number into database*/
        MYSQL_ROW row = mysql_fetch_row(result);
        if(strcmp(row[0],"0")==0)
        {
            printf("\n Database is empty!\n");
            returnValue=1;
        }
        else
            returnValue=0;

    }
    else
    {
        printf("\n query error: %s\n",mysql_error(&mysql));
        returnValue=-1;
    }
    mysql_free_result(result);
    return returnValue;


}

void print_error(char *error)
{
    printf("\n==========================================="
           "\n %s"
           "\n===========================================\n",error);
    exit(-1);
}
