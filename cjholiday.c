#include <Python.h>
#include <datetime.h>
#define CJHOLIDAY_MODULE
#include "cjholiday.h"

/*
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//_/
//_/  CopyRight(C) K.Tsunoda(AddinBox) 2001 All Rights Reserved.
//_/  ( AddinBox  http://addinbox.sakura.ne.jp/index.htm )
//_/  (  旧サイト  http://www.h3.dion.ne.jp/~sakatsu/index.htm )
//_/
//_/    この祝日判定コードは『Excel:kt関数アドイン』で使用しているものです。
//_/
//_/    この関数では、２０１９年施行の「天皇誕生日の変更」までを
//_/    サポートしています。
//_/
//_/  (*1)このコードを引用するに当たっては、必ずこのコメントも
//_/      一緒に引用する事とします。
//_/  (*2)他サイト上で本マクロを直接引用する事は、ご遠慮願います。
//_/      【 http://addinbox.sakura.ne.jp/holiday_logic.htm 】
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
 * SETOGUCHI Mitsuhiro (https://straitmouth.jp/blog/setomits/) 氏のスクリプト
 * (http://addinbox.sakura.ne.jp/holiday_logic5.htm#Python)
 * を fgshun が C エクステンションとして組みなおしたものです。
 */

/* datetime.timedelta(days=1) */
static PyObject *Delta_Day1;
/* 元日 */
static PyObject *GANJITSU;
/* 成人の日 */
static PyObject *SEIJINNOHI;
/* 建国記念の日 */
static PyObject *KENKOKUKINENNOHI;
/* 春分の日 */
static PyObject *SHUNBUNNOHI;
/* 昭和の日 */
static PyObject *SHOWANOHI;
/* 憲法記念日 */
static PyObject *KENPOKINENBI;
/* みどりの日 */
static PyObject *MIDORINOHI;
/* こどもの日 */
static PyObject *KODOMONOHI;
/* 海の日 */
static PyObject *UMINOHI;
/* 山の日 */
static PyObject *YAMANOHI;
/* 敬老の日 */
static PyObject *KEIRONOHI;
/* 秋分の日 */
static PyObject *SHUBUNNOHI;
/* 体育の日 */
static PyObject *TAIKUNOHI;
/* 文化の日 */
static PyObject *BUNKANOHI;
/* 勤労感謝の日 */
static PyObject *KINROKANSHANOHI;
/* 天皇誕生日 */
static PyObject *TENNOTANJOBI;
/* 振替休日 */
static PyObject *FURIKAEKYUJITSU;
/* 国民の休日 */
static PyObject *KOKUMINNOKYUJITSU;
/* 皇太子明仁親王の結婚の儀 */
static PyObject *KOUTAISHIAKIHITOSHINNOUNOKEKKONNOGI;
/* 昭和天皇の大喪の礼 */
static PyObject *SHOWATENNOUNOTAIMOUNOREI;
/* 即位礼正殿の儀 */
static PyObject *SOKUIREISEIDENNOGI;
/* 皇太子徳仁親王の結婚の儀 */
static PyObject *KOUTAISHINARUHITOSHINNOUNOKEKKONNOGI;

static struct PyModuleDef cjholiday_module;
#define cjholidaystate(o) ((cjholidayState*)PyModule_GetState(o))

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

    date =  PyDate_FromDate(year, month, day);
    if (date == NULL) { return NULL; }

    result = CJHoliday_HolidayNameDate(date);
    Py_DECREF(date);
    return result;
}

static PyObject *
CJHoliday_HolidayNameDate(PyObject *date) {
    /* 祝日名を返す。祝日ではないときは None を返す。
       失敗時、例外を設定し NULL を返す。
     */
    long year, month, day;
    long autumn;
    long weekday = -1;
    PyObject *name = Py_None;

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
                name = GANJITSU;
            }
            else if (year >= 2000) {
                if ((day - 1) / 7 == 1) {
                    if ((weekday = get_weekday(date)) == -1) { return NULL; }
                    if (weekday == 0) {
                        name = SEIJINNOHI;
                    }
                }
            }
            else if (day == 15) {
                name = SEIJINNOHI;
            }
            break;
        case 2:
            if (day == 11 && year >= 1967) {
                name = KENKOKUKINENNOHI;
            }
            else if (day == 23 && year >= 2020) {
                name = TENNOTANJOBI;
            }
            else if (year == 1989 && day == 24) {
                /* 1989/2/24 */
                name = SHOWATENNOUNOTAIMOUNOREI;
            }
            break;
        case 3:
            if (day == vernal_equinox(year)) {
                name = SHUNBUNNOHI;
            }
            break;
        case 4:
            if (day == 29) {
                if (year >= 2007) {
                    name = SHOWANOHI;
                }
                else if (year >= 1989) {
                    name = MIDORINOHI;
                }
                else {
                    /* 昭和天皇 */
                    name = TENNOTANJOBI;
                }
            }
            else if (year == 1959 && day == 10) {
                /* 1959/4/10 */
                name = KOUTAISHIAKIHITOSHINNOUNOKEKKONNOGI;
            }
            break;
        case 5:
            if (day == 3) {
                name = KENPOKINENBI;
            }
            else if (day == 4) {
                if (year >= 2007) {
                    name = MIDORINOHI;
                }
                else if (year >= 1986) {
                    if ((weekday = get_weekday(date)) == -1) { return NULL; }
                    if (weekday != 0) {
                        name = KOKUMINNOKYUJITSU;
                    }
                }
            }
            else if (day == 5) {
                name = KODOMONOHI;
            }
            else if (day == 6) {
                if (year >= 2007) {
                    if ((weekday = get_weekday(date)) == -1) { return NULL; }
                    if (weekday == 1 || weekday == 2) {
                        name = FURIKAEKYUJITSU;
                    }
                }
            }
            break;
        case 6:
            if (year == 1993 && day == 9) {
                /* 1993/6/9 */
                name = KOUTAISHINARUHITOSHINNOUNOKEKKONNOGI;
            }
            break;
        case 7:
            if (year >= 2003) {
                if ((day - 1) / 7 == 2) {
                    if ((weekday = get_weekday(date)) == -1) { return NULL; }
                    if (weekday == 0) {
                        name = UMINOHI;
                    }
                }
            }
            else if (year >= 1996 && day == 20) {
                name = UMINOHI;
            }
            break;
        case 8:
            if (year >= 2016 && day == 11) {
                name = YAMANOHI;
            }
            break;
        case 9:
            autumn = autumn_equinox(year);
            if (day == autumn) {
                name = SHUBUNNOHI;
            }
            else {
                if (year >= 2003) {
                    if ((weekday = get_weekday(date)) == -1) { return NULL; }
                    if ((day - 1) / 7 == 2 && weekday == 0) {
                        name = KEIRONOHI;
                    }
                    else if (weekday == 1 && day == autumn - 1) {
                        name = KOKUMINNOKYUJITSU;
                    }
                }
                else if (year >= 1966 && day == 15) {
                    name = KEIRONOHI;
                }
            }
            break;
        case 10:
            if (year >= 2000) {
                if ((day - 1) / 7 == 1) {
                    if ((weekday = get_weekday(date)) == -1) { return NULL; }
                    if (weekday == 0) {
                        name = TAIKUNOHI;
                    }
                }
            }
            else if (year >= 1966 && day == 10) {
                name = TAIKUNOHI;
            }
            break;
        case 11:
            if (day == 3) {
                name = BUNKANOHI;
            }
            else if (day == 23) {
                name = KINROKANSHANOHI;
            }
            else if (year == 1990 && day == 12) {
                name = SOKUIREISEIDENNOGI;
            }
            break;
        case 12:
            if (day == 23 && year >= 1989 && year <= 2018) {
                /* 平成天皇 */
                name = TENNOTANJOBI;
            }
    }

    if (name == Py_None) {
        if (weekday == -1) {
            if ((weekday = get_weekday(date)) == -1) { return NULL; }
        }
        if (weekday == 0) {
            PyObject *prev_date, *prev_name;

            prev_date = PyNumber_Subtract(date, Delta_Day1);
            if (prev_date == NULL) { return NULL; }

            prev_name = CJHoliday_HolidayNameDate(prev_date);
            Py_DECREF(prev_date);
            if (prev_name == NULL) { return NULL; }

            if (prev_name == Py_None) {
                return prev_name;
            }
            Py_DECREF(prev_name);

            name = FURIKAEKYUJITSU;
        }
    }

    Py_INCREF(name);
    return name;
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
    {"holiday_name", (PyCFunction)holiday_name,
     METH_VARARGS | METH_KEYWORDS,
     "get name of holiday from ymd or datetime.date."},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef cjholiday_module = {
    PyModuleDef_HEAD_INIT,
    "cjholiday",
    NULL,
    0,
    cjholiday_method,
    NULL,
    NULL,
    NULL,
    NULL
};

PyMODINIT_FUNC PyInit_cjholiday(void) {
    PyObject *module = NULL;
    PyObject *c_api_object = NULL;
    static void *CJHoliday_API[CJHoliday_API_pointers];

    module = PyModule_Create(&cjholiday_module);
    if (module == NULL) { goto fail; }

    /* version */
    if (PyModule_AddStringConstant(module, "version", "1.1.3")) { goto fail; }

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
    if (Delta_Day1 == NULL && (Delta_Day1 = PyDelta_FromDSU(1, 0, 0)) == NULL) { goto fail; }

    /* 祝日名 */
    if (GANJITSU == NULL && (GANJITSU = PyUnicode_FromString("元日")) == NULL) { goto fail; }
    if (SEIJINNOHI == NULL && (SEIJINNOHI = PyUnicode_FromString("成人の日")) == NULL) { goto fail; }
    if (KENKOKUKINENNOHI == NULL && (KENKOKUKINENNOHI = PyUnicode_FromString("建国記念の日")) == NULL) { goto fail; }
    if (SHUNBUNNOHI == NULL && (SHUNBUNNOHI = PyUnicode_FromString("春分の日")) == NULL) { goto fail; }
    if (SHOWANOHI == NULL && (SHOWANOHI = PyUnicode_FromString("昭和の日")) == NULL) { goto fail; }
    if (KENPOKINENBI == NULL && (KENPOKINENBI = PyUnicode_FromString("憲法記念日")) == NULL) { goto fail; }
    if (MIDORINOHI == NULL && (MIDORINOHI = PyUnicode_FromString("みどりの日")) == NULL) { goto fail; }
    if (KODOMONOHI == NULL && (KODOMONOHI = PyUnicode_FromString("こどもの日")) == NULL) { goto fail; }
    if (UMINOHI == NULL && (UMINOHI = PyUnicode_FromString("海の日")) == NULL) { goto fail; }
    if (YAMANOHI == NULL && (YAMANOHI = PyUnicode_FromString("山の日")) == NULL) { goto fail; }
    if (KEIRONOHI == NULL && (KEIRONOHI = PyUnicode_FromString("敬老の日")) == NULL) { goto fail; }
    if (SHUBUNNOHI == NULL && (SHUBUNNOHI = PyUnicode_FromString("秋分の日")) == NULL) { goto fail; }
    if (TAIKUNOHI == NULL && (TAIKUNOHI = PyUnicode_FromString("体育の日")) == NULL) { goto fail; }
    if (BUNKANOHI == NULL && (BUNKANOHI = PyUnicode_FromString("文化の日")) == NULL) { goto fail; }
    if (KINROKANSHANOHI == NULL && (KINROKANSHANOHI = PyUnicode_FromString("勤労感謝の日")) == NULL) { goto fail; }
    if (TENNOTANJOBI == NULL && (TENNOTANJOBI = PyUnicode_FromString("天皇誕生日")) == NULL) { goto fail; }
    if (FURIKAEKYUJITSU == NULL && (FURIKAEKYUJITSU = PyUnicode_FromString("振替休日")) == NULL) { goto fail; }
    if (KOKUMINNOKYUJITSU == NULL && (KOKUMINNOKYUJITSU = PyUnicode_FromString("国民の休日")) == NULL) { goto fail; }
    if (KOUTAISHIAKIHITOSHINNOUNOKEKKONNOGI == NULL && (KOUTAISHIAKIHITOSHINNOUNOKEKKONNOGI = PyUnicode_FromString("皇太子明仁親王の結婚の儀")) == NULL) { goto fail; }
    if (SHOWATENNOUNOTAIMOUNOREI == NULL && (SHOWATENNOUNOTAIMOUNOREI = PyUnicode_FromString("昭和天皇の大喪の礼")) == NULL) { goto fail; }
    if (SOKUIREISEIDENNOGI == NULL && (SOKUIREISEIDENNOGI = PyUnicode_FromString("即位礼正殿の儀")) == NULL) { goto fail; }
    if (KOUTAISHINARUHITOSHINNOUNOKEKKONNOGI == NULL && (KOUTAISHINARUHITOSHINNOUNOKEKKONNOGI = PyUnicode_FromString("皇太子徳仁親王の結婚の儀")) == NULL) { goto fail; }

    return module;

fail:
    Py_XDECREF(module);
    Py_CLEAR(c_api_object);
    Py_CLEAR(Delta_Day1);
    Py_CLEAR(GANJITSU);
    Py_CLEAR(SEIJINNOHI);
    Py_CLEAR(KENKOKUKINENNOHI);
    Py_CLEAR(SHUNBUNNOHI);
    Py_CLEAR(SHOWANOHI);
    Py_CLEAR(KENPOKINENBI);
    Py_CLEAR(MIDORINOHI);
    Py_CLEAR(KODOMONOHI);
    Py_CLEAR(UMINOHI);
    Py_CLEAR(YAMANOHI);
    Py_CLEAR(KEIRONOHI);
    Py_CLEAR(SHUBUNNOHI);
    Py_CLEAR(TAIKUNOHI);
    Py_CLEAR(BUNKANOHI);
    Py_CLEAR(KINROKANSHANOHI);
    Py_CLEAR(TENNOTANJOBI);
    Py_CLEAR(FURIKAEKYUJITSU);
    Py_CLEAR(KOKUMINNOKYUJITSU);
    Py_CLEAR(KOUTAISHIAKIHITOSHINNOUNOKEKKONNOGI);
    Py_CLEAR(SHOWATENNOUNOTAIMOUNOREI);
    Py_CLEAR(SOKUIREISEIDENNOGI);
    Py_CLEAR(KOUTAISHINARUHITOSHINNOUNOKEKKONNOGI);
    return NULL;
}
/*
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//_/ CopyRight(C) K.Tsunoda(AddinBox) 2001 All Rights Reserved.
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
*/
