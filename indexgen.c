#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dirent.h>
#include <errno.h>


void genindex(char *dir_name)
{
    DIR * d;
//    char * dir_name = ".";
    char path[99999];

    strcpy(path, dir_name);
    strcpy(&path[strlen(dir_name)], "index.html");
    FILE *fp = fopen(path, "r");

    /* Open the current directory. */
    if (fp){
            printf("index.html exist\n");
            fclose(fp);

     }
    
    else{
            fp = fopen(path, "w");
            d = opendir (dir_name);



            if (! d) {
                fprintf (stderr, "Cannot open directory '%s': %s\n",
                         dir_name, strerror (errno));
                exit (EXIT_FAILURE);
            }
            while (1) {
                struct dirent * entry;
     
                entry = readdir (d);
                if (! entry) {
                    break;
                }
		if (strncmp(entry->d_name, "index.html\0", 11)!=0 ){ 
                fprintf (fp, "<a href=\"%s\">%s</a><br>\n", entry->d_name, entry->d_name);
		}
            }
            /* Close the directory. */
            if (closedir (d)) {
                fprintf (stderr, "Could not close '%s': %s\n",
                         dir_name, strerror (errno));
                exit (EXIT_FAILURE);
            }
            fclose(fp);
            }
}
