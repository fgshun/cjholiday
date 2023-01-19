/*[clinic input]
preserve
[clinic start generated code]*/

#if defined(Py_BUILD_CORE) && !defined(Py_BUILD_CORE_MODULE)
#  include "pycore_gc.h"            // PyGC_Head
#  include "pycore_runtime.h"       // _Py_ID()
#endif


PyDoc_STRVAR(cjholiday_holiday_name__doc__,
"holiday_name($module, /, year=<unrepresentable>,\n"
"             month=<unrepresentable>, day=<unrepresentable>,\n"
"             date=<unrepresentable>)\n"
"--\n"
"\n"
"get name of holiday from ymd or datetime.date.");

#define CJHOLIDAY_HOLIDAY_NAME_METHODDEF    \
    {"holiday_name", _PyCFunction_CAST(cjholiday_holiday_name), METH_FASTCALL|METH_KEYWORDS, cjholiday_holiday_name__doc__},

static PyObject *
cjholiday_holiday_name_impl(PyObject *module, PyObject *pyyear,
                            PyObject *pymonth, PyObject *pyday,
                            PyObject *date);

static PyObject *
cjholiday_holiday_name(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    #if defined(Py_BUILD_CORE) && !defined(Py_BUILD_CORE_MODULE)

    #define NUM_KEYWORDS 4
    static struct {
        PyGC_Head _this_is_not_used;
        PyObject_VAR_HEAD
        PyObject *ob_item[NUM_KEYWORDS];
    } _kwtuple = {
        .ob_base = PyVarObject_HEAD_INIT(&PyTuple_Type, NUM_KEYWORDS)
        .ob_item = { &_Py_ID(year), &_Py_ID(month), &_Py_ID(day), &_Py_ID(date), },
    };
    #undef NUM_KEYWORDS
    #define KWTUPLE (&_kwtuple.ob_base.ob_base)

    #else  // !Py_BUILD_CORE
    #  define KWTUPLE NULL
    #endif  // !Py_BUILD_CORE

    static const char * const _keywords[] = {"year", "month", "day", "date", NULL};
    static _PyArg_Parser _parser = {
        .keywords = _keywords,
        .fname = "holiday_name",
        .kwtuple = KWTUPLE,
    };
    #undef KWTUPLE
    PyObject *argsbuf[4];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 0;
    PyObject *pyyear = NULL;
    PyObject *pymonth = NULL;
    PyObject *pyday = NULL;
    PyObject *date = NULL;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 0, 4, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!noptargs) {
        goto skip_optional_pos;
    }
    if (args[0]) {
        pyyear = args[0];
        if (!--noptargs) {
            goto skip_optional_pos;
        }
    }
    if (args[1]) {
        pymonth = args[1];
        if (!--noptargs) {
            goto skip_optional_pos;
        }
    }
    if (args[2]) {
        pyday = args[2];
        if (!--noptargs) {
            goto skip_optional_pos;
        }
    }
    date = args[3];
skip_optional_pos:
    return_value = cjholiday_holiday_name_impl(module, pyyear, pymonth, pyday, date);

exit:
    return return_value;
}
/*[clinic end generated code: output=52d4b8fc0cffa256 input=a9049054013a1b77]*/
