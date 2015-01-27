/*
 authors:
 -Cristina Parasiliti Parracello
 -Dario Safarally
 -Roberta Maccarrone
 */

/*function that pings all address within the database*/
int ping()
{
    char ping[PING_MAX_LENGTH];
    MYSQL_RES *result;
    sprintf(query_select_domain_part,"SELECT domain_part FROM Domain_part;");
    if (mysql_query(&mysql,query_select_domain_part)==0)
    {
        result = mysql_store_result(&mysql);
        if(result==NULL)
        {
            printf("\n Error: result = null \n");
        }
        else
        {	
            MYSQL_ROW row;
	    /*row contains domain_part */
            while ((row = mysql_fetch_row(result)))
            {
                sprintf(ping,"ping -c 1 -w 5 www.%s",row[0]); /* -c: stop after sending count , -w: specify  a  timeout in seconds */
		gettimeofday(&start_time,NULL);
                if(system (ping)!=0) 
		{   /*error occurred */
                    printf("\n %s: NO PONG! \n",row[0]);
                    sprintf(query_insert_flag_domain_part_A,"UPDATE Address SET invalid_domain='1' WHERE domain_part='%s';",row[0]);
		    sprintf(query_insert_flag_domain_part_Dp,"UPDATE Domain_part SET invalid_domain='1' WHERE domain_part='%s';",row[0]);
		    
		    
		    /*set invalid_domain=1 if server doesn't respond or doesn't exist */
                    if((mysql_query(&mysql,query_insert_flag_domain_part_A)==0) && (mysql_query(&mysql,query_insert_flag_domain_part_Dp)==0))
                    {
                        printf("\n %s: invalid domain. Flag inserted! \n",row[0]);
                    }
                    else
                    {
                        printf("\n query error \n");
                    }
                }
                else
		  /*server responds */
                    printf("\n%s: PONG! \n",row[0]);
		    time(start_time);
            }
        }
       
        count_ping_no_respons();
        return 1;
    }
    else
    {
        printf("\n select query error \n");
       
    }
    return 0;
}

/*counts the number of servers that do not respond after being pinged */
void count_ping_no_respons(){
  MYSQL_RES *result;
  sprintf(query_count_flag_domain_part, "SELECT count(domain_part) FROM Domain_part WHERE invalid_domain='1';");
  if(mysql_query(&mysql,query_count_flag_domain_part)==0){
    result = mysql_store_result(&mysql);
        if (result == NULL)
            printf("\n Error: result = null\n");
    else {
        MYSQL_ROW row = mysql_fetch_row(result);
	  printf(" number of servers that do not respond %s\n",row[0]);
}
  }
  else 
    printf("\n query error \n");
}
