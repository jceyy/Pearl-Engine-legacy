/** Library tools.h 0.8 by Jc Ringenbach
    This will just provide you some tools and helps for developing
    Thanks for using =)**/
#ifndef TOOLS_H_INCLUDED
#define TOOLS_H_INCLUDED

#define ERROR (-2)

int fcopy(const char FileNameToCopy[], const char NewFileName[])
{
    FILE *FileToCopy=fopen(FileNameToCopy, "r+");
    FILE *NewFile=fopen(NewFileName, "w+");
    rewind(FileToCopy);
    rewind(NewFile);
    int carractere=fgetc(FileToCopy);

    while (carractere!=EOF&&FileToCopy!=NULL&&NewFile!=NULL)
    {
        fputc(carractere, NewFile);
        carractere=fgetc(FileToCopy);
    }
    if (FileToCopy==NULL||NewFile==NULL)
    {
        return ERROR;
    }
    else
    {
        rewind(FileToCopy);
        rewind(NewFile);
        fclose(NewFile);
        fclose(FileToCopy);
        return 0;
    }
}

int fsetline (FILE *File, int LineNb)
{
    rewind (File);
    char Carractere;
    int i=1;
    while (i<LineNb)
    {
        Carractere=fgetc(File);
        if (Carractere=='\n')
        {
            i++;
        }
        else if (Carractere==EOF)
        {
            return EOF;
            i=LineNb*2;
        }
    }
    if (i==LineNb)
    {
        fseek(File, 0, SEEK_CUR);
        return 0;
    }
    else if (LineNb<i)
    {
        return ERROR;
    }
}

int ferase(const char FileName[])//use a string : if the file is closed
{
    FILE *File=fopen(FileName, "w+");
    if (File!=NULL)
    {
        fclose(File);
        return 0;
    }
    else
        return ERROR;
}

void cleanbuffer(void)
{
    int c=getchar();
    while (c!='\n'&&c!=EOF)
    {
        c=getchar();
    }
}

int sread(char *str, int length)
{
    char *EnterPos=NULL;
    if (fgets(str, length, stdin)!=NULL)
    {
        EnterPos=strchr(str, '\n');
        if (EnterPos!=NULL)
        {
            *EnterPos='\0';
            return 0;
        }
        else
        {
            cleanbuffer();
        }
    }
    else
    {
        cleanbuffer();
        return ERROR;
    }
}

int fgetlines(FILE* File)
{
    if (File!=NULL)
    {
        rewind(File);
        int i=1, c=0;
        while (c!=EOF)
        {
            c=fgetc(File);
            if (c=='\n')
            {
                i++;
            }
        }
        return i;
    }
    else
        return ERROR;
}

int flongestline(FILE* File)
{
    int car=0, line=1, tline=fgetlines(File);
    int linelenght=0, linelenght_new=0;

    if (File!=NULL)
    {
        if (tline!=ERROR)
        {
            rewind(File);
            while (line<=tline&&car!=EOF)
            {
                car=fgetc(File);

                if (car!=EOF&&car!='\n')
                {
                    linelenght++;
                }
                else if(car=='\n')
                {
                    linelenght++;
                    line++;
                    if (linelenght>linelenght_new)
                    {
                        linelenght_new=linelenght;
                    }
                    linelenght=0;
                }
                else if(car==EOF)
                {
                    linelenght++;
                    line++;
                    if (linelenght>linelenght_new)
                    {
                        linelenght_new=linelenght;
                    }
                    linelenght=0;
                }
            }
            return linelenght_new;
        }
        else
            return ERROR;
    }
    else
    {
        return ERROR;
    }
}

void sclean(char *str[])
{
    int i=0;
    while (str[i]!='\0')
    {
        str[i]='\0';
        i++;
    }
}

#endif // TOOLS_H_INCLUDED
