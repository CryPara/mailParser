#include <stdlib.h>
#include <stdio.h>
#include <mysql/mysql.h>
#include <string.h>
#include <regex.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
/*
 authors:
 -Cristina Parasiliti Parracello
 -Dario Safarally
 -Roberta Maccarrone
 */

#include <unistd.h>
#include <sys/time.h>

#define BUFFER_LENGTH 200               
#define EMAIL_MAX_LENGTH 90
#define LOCAL_PART_MIN_LENGTH 1
#define LOCAL_PART_MAX_LENGTH 64
#define LOCAL_PART_WARNING_THRESHOLD 25
#define DOMAIN_PART_MIN_LENGTH 1
#define DOMAIN_PART_MAX_LENGTH 25
#define HOST_MAX_LENGTH 256             
#define DATABASE_NAME_MAX_LENGTH 256    
#define USERNAME_MAX_LENGTH 256         
#define PASSWORD_MAX_LENGTH 256         
#define FILE_PATH_MAX_LENGTH 255        
#define PATH_MAX_LENGTH 500             
#define FILENAME_LENGTH 15
#define QUERY_MAX_LENGTH 500            
#define MALLOC 100
#define PING_MAX_LENGTH 100

struct timeval start_time;
MYSQL mysql;
FILE *fd;
FILE *dm;
FILE *exc;
char delimiters[]="\\,<\">:;[](){} ";
char host[HOST_MAX_LENGTH];
char user[USERNAME_MAX_LENGTH];
char passw[PASSWORD_MAX_LENGTH];
char db_name[DATABASE_NAME_MAX_LENGTH];
char file_path[FILE_PATH_MAX_LENGTH];
char file_path_delete[FILE_PATH_MAX_LENGTH];
char buf[BUFFER_LENGTH];
char buf2[BUFFER_LENGTH];
char fileformat[MALLOC];
char query_create_database[QUERY_MAX_LENGTH];
char query_insert_address[QUERY_MAX_LENGTH];
char query_insert_domain_part[QUERY_MAX_LENGTH];
char query_insert_flag_domain_part_Dp[QUERY_MAX_LENGTH];
char query_insert_flag_domain_part_A[QUERY_MAX_LENGTH];
char query_delete_address[QUERY_MAX_LENGTH];
char query_search_address[QUERY_MAX_LENGTH];
char query_save_address[QUERY_MAX_LENGTH];
char query_nr_addresses_start_with[QUERY_MAX_LENGTH];
char query_db_is_empty[]="SELECT count(address_id) FROM Address;";
char query_show_tables_like[]="SHOW TABLES LIKE 'Address';";
char query_create_table[]="CREATE TABLE Address (address_id varchar(150) NOT NULL,local_part varchar(70) NOT NULL,domain_part varchar(25) NOT NULL,invalid_domain int(1),PRIMARY KEY (address_id)) ENGINE=InnoDB DEFAULT CHARSET=ascii;";
char query_create_domain_table[]="CREATE TABLE Domain_part(domain_part varchar (25) NOT NULL, invalid_domain int(1),PRIMARY KEY (domain_part)) ENGINE=InnoDB DEFAULT CHARSET=ascii;";
char query_select_domain_part[QUERY_MAX_LENGTH];
char query_count_flag_domain_part [QUERY_MAX_LENGTH];
char first_choice[]="0",second_choice[]="0";