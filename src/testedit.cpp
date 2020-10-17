#include <gtest/gtest.h> 	 	    		
#include "EditBuffer.h"
#include <string.h>

TEST(EditBuffer, EmptyTest){
    SEditBufferRef EditBuffer = EditBufferCreate();

    ASSERT_TRUE(EditBuffer != NULL);
    EXPECT_EQ(EditBufferSize(EditBuffer), 0);
    EXPECT_EQ(EditBufferMoveCursor(EditBuffer, 0, EDIT_BUFFER_ORIGIN_CURRENT), 0);
    EditBufferDestroy(EditBuffer);
}

TEST(EditBuffer, LoadTest){
    SEditBufferRef EditBuffer = EditBufferCreate();

    ASSERT_TRUE(EditBuffer != NULL);
    EXPECT_EQ(EditBufferSize(EditBuffer), 0);
    EXPECT_TRUE(EditBufferLoad(EditBuffer, "Hello World!"));
    EXPECT_EQ(EditBufferMoveCursor(EditBuffer, 0, EDIT_BUFFER_ORIGIN_CURRENT), 0);
    EXPECT_EQ(EditBufferSize(EditBuffer), 12);
    EditBufferDestroy(EditBuffer);
}

TEST(EditBuffer, MoveTest){
    SEditBufferRef EditBuffer = EditBufferCreate();

    ASSERT_TRUE(EditBuffer != NULL);
    EXPECT_EQ(EditBufferSize(EditBuffer), 0);
    EXPECT_EQ(EditBufferLoad(EditBuffer, "Hello World!"),12);
    EXPECT_EQ(EditBufferMoveCursor(EditBuffer, 0, EDIT_BUFFER_ORIGIN_CURRENT), 0);
    EXPECT_EQ(EditBufferSize(EditBuffer), 12);
    EXPECT_EQ(EditBufferMoveCursor(EditBuffer, -6, EDIT_BUFFER_ORIGIN_END), 6);
    EXPECT_EQ(EditBufferMoveCursor(EditBuffer, 2, EDIT_BUFFER_ORIGIN_CURRENT), 8);
    EXPECT_EQ(EditBufferMoveCursor(EditBuffer, 0, EDIT_BUFFER_ORIGIN_BEGINNING), 0);
    EditBufferDestroy(EditBuffer);
}

TEST(EditBuffer, ReadTest){
    SEditBufferRef EditBuffer = EditBufferCreate();
    char CharBuffer[32];

    ASSERT_TRUE(EditBuffer != NULL);
    EXPECT_EQ(EditBufferSize(EditBuffer), 0);
    EXPECT_EQ(EditBufferLoad(EditBuffer, "Hello World!"),12);
    EXPECT_EQ(EditBufferMoveCursor(EditBuffer, 0, EDIT_BUFFER_ORIGIN_CURRENT), 0);
    EXPECT_EQ(EditBufferSize(EditBuffer), 12);
    EXPECT_EQ(EditBufferMoveCursor(EditBuffer, 6, EDIT_BUFFER_ORIGIN_CURRENT), 6);
    EXPECT_EQ(EditBufferRead(EditBuffer, CharBuffer, 6), 6);
    CharBuffer[6] = '\0';
    EXPECT_EQ(strcmp(CharBuffer, "World!"), 0);
    EditBufferDestroy(EditBuffer);
}

TEST(EditBuffer, InsertTest){
    SEditBufferRef EditBuffer = EditBufferCreate();
    char CharBuffer[32];

    ASSERT_TRUE(EditBuffer != NULL);
    EXPECT_EQ(EditBufferSize(EditBuffer), 0);
    EXPECT_EQ(EditBufferLoad(EditBuffer, "Hello World!"),12);
    EXPECT_EQ(EditBufferMoveCursor(EditBuffer, 0, EDIT_BUFFER_ORIGIN_CURRENT), 0);
    EXPECT_EQ(EditBufferSize(EditBuffer), 12);
    EXPECT_EQ(EditBufferMoveCursor(EditBuffer, 6, EDIT_BUFFER_ORIGIN_CURRENT), 6);
    EXPECT_EQ(EditBufferInsert(EditBuffer, "There ", 6),6);
    EXPECT_EQ(EditBufferMoveCursor(EditBuffer, 0, EDIT_BUFFER_ORIGIN_BEGINNING), 0);
    EXPECT_EQ(EditBufferRead(EditBuffer, CharBuffer, 18),18);
    CharBuffer[18] = '\0';
    EXPECT_EQ(strcmp(CharBuffer, "Hello There World!"), 0);
    EditBufferDestroy(EditBuffer);
}

TEST(EditBuffer, DeleteTest){
    SEditBufferRef EditBuffer = EditBufferCreate();
    char CharBuffer[32];

    ASSERT_TRUE(EditBuffer != NULL);
    EXPECT_EQ(EditBufferSize(EditBuffer), 0);
    EXPECT_EQ(EditBufferLoad(EditBuffer, "Hello World!"),12);
    EXPECT_EQ(EditBufferMoveCursor(EditBuffer, 0, EDIT_BUFFER_ORIGIN_CURRENT), 0);
    EXPECT_EQ(EditBufferSize(EditBuffer), 12);
    EXPECT_EQ(EditBufferMoveCursor(EditBuffer, 6, EDIT_BUFFER_ORIGIN_CURRENT), 6);
    EXPECT_EQ(EditBufferDelete(EditBuffer, 5),5);
    EXPECT_EQ(EditBufferInsert(EditBuffer, "You", 3),3);
    EXPECT_EQ(EditBufferMoveCursor(EditBuffer, 0, EDIT_BUFFER_ORIGIN_BEGINNING), 0);
    EXPECT_EQ(EditBufferRead(EditBuffer, CharBuffer, 10),10);
    CharBuffer[10] = '\0';
    EXPECT_EQ(strcmp(CharBuffer, "Hello You!"), 0);
    EditBufferDestroy(EditBuffer);
}

TEST(EditBuffer, MultipleInsertDeleteTest){
    SEditBufferRef EditBuffer = EditBufferCreate();
    char CharBuffer[32];

    ASSERT_TRUE(EditBuffer != NULL);
    EXPECT_EQ(EditBufferSize(EditBuffer), 0);
    EXPECT_EQ(EditBufferLoad(EditBuffer, "02468"),5);
    EXPECT_EQ(EditBufferMoveCursor(EditBuffer, 0, EDIT_BUFFER_ORIGIN_CURRENT), 0);
    EXPECT_EQ(EditBufferSize(EditBuffer), 5);
    EXPECT_EQ(EditBufferMoveCursor(EditBuffer, 1, EDIT_BUFFER_ORIGIN_CURRENT), 1);
    EXPECT_EQ(EditBufferInsert(EditBuffer, "1", 1),1);
    EXPECT_EQ(EditBufferMoveCursor(EditBuffer, 1, EDIT_BUFFER_ORIGIN_CURRENT), 3);
    EXPECT_EQ(EditBufferInsert(EditBuffer, "3", 1),1);
    EXPECT_EQ(EditBufferMoveCursor(EditBuffer, 1, EDIT_BUFFER_ORIGIN_CURRENT), 5);
    EXPECT_EQ(EditBufferInsert(EditBuffer, "5", 1),1);
    EXPECT_EQ(EditBufferMoveCursor(EditBuffer, 1, EDIT_BUFFER_ORIGIN_CURRENT), 7);
    EXPECT_EQ(EditBufferInsert(EditBuffer, "7", 1),1);
    EXPECT_EQ(EditBufferMoveCursor(EditBuffer, 1, EDIT_BUFFER_ORIGIN_CURRENT), 9);
    EXPECT_EQ(EditBufferInsert(EditBuffer, "9", 1),1);
    EXPECT_EQ(EditBufferMoveCursor(EditBuffer, 0, EDIT_BUFFER_ORIGIN_BEGINNING), 0);
    EXPECT_EQ(EditBufferRead(EditBuffer, CharBuffer, 10),10);
    CharBuffer[10] = '\0';
    EXPECT_EQ(strcmp(CharBuffer, "0123456789"), 0);
    EXPECT_EQ(EditBufferMoveCursor(EditBuffer, 0, EDIT_BUFFER_ORIGIN_BEGINNING), 0);
    EXPECT_EQ(EditBufferDelete(EditBuffer, 1),1);
    EXPECT_EQ(EditBufferMoveCursor(EditBuffer, 1, EDIT_BUFFER_ORIGIN_CURRENT), 1);
    EXPECT_EQ(EditBufferDelete(EditBuffer, 1),1);
    EXPECT_EQ(EditBufferMoveCursor(EditBuffer, 1, EDIT_BUFFER_ORIGIN_CURRENT), 2);
    EXPECT_EQ(EditBufferDelete(EditBuffer, 1),1);
    EXPECT_EQ(EditBufferMoveCursor(EditBuffer, 1, EDIT_BUFFER_ORIGIN_CURRENT), 3);
    EXPECT_EQ(EditBufferDelete(EditBuffer, 1),1);
    EXPECT_EQ(EditBufferMoveCursor(EditBuffer, 1, EDIT_BUFFER_ORIGIN_CURRENT), 4);
    EXPECT_EQ(EditBufferDelete(EditBuffer, 1),1);
    EXPECT_EQ(EditBufferMoveCursor(EditBuffer, 0, EDIT_BUFFER_ORIGIN_BEGINNING), 0);
    EXPECT_EQ(EditBufferRead(EditBuffer, CharBuffer, 5),5);
    CharBuffer[5] = '\0';
    EXPECT_EQ(strcmp(CharBuffer, "13579"), 0);
    EditBufferDestroy(EditBuffer);
}