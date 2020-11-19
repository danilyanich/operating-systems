#pragma once

#include "utils.c"
#include "../src/mmemory.c"

void afterTest() {
    segmentsTable = NULL;
}

void shouldReturnSuccessfulCodeWithValidArgumentsForInit() {
    int segmentsAmount = 1;
    int segmentSize = 1;

    int actualCode = _init(segmentsAmount, segmentSize);

    int expectedCode = SUCCESSFUL_CODE;
    assertEquals("shouldReturnSuccessfulCodeWithValidArgumentsForInit()", actualCode, expectedCode);
}

void shouldReturnWrongParametersCodeWithInvalidArgumentsForInit() {
    int segmentsAmount = 1;
    int segmentSize = -1;

    int actualCode = _init(segmentsAmount, segmentSize);

    int expectedCode = WRONG_PARAMETERS;
    assertEquals("shouldReturnWrongParametersCodeWithInvalidArgumentsForInit()", actualCode, expectedCode);
}

void shouldReturnUnknownErrorWhenDoubleInvokeForInit() {
    int segmentsAmount = 1;
    int segmentSize = 1;

    _init(segmentsAmount, segmentSize);
    int actualCode = _init(segmentsAmount, segmentSize);

    int expectedCode = UNKNOWN_ERROR;
    assertEquals("shouldReturnUnknownErrorWhenDoubleInvokeForInit()", actualCode, expectedCode);
}

void shouldReturnUnknownErrorCodeWithoutInitForMalloc() {
    int segmentSize = 1;
    VA address;

    int actualCode = _malloc(&address, segmentSize);

    int expectedCode = UNKNOWN_ERROR;
    assertEquals("shouldReturnUnknownErrorCodeWithoutInitForMalloc()", actualCode, expectedCode);
}

void shouldReturnWrongParametersCodeVANullForMalloc() {
    int segmentsAmount = 1;
    int segmentSize = 1;
    _init(segmentsAmount, segmentSize);

    int actualCode = _malloc(NULL, segmentSize);

    int expectedCode = WRONG_PARAMETERS;
    assertEquals("shouldReturnWrongParametersCodeVANullForMalloc()", actualCode, expectedCode);
}

void shouldReturnWrongParametersCodeInvalidSegmentSizeForMalloc() {
    int segmentsAmount = 1;
    int segmentSize = 1;
    _init(segmentsAmount, segmentSize);
    int invalidSegmentSize = 0;
    VA address;

    int actualCode = _malloc(&address, invalidSegmentSize);

    int expectedCode = WRONG_PARAMETERS;
    assertEquals("shouldReturnWrongParametersCodeInvalidSegmentSizeForMalloc()", actualCode, expectedCode);
}

void shouldReturnOutOfMemoryCodeForMalloc() {
    int segmentsAmount = 1;
    int segmentSize = 1;
    _init(segmentsAmount, segmentSize);
    VA address;

    _malloc(&address, segmentSize);
    int actualCode = _malloc(&address, segmentSize);

    int expectedCode = OUT_OF_MEMORY;
    assertEquals("shouldReturnOutOfMemoryCodeForMalloc()", actualCode, expectedCode);
}

void shouldReturnSuccessfulCodeForMalloc() {
    int segmentsAmount = 1;
    int segmentSize = 1;
    _init(segmentsAmount, segmentSize);
    VA address;

    int actualCode = _malloc(&address, segmentSize);

    int expectedCode = SUCCESSFUL_CODE;
    assertEquals("shouldReturnSuccessfulCodeForMalloc()", actualCode, expectedCode);
}

void shouldReturnUnknownErrorCodeWithoutInitForFree() {
    VA address = malloc(sizeof(VA));

    int actualCode = _free(address);

    int expectedCode = UNKNOWN_ERROR;
    assertEquals("shouldReturnUnknownErrorCodeWithoutInitForFree()", actualCode, expectedCode);
}

void shouldReturnWrongParametersCodeVANullForFree() {
    int segmentsAmount = 1;
    int segmentSize = 1;
    _init(segmentsAmount, segmentSize);

    int actualCode = _free(NULL);

    int expectedCode = WRONG_PARAMETERS;
    assertEquals("shouldReturnWrongParametersCodeVANullForFree()", actualCode, expectedCode);
}

void shouldReturnUnknownErrorCodeWrongVAForFree() {
    int segmentsAmount = 1;
    int segmentSize = 1;
    _init(segmentsAmount, segmentSize);
    VA address;
    _malloc(&address, segmentSize);
    address = "2";

    int actualCode = _free(address);

    int expectedCode = UNKNOWN_ERROR;
    assertEquals("shouldReturnUnknownErrorCodeWrongVAForFree()", actualCode, expectedCode);
}

void shouldReturnSuccessfulCodeForFree() {
    int segmentsAmount = 1;
    int segmentSize = 1;
    _init(segmentsAmount, segmentSize);
    VA address;
    _malloc(&address, segmentSize);

    int actualCode = _free(address);

    int expectedCode = SUCCESSFUL_CODE;
    assertEquals("shouldReturnSuccessfulCodeForFree()", actualCode, expectedCode);
}

void shouldReturnUnknownErrorWhenDoubleInvokeForFree() {
    int segmentsAmount = 1;
    int segmentSize = 1;
    _init(segmentsAmount, segmentSize);
    VA address;
    _malloc(&address, segmentSize);

    _free(address);
    int actualCode = _free(address);

    int expectedCode = UNKNOWN_ERROR;
    assertEquals("shouldReturnUnknownErrorWhenDoubleInvokeForFree()", actualCode, expectedCode);
}

void shouldReturnSuccessfulCodeForWrite() {
    int segmentsAmount = 1;
    int segmentSize = 1;
    _init(segmentsAmount, segmentSize);
    VA address;
    _malloc(&address, segmentSize);
    char *buffer = "1";

    int actualCode = _write(address, buffer, segmentSize);

    int expectedCode = SUCCESSFUL_CODE;
    assertEquals("shouldReturnSuccessfulCodeForWrite()", actualCode, expectedCode);
}

void shouldReturnUnknownErrorCodeWithoutInitForWrite() {
    int segmentSize = 1;
    VA address = malloc(sizeof(VA));
    char *buffer = "1";

    int actualCode = _write(address, buffer, segmentSize);

    int expectedCode = UNKNOWN_ERROR;
    assertEquals("shouldReturnUnknownErrorCodeWithoutInitForWrite()", actualCode, expectedCode);
}

void shouldReturnWrongParametersCodeVANullForWrite() {
    int segmentsAmount = 1;
    int segmentSize = 1;
    char *buffer = "1";
    _init(segmentsAmount, segmentSize);

    int actualCode = _write(NULL, buffer, segmentSize);

    int expectedCode = WRONG_PARAMETERS;
    assertEquals("shouldReturnWrongParametersCodeVANullForWrite()", actualCode, expectedCode);
}

void shouldReturnWrongParametersCodeBufferNullForWrite() {
    int segmentsAmount = 1;
    int segmentSize = 1;
    VA address = malloc(sizeof(VA));
    _init(segmentsAmount, segmentSize);

    int actualCode = _write(address, NULL, segmentSize);

    int expectedCode = WRONG_PARAMETERS;
    assertEquals("shouldReturnWrongParametersCodeBufferNullForWrite()", actualCode, expectedCode);
}

void shouldReturnWrongParametersCodeInvalidSizeForWrite() {
    int segmentsAmount = 1;
    int segmentSize = 1;
    VA address = malloc(sizeof(VA));
    char *buffer = "1";
    int invalidBufferSize = 0;
    _init(segmentsAmount, segmentSize);

    int actualCode = _write(address, buffer, invalidBufferSize);

    int expectedCode = WRONG_PARAMETERS;
    assertEquals("shouldReturnWrongParametersCodeInvalidSizeForWrite()", actualCode, expectedCode);
}

void shouldReturnUnknownErrorCodeWithoutMallocForWrite() {
    int segmentsAmount = 1;
    int segmentSize = 1;
    char *buffer = "1";
    VA address = malloc(sizeof(VA));
    _init(segmentsAmount, segmentSize);

    int actualCode = _write(address, buffer, segmentSize);

    int expectedCode = UNKNOWN_ERROR;
    assertEquals("shouldReturnUnknownErrorCodeWithoutMallocForWrite()", actualCode, expectedCode);
}

void shouldReturnOutOfMemoryCodeWithIllegalBufferSizeForWrite() {
    int segmentsAmount = 1;
    int segmentSize = 1;
    char *buffer = "11";
    int bufferSize = 2;
    VA address;
    _init(segmentsAmount, segmentSize);
    _malloc(&address, segmentSize);

    int actualCode = _write(address, buffer, bufferSize);

    int expectedCode = OUT_OF_MEMORY;
    assertEquals("shouldReturnOutOfMemoryCodeWithIllegalBufferSizeForWrite()", actualCode, expectedCode);
}

void shouldReturnSuccessfulCodeWhenRewriteSegmentForWrite() {
    int segmentsAmount = 1;
    int segmentSize = 1;
    _init(segmentsAmount, segmentSize);
    VA address;
    _malloc(&address, segmentSize);
    char *buffer = "1";

    _write(address, buffer, segmentSize);
    int actualCode = _write(address, buffer, segmentSize);

    int expectedCode = SUCCESSFUL_CODE;
    assertEquals("shouldReturnSuccessfulCodeWhenRewriteSegmentForWrite()", actualCode, expectedCode);
}

void shouldReturnUnknownErrorCodeWithoutInitForRead() {
    int segmentSize = 1;
    VA address = malloc(sizeof(VA));
    char *buffer = "1";

    int actualCode = _read(address, buffer, segmentSize);

    int expectedCode = UNKNOWN_ERROR;
    assertEquals("shouldReturnUnknownErrorCodeWithoutInitForRead()", actualCode, expectedCode);
}

void shouldReturnWrongParametersCodeVANullForRead() {
    int segmentsAmount = 1;
    int segmentSize = 1;
    char *buffer = "1";
    _init(segmentsAmount, segmentSize);

    int actualCode = _read(NULL, buffer, segmentSize);

    int expectedCode = WRONG_PARAMETERS;
    assertEquals("shouldReturnWrongParametersCodeVANullForRead()", actualCode, expectedCode);
}

void shouldReturnWrongParametersCodeBufferNullForRead() {
    int segmentsAmount = 1;
    int segmentSize = 1;
    VA address = malloc(sizeof(VA));
    _init(segmentsAmount, segmentSize);

    int actualCode = _read(address, NULL, segmentSize);

    int expectedCode = WRONG_PARAMETERS;
    assertEquals("shouldReturnWrongParametersCodeBufferNullForRead()", actualCode, expectedCode);
}

void shouldReturnWrongParametersCodeInvalidSizeForRead() {
    int segmentsAmount = 1;
    int segmentSize = 1;
    VA address = malloc(sizeof(VA));
    char *buffer = "1";
    int invalidBufferSize = 0;
    _init(segmentsAmount, segmentSize);

    int actualCode = _read(address, buffer, invalidBufferSize);

    int expectedCode = WRONG_PARAMETERS;
    assertEquals("shouldReturnWrongParametersCodeInvalidSizeForRead()", actualCode, expectedCode);
}

void shouldReturnUnknownErrorCodeWithoutMallocForRead() {
    int segmentsAmount = 1;
    int segmentSize = 1;
    char *buffer = "1";
    VA address = malloc(sizeof(VA));
    _init(segmentsAmount, segmentSize);

    int actualCode = _read(address, buffer, segmentSize);

    int expectedCode = UNKNOWN_ERROR;
    assertEquals("shouldReturnUnknownErrorCodeWithoutMallocForRead()", actualCode, expectedCode);
}

void shouldReturnOutOfMemoryCodeWithIllegalBufferSizeForRead() {
    int segmentsAmount = 1;
    int segmentSize = 1;
    char *buffer = "11";
    int bufferSize = 2;
    VA address;
    _init(segmentsAmount, segmentSize);
    _malloc(&address, segmentSize);

    int actualCode = _read(address, buffer, bufferSize);

    int expectedCode = OUT_OF_MEMORY;
    assertEquals("shouldReturnOutOfMemoryCodeWithIllegalBufferSizeForRead()", actualCode, expectedCode);
}

void shouldReturnSuccessfulCodeForRead() {
    int segmentsAmount = 1;
    int segmentSize = 4;
    _init(segmentsAmount, segmentSize);
    VA address;
    _malloc(&address, segmentSize);
    char *expected = "1234";
    _write(address, expected, segmentSize);

    char *actual = calloc(segmentSize, sizeof(char));
    _read(address, actual, segmentSize);

    assertArraysEquals("shouldReturnSuccessfulCodeForRead()", actual, expected, segmentSize);
}