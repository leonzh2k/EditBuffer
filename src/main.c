#include <stdio.h> 	 	    		
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "EditBuffer.h"

bool LoadOriginalFile(SEditBufferRef ebuf, const char *filename);
bool ApplyEditFile(SEditBufferRef ebuf, const char *filename, double *edittime);
bool ParseString(char *out, const char *in);

int main(int argc, char *argv[]){
    SEditBufferRef EditBuffer;
    char OutputBuffer[1024];
    size_t BytesRead;
    double EditTime;
    FILE *OutputFile = stdout;

    if(3 > argc){
        printf("Syntax Error: proj2 orig.txt edit.txt [result.txt]\n");
        return EXIT_FAILURE;
    }
    EditBuffer = EditBufferCreate();
    if(!LoadOriginalFile(EditBuffer,argv[1])){
        EditBufferDestroy(EditBuffer);
        printf("Failed to load file %s!",argv[1]);
        return EXIT_FAILURE;
    }
    if(!ApplyEditFile(EditBuffer,argv[2],&EditTime)){
        EditBufferDestroy(EditBuffer);
        printf("Failed to apply edits from %s!",argv[2]);
        return EXIT_FAILURE;
    }
    if(3 < argc){
        OutputFile = fopen(argv[3],"wb");
        if(NULL == OutputFile){
            printf("Failed to load output file %s!",argv[3]);
            return EXIT_FAILURE;
        }
    }
    EditBufferMoveCursor(EditBuffer,0,EDIT_BUFFER_ORIGIN_BEGINNING);
    while((BytesRead = EditBufferRead(EditBuffer,OutputBuffer,sizeof(OutputBuffer)))){
        fwrite(OutputBuffer,sizeof(char),BytesRead,OutputFile);
    }
    if(OutputFile != stdout){
        fclose(OutputFile);
    }
    fprintf(stderr,"Edits took %.3lf secs\n",EditTime);
    return EXIT_SUCCESS;
}

bool LoadOriginalFile(SEditBufferRef ebuf, const char *filename){
    FILE *InputFile = fopen(filename,"rb");
    char *Buffer;
    size_t FileSize;

    if(NULL == InputFile){
        return false;
    }
    fseek(InputFile,0,SEEK_END);
    FileSize = ftell(InputFile);
    fseek(InputFile,0,SEEK_SET);
    Buffer = malloc((FileSize + 1) * sizeof(char));
    FileSize = fread(Buffer,sizeof(char),FileSize,InputFile);
    Buffer[FileSize] = '\0';
    //printf("\"%s\"",Buffer);
    EditBufferLoad(ebuf, Buffer);
    free(Buffer);
    return true;
}

typedef struct{
    char DType;
    size_t DOffset;
    size_t DLength;
    char *DBuffer;
} STextEdit;

bool ApplyEditFile(SEditBufferRef ebuf, const char *filename, double *edittime){
    FILE *InputFile = fopen(filename,"rb");
    char Format[32];
    size_t TextEditSize = 1024, TextEditCount = 0;
    STextEdit *TextEdits;
    clock_t StartTime, EndTime;
    bool Success = true;

    if(NULL == InputFile){
        return false;
    }
    TextEdits = malloc(sizeof(STextEdit) * TextEditSize);
    memset(TextEdits, 0, sizeof(STextEdit) * TextEditSize);
    while(!feof(InputFile)){
        if(TextEditSize <= TextEditCount){
            STextEdit *TempTextEdits = malloc(sizeof(STextEdit) * TextEditSize * 2);
            memcpy(TempTextEdits, TextEdits, sizeof(STextEdit) * TextEditSize);
            memset(TempTextEdits + TextEditSize, 0, sizeof(STextEdit) * TextEditSize);
            TextEditSize *= 2;  
            free(TextEdits);
            TextEdits = TempTextEdits;
        }
        if(3 != fscanf(InputFile,"%c %zu %zu", &TextEdits[TextEditCount].DType, &TextEdits[TextEditCount].DOffset, &TextEdits[TextEditCount].DLength)){
            break;
        }
        if('-' == TextEdits[TextEditCount].DType){
            fscanf(InputFile, "\n");
        }
        else if('?' == TextEdits[TextEditCount].DType){
            fscanf(InputFile, "\n");
            TextEdits[TextEditCount].DBuffer = malloc(sizeof(char) * TextEdits[TextEditCount].DLength);
        }
        else{
            size_t BufferSize = (TextEdits[TextEditCount].DLength + 2) * 2;
            TextEdits[TextEditCount].DBuffer = malloc(sizeof(char) * BufferSize);
            sprintf(Format, " %%%zus\n", BufferSize);
            fscanf(InputFile, Format, TextEdits[TextEditCount].DBuffer);
            ParseString(TextEdits[TextEditCount].DBuffer, TextEdits[TextEditCount].DBuffer);
        }
        TextEditCount++;
    }
    fclose(InputFile);
    StartTime = clock();
    for(size_t Index = 0; Index < TextEditCount; Index++){
        size_t SizeBefore = EditBufferSize(ebuf);
        EditBufferMoveCursor(ebuf, TextEdits[Index].DOffset, EDIT_BUFFER_ORIGIN_BEGINNING);
        if('-' == TextEdits[Index].DType){
            if(!TextEdits[Index].DLength){
                continue;
            }
            EditBufferDelete(ebuf, TextEdits[Index].DLength);
            if(SizeBefore != EditBufferSize(ebuf) + TextEdits[Index].DLength){
                printf("Delete Failed: Previously %zu now is %zu after delete of %zu!\n", SizeBefore, EditBufferSize(ebuf), TextEdits[Index].DLength);
                Success = false;
                break;
            }
        }
        else if('?' == TextEdits[Index].DType){
            if(!TextEdits[Index].DLength){
                continue;
            }
            EditBufferRead(ebuf, TextEdits[Index].DBuffer, TextEdits[Index].DLength);
        }
        else{
            EditBufferInsert(ebuf, TextEdits[Index].DBuffer, TextEdits[Index].DLength);
            if(SizeBefore + TextEdits[Index].DLength != EditBufferSize(ebuf)){
                printf("Insert Failed: Previously %zu now is %zu after insert of %zu!\n", SizeBefore, EditBufferSize(ebuf), TextEdits[Index].DLength);
                Success = false;
                break;
            }
        }
    }
    EndTime = clock();
    *edittime = (double)(EndTime - StartTime) / CLOCKS_PER_SEC;
    for(size_t Index = 0; Index < TextEditCount; Index++){
        if(TextEdits[TextEditCount].DBuffer){
            free(TextEdits[TextEditCount].DBuffer);
        }
    }    
    free(TextEdits);
    return Success;
}

bool ParseString(char *out, const char *in){
    while(*in){
        if('"' == *in){
            in++;
            while(*in && ('"' != *in)){
                if('\\' == *in){
                    in++;
                    switch(*in){
                        case 'n':   *out++ = '\n';
                                    break;
                        case 't':   *out++ = '\t';
                                    break;
                        case '0':   *out++ = '\0';
                                    break;
                        case 'r':   *out++ = '\r';
                                    break;
                        case '"':   *out++ = '"';
                                    break;
                        case '\'':  *out++ = '\'';
                                    break;
                        default:    *out++ = *in;
                                    break;
                    }
                    in++;
                }
                else{
                    *out++ = *in++;
                }
            }
        }
        else{
            *out++ = *in++;
        }
    }
    *out = '\0';
    return true;
}
