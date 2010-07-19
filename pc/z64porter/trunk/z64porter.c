#include "z64porter.h"

/* Global variables */
int verbose = 1, warnflag = 0, warnignore = 0;
int scene_to, scene_from;
N64Rom *N64_TO, *N64_FROM;
Z64 *Z64_TO, *Z64_FROM;
int music;

/* cleanup function */
void
cleanup()
{
    if(N64_TO != NULL)
        n64rom_close(N64_TO);
    if(N64_FROM != NULL)
        n64rom_close(N64_FROM);
    if(Z64_TO != NULL)
        z64_close(Z64_TO);
    if(Z64_FROM != NULL)
        z64_close(Z64_FROM);
}

/* Parse arguments */
void
parse_args(int argc, char ** argv)
{
    int argn=5;
    
    if(argc < 5)
    {
        /* maybe asking for -h, --help, --about */
        if(argc == 2)
        {
            argn = 1;
            goto optionscan;
        }
        /* nope, just an idiot */
        error("not enough arguments");
    }
    
    /* ROM to and ROM from the same ? */
    if(!strcmp(argv[1], argv[2]))
        error("destination and source ROMs are the same");
    
    /* ROM to */
    if((N64_TO = n64rom_load(argv[1])) == NULL)
        error("%s", n64rom_error());
    
    /* ROM from */
    if((N64_FROM = n64rom_load(argv[2])) == NULL)
        error("%s", n64rom_error());
    
    /* Scene to and from */
    scene_to = strtoint(argv[3]);
    scene_from = strtoint(argv[4]);
    
  optionscan:
    /* Parse extra arguments */
    for(;argn<argc;argn++)
    {
        if(argv[argn][0] == '-')
        {
            if(strlen(argv[argn]) == 2)
            {
                switch(argv[argn][1])
                {
                case 'v':
                    if(++argn < argc)
                    {
                        verbose = strtoint(argv[argn]);
                    }
                    else
                        error("option for -v is missing.");
                    break;
                case 'm':
                    if(++argn < argc)
                        music = strtoint(argv[argn]);
                    else
                        error("option for -m is missing.");
                    break;
                case 'h':
                    goto help;
                default:
                    goto unhandled;
                }
            }
            else if(argv[argn][1] == '-')
            {
                if(!strcmp(argv[argn], "--help"))
                {
              help:
                    fprintf(stdout, "Usage: %s [ROM to] [ROM from] [scene to] [scene from] [options]\n", argv[0]);
                    fprintf(stdout, "Options:\n");
                    fprintf(stdout, " -v level\t\tSet verbosity (0-3)\n");
                    fprintf(stdout, " -m music\t\tSet music value for destination scene\n");
                    fprintf(stdout, " --force-warnings\tIf any warnings, ignore and do port anyways\n");
                    fprintf(stdout, " -h, --help\t\tThis message\n");
                    fprintf(stdout, " --about\t\tAbout\n");
                    exit(EXIT_SUCCESS);
                }
                else if(!strcmp(argv[argn], "--about"))
                {
                    fprintf(stdout, "about: todo\n");
                    exit(EXIT_SUCCESS);
                }
                else if(!strcmp(argv[argn], "--force-warnings"))
                {
                    warnignore = 4;
                }
                else
              unhandled:
                    error("unhandled option `%s'", argv[argn]);
            }
            else
                goto unhandled;
        }
        else
            goto unhandled;
    }
}

void
check_setup()
{
    int checkval;
    
    /* bounds check verbosity - not specifically required, just to be safe */
    if(verbose < 0)
        verbose = 0;
    if(verbose > 3)
        verbose = 3;
    
    /* Set up Z64 ROMs */
    if((Z64_TO = z64_open(N64_TO)) == NULL)
        error("cannot open %s as a Zelda ROM", N64_TO->filename);
    if((Z64_FROM = z64_open(N64_FROM)) == NULL)
        error("cannot open %s as a Zelda ROM", N64_FROM->filename);
    
    /* Bounds check music value */
    if(music < 0)
    {
        msg(warnignore, "Warning: Music value too low; defaulting to 0");
        warnflag = 1;
        music = 0;
    }
    checkval = (Z64_TO->st->game == GameOOT) ? MAX_MUSIC_OOT : MAX_MUSIC_MM;
    if(music > checkval)
    {
        msg(warnignore, "Warning: Music value too high; defaulting to %i", checkval);
        warnflag = 1;
        music = checkval;
    }
    
    /* Bounds check scene values */
    if(scene_to < 0)
    {
        msg(warnignore, "Warning: Scene (to) value too low; defaulting to 0");
        warnflag = 1;
        scene_to = 0;
    }
    if(scene_from < 0)
    {
        msg(warnignore, "Warning: Scene (from) value too low; defaulting to 0");
        warnflag = 1;
        scene_from = 0;
    }
    
    /* ROM to checks */
    if(scene_to > Z_ST_COUNT( Z64_TO ))
    {
        msg(warnignore, "Warning: Scene (to) value too high; defaulting to %i", Z_ST_COUNT( Z64_TO ));
        warnflag = 1;
        scene_to = Z_ST_COUNT( Z64_TO );
    }
    
    /* ROM from checks */    
    if(scene_from > Z_ST_COUNT( Z64_FROM ))
    {
        msg(warnignore, "Warning: Scene (from) value too high; defaulting to %i", Z_ST_COUNT( Z64_FROM ));
        warnflag = 1;
        scene_from = Z_ST_COUNT( Z64_FROM );
    }
    /* okay, everything is sane */
}

int
do_port()
{
    msg(1, "To: %08X - %08X", z64st_getstart( Z64_TO, scene_to ), z64st_getend( Z64_TO, scene_to ) );
    msg(1, "From: %08X - %08X", z64st_getstart( Z64_FROM, scene_from ), z64st_getend( Z64_FROM, scene_from ) );
    return 1;
}

int
main(int argc, char ** argv)
{
    /* Initialize variables */
    char warn_yesno[2];
    N64_TO = NULL, N64_FROM = NULL;
    Z64_TO = NULL, Z64_FROM = NULL;
    scene_to = 0, scene_from = 0;
    music = 0;
    
    parse_args(argc, argv);
    check_setup();
    
    dump(verbose, "%i");
    dump(music, "%i");
    dump(warnflag, "%i");
    dump(scene_from, "%i");
    dump(scene_to, "%i");
    dump(N64_FROM->filename, "%s");
    dump(N64_TO->filename, "%s");
    
    if(warnflag && !warnignore)
    {
        printf("Continue despite warnings? (y/n): ");
        fgets(warn_yesno, sizeof(warn_yesno), stdin);
        if(tolower(warn_yesno[0]) == 'y')
            goto port;
        else{
            cleanup();
            return EXIT_FAILURE;
        }
    }
  port:
    msg(3, "Doing port...");
    do_port();
    
    cleanup();
    
    return EXIT_SUCCESS;
}
