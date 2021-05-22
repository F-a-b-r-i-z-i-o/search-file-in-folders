#include "definizioni.h"

#if !defined(MAXFILENAME)
#define MAXFILENAME 2048
#endif

//Macro chiamate sistema
#define SYSCALL(name, systemcall, returnval, check, str, ...) \
    if ((returnval = systemcall) == check)                   \
    {                                       \
        perror(#name);                      \
        int errno_copy = errno;             \
        print_error(str, __VA_ARGS__);      \
        exit(errno_copy);                   \
    }



//Funzione che gestisce erorri utilizzata da SYSCALL 
void print_error(const char *str, ...){
    const char err[] = "[ERRORE] ";
    va_list argp;
    char *p = (char *)malloc(strlen(str) + strlen(err) + 512);
    if (!p)
    {
        perror("malloc");
        // fprintf(stderr, "FATAL ERROR nella funzione 'print_error'\n");
        return;
    }
    strcpy(p, err);
    strcpy(p + strlen(err), str);
    va_start(argp, str);
    vfprintf(stderr, p, argp);
    va_end(argp);
    free(p);
}

int isDot(const char dir[]){
    return (strlen(dir) > 0 && dir[strlen(dir) - 1] == '.');
}

//Funzione ricorsiva principale che mi permette di cercare nel sotto albero radicato.
void Esplorazione(const char nomedirectory[], const char es[]){
    
    struct stat statbuf;
    int dummy_return_val;
    SYSCALL(stat, stat(nomedirectory, &statbuf), dummy_return_val, -1, "NON e' stata trovata nessuna directory %s: errno=%d\n", nomedirectory, errno);

    //controllo argomento
    if (S_ISDIR(statbuf.st_mode)){
        //fprintf(stdout, "%s e' una cartella!\n", nomedirectory);
    }
    else{
        fprintf(stderr, "[ERRORE] %s non e' una directory", nomedirectory);
        exit(EXIT_FAILURE);
    }
    //apertutra della directory
    DIR *dir;

    SYSCALL(opendir, opendir(nomedirectory), dir, NULL, "Facendo opendir su %s: errno=%d\n", nomedirectory, errno);


    //ciclo principale del programma, che controllera' le directory ed eventuali estensioni
    struct dirent *file = NULL;
    while ((errno = 0, file = readdir(dir)) != NULL){
        char filename[MAXFILENAME];

        if (strlen(nomedirectory) + strlen(file->d_name) + 2 > MAXFILENAME){
            fprintf(stderr, "ERRORE: MAXFILENAME troppo piccolo\n");
            exit(EXIT_FAILURE);
        }

        strncpy(filename, nomedirectory, MAXFILENAME - 1);
        strncat(filename, "/", MAXFILENAME - 1);
        strncat(filename, file->d_name, MAXFILENAME - 1);
        

        SYSCALL(stat, stat(filename, &statbuf), dummy_return_val, -1, "Facendo stat del nome %s: errno=%d\n", filename, errno);
        //controllo per verificare directory interne e verificare estensione passata
        if (S_ISDIR(statbuf.st_mode)){
            if (!isDot(filename))
                Esplorazione(filename, es);
        }
        //controllo per estensione e stampa della data dell'ultima modifica del file di cui cerchiamo estensione
        else{  
            //strrchr ritorna i caratteri dopo il punto.
            char *ext = strrchr(filename, '.');
            //creo variabile confronto che si occupera' di far tornare l'estensione
            int confronto = strcmp(es, ext);
            //uso puntatore tempo definito dentro struct tm per stampare tempo ultima modifica del file di cui cerchiamo estensione.
            struct tm *tempo;
            tempo=localtime(&statbuf.st_mtime);
            if (confronto==0){
                char path[MAXFILENAME];
                realpath(filename, path);
                printf("%s\t",path);
                printf("%s", asctime(tempo));
            }
                
            
           
        }
    }
    //chiusura directory
    if (errno != 0)
        perror("readdir");
    SYSCALL(closedir, closedir(dir), dummy_return_val, -1, "Facendo closedir su %s: errno=%d\n", dir, errno);
}

int main(int argc, char *argv[]){
    //controllo se argomenti passati sono 2 e gestisco eventuali errori.
    //in base al caso che si verifica richiamo Esplorazione.
    if (argc != 3){
        if (argc == 2){
            Esplorazione(".", argv[1]);
            }else {
             fprintf(stderr, "Devi inserire un'estensione: %s .estensione\n", argv[0]);
             return EXIT_FAILURE;
            }
    }else {
            Esplorazione(argv[2], argv[1]);
            return 0;
        }
}


