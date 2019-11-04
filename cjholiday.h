﻿#ifndef CJHOLIDAY_H
#define CJHOLIDAY_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    PyObject *(*HolidayName)(int, int, int);
    PyObject *(*HolidayNameDate)(PyObject*);
} CJHoliday_CAPI;


#ifdef CJHOLIDAY_MODULE
static PyObject *CJHoliday_HolidayName(int year, int month, int day);
static PyObject *CJHoliday_HolidayNameDate(PyObject *date);

#else
static CJHoliday_CAPI *CJHoliday_API;

#define CJHoliday_HolidayName(year, month, day) CJHoliday_API->HolidayName(year, month, day);
#define CJHoliday_HolidayNameDate(date) CJHoliday_API->HolidayNameDate(date);

#define CJHoliday_IMPORT CJHoliday_API = (CJHoliday_CAPI*)PyCapsule_Import("cjholiday.cjholiday._C_API", 0)

#endif

#ifdef __cplusplus
}
#endif

#endif
