/*create the database*/
int create_db()
{
    sprintf(query_create_database,"CREATE DATABASE IF NOT EXISTS %s;",db_name);
    if(mysql_query(&mysql,query_create_database)==0)
    {
        printf("\n Database Created ");
        return 1;

    }
    printf("\n Failed to create new database.  Error: %s\n",mysql_error(&mysql));
    return 0;

}

/* select the database */
int select_db()
{
    int select,nr_try=1;
    do{
        if ((select=mysql_select_db (&mysql, db_name))!=0)
        {
            printf ("\n Error: bad selection -> Insert db_name: ");
            scanf("%s",db_name);
        }
        nr_try++;

    }while(select!=0 && nr_try!=3);

    if(select==0)
        return 1;

    return -1;
}

/*create Address table and Domain_part table */
void create_table()
{
    if(mysql_query(&mysql,query_create_table)==0 && mysql_query(&mysql,query_create_domain_table)==0)
        printf( "\n Table Created\n");
    else
    {
        print_error("Failed to create new table Address\n");
    }
}

