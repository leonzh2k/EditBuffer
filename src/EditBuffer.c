#include "../include/EditBuffer.h" 	 	    		
#include <stdlib.h> 
#include <string.h>
#include <stdio.h>
#define DEFAULT_BUF_SIZE 64
// Data Structure: Gap Buffer

//declaration of the editbuffer object. It will contain the size of the buffer, chars left of the gap,
//chars right of the gap, and the actual editbuffer array.
struct STRUCT_EDIT_BUFFER_TAG {
    char* ActualEditBuffer;
    size_t buffer_size;
    size_t chars_left_of_gap;
    size_t chars_right_of_gap;
    size_t left_gap_start;
    size_t right_gap_start;
};

/*Creates an empty Edit Buffer and returns a reference to it.*/
//SEditBuffer is the type. SEditBufferRef is simply a pointer to type SEditBuffer. We use the same pointer for all
//functions. 
SEditBufferRef EditBufferCreate() {
    //Creating the object EditBuffer, initializing its values
    //gap is initially the size of the entire array; so chars left and right are 0 since nothing has been added yet.
    SEditBufferRef EditBuffer = (SEditBufferRef)malloc(sizeof(SEditBuffer));
    //Creating a pointer to the EditBuffer object, is a double pointer
    SEditBufferRef *EditBufferRef = &EditBuffer;
    (*EditBufferRef)->ActualEditBuffer = (char*)malloc(sizeof(char) * DEFAULT_BUF_SIZE);
    (*EditBufferRef)->buffer_size = DEFAULT_BUF_SIZE;
    (*EditBufferRef)->chars_left_of_gap = 0;
    (*EditBufferRef)->chars_right_of_gap = 0;
    (*EditBufferRef)->left_gap_start = 0;
    (*EditBufferRef)->right_gap_start = (*EditBufferRef)->buffer_size - 1;
    for (int i = 0; i < (*EditBufferRef)->buffer_size; i++) {
        (*EditBufferRef)->ActualEditBuffer[i] = '-';
    }
    return *EditBufferRef; //Dereferencing here yields the pointer to EditBuffer, which is what we want
}

/* Destroys the Edit Buffer referenced by ebuf. */
void EditBufferDestroy(SEditBufferRef ebuf) {
    free(ebuf->ActualEditBuffer);
    free(ebuf);
}

/* Returns the number of characters that are currently in the Edit Buffer. */
size_t EditBufferChars(SEditBufferRef ebuf) {
    size_t total_chars = ebuf->chars_left_of_gap + ebuf->chars_right_of_gap;
    // printf("total chars is %d\n", total_chars);
    return total_chars;
}

size_t EditBufferSize(SEditBufferRef ebuf) {
    return ebuf->buffer_size;
}

void PrintEditBuffer(SEditBufferRef ebuf) {
    printf("\n[");
    for (int i = 0; i < ebuf->buffer_size; i++) {
        printf("%c", ebuf->ActualEditBuffer[i]);
    }
    printf("] ");
    if (EditBufferChars(ebuf) == 0) {
        printf("(Empty)\n\n");
    }
    else {
        printf("\n\n");
    }
    
}

/* Moves the cursor of the Edit Buffer. The origin will be one of the three values EDIT_BUFFER_ORIGIN_BEGINNING, 
EDIT_BUFFER_ORIGIN_CURRENT, or EDIT_BUFFER_ORIGIN_END representing that the offset should be relative to the 
beginning, current or end location. The offset from the beginning of the Edit Buffer is returned. */

size_t EditBufferMoveCursor(SEditBufferRef ebuf, int offset, int origin) {
    /* YOUR CODE HERE */
    size_t offset_beginning = 0;
    if (origin == EDIT_BUFFER_ORIGIN_BEGINNING) {
        //catch negative offset
        if (offset < 0) {
            printf("Cannot have negative characters between cursor and beginning. Move not performed.\n");
            return ebuf->left_gap_start;
        }
        //moving the cursor left, chars right increase, left decrease
        if (ebuf->chars_left_of_gap < offset) {
            while (ebuf->chars_left_of_gap != offset) { 
                //Copy values from right to left side
                ebuf->ActualEditBuffer[ebuf->left_gap_start] = ebuf->ActualEditBuffer[ebuf->right_gap_start + 1];
                ebuf->ActualEditBuffer[ebuf->right_gap_start + 1] = '-'; //may replace this
                //Moving related index pointers.
                ebuf->chars_left_of_gap = ebuf->chars_left_of_gap + 1;
                ebuf->chars_right_of_gap = ebuf->chars_right_of_gap - 1;
                ebuf->left_gap_start = ebuf->left_gap_start + 1;
                ebuf->right_gap_start = ebuf->right_gap_start + 1;
            }
            ebuf->left_gap_start = ebuf->chars_left_of_gap;
            offset_beginning = ebuf->left_gap_start;
            return offset_beginning;
        }
        //moving left to right
        else {
            while (ebuf->chars_left_of_gap != offset) {
                ebuf->ActualEditBuffer[ebuf->right_gap_start] = ebuf->ActualEditBuffer[ebuf->left_gap_start - 1];
                ebuf->ActualEditBuffer[ebuf->left_gap_start - 1] = '-';
                //Moving related index pointers.
                ebuf->chars_left_of_gap = ebuf->chars_left_of_gap - 1;
                ebuf->chars_right_of_gap = ebuf->chars_right_of_gap + 1;
                ebuf->left_gap_start = ebuf->left_gap_start - 1;
                ebuf->right_gap_start = ebuf->right_gap_start - 1;
            }
            offset_beginning = ebuf->left_gap_start;
            return offset_beginning;
        }
    }
    else if (origin == EDIT_BUFFER_ORIGIN_CURRENT) {
        //case of negative offset
        if (offset < 0) {
            if (ebuf->chars_left_of_gap == 0) {
                printf("Cannot move cursor to the left; no characters to the left of the cursor!\n");
                return 0;
            }
            else if (abs(offset) > ebuf->chars_left_of_gap) {
                printf("Offset is greater than # characters to left of cursor. Move not performed.\n");
                return 0;
            }
            else {
                size_t real_offset = ebuf->chars_left_of_gap - abs(offset); //offset is negative so need to add it
                while (ebuf->chars_left_of_gap != real_offset) { 
                    //Copy values from left to right side
                    ebuf->ActualEditBuffer[ebuf->right_gap_start] = ebuf->ActualEditBuffer[ebuf->left_gap_start - 1];
                    ebuf->ActualEditBuffer[ebuf->left_gap_start - 1] = '-'; //may replace this
                    //Moving related index pointers.
                    ebuf->chars_left_of_gap = ebuf->chars_left_of_gap - 1;
                    ebuf->chars_right_of_gap = ebuf->chars_right_of_gap + 1;
                    ebuf->left_gap_start = ebuf->left_gap_start - 1;
                    ebuf->right_gap_start = ebuf->right_gap_start - 1;
                }
                offset_beginning = ebuf->left_gap_start;
                return offset_beginning;
            }
            
        }
        //case of positive offset
        else {
            size_t real_offset = ebuf->chars_left_of_gap + offset;
            if (ebuf->chars_right_of_gap == 0) {
                printf("Cannot move cursor to the right; no characters to the right of the cursor!\n");
                return 0;
            }
            else if (abs(offset) > ebuf->chars_right_of_gap) {
                printf("Offset is greater than # characters to right of cursor. Move not performed.\n");
                return 0;
            }
            else {
                while (ebuf->chars_left_of_gap != real_offset) { 
                    //Copy values from right to left side
                    ebuf->ActualEditBuffer[ebuf->left_gap_start] = ebuf->ActualEditBuffer[ebuf->right_gap_start + 1];
                    ebuf->ActualEditBuffer[ebuf->right_gap_start + 1] = '-'; //may replace this
                    //Moving related index pointers.
                    ebuf->chars_left_of_gap = ebuf->chars_left_of_gap + 1;
                    ebuf->chars_right_of_gap = ebuf->chars_right_of_gap - 1;
                    ebuf->left_gap_start = ebuf->left_gap_start + 1;
                    ebuf->right_gap_start = ebuf->right_gap_start + 1;
                }
                offset_beginning = ebuf->left_gap_start;
                return offset_beginning;
            }
        }
    }
    else if (origin == EDIT_BUFFER_ORIGIN_END) {
        if (offset < 0) {
            printf("Cannot have negative characters between cursor and end. Move not performed.\n");
            return ebuf->left_gap_start;
        }
        //you actually don't always move it right. you might have to move it left depending on where characteres are
        
        //case where no move is needed
        if (ebuf->chars_right_of_gap == abs(offset)) {
            return ebuf->left_gap_start;
        }
        //if characters right of gap are less than intended, then have to move gap left
        else if (ebuf->chars_right_of_gap < abs(offset)) {
             while (ebuf->chars_right_of_gap != abs(offset)) {
                //Copy values from left to right side
                ebuf->ActualEditBuffer[ebuf->right_gap_start] = ebuf->ActualEditBuffer[ebuf->left_gap_start - 1];
                ebuf->ActualEditBuffer[ebuf->left_gap_start - 1] = '-'; //may replace this
                //Moving related index pointers.
                ebuf->chars_left_of_gap = ebuf->chars_left_of_gap - 1;
                ebuf->chars_right_of_gap = ebuf->chars_right_of_gap + 1;
                ebuf->left_gap_start = ebuf->left_gap_start - 1;
                ebuf->right_gap_start = ebuf->right_gap_start - 1;
            }
        }
        //if characters right of gap are more than intended, then have to move gap right
        else {
            while (ebuf->chars_right_of_gap != abs(offset)) {
                //Copy values from right to left side
                ebuf->ActualEditBuffer[ebuf->left_gap_start] = ebuf->ActualEditBuffer[ebuf->right_gap_start + 1];
                ebuf->ActualEditBuffer[ebuf->right_gap_start + 1] = '-'; //may replace this
                //Moving related index pointers.
                ebuf->chars_left_of_gap = ebuf->chars_left_of_gap + 1;
                ebuf->chars_right_of_gap = ebuf->chars_right_of_gap - 1;
                ebuf->left_gap_start = ebuf->left_gap_start + 1;
                ebuf->right_gap_start = ebuf->right_gap_start + 1;
            }
        }
        offset_beginning = ebuf->left_gap_start;
        return offset_beginning;
    }
    else {
        printf("THIS SHOULD NOT OCCUR\n");
        return offset;
    }
}

/* Loads the Edit Buffer with the string specified by buf. The number of characters that are now in 
the Edit Buffer is returned. This should equal the string length of buf. */
size_t EditBufferLoad(SEditBufferRef ebuf, const char *buf){
    size_t chars_loaded = 0;
    size_t buf_len = strlen(buf);
    //first clear anything in the buffer
    char *FreshBuffer = (char*)malloc(sizeof(char) * DEFAULT_BUF_SIZE);
    free(ebuf->ActualEditBuffer);
    //since everything is erased you can shrink the buffer
    ebuf->buffer_size = DEFAULT_BUF_SIZE;
    ebuf->ActualEditBuffer = FreshBuffer;
    ebuf->chars_left_of_gap = 0;
    ebuf->chars_right_of_gap = 0;
    ebuf->left_gap_start = 0;
    ebuf->right_gap_start = ebuf->buffer_size - 1;
    for (int i = 0; i < ebuf->buffer_size; i++) {
        ebuf->ActualEditBuffer[i] = '-';
    }
    //if size of string to be loaded is bigger than buffer size
    if (buf_len > ebuf->buffer_size) {
        size_t new_buf_size = 2 * buf_len;
        char *LongerBuffer = (char*)malloc(sizeof(char) * (new_buf_size));
        ebuf->buffer_size = new_buf_size;
        free(ebuf->ActualEditBuffer);
        ebuf->ActualEditBuffer = LongerBuffer;
        for (int i = 0; i < ebuf->buffer_size; i++) {
            ebuf->ActualEditBuffer[i] = '-';
        }
    }
    //loading in string 
    int i = 0;
    int j = 0;
    ebuf->right_gap_start = (ebuf->buffer_size - 1) - buf_len;
    i = ebuf->right_gap_start + 1;
    while (i <= ebuf->buffer_size - 1) {
        ebuf->ActualEditBuffer[i] = buf[j];
        ebuf->chars_right_of_gap++;
        chars_loaded++;
        i++;
        j++;
    }
    
    return chars_loaded;
}

/* Attempts to read count number of characters from the Edit Buffer into the buffer pointed to by buf. 
The beginning of the characters that are read into the buf start at the current cursor offset. The cursor 
is moved forward by thenumber of characters that are read. If more characters are requested than remain in 
the buffer after the cursor, the remaining number of characters are read. The actual number of characters 
read will be returned. */

size_t EditBufferRead(SEditBufferRef ebuf, char *buf, size_t count){
    size_t actual_chars_read = 0;
    //if count is greater than characters on right side
    if (ebuf->chars_right_of_gap < count) {
        size_t buf_index = 0;
        actual_chars_read = ebuf->chars_right_of_gap;
        while (ebuf->chars_right_of_gap != 0) { 
            //Reading into buffer....
            buf[buf_index] = ebuf->ActualEditBuffer[ebuf->right_gap_start + 1];
            //Copy values from right to left side
            ebuf->ActualEditBuffer[ebuf->left_gap_start] = ebuf->ActualEditBuffer[ebuf->right_gap_start + 1];
            ebuf->ActualEditBuffer[ebuf->right_gap_start + 1] = '-'; //may replace this
            //Moving editbuffer index pointers.
            ebuf->chars_left_of_gap = ebuf->chars_left_of_gap + 1;
            ebuf->chars_right_of_gap = ebuf->chars_right_of_gap - 1;
            ebuf->left_gap_start = ebuf->left_gap_start + 1;
            ebuf->right_gap_start = ebuf->right_gap_start + 1;
            //moving buf index pointer
            buf_index++;
        }
        return actual_chars_read;
    }
    else {
        size_t buf_index = 0;
        actual_chars_read = count;
        while (count > 0) { 
            buf[buf_index] = ebuf->ActualEditBuffer[ebuf->right_gap_start + 1];
            //Copy values from right to left side
            ebuf->ActualEditBuffer[ebuf->left_gap_start] = ebuf->ActualEditBuffer[ebuf->right_gap_start + 1];
            ebuf->ActualEditBuffer[ebuf->right_gap_start + 1] = '-'; //may replace this
            //Moving editbuffer index pointers.
            ebuf->chars_left_of_gap = ebuf->chars_left_of_gap + 1;
            ebuf->chars_right_of_gap = ebuf->chars_right_of_gap - 1;
            ebuf->left_gap_start = ebuf->left_gap_start + 1;
            ebuf->right_gap_start = ebuf->right_gap_start + 1;
            //moving buf index pointers
            buf_index++;
            count--;
        }
        return actual_chars_read;
    }
}

/* Inserts count characters at the cursor location. The characters to be inserted are pointed to by buf. The cursor 
is moved forward by the number of characters that are read. The number of characters inserted will be returned.*/


size_t EditBufferInsert(SEditBufferRef ebuf, const char *buf, size_t count){
    int i = 0;
    while (i < count) {
        if (ebuf->left_gap_start == ebuf->right_gap_start) {
            //make some new space
            size_t new_buf_size = 2 * ebuf->buffer_size;
            char *NewBuffer = (char*)malloc(sizeof(char) * (new_buf_size));
            //fill space with stuff that won't cause a crash if accessed
            for (int i = 0; i < new_buf_size; i++) {
                NewBuffer[i] = '-';
            }
            //copy over left side of gap
            int j = 0;
            int k = 0;
            while (j < ebuf->chars_left_of_gap) {
                NewBuffer[j] = ebuf->ActualEditBuffer[k];
                j++;
                k++;
            }
            //copy over right side of gap
            j = (ebuf->buffer_size - 1) - ebuf->chars_right_of_gap + 1;
            k = (new_buf_size - 1) - ebuf->chars_right_of_gap + 1;
            while (j < ebuf->buffer_size) {
                NewBuffer[k] = ebuf->ActualEditBuffer[j];
                j++;
                k++;
            }
            ebuf->right_gap_start = (new_buf_size - 1) - ebuf->chars_right_of_gap;
            ebuf->buffer_size = new_buf_size;
            //free up old space, point to new space
            free(ebuf->ActualEditBuffer);
            ebuf->ActualEditBuffer = NewBuffer;
        }
        //do moving
        ebuf->ActualEditBuffer[ebuf->left_gap_start] = buf[i];
        ebuf->chars_left_of_gap++;
        ebuf->left_gap_start++;
        i++;
    }
    return count;
}

/* Deletes count characters that are following the cursor location. The cursor is not moved. If fewer characters 
exist after the cursor in the Edit Buffer, all of the remaining characters are removed following the cursor. 
The number of characters that were deleted will be returned. */

size_t EditBufferDelete(SEditBufferRef ebuf, size_t count){
    /* YOUR CODE HERE */
    size_t chars_to_be_deleted = ebuf->chars_right_of_gap;
    if (chars_to_be_deleted < count) {
        for (int i = 0; i < chars_to_be_deleted; i++) {
            ebuf->ActualEditBuffer[ebuf->right_gap_start + 1] = '-';
            ebuf->right_gap_start++;
            ebuf->chars_right_of_gap--;
        }
        printf("Characters to be deleted is greater than characters to the right of cursor. All characters to the right deleted.\n");
        return chars_to_be_deleted;
    }
    else  {
        for (int i = 0; i < count; i++) {
            ebuf->ActualEditBuffer[ebuf->right_gap_start + 1] = '-';
            ebuf->right_gap_start++;
            ebuf->chars_right_of_gap--;
        }
        return count;
    }
}



 