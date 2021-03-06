#include "DirOps.h"

int copy(char* from, char* to){
   char ch, source_file[PATH_MAX], target_file[PATH_MAX];
   FILE *source, *target;

   source = fopen(from, "r");

   if( source == NULL )
   {
     printf("Could not locate file to copy: %s\n", from);
     return -1;


   }

   /* Set to w+ because we want to overwrite the contents of the file   */
   target = fopen(to, "w+");

   if( target == NULL )
   {
     printf("Could not locate destination at: %s\n", to);
     fclose(source);
     return -1;
   }

   while( ( ch = fgetc(source) ) != EOF )
      fputc(ch, target);


   fclose(source);
   fclose(target);
   return 0;

}

int remove_directory(const char *path)
{
   DIR *d = opendir(path);
   size_t path_len = strlen(path);
   int r = -1;
   if (d)
   {
      struct dirent *p;

      r = 0;

      while (!r && (p=readdir(d)))
      {
          int r2 = -1;
          char *buf;
          size_t len;

          /* Skip the names "." and ".." as we don't want to recurse on them. */
          if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
          {
             continue;
          }

          len = path_len + strlen(p->d_name) + 2; 
          buf = malloc(len);

          if (buf)
          {
             struct stat statbuf;

             snprintf(buf, len, "%s/%s", path, p->d_name);

             if (!stat(buf, &statbuf))
             {
                if (S_ISDIR(statbuf.st_mode))
                {
                   r2 = remove_directory(buf);
                }
                else
                {
                   r2 = unlink(buf);
                }
             }

             free(buf);
          }

          r = r2;
      }

      closedir(d);
   }

   if (!r)
   {
      r = rmdir(path);
   }

   return r;
}

void ls( char* dir_name, void (*lambda)(char*) )
{
     struct dirent *de;  // Pointer for directory entry
 
    // opendir() returns a pointer of DIR type. 
    DIR *dr = opendir(dir_name);
 
    if (dr == NULL)  // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory" );
    }
 
    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html
    // for readdir()
    while ((de = readdir(dr)) != NULL) {
        if ( strstr(de->d_name, ".vim") != NULL ) {
            (*lambda)(de->d_name);
        }
    } 
    closedir(dr);     
}

