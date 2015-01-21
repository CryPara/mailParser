/* check the validation of an address */
int validate(char *address)
{

    char tmp_address[strlen(address)];
    char localpart[LOCAL_PART_MAX_LENGTH]; /*localpart to address  */
    char domainpart[DOMAIN_PART_MAX_LENGTH]; /*domainpart to address */
    char tmp_domainpart[DOMAIN_PART_MAX_LENGTH];
    char tmp_domain[DOMAIN_PART_MAX_LENGTH];
    char *tmp;
    exc=fopen("warning.txt", "w"); /*file warning contentent address with long localpart*/
    char er[]="^[_a-zA-Z0-9-]+(\\.[_a-zA-Z0-9-]+)*@[a-zA-Z0-9-]+(\\.[a-zA-Z0-9-]+)*(\\.[a-zA-Z]{2,13})$"; /*regular expression */
    int returnValue=0;

    if(match(address,er)==1)
    {

        strcpy(tmp_address,address);
        strcpy(localpart,strtok(tmp_address,"@"));
        strcpy(domainpart,strtok(NULL,"@"));

	/*check if address is valide */
        if(strlen(localpart)>=LOCAL_PART_MIN_LENGTH && strlen(localpart)<=LOCAL_PART_MAX_LENGTH &&       /* check if 1<=localpart<=64 */
                strlen(domainpart)>=DOMAIN_PART_MIN_LENGTH && strlen(domainpart)<=DOMAIN_PART_MAX_LENGTH)  /* check if 1<=domainpart<=25 */
        {


	    /*check if domainpart esist */
            dm=fopen("top_level_domain.txt", "r"); /*opens the file containing all the top level domains */
            if(dm==NULL) 
            {
                print_error("\n error when opening file: top_level_domain.txt \n"); 
            }

            strcpy(tmp_domainpart,domainpart);

            for(tmp=strtok(tmp_domainpart,"."); tmp!=NULL; tmp=strtok(NULL,".")) /*extract the domain from domainpart */
            {
                strcpy(tmp_domain,tmp);  
            }

            while(!feof(dm))
            {
                fscanf(dm,"%s\n", buf2); 
                if(strcmp(tmp_domain,buf2)==0)  /*check if the domain is present in top_level_domain.txt */
                {
                    if (strlen(localpart)>LOCAL_PART_WARNING_THRESHOLD) /*check if the localpart>25*/
		      
                        fprintf (exc, "%s\n", address); /*write the address in warning.txt */
                    returnValue=1;
                }

            }
            fflush(dm);
            fclose(dm); 
        }
    }
    fflush(exc);
    fclose(exc); 
    return returnValue;
}

int match (const char *s,char * regex)
{

    regex_t re;

    if(regcomp(&re,regex,REG_EXTENDED|REG_NOSUB) != 0) /*compiles a pattern regex_t associated with my regex */
        return -1;

    if(regexec(&re,s,(size_t) 0, NULL, 0)) /* apply the pattern to the string passed (address)*/
    {
        regfree(&re);
        return 0; /* match not found: address is not valid */
    }
    regfree(&re);
    return 1; /* match found: address is valid */

}