#define Py_LIMITED_API 0x03050000
#include <Python.h>

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

typedef struct {
    PyObject *DateClass;
    PyObject *Delta_Day1;
    /* 元日 */
    PyObject *GANJITSU;
    /* 成人の日 */
    PyObject *SEIJINNOHI;
    /* 建国記念の日 */
    PyObject *KENKOKUKINENNOHI;
    /* 春分の日 */
    PyObject *SHUNBUNNOHI;
    /* 昭和の日 */
    PyObject *SHOWANOHI;
    /* 憲法記念日 */
    PyObject *KENPOKINENBI;
    /* みどりの日 */
    PyObject *MIDORINOHI;
    /* こどもの日 */
    PyObject *KODOMONOHI;
    /* 海の日 */
    PyObject *UMINOHI;
    /* 山の日 */
    PyObject *YAMANOHI;
    /* 敬老の日 */
    PyObject *KEIRONOHI;
    /* 秋分の日 */
    PyObject *SHUBUNNOHI;
    /* 体育の日 */
    PyObject *TAIKUNOHI;
    /* 文化の日 */
    PyObject *BUNKANOHI;
    /* 勤労感謝の日 */
    PyObject *KINROKANSHANOHI;
    /* 天皇誕生日 */
    PyObject *TENNOTANJOBI;
    /* 振替休日 */
    PyObject *FURIKAEKYUJITSU;
    /* 国民の休日 */
    PyObject *KOKUMINNOKYUJITSU;
    /* 皇太子明仁親王の結婚の儀 */
    PyObject *KOUTAISHIAKIHITOSHINNOUNOKEKKONNOGI;
    /* 昭和天皇の大喪の礼 */
    PyObject *SHOWATENNOUNOTAIMOUNOREI;
    /* 即位礼正殿の儀 */
    PyObject *SOKUIREISEIDENNOGI;
    /* 皇太子徳仁親王の結婚の儀 */
    PyObject *KOUTAISHINARUHITOSHINNOUNOKEKKONNOGI;
} cjholidayState;


#define cjholidaystate(o) ((cjholidayState*)PyModule_GetState(o))

static int cjholiday_traverse(PyObject *m, visitproc visit, void *arg) {
    Py_VISIT(cjholidaystate(m)->DateClass);
    Py_VISIT(cjholidaystate(m)->Delta_Day1);
    Py_VISIT(cjholidaystate(m)->GANJITSU);
    Py_VISIT(cjholidaystate(m)->SEIJINNOHI);
    Py_VISIT(cjholidaystate(m)->KENKOKUKINENNOHI);
    Py_VISIT(cjholidaystate(m)->SHUNBUNNOHI);
    Py_VISIT(cjholidaystate(m)->SHOWANOHI);
    Py_VISIT(cjholidaystate(m)->KENPOKINENBI);
    Py_VISIT(cjholidaystate(m)->MIDORINOHI);
    Py_VISIT(cjholidaystate(m)->KODOMONOHI);
    Py_VISIT(cjholidaystate(m)->UMINOHI);
    Py_VISIT(cjholidaystate(m)->YAMANOHI);
    Py_VISIT(cjholidaystate(m)->KEIRONOHI);
    Py_VISIT(cjholidaystate(m)->SHUBUNNOHI);
    Py_VISIT(cjholidaystate(m)->TAIKUNOHI);
    Py_VISIT(cjholidaystate(m)->BUNKANOHI);
    Py_VISIT(cjholidaystate(m)->KINROKANSHANOHI);
    Py_VISIT(cjholidaystate(m)->TENNOTANJOBI);
    Py_VISIT(cjholidaystate(m)->FURIKAEKYUJITSU);
    Py_VISIT(cjholidaystate(m)->KOKUMINNOKYUJITSU);
    Py_VISIT(cjholidaystate(m)->KOUTAISHIAKIHITOSHINNOUNOKEKKONNOGI);
    Py_VISIT(cjholidaystate(m)->SHOWATENNOUNOTAIMOUNOREI);
    Py_VISIT(cjholidaystate(m)->SOKUIREISEIDENNOGI);
    Py_VISIT(cjholidaystate(m)->KOUTAISHINARUHITOSHINNOUNOKEKKONNOGI);
    return 0;
}

static int cjholiday_clear(PyObject *m) {
    Py_CLEAR(cjholidaystate(m)->DateClass);
    Py_CLEAR(cjholidaystate(m)->Delta_Day1);
    Py_CLEAR(cjholidaystate(m)->GANJITSU);
    Py_CLEAR(cjholidaystate(m)->SEIJINNOHI);
    Py_CLEAR(cjholidaystate(m)->KENKOKUKINENNOHI);
    Py_CLEAR(cjholidaystate(m)->SHUNBUNNOHI);
    Py_CLEAR(cjholidaystate(m)->SHOWANOHI);
    Py_CLEAR(cjholidaystate(m)->KENPOKINENBI);
    Py_CLEAR(cjholidaystate(m)->MIDORINOHI);
    Py_CLEAR(cjholidaystate(m)->KODOMONOHI);
    Py_CLEAR(cjholidaystate(m)->UMINOHI);
    Py_CLEAR(cjholidaystate(m)->YAMANOHI);
    Py_CLEAR(cjholidaystate(m)->KEIRONOHI);
    Py_CLEAR(cjholidaystate(m)->SHUBUNNOHI);
    Py_CLEAR(cjholidaystate(m)->TAIKUNOHI);
    Py_CLEAR(cjholidaystate(m)->BUNKANOHI);
    Py_CLEAR(cjholidaystate(m)->KINROKANSHANOHI);
    Py_CLEAR(cjholidaystate(m)->TENNOTANJOBI);
    Py_CLEAR(cjholidaystate(m)->FURIKAEKYUJITSU);
    Py_CLEAR(cjholidaystate(m)->KOKUMINNOKYUJITSU);
    Py_CLEAR(cjholidaystate(m)->KOUTAISHIAKIHITOSHINNOUNOKEKKONNOGI);
    Py_CLEAR(cjholidaystate(m)->SHOWATENNOUNOTAIMOUNOREI);
    Py_CLEAR(cjholidaystate(m)->SOKUIREISEIDENNOGI);
    Py_CLEAR(cjholidaystate(m)->KOUTAISHINARUHITOSHINNOUNOKEKKONNOGI);
    return 0;
}

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
calc_holiday(PyObject *module, PyObject *date) {
    /* 祝日名を返す。祝日ではないときは None を返す。
       失敗時、例外を設定し NULL を返す。
     */
    long year, month, day;
    long autumn;
    long weekday = -1;
    PyObject *name = Py_None;
    PyObject *temp;

    temp = PyObject_GetAttrString(date, "year");
    if (temp == NULL) { return NULL; }
    if (!PyLong_Check(temp)) { Py_DECREF(temp); return NULL; }
    year = PyLong_AsLong(temp);
    Py_DECREF(temp);

    temp = PyObject_GetAttrString(date, "month");
    if (temp == NULL) { return NULL; }
    if (!PyLong_Check(temp)) { Py_DECREF(temp); return NULL; }
    month = PyLong_AsLong(temp);
    Py_DECREF(temp);

    temp = PyObject_GetAttrString(date, "day");
    if (temp == NULL) { return NULL; }
    if (!PyLong_Check(temp)) { Py_DECREF(temp); return NULL; }
    day = PyLong_AsLong(temp);
    Py_DECREF(temp);


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
                name = cjholidaystate(module)->GANJITSU;
            }
            else if (year >= 2000) {
                if ((day - 1) / 7 == 1) {
                    if ((weekday = get_weekday(date)) == -1) { return NULL; }
                    if (weekday == 0) {
                        name = cjholidaystate(module)->SEIJINNOHI;
                    }
                }
            }
            else if (day == 15) {
                name = cjholidaystate(module)->SEIJINNOHI;
            }
            break;
        case 2:
            if (day == 11 && year >= 1967) {
                name = cjholidaystate(module)->KENKOKUKINENNOHI;
            }
            else if (day == 23 && year >= 2020) {
                name = cjholidaystate(module)->TENNOTANJOBI;
            }
            else if (year == 1989 && day == 24) {
                /* 1989/2/24 */
                name = cjholidaystate(module)->SHOWATENNOUNOTAIMOUNOREI;
            }
            break;
        case 3:
            if (day == vernal_equinox(year)) {
                name = cjholidaystate(module)->SHUNBUNNOHI;
            }
            break;
        case 4:
            if (day == 29) {
                if (year >= 2007) {
                    name = cjholidaystate(module)->SHOWANOHI;
                }
                else if (year >= 1989) {
                    name = cjholidaystate(module)->MIDORINOHI;
                }
                else {
                    /* 昭和天皇 */
                    name = cjholidaystate(module)->TENNOTANJOBI;
                }
            }
            else if (year == 1959 && day == 10) {
                /* 1959/4/10 */
                name = cjholidaystate(module)->KOUTAISHIAKIHITOSHINNOUNOKEKKONNOGI;
            }
            break;
        case 5:
            if (day == 3) {
                name = cjholidaystate(module)->KENPOKINENBI;
            }
            else if (day == 4) {
                if (year >= 2007) {
                    name = cjholidaystate(module)->MIDORINOHI;
                }
                else if (year >= 1986) {
                    if ((weekday = get_weekday(date)) == -1) { return NULL; }
                    if (weekday != 0) {
                        name = cjholidaystate(module)->KOKUMINNOKYUJITSU;
                    }
                }
            }
            else if (day == 5) {
                name = cjholidaystate(module)->KODOMONOHI;
            }
            else if (day == 6) {
                if (year >= 2007) {
                    if ((weekday = get_weekday(date)) == -1) { return NULL; }
                    if (weekday == 1 || weekday == 2) {
                        name = cjholidaystate(module)->FURIKAEKYUJITSU;
                    }
                }
            }
            break;
        case 6:
            if (year == 1993 && day == 9) {
                /* 1993/6/9 */
                name = cjholidaystate(module)->KOUTAISHINARUHITOSHINNOUNOKEKKONNOGI;
            }
            break;
        case 7:
            if (year >= 2003) {
                if ((day - 1) / 7 == 2) {
                    if ((weekday = get_weekday(date)) == -1) { return NULL; }
                    if (weekday == 0) {
                        name = cjholidaystate(module)->UMINOHI;
                    }
                }
            }
            else if (year >= 1996 && day == 20) {
                name = cjholidaystate(module)->UMINOHI;
            }
            break;
        case 8:
            if (year >= 2016 && day == 11) {
                name = cjholidaystate(module)->YAMANOHI;
            }
            break;
        case 9:
            autumn = autumn_equinox(year);
            if (day == autumn) {
                name = cjholidaystate(module)->SHUBUNNOHI;
            }
            else {
                if (year >= 2003) {
                    if ((weekday = get_weekday(date)) == -1) { return NULL; }
                    if ((day - 1) / 7 == 2 && weekday == 0) {
                        name = cjholidaystate(module)->KEIRONOHI;
                    }
                    else if (weekday == 1 && day == autumn - 1) {
                        name = cjholidaystate(module)->KOKUMINNOKYUJITSU;
                    }
                }
                else if (year >= 1966 && day == 15) {
                    name = cjholidaystate(module)->KEIRONOHI;
                }
            }
            break;
        case 10:
            if (year >= 2000) {
                if ((day - 1) / 7 == 1) {
                    if ((weekday = get_weekday(date)) == -1) { return NULL; }
                    if (weekday == 0) {
                        name = cjholidaystate(module)->TAIKUNOHI;
                    }
                }
            }
            else if (year >= 1966 && day == 10) {
                name = cjholidaystate(module)->TAIKUNOHI;
            }
            break;
        case 11:
            if (day == 3) {
                name = cjholidaystate(module)->BUNKANOHI;
            }
            else if (day == 23) {
                name = cjholidaystate(module)->KINROKANSHANOHI;
            }
            else if (year == 1990 && day == 12) {
                name = cjholidaystate(module)->SOKUIREISEIDENNOGI;
            }
            break;
        case 12:
            if (day == 23 && year >= 1989 && year <= 2018) {
                /* 平成天皇 */
                name = cjholidaystate(module)->TENNOTANJOBI;
            }
    }

    if (name == Py_None) {
        if (weekday == -1) {
            if ((weekday = get_weekday(date)) == -1) { return NULL; }
        }
        if (weekday == 0) {
            PyObject *prev_date, *prev_name;

            prev_date = PyNumber_Subtract(date, cjholidaystate(module)->Delta_Day1);
            if (prev_date == NULL) { return NULL; }

            prev_name = calc_holiday(module, prev_date);
            Py_DECREF(prev_date);
            if (prev_name == NULL) { return NULL; }

            if (prev_name == Py_None) {
                return prev_name;
            }
            Py_DECREF(prev_name);

            name = cjholidaystate(module)->FURIKAEKYUJITSU;
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

    if (date != NULL && year == NULL && month == NULL && day == NULL) {
        Py_INCREF(date);
    } else if (year != NULL && month != NULL && day != NULL) {
        date = PyObject_CallFunctionObjArgs(cjholidaystate(self)->DateClass, year, month, day, NULL);
        if (date == NULL) { return NULL; }
    } else {
        PyErr_SetString(PyExc_TypeError, "year, month, day is not int or date is not datetime.date");
        return NULL;
    }

    result = calc_holiday(self, date);

    Py_DECREF(date);
    return result;
}

static int cjholiday_exec(PyObject *module)
{
    PyObject *datetime_module, *date_cls, *day1;

    /* module.version = version */
    if (PyModule_AddStringConstant(module, "version", "1.1")) {
        Py_DECREF(module);
        return -1;
    }

    datetime_module = PyImport_ImportModule("datetime");
    if (datetime_module == NULL) {
        Py_DECREF(module);
        return -1;
    }

    /* datetime.date */
    date_cls = PyObject_GetAttrString(datetime_module, "date");
    if (date_cls == NULL) {
        Py_DECREF(datetime_module);
        Py_DECREF(module);
        return -1;
    }
    cjholidaystate(module)->DateClass = date_cls;

    /* datetime.timedelta(days=1) */
    day1 = PyObject_CallMethod(datetime_module, "timedelta", "i", 1);
    Py_DECREF(datetime_module);
    if (day1 == NULL) {
        Py_DECREF(module);
        return -1;
    }
    cjholidaystate(module)->Delta_Day1 = day1;

    /* 祝日名 */
    if ((cjholidaystate(module)->GANJITSU = PyUnicode_FromString("元日")) == NULL) { Py_DECREF(module); return -1; }
    if ((cjholidaystate(module)->SEIJINNOHI = PyUnicode_FromString("成人の日")) == NULL) { Py_DECREF(module); return -1; }
    if ((cjholidaystate(module)->KENKOKUKINENNOHI = PyUnicode_FromString("建国記念の日")) == NULL) { Py_DECREF(module); return -1; }
    if ((cjholidaystate(module)->SHUNBUNNOHI = PyUnicode_FromString("春分の日")) == NULL) { Py_DECREF(module); return -1; }
    if ((cjholidaystate(module)->SHOWANOHI = PyUnicode_FromString("昭和の日")) == NULL) { Py_DECREF(module); return -1; }
    if ((cjholidaystate(module)->KENPOKINENBI = PyUnicode_FromString("憲法記念日")) == NULL) { Py_DECREF(module); return -1; }
    if ((cjholidaystate(module)->MIDORINOHI = PyUnicode_FromString("みどりの日")) == NULL) { Py_DECREF(module); return -1; }
    if ((cjholidaystate(module)->KODOMONOHI = PyUnicode_FromString("こどもの日")) == NULL) { Py_DECREF(module); return -1; }
    if ((cjholidaystate(module)->UMINOHI = PyUnicode_FromString("海の日")) == NULL) { Py_DECREF(module); return -1; }
    if ((cjholidaystate(module)->YAMANOHI = PyUnicode_FromString("山の日")) == NULL) { Py_DECREF(module); return -1; }
    if ((cjholidaystate(module)->KEIRONOHI = PyUnicode_FromString("敬老の日")) == NULL) { Py_DECREF(module); return -1; }
    if ((cjholidaystate(module)->SHUBUNNOHI = PyUnicode_FromString("秋分の日")) == NULL) { Py_DECREF(module); return -1; }
    if ((cjholidaystate(module)->TAIKUNOHI = PyUnicode_FromString("体育の日")) == NULL) { Py_DECREF(module); return -1; }
    if ((cjholidaystate(module)->BUNKANOHI = PyUnicode_FromString("文化の日")) == NULL) { Py_DECREF(module); return -1; }
    if ((cjholidaystate(module)->KINROKANSHANOHI = PyUnicode_FromString("勤労感謝の日")) == NULL) { Py_DECREF(module); return -1; }
    if ((cjholidaystate(module)->TENNOTANJOBI = PyUnicode_FromString("天皇誕生日")) == NULL) { Py_DECREF(module); return -1; }
    if ((cjholidaystate(module)->FURIKAEKYUJITSU = PyUnicode_FromString("振替休日")) == NULL) { Py_DECREF(module); return -1; }
    if ((cjholidaystate(module)->KOKUMINNOKYUJITSU = PyUnicode_FromString("国民の休日")) == NULL) { Py_DECREF(module); return -1; }
    if ((cjholidaystate(module)->KOUTAISHIAKIHITOSHINNOUNOKEKKONNOGI = PyUnicode_FromString("皇太子明仁親王の結婚の儀")) == NULL) { Py_DECREF(module); return -1; }
    if ((cjholidaystate(module)->SHOWATENNOUNOTAIMOUNOREI = PyUnicode_FromString("昭和天皇の大喪の礼")) == NULL) { Py_DECREF(module); return -1; }
    if ((cjholidaystate(module)->SOKUIREISEIDENNOGI = PyUnicode_FromString("即位礼正殿の儀")) == NULL) { Py_DECREF(module); return -1; }
    if ((cjholidaystate(module)->KOUTAISHINARUHITOSHINNOUNOKEKKONNOGI = PyUnicode_FromString("皇太子徳仁親王の結婚の儀")) == NULL) { Py_DECREF(module); return -1; }

    return 0;
}

static PyModuleDef_Slot cjholiday_slots[] = {
    {Py_mod_exec, cjholiday_exec},
    {0, NULL}
};

static PyMethodDef cjholiday_method[] = {
    {"holiday_name", (PyCFunction)holiday_name,
     METH_VARARGS | METH_KEYWORDS,
     "get name of holiday from ymd or datetime.date."},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef cjholiday_module = {
    PyModuleDef_HEAD_INIT,
    "cjholiday",            /* m_name */
    NULL,                   /* m_doc */
    sizeof(cjholidayState), /* m_size */
    cjholiday_method,       /* m_methods */
    cjholiday_slots,        /* m_slots */
    cjholiday_traverse,     /* m_traverse */
    cjholiday_clear,        /* m_clear */
    NULL                    /* m_free */
};

PyMODINIT_FUNC PyInit_cjholiday(void) {
    return PyModuleDef_Init(&cjholiday_module);
}
/*
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//_/ CopyRight(C) K.Tsunoda(AddinBox) 2001 All Rights Reserved.
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
*/
