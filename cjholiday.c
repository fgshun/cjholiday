#include <Python.h>
#include <datetime.h>
#define CJHOLIDAY_MODULE
#include "cjholiday.h"

/*
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//_/
//_/  CopyRight(C) K.Tsunoda(AddinBox) 2001 All Rights Reserved.
//_/  ( http://www.h3.dion.ne.jp/~sakatsu/index.htm )
//_/
//_/    この祝日判定コードは『Excel:kt関数アドイン』で使用しているものです。
//_/    この関数では、２０１６年施行の改正祝日法(山の日)までを
//_/  　サポートしています。
//_/
//_/  (*1)このコードを引用するに当たっては、必ずこのコメントも
//_/      一緒に引用する事とします。
//_/  (*2)他サイト上で本マクロを直接引用する事は、ご遠慮願います。
//_/      【 http://www.h3.dion.ne.jp/~sakatsu/holiday_logic.htm 】
//_/      へのリンクによる紹介で対応して下さい。
//_/  (*3)[ktHolidayName]という関数名そのものは、各自の環境に
//_/      おける命名規則に沿って変更しても構いません。
//_/  
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
*/

/*
 * fgshun  http://d.hatena.ne.jp/fgshun/
 *
 * このコードは
 * SETOGUCHI Mitsuhiro (http://matatabi.homeip.net/) 氏のスクリプト
 * (http://www.h3.dion.ne.jp/~sakatsu/holiday_logic5.htm#Python)
 * を fgshun が C エクステンションとして組みなおしたものです。
 */

typedef struct {
    const Py_UCS2 *name;
    const Py_ssize_t length;
} HolidayName;

/* 元日 */
static const Py_UCS2 _GANJITSU[] = {
    0x5143, 0x65e5};
static const HolidayName GANJITSU = {
    _GANJITSU, 2};
/* 成人の日 */
static const Py_UCS2 _SEIJINNOHI[] = {
    0x6210, 0x4eba, 0x306e, 0x65e5};
static const HolidayName SEIJINNOHI = {
    _SEIJINNOHI, 4};
/* 建国記念の日 */
static const Py_UCS2 _KENKOKUKINENNOHI[] = {
    0x5efa, 0x56fd, 0x8a18, 0x5ff5, 0x306e, 0x65e5};
static const HolidayName KENKOKUKINENNOHI = {
    _KENKOKUKINENNOHI, 6};
/* 春分の日 */
static const Py_UCS2 _SHUNBUNNOHI[] = {
    0x6625, 0x5206, 0x306e, 0x65e5};
static const HolidayName SHUNBUNNOHI = {
    _SHUNBUNNOHI, 4};
/* 昭和の日 */
static const Py_UCS2 _SHOWANOHI[] = {
    0x662d, 0x548c, 0x306e, 0x65e5};
static const HolidayName SHOWANOHI = {
    _SHOWANOHI, 4};
/* 憲法記念日 */
static const Py_UCS2 _KENPOKINENBI[] = {
    0x61b2, 0x6cd5, 0x8a18, 0x5ff5, 0x65e5};
static const HolidayName KENPOKINENBI = {
    _KENPOKINENBI, 5};
/* みどりの日 */
static const Py_UCS2 _MIDORINOHI[] = {
    0x307f, 0x3069, 0x308a, 0x306e, 0x65e5};
static const HolidayName MIDORINOHI = {
    _MIDORINOHI, 5};
/* こどもの日 */
static const Py_UCS2 _KODOMONOHI[] = {
    0x3053, 0x3069, 0x3082, 0x306e, 0x65e5};
static const HolidayName KODOMONOHI = {
    _KODOMONOHI, 5};
/* 海の日 */
static const Py_UCS2 _UMINOHI[] = {
    0x6d77, 0x306e, 0x65e5};
static const HolidayName UMINOHI = {
    _UMINOHI, 3};
/* 山の日 */
static const Py_UCS2 _YAMANOHI[] = {
    0x5c71, 0x306e, 0x65e5};
static const HolidayName YAMANOHI = {
    _YAMANOHI, 3};
/* 敬老の日 */
static const Py_UCS2 _KEIRONOHI[] = {
    0x656c, 0x8001, 0x306e, 0x65e5};
static const HolidayName KEIRONOHI = {
    _KEIRONOHI, 4};
/* 秋分の日 */
static const Py_UCS2 _SHUBUNNOHI[] = {
    0x79cb, 0x5206, 0x306e, 0x65e5};
static const HolidayName SHUBUNNOHI = {
    _SHUBUNNOHI, 4};
/* 体育の日 */
static const Py_UCS2 _TAIKUNOHI[] = {
    0x4f53, 0x80b2, 0x306e, 0x65e5};
static const HolidayName TAIKUNOHI = {
    _TAIKUNOHI, 4};
/* 文化の日 */
static const Py_UCS2 _BUNKANOHI[] = {
    0x6587, 0x5316, 0x306e, 0x65e5};
static const HolidayName BUNKANOHI = {
    _BUNKANOHI, 4};
/* 勤労感謝の日 */
static const Py_UCS2 _KINROKANSHANOHI[] = {
    0x52e4, 0x52b4, 0x611f, 0x8b1d, 0x306e, 0x65e5};
static const HolidayName KINROKANSHANOHI = {
    _KINROKANSHANOHI, 6};
/* 天皇誕生日 */
static const Py_UCS2 _TENNOTANJOBI[] = {
    0x5929, 0x7687, 0x8a95, 0x751f, 0x65e5};
static const HolidayName TENNOTANJOBI = {
    _TENNOTANJOBI, 5};
/* 振替休日 */
static const Py_UCS2 _FURIKAEKYUJITSU[] = {
    0x632f, 0x66ff, 0x4f11, 0x65e5};
static const HolidayName FURIKAEKYUJITSU = {
    _FURIKAEKYUJITSU, 4};
/* 国民の休日 */
static const Py_UCS2 _KOKUMINNOKYUJITSU[] = {
    0x56fd, 0x6c11, 0x306e, 0x4f11, 0x65e5};
static const HolidayName KOKUMINNOKYUJITSU = {
    _KOKUMINNOKYUJITSU, 5};
/* 皇太子明仁親王の結婚の儀 */
static const Py_UCS2 _KOUTAISHIAKIHITOSHINNOUNOKEKKONNOGI[] = {
    0x7687, 0x592a, 0x5b50, 0x660e, 0x4ec1, 0x89aa, 0x738b, 0x306e,
    0x7d50, 0x5a5a, 0x306e, 0x5100};
static const HolidayName KOUTAISHIAKIHITOSHINNOUNOKEKKONNOGI = {
    _KOUTAISHIAKIHITOSHINNOUNOKEKKONNOGI, 12};
/* 昭和天皇の大喪の礼 */
static const Py_UCS2 _SHOWATENNOUNOTAIMOUNOREI[] = {
    0x662d, 0x548c, 0x5929, 0x7687, 0x306e, 0x5927, 0x55aa, 0x306e,
    0x793c};
static const HolidayName SHOWATENNOUNOTAIMOUNOREI = {
    _SHOWATENNOUNOTAIMOUNOREI, 9};
/* 即位礼正殿の儀 */
static const Py_UCS2 _SOKUIREISEIDENNOGI[] = {
    0x5373, 0x4f4d, 0x793c, 0x6b63, 0x6bbf, 0x306e, 0x5100};
static const HolidayName SOKUIREISEIDENNOGI = {
    _SOKUIREISEIDENNOGI, 7};
/* 皇太子徳仁親王の結婚の儀 */
static const Py_UCS2 _KOUTAISHINARUHITOSHINNOUNOKEKKONNOGI[] = {
    0x7687, 0x592a, 0x5b50, 0x5fb3, 0x4ec1, 0x89aa, 0x738b, 0x306e,
    0x7d50, 0x5a5a, 0x306e, 0x5100};
static const HolidayName KOUTAISHINARUHITOSHINNOUNOKEKKONNOGI = {
    _KOUTAISHINARUHITOSHINNOUNOKEKKONNOGI, 12};



typedef struct {
    PyObject *Delta_Day1;
} cjholidayState;

#define cjholidaystate(o) ((cjholidayState*)PyModule_GetState(o))

static int cjholiday_traverse(PyObject *m, visitproc visit, void *arg) {
    Py_VISIT(cjholidaystate(m)->Delta_Day1);
    return 0;
}

static int cjholiday_clear(PyObject *m) {
    Py_CLEAR(cjholidaystate(m)->Delta_Day1);
    return 0;
}

static struct PyModuleDef cjholiday_module;

#define cjholidaystate_gloval ((cjholidayState*)PyModule_GetState(PyState_FindModule(&cjholiday_module)))



static long get_weekday(PyObject *date) {
    /* date.weekday()
       失敗時、例外を設定し -1 を返す。*/
    long weekday;
    PyObject *weekday_py;

    weekday_py = PyObject_CallMethod(date, "weekday", NULL);
    if (weekday_py == NULL) { return -1; }
    weekday = PyLong_AS_LONG(weekday_py);
    if (weekday == -1 && PyErr_Occurred()) { return -1; }
    Py_DECREF(weekday_py);

    return weekday;
}

static long vernal_equinox(long year) {
    /* 春分の日判定 */
    long day;
    if (year <= 1947){
        day = 0;
    } else if (year <= 1979){
        day = (long)(20.8357 + (0.242194 * (year - 1980)) -
                (year - 1983) / 4);
    } else if (year <= 2099){
        day = (long)(20.8431 + (0.242194 * (year - 1980)) -
                (year - 1980) / 4);
    } else if (year <= 2150){
        day = (long)(21.851 + (0.242194 * (year - 1980)) -
                (year - 1980) / 4);
    } else {
        day = 0;
    }

    return day;
}

static long autumn_equinox(long year) {
    /* 秋分の日判定 */
    long day;
    if (year <= 1947){
        day = 0;
    } else if (year <= 1979){
        day = (long)(23.2588 + (0.242194 * (year - 1980)) -
                (year - 1983) / 4);
    } else if (year <= 2099){
        day = (long)(23.2488 + (0.242194 * (year - 1980)) -
                (year - 1980) / 4);
    } else if (year <= 2150){
        day = (long)(24.2488 + (0.242194 * (year - 1980)) -
                (year - 1980) / 4);
    } else {
        day = 0;
    }

    return day;
}

static PyObject *
CJHoliday_HolidayName(long year, long month, long day) {
    /* year, month, day の祝日名を返す。祝日ではないときは None を返す。
       失敗時、例外を設定し NULL を返す。
     */
    PyObject *date;
    PyObject *result;

    date =  PyDateTime_FromDateAndTime(year, month, day, 0, 0, 0, 0);
    if (date == NULL) { return NULL; }

    result = CJHoliday_HolidayNameDate(date);
    Py_DECREF(date);
    return result;
}

static PyObject *
CJHoliday_HolidayNameDate(PyObject *date) {
    /* date の祝日名を返す。祝日ではないときは None を返す。
       失敗時、例外を設定し NULL を返す。
     */
    long year, month, day;
    long autumn;
    long weekday = -1;
    const HolidayName *name = NULL;
    PyObject *result;

    if (!PyDate_Check(date)) { return NULL; }

    year = PyDateTime_GET_YEAR(date);
    month = PyDateTime_GET_MONTH(date);
    day = PyDateTime_GET_DAY(date);

    if (year < 1948) {
        Py_RETURN_NONE;
    }
    else if (year == 1948) {
        if (month < 7) {
            Py_RETURN_NONE;
        }
        else if (month == 7 && day < 20) {
            Py_RETURN_NONE;
        }
    }

    switch (month) {
        case 1:
            if (day == 1) {
                name = &GANJITSU;
            }
            else if (year >= 2000) {
                if ((day - 1) / 7 == 1) {
                    if ((weekday = get_weekday(date)) == -1) { return NULL; }
                    if (weekday == 0) {
                        name = &SEIJINNOHI;
                    }
                }
            }
            else if (day == 15) {
                name = &SEIJINNOHI;
            }
            break;
        case 2:
            if (day == 11 && year >= 1967) {
                name = &KENKOKUKINENNOHI;
            }
            else if (year == 1989 && day == 24) {
                /* 1989/2/24 */
                name = &SHOWATENNOUNOTAIMOUNOREI;
            }
            break;
        case 3:
            if (day == vernal_equinox(year)) {
                name = &SHUNBUNNOHI;
            }
            break;
        case 4:
            if (day == 29) {
                if (year >= 2007) {
                    name = &SHOWANOHI;
                }
                else if (year >= 1989) {
                    name = &MIDORINOHI;
                }
                else {
                    name = &TENNOTANJOBI;
                }
            }
            else if (year == 1959 && day == 10) {
                /* 1959/4/10 */
                name = &KOUTAISHIAKIHITOSHINNOUNOKEKKONNOGI;
            }
            break;
        case 5:
            if (day == 3) {
                name = &KENPOKINENBI;
            }
            else if (day == 4) {
                if (year >= 2007) {
                    name = &MIDORINOHI;
                }
                else if (year >= 1986) {
                    if ((weekday = get_weekday(date)) == -1) { return NULL; }
                    if (weekday != 0) {
                        name = &KOKUMINNOKYUJITSU;
                    }
                }
            }
            else if (day == 5) {
                name = &KODOMONOHI;
            }
            else if (day == 6) {
                if (year >= 2007) {
                    if ((weekday = get_weekday(date)) == -1) { return NULL; }
                    if (weekday == 1 || weekday == 2) {
                        name = &FURIKAEKYUJITSU;
                    }
                }
            }
            break;
        case 6:
            if (year == 1993 && day == 9) {
                /* 1993/6/9 */
                name = &KOUTAISHINARUHITOSHINNOUNOKEKKONNOGI;
            }
            break;
        case 7:
            if (year >= 2003) {
                if ((day - 1) / 7 == 2) {
                    if ((weekday = get_weekday(date)) == -1) { return NULL; }
                    if (weekday == 0) {
                        name = &UMINOHI;
                    }
                }
            }
            else if (year >= 1996 && day == 20) {
                name = &UMINOHI;
            }
            break;
        case 8:
            if (year >= 2016 && day == 11) {
                name = &YAMANOHI;
            }
            break;
        case 9:
            autumn = autumn_equinox(year);
            if (day == autumn) {
                name = &SHUBUNNOHI;
            }
            else {
                if (year >= 2003) {
                    if ((weekday = get_weekday(date)) == -1) { return NULL; }
                    if ((day - 1) / 7 == 2 && weekday == 0) {
                        name = &KEIRONOHI;
                    }
                    else if (weekday == 1 && day == autumn - 1) {
                        name = &KOKUMINNOKYUJITSU;
                    }
                }
                else if (year >= 1966 && day == 15) {
                    name = &KEIRONOHI;
                }
            }
            break;
        case 10:
            if (year >= 2000) {
                if ((day - 1) / 7 == 1) {
                    if ((weekday = get_weekday(date)) == -1) { return NULL; }
                    if (weekday == 0) {
                        name = &TAIKUNOHI;
                    }
                }
            }
            else if (year >= 1966 && day == 10) {
                name = &TAIKUNOHI;
            }
            break;
        case 11:
            if (day == 3) {
                name = &BUNKANOHI;
            }
            else if (day == 23) {
                name = &KINROKANSHANOHI;
            }
            else if (year == 1990 && day == 12) {
                name = &SOKUIREISEIDENNOGI;
            }
            break;
        case 12:
            if (day == 23 && year >= 1989) {
                name = &TENNOTANJOBI;
            }
    }

    if (name == NULL) {
        if (weekday == -1) {
            if ((weekday = get_weekday(date)) == -1) { return NULL; }
        }
        if (weekday == 0) {
            PyObject *prev, *prev_name;

            prev = PyNumber_Subtract(date, cjholidaystate_gloval->Delta_Day1);
            if (prev == NULL) { return NULL; }

            prev_name = CJHoliday_HolidayNameDate(prev);
            Py_DECREF(prev);
            if (prev_name == NULL) { return NULL; }

            if (prev_name == Py_None) {
                return prev_name;
            }
            Py_DECREF(prev_name);

            name = &FURIKAEKYUJITSU;
        }
    }

    if (name == NULL) {
        Py_RETURN_NONE;
    }

    result = PyUnicode_FromKindAndData(PyUnicode_2BYTE_KIND, name->name, name->length);
    PyUnicode_InternInPlace(&result);
    return result;
}

static PyObject *
holiday_name_date(PyObject *self, PyObject *args, PyObject *kwargs) {
    /* 非推奨。 date の祝日判定 */
    PyObject *date;
    static char *kwlist[] = {"date", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O", kwlist, &date)) {
        return NULL;
    }

    return CJHoliday_HolidayNameDate(date);
}

static PyObject *
holiday_name(PyObject *self, PyObject *args, PyObject *kwargs) {
    /* 年月日を表す 3 整数もしくは date の祝日判定 */
    PyObject *result;
    PyObject *year = NULL, *month = NULL, *day = NULL, *date = NULL;
    static char *kwlist[] = {"year", "month", "day", "date", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|OOOO", kwlist, &year, &month, &day, &date)) {
        return NULL;
    }

    if (date != NULL && PyDate_Check(date)) {
        result = CJHoliday_HolidayNameDate(date);
    } else if (year != NULL && PyLong_Check(year) &&
               month != NULL && PyLong_Check(month) &&
               day != NULL && PyLong_Check(day)) {
        result = CJHoliday_HolidayName(
                PyLong_AsLong(year), PyLong_AsLong(month), PyLong_AsLong(day));
    } else {
        PyErr_SetString(PyExc_TypeError, "year, month, day is not int or date is not datetime.date");
        return NULL;
    }

    return result;
}

static PyMethodDef cjholiday_method[] = {
    {"holiday_name_date", (PyCFunction)holiday_name_date,
     METH_VARARGS | METH_KEYWORDS,
     "deplicated. get name of holiday from datetime.date."},
    {"holiday_name", (PyCFunction)holiday_name,
     METH_VARARGS | METH_KEYWORDS,
     "get name of holiday from ymd or datetime.date."},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef cjholiday_module = {
    PyModuleDef_HEAD_INIT,
    "cjholiday",   /* name of module */
    NULL, /* module documentation, may be NULL */
    sizeof(cjholidayState), /* size of per-interpreter state of the module */
    cjholiday_method,
    NULL, /* m_reload, currently unused */
    cjholiday_traverse,
    cjholiday_clear,
    NULL /* m_free, A function to call during deallocation of the module object */
};

PyMODINIT_FUNC PyInit_cjholiday(void) {
    PyObject *module = NULL;
    PyObject *c_api_object = NULL;
    static void *CJHoliday_API[CJHoliday_API_pointers];

    module = PyModule_Create(&cjholiday_module);
    if (module == NULL) { goto fail; }

    /* version */
    if (PyModule_AddStringConstant(module, "version", "1.0.4")) { goto fail; }

    /* Initialize the C API pointer array */
    CJHoliday_API[CJHoliday_HolidayName_NUM] = (void *)CJHoliday_HolidayName;
    CJHoliday_API[CJHoliday_HolidayNameDate_NUM] = (void *)CJHoliday_HolidayNameDate;

    /* Create a Capsule containing the API pointer array's address */
    c_api_object = PyCapsule_New((void *)CJHoliday_API, "cjholiday._C_API", NULL);
    if (c_api_object == NULL) { goto fail; }

    if (PyModule_AddObject(module, "_C_API", c_api_object) == -1) { goto fail; }

    /* datetime 関連の C API を使えるようにする */
    PyDateTime_IMPORT;
    if (!PyDateTimeAPI) { goto fail; }

    /* datetime.timedelta(days=1) */
    if ((cjholidaystate(module)->Delta_Day1 = PyDelta_FromDSU(1, 0, 0)) == NULL) { goto fail; }

    return module;

fail:
    Py_XDECREF(module);
    Py_XDECREF(c_api_object);
    return NULL;
}
/*
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//_/ CopyRight(C) K.Tsunoda(AddinBox) 2001 All Rights Reserved.
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
*/
