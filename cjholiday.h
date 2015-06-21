#ifndef CJHOLIDAY_H
#define CJHOLIDAY_H
#ifdef __cplusplus
extern "C" {
#endif

/* C API functions */
#define CJHoliday_HolidayName_NUM 0
#define CJHoliday_HolidayName_RETURN PyObject *
#define CJHoliday_HolidayName_PROTO (long year, long month, long day)
#define CJHoliday_HolidayNameDate_NUM 1
#define CJHoliday_HolidayNameDate_RETURN PyObject *
#define CJHoliday_HolidayNameDate_PROTO (PyObject *date)

/* Total number of C API pointers */
#define CJHoliday_API_pointers 2


#ifdef CJHOLIDAY_MODULE
/* This section is used when compiling cjholiday.c */

static CJHoliday_HolidayName_RETURN CJHoliday_HolidayName CJHoliday_HolidayName_PROTO;
static CJHoliday_HolidayNameDate_RETURN CJHoliday_HolidayNameDate CJHoliday_HolidayNameDate_PROTO;

#else
/* This section is used in modules that use cjholiday's API */

static void **CJHoliday_API;

#define CJHoliday_HolidayName \
 (*(CJHoliday_HolidayName_RETURN (*)CJHoliday_HolidayName_PROTO) CJHoliday_API[CJHoliday_HolidayName_NUM])
#define CJHoliday_HolidayNameDate \
 (*(CJHoliday_HolidayNameDate_RETURN (*)CJHoliday_HolidayNameDate_PROTO) CJHoliday_API[CJHoliday_HolidayNameDate_NUM])

static int
import_cjholiday(void)
{
    CJHoliday_API = (void **)PyCapsule_Import("cjholiday._C_API", 0);
    return (CJHoliday_API != NULL) ? 0 : -1;
}

#endif

#ifdef __cplusplus
}
#endif

#endif /* !defined(CJHOLIDAY_H) */
